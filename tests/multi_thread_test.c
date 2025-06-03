/* tests/multi_thread_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpu.h>
#include <memory.h>
#include <parser.h>

void print_thread_info(Memory *mem, int thread_id) {
    // DÜZELTME: OS data base address'ini ekle
    long int base = OS_DATA_START_ADDR + 500 + (thread_id * 8);
    printf("Thread %d: State=%ld, PC=%ld, SP=%ld, DB=%ld, IB=%ld, IC=%ld, Wakeup=%ld\n",
           thread_id,
           mem_read(mem, base + 1, KERNEL),
           mem_read(mem, base + 2, KERNEL),
           mem_read(mem, base + 3, KERNEL),
           mem_read(mem, base + 4, KERNEL),
           mem_read(mem, base + 5, KERNEL),
           mem_read(mem, base + 6, KERNEL),
           mem_read(mem, base + 7, KERNEL));
}

void debug_thread_table_raw(Memory *mem) {
    printf("\n=== RAW THREAD TABLE DEBUG ===\n");

    // DÜZELTME: Thread table gerçek adresi: 20 + 500 = 520
    int thread_table_start = OS_DATA_START_ADDR + 500; // 520
    int thread_table_end = thread_table_start + 87;     // 607

    printf("Raw memory dump from address %d-%d:\n", thread_table_start, thread_table_end);
    for (int addr = thread_table_start; addr <= thread_table_end; addr++) {
        long int value = mem_read(mem, addr, KERNEL);
        printf("M[%d] = %ld\n", addr, value);

        // Her thread'in başında break
        if ((addr - thread_table_start) % 8 == 7) {
            printf("--- End of Thread %d ---\n", (addr - thread_table_start) / 8);
        }
    }

    printf("\n=== EXPECTED VALUES ===\n");
    printf("Thread 0 (OS): %d-%d\n", thread_table_start, thread_table_start + 7);
    printf("  Expected: [0, 1, 1000, 1999, 20, 1000, 0, 0]\n");

    printf("Thread 1 (Idle): %d-%d\n", thread_table_start + 8, thread_table_start + 15);
    printf("  Expected: [1, 0, 3000, 3999, 2000, 3000, 0, 0]\n");

    printf("Thread 2 (Bubble): %d-%d\n", thread_table_start + 16, thread_table_start + 23);
    printf("  Expected: [2, 0, 5000, 5999, 4000, 5000, 0, 0]\n");

    printf("Thread 3 (Search): %d-%d\n", thread_table_start + 24, thread_table_start + 31);
    printf("  Expected: [3, 0, 7000, 7999, 6000, 7000, 0, 0]\n");

    printf("Thread 4 (Mult): %d-%d\n", thread_table_start + 32, thread_table_start + 39);
    printf("  Expected: [4, 0, 9000, 9999, 8000, 9000, 0, 0]\n");
    printf("========================\n\n");
}

void print_system_state(CPU *cpu, Memory *mem) {
	printf("\n=== System State ===\n");
	printf("Current Thread: %d, Mode: %s, PC: %ld\n",
		   cpu->curr_thread_id,
		   cpu->mode == KERNEL ? "KERNEL" : "USER",
		   mem_read(mem, REG_PC, KERNEL));
	printf("M[100] (current_running_thread_id): %ld\n",
		   mem_read(mem, OS_DATA_START_ADDR + 100, KERNEL));

	debug_thread_table_raw(mem);

	// Active threads info - SADECE 0-4 arası (OS + 4 aktif thread)
	printf("\nThread States:\n");
	for (int i = 0; i <= 4; i++) {  // 0-4 arası, 7 yerine 4
		print_thread_info(mem, i);
	}
	printf("==================\n\n");
}

int main(void) {
	printf("=== Multi-Thread OS Test ===\n\n");

	// Initialize memory
	Memory mem;
	mem_init(&mem);

	// Load the multi-thread program
	const char *asm_file = "programs/multi_thread.asm";
	if (load_program_from_file(asm_file, &mem) != 0) {
		fprintf(stderr, "Failed to load assembly file: %s\n", asm_file);
		mem_free(&mem);
		return EXIT_FAILURE;
	}

	// Initialize CPU
	CPU cpu;
	cpu_init(&cpu, &mem);

	printf("System initialized. Starting execution...\n");
	printf("OS Entry Point: %d\n", OS_ENTRY_POINT_BOOT);
	printf("Thread Count: 4 active threads + 1 idle thread\n\n");

	// Execution parameters
	int max_instructions = 2000;
	int instruction_count = 0;
	int context_switches = 0;
	int syscalls = 0;

	// Track last thread ID for context switch detection
	int last_thread_id = cpu.curr_thread_id;

	// Main execution loop
	while (!cpu_is_halted(&cpu) && instruction_count < max_instructions) {
		// Check for context switch
		if (cpu.curr_thread_id != last_thread_id) {
			context_switches++;
			printf("\n*** Context Switch: Thread %d → Thread %d ***\n",
				   last_thread_id, cpu.curr_thread_id);
			last_thread_id = cpu.curr_thread_id;
		}

		// Get current instruction info
		long int current_pc = mem_read(&mem, REG_PC, KERNEL);
		long int opcode = mem_read(&mem, current_pc, KERNEL);

		// Track syscalls
		if (opcode >= OPCODE_SYSCALL_PRN && opcode <= OPCODE_SYSCALL_YIELD) {
			syscalls++;
			printf("\n[%d] Thread %d executing ", instruction_count, cpu.curr_thread_id);
			switch(opcode) {
				case OPCODE_SYSCALL_PRN:
					printf("SYSCALL PRN");
					break;
				case OPCODE_SYSCALL_HLT:
					printf("SYSCALL HLT");
					break;
				case OPCODE_SYSCALL_YIELD:
					printf("SYSCALL YIELD");
					break;
			}
			printf(" at PC=%ld\n", current_pc);
		}

		// Execute instruction
		cpu_execute_instruction(&cpu);
		instruction_count++;

		// Periodic state dump
		if (instruction_count % 200 == 0 || context_switches == 1 || context_switches == 5) {
			print_system_state(&cpu, &mem);
		}

		// Safety check for infinite loops - Sadece Thread 1-4 için kontrol
		if (cpu.curr_thread_id >= 1 && cpu.curr_thread_id <= 4 &&
		    instruction_count > 100 && context_switches < 2) {
			printf("\nWarning: Thread %d running too long, possible scheduler issue\n",
			       cpu.curr_thread_id);
			break;
		}
		// Eğer Thread 5+ seçildiyse dur
		if (cpu.curr_thread_id > 4) {
			printf("\nWarning: Scheduler selected Thread %d (>4), stopping test\n",
			       cpu.curr_thread_id);
			break;
		}
	}

	// Final summary
	printf("\n\n=== Execution Summary ===\n");
	printf("Total Instructions Executed: %d\n", instruction_count);
	printf("Context Switches: %d\n", context_switches);
	printf("System Calls: %d\n", syscalls);

	if (instruction_count >= max_instructions) {
		printf("WARNING: Execution stopped at instruction limit\n");
	}

	if (cpu_is_halted(&cpu)) {
		printf("CPU Halted normally\n");
	}

	// Final system state
	print_system_state(&cpu, &mem);

	// Check some expected results
	printf("\n=== Test Results ===\n");
	// Test case düzeltmeleri
	bool test_passed = true;

	// DÜZELTME: Thread table doğru adreslerinden oku
	// Thread 2 should have sorted array if it completed
	long int thread2_state_addr = OS_DATA_START_ADDR + 500 + (2 * 8) + 1; // 520 + 16 + 1 = 537
	if (mem_read(&mem, thread2_state_addr, KERNEL) == 3) { // Thread 2 terminated
	    printf("Thread 2 (Bubble Sort): ");
	    long int arr[5];
	    for (int i = 0; i < 5; i++) {
	        arr[i] = mem_read(&mem, THREAD_DATA_START(2) + i, KERNEL);
	    }
	    if (arr[0] == -98 && arr[1] == 3 && arr[2] == 5 && arr[3] == 999) {
	        printf("PASSED - Array sorted correctly\n");
	    } else {
	        printf("FAILED - Array not sorted\n");
	        test_passed = false;
	    }
	}

	// Thread 3 düzeltmesi
	long int thread3_state_addr = OS_DATA_START_ADDR + 500 + (3 * 8) + 1; // 545
	if (mem_read(&mem, thread3_state_addr, KERNEL) == 3) { // Thread 3 terminated
	    long int result = mem_read(&mem, THREAD_DATA_START(3) + 12, KERNEL);
	    printf("Thread 3 (Linear Search): ");
	    if (result == 5) {
	        printf("PASSED - Found -345 at index 5\n");
	    } else {
	        printf("FAILED - Expected index 5, got %ld\n", result);
	        test_passed = false;
	    }
	}

	// Thread 4 düzeltmesi
	long int thread4_state_addr = OS_DATA_START_ADDR + 500 + (4 * 8) + 1; // 553
	if (mem_read(&mem, thread4_state_addr, KERNEL) == 3) { // Thread 4 terminated
	    long int result = mem_read(&mem, THREAD_DATA_START(4) + 3, KERNEL);
	    printf("Thread 4 (Multiplication): ");
	    if (result == -960000) {
	        printf("PASSED - Calculated -960000\n");
	    } else {
	        printf("FAILED - Expected -960000, got %ld\n", result);
	        test_passed = false;
	    }
	}

	printf("\nOverall Test: %s\n", test_passed ? "PASSED" : "FAILED");

	// Cleanup
	mem_free(&mem);
	return test_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
