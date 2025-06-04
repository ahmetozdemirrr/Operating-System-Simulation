/* tests/os_system_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpu.h>
#include <memory.h>
#include <parser.h>

// Helper function to print thread table entry
void print_thread_entry(Memory *mem, int thread_id) {
    // Thread table OS data section'ın 200. offset'inde, yani absolute 220'de başlıyor
    long int base = OS_DATA_START_ADDR + 200 + (thread_id * 8);
    printf("Thread %d: ", thread_id);
    printf("ID=%ld, State=%ld, PC=%ld, SP=%ld, DB=%ld, IB=%ld, IC=%ld, Wakeup=%ld\n",
           mem_read(mem, base, KERNEL),
           mem_read(mem, base + 1, KERNEL),
           mem_read(mem, base + 2, KERNEL),
           mem_read(mem, base + 3, KERNEL),
           mem_read(mem, base + 4, KERNEL),
           mem_read(mem, base + 5, KERNEL),
           mem_read(mem, base + 6, KERNEL),
           mem_read(mem, base + 7, KERNEL));
}

// Helper to print mailbox contents
void print_mailbox(Memory *mem) {
    // Mailbox OS data section'ın 600-607 offset'lerinde
    long int mailbox_base = OS_DATA_START_ADDR + 600;
    printf("Mailbox Contents (offsets 600-607):\n");
    printf("  Thread ID: %ld\n", mem_read(mem, mailbox_base, KERNEL));
    printf("  State: %ld\n", mem_read(mem, mailbox_base + 1, KERNEL));
    printf("  PC: %ld\n", mem_read(mem, mailbox_base + 2, KERNEL));
    printf("  SP: %ld\n", mem_read(mem, mailbox_base + 3, KERNEL));
    printf("  Data Base: %ld\n", mem_read(mem, mailbox_base + 4, KERNEL));
    printf("  Instr Base: %ld\n", mem_read(mem, mailbox_base + 5, KERNEL));
    printf("  Instr Count: %ld\n", mem_read(mem, mailbox_base + 6, KERNEL));
    printf("  Wakeup Count: %ld\n", mem_read(mem, mailbox_base + 7, KERNEL));
}

// Test thread 2 - Bubble Sort
void check_thread2_sort(Memory *mem) {
    printf("\n=== Thread 2 (Bubble Sort) Test ===\n");
    long int arr[5];
    for (int i = 0; i < 5; i++) {
        arr[i] = mem_read(mem, THREAD_DATA_START(2) + i, KERNEL);
    }

    printf("Final array: [%ld, %ld, %ld, %ld, %ld]\n", arr[0], arr[1], arr[2], arr[3], arr[4]);

    // Check if sorted
    if (arr[0] == -98 && arr[1] == 3 && arr[2] == 5 && arr[3] == 999 && arr[4] == 8579348734678LL) {
        printf("✓ PASSED: Array is correctly sorted!\n");
    } else {
        printf("✗ FAILED: Array is NOT correctly sorted\n");
        printf("Expected: [-98, 3, 5, 999, 8579348734678]\n");
    }
}

// Test thread 3 - Linear Search
void check_thread3_search(Memory *mem) {
    printf("\n=== Thread 3 (Linear Search) Test ===\n");
    long int result = mem_read(mem, THREAD_DATA_START(3) + 12, KERNEL);
    long int key = mem_read(mem, THREAD_DATA_START(3) + 11, KERNEL);

    printf("Searching for: %ld\n", key);
    printf("Result index: %ld\n", result);

    if (result == 5) {
        printf("✓ PASSED: Correctly found -345 at index 5\n");
    } else {
        printf("✗ FAILED: Expected index 5, got %ld\n", result);
    }
}

// Test thread 4 - Multiplication
void check_thread4_multiplication(Memory *mem) {
    printf("\n=== Thread 4 (Multiplication) Test ===\n");
    long int op1 = mem_read(mem, THREAD_DATA_START(4) + 0, KERNEL);
    long int op2 = mem_read(mem, THREAD_DATA_START(4) + 1, KERNEL);
    long int result = mem_read(mem, THREAD_DATA_START(4) + 3, KERNEL);
    long int expected = op1 * op2;

    printf("Calculating: %ld * %ld\n", op1, op2);
    printf("Result: %ld\n", result);
    printf("Expected: %ld\n", expected);

    if (result == expected) {
        printf("✓ PASSED: Multiplication result is correct\n");
    } else {
        printf("✗ FAILED: Expected %ld, got %ld\n", expected, result);
    }
}

int main(void) {
    printf("=== OS System Test ===\n\n");

    // Initialize memory
    Memory mem;
    mem_init(&mem);

    // Load OS and threads from file
    const char *asm_file = "programs/os.asm";
    printf("Loading program from: %s\n", asm_file);

    if (load_program_from_file(asm_file, &mem) != 0) {
        fprintf(stderr, "Failed to load assembly file: %s\n", asm_file);
        mem_free(&mem);
        return EXIT_FAILURE;
    }

    printf("Program loaded successfully!\n\n");

    // Initialize CPU
    CPU cpu;
    cpu_init(&cpu, &mem);

    // Display initial system state
    printf("=== Initial System State ===\n");
    printf("OS Entry Point: %d\n", OS_INSTRUCTION_START_ADDR);
    printf("Current Thread ID (M[100]): %ld\n", mem_read(&mem, OS_DATA_START_ADDR + 100, KERNEL));
    printf("\nInitial Thread Table:\n");
    for (int i = 0; i <= 4; i++) {
        print_thread_entry(&mem, i);
    }

    // Execution parameters
    int max_instructions = 1000000;  // Enough for all threads to complete
    int instruction_count = 0;
    int context_switches = 0;
    int last_thread_id = cpu.curr_thread_id;

    // Track thread completion
    bool thread2_completed = false;
    bool thread3_completed = false;
    bool thread4_completed = false;

    printf("\n=== Starting Execution ===\n");

    // Main execution loop
    while (!cpu_is_halted(&cpu) && instruction_count < max_instructions) {
        // Check for context switch
        if (cpu.curr_thread_id != last_thread_id) {
            context_switches++;
            printf("\n[%d] Context Switch: Thread %d → Thread %d\n",
                   instruction_count, last_thread_id, cpu.curr_thread_id);
            last_thread_id = cpu.curr_thread_id;
        }

        // Execute instruction
        cpu_execute_instruction(&cpu);
        instruction_count++;

        // Check if threads completed (state = TERMINATED = 3)
        long int thread2_state = mem_read(&mem, OS_DATA_START_ADDR + 217, KERNEL);  // 220 + 2*8 + 1
        long int thread3_state = mem_read(&mem, OS_DATA_START_ADDR + 225, KERNEL);  // 220 + 3*8 + 1
        long int thread4_state = mem_read(&mem, OS_DATA_START_ADDR + 233, KERNEL);  // 220 + 4*8 + 1
        
        if (thread2_state == 3 && !thread2_completed) {
            printf("\n[%d] Thread 2 (Bubble Sort) completed!\n", instruction_count);
            check_thread2_sort(&mem);
            thread2_completed = true;
        }
        
        if (thread3_state == 3 && !thread3_completed) {
            printf("\n[%d] Thread 3 (Linear Search) completed!\n", instruction_count);
            check_thread3_search(&mem);
            thread3_completed = true;
        }
        
        if (thread4_state == 3 && !thread4_completed) {
            printf("\n[%d] Thread 4 (Multiplication) completed!\n", instruction_count);
            check_thread4_multiplication(&mem);
            thread4_completed = true;
        }
        
        // Exit if all threads completed
        if (thread2_completed && thread3_completed && thread4_completed) {
            printf("\nAll threads completed successfully!\n");
            break;
        }
        
        // Periodic status update
        if (instruction_count % 10000 == 0) {
            printf("[%d] Still running... Current thread: %d\n", 
                   instruction_count, cpu.curr_thread_id);
        }
    }
    
    // Final summary
    printf("\n=== Execution Summary ===\n");
    printf("Total Instructions Executed: %d\n", instruction_count);
    printf("Context Switches: %d\n", context_switches);
    printf("CPU Halted: %s\n", cpu_is_halted(&cpu) ? "Yes" : "No");
    
    if (instruction_count >= max_instructions) {
        printf("WARNING: Execution stopped at instruction limit\n");
    }
    
    // Final thread table
    printf("\n=== Final Thread Table ===\n");
    for (int i = 0; i <= 4; i++) {
        print_thread_entry(&mem, i);
    }
    
    // Test results summary
    printf("\n=== Test Results Summary ===\n");
    int tests_passed = 0;
    int total_tests = 3;
    
    if (thread2_completed) {
        long int arr0 = mem_read(&mem, THREAD_DATA_START(2), KERNEL);
        if (arr0 == -98) tests_passed++;
    }
    
    if (thread3_completed) {
        long int result = mem_read(&mem, THREAD_DATA_START(3) + 12, KERNEL);
        if (result == 5) tests_passed++;
    }
    
    if (thread4_completed) {
        long int result = mem_read(&mem, THREAD_DATA_START(4) + 3, KERNEL);
        if (result == -960000) tests_passed++;
    }
    
    printf("Tests Passed: %d/%d\n", tests_passed, total_tests);
    printf("Overall Result: %s\n", 
           tests_passed == total_tests ? "ALL TESTS PASSED ✓" : "SOME TESTS FAILED ✗");
    
    // Cleanup
    mem_free(&mem);
    
    return (tests_passed == total_tests) ? EXIT_SUCCESS : EXIT_FAILURE;
}
