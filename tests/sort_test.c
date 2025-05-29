/* tests/sort_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpu.h>
#include <memory.h>
#include <parser.h>

int main() {
    // 1. Belleği başlat
    Memory mem;
    mem_init(&mem);

    // 2. Assembly dosyasını oku ve belleğe yükle
    const char * asm_file = "tests/test_source/sort_test.asm";

    if (load_program_from_file(asm_file, &mem) != 0) {
        fprintf(stderr, "Failed to load assembly file: %s\n", asm_file);
        mem_free(&mem);
        return EXIT_FAILURE;
    }

    // 3. Jump address'i ayarla (OS'un USER komutu için)
    // OS data[50]'ye Thread 1'in instruction başlangıcını yaz
    mem_write(&mem, OS_DATA_START_ADDR + 50, THREAD_INSTR_START(1), KERNEL);

    printf("OS will jump to address: %ld\n", mem_read(&mem, OS_DATA_START_ADDR + 50, KERNEL));

    // 4. CPU'yu başlat
    CPU cpu;
    cpu_init(&cpu, &mem);

    // OS olarak başlıyoruz
    printf("Starting execution in OS mode...\n");
    printf("Initial array in Thread 1: ");
    for (int i = 0; i < 5; i++) {
        printf("%ld ", mem_read(&mem, THREAD_DATA_START(1) + i, KERNEL));
    }
    printf("\n\n");
//mem_dump(&mem, 0, 4000);
    // 5. Debug bilgileri
    printf("Memory Layout:\n");
    printf("- OS Data: %d-%d\n", OS_DATA_START_ADDR, OS_DATA_END_ADDR);
    printf("- OS Instructions: %d-%d\n", OS_INSTRUCTION_START_ADDR, OS_INSTRUCTION_END_ADDR);
    printf("- Thread 1 Data: %d-%d\n", THREAD_DATA_START(1), THREAD_DATA_END(1));
    printf("- Thread 1 Instructions: %d-%d\n", THREAD_INSTR_START(1), THREAD_INSTR_END(1));
    printf("\n");

    // 6. Execution loop
    int instruction_count = 0;
    int max_instructions = 200;
    int os_instructions = 0;
    int thread_instructions = 0;

    while (!cpu_is_halted(&cpu) && instruction_count < max_instructions) {
        // Hangi modda olduğumuzu göster
        if (instruction_count < 10 || (instruction_count % 30) == 0) {
            printf("\n[%d] Mode: %s, Thread: %d, PC: %ld\n",
                   instruction_count,
                   cpu.mode == KERNEL ? "KERNEL" : "USER",
                   cpu.curr_thread_id,
                   mem_read(&mem, REG_PC, KERNEL));

            if (cpu.curr_thread_id == 1) {
                printf("Array state: ");
                for (int i = 0; i < 5; i++) {
                    printf("%ld ", mem_read(&mem, THREAD_DATA_START(1) + i, cpu.mode));
                }
                printf("\n");
            }
        }

        // Mode'a göre sayaç tut
        if (cpu.mode == KERNEL) {
            os_instructions++;
        } else {
            thread_instructions++;
        }

        cpu_execute_instruction(&cpu);
        instruction_count++;
    }

    if (instruction_count >= max_instructions) {
        fprintf(stderr, "\nWARNING: Execution stopped after %d instructions (possible infinite loop)\n", max_instructions);
    }

    printf("\n\nExecution Summary:\n");
    printf("- Total instructions: %d\n", instruction_count);
    printf("- OS instructions: %d\n", os_instructions);
    printf("- Thread instructions: %d\n", thread_instructions);

    // 7. Sonuçları kontrol et
    printf("\nFinal array in Thread 1: ");
    long int sorted_array[5];
    for (int i = 0; i < 5; i++) {
        sorted_array[i] = mem_read(&mem, THREAD_DATA_START(1) + i, KERNEL);
        printf("%ld ", sorted_array[i]);
    }
    printf("\n");

    // Sıralama kontrolü
    int is_sorted = 1;
    long int expected[] = {1, 2, 3, 5, 8};

    for (int i = 0; i < 5; i++) {
        if (sorted_array[i] != expected[i]) {
            is_sorted = 0;
            break;
        }
    }

    if (is_sorted) {
        printf("\nTEST PASSED: Array is correctly sorted!\n");
    } else {
        fprintf(stderr, "\nTEST FAILED: Array is not correctly sorted\n");
        fprintf(stderr, "Expected: ");
        for (int i = 0; i < 5; i++) {
            fprintf(stderr, "%ld ", expected[i]);
        }
        fprintf(stderr, "\n");
    }

    // 8. Memory dumps
    printf("\nMemory dump (OS Data Section):\n");
    mem_dump(&mem, OS_DATA_START_ADDR, 20);

    printf("\nMemory dump (Thread 1 Data Section - first 10):\n");
    mem_dump(&mem, THREAD_DATA_START(1), 10);

    // 9. Register durumu
    printf("\nFinal CPU state:\n");
    cpu_dump_registers(&cpu);

    // 10. Belleği temizle
    mem_free(&mem);

    return is_sorted ? EXIT_SUCCESS : EXIT_FAILURE;
}
