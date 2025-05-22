/* tests/cpu_test.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cpu.h>
#include <memory.h>

int main(void)
{
    // 1. Manuel memory array tanımla
    long int memory[MEM_SIZE] = {
        // Register'lar (0-20)
        276, 999, 0, 0,  // REG_PC, REG_SP, REG_SYSCALL_RESULT, REG_INSTR_COUNT
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Rezerve (4-20)

        // Data Section (21-275)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 25-255 (boş)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        // Instruction Section (276-999)
        // Test 1: ADDI (data[0] ve data[1] kullanılır)
        0, 10, 1,    // SET 10 1 → data[1] = 10
        0, 20, 0,    // SET 20 0 → data[0] = 20
        4, 0, 1,     // ADDI 0 1 → data[0] = data[0] + data[1] = 20 + 10 = 30
        13, 0, 0,    // SYSCALL_PRN 0 → 30 yazdır

        // Test 2: PUSH/POP (data[0] ve data[2] kullanılır)
        7, 0, 0,     // PUSH 0 → data[0]’ı (30) stack’e
        8, 2, 0,     // POP 2 → stack’ten data[2]’ye (30 yazılmalı)
        13, 2, 0,    // SYSCALL_PRN 2 → 30 yazdır

        // Test 3: JIF (data[3], data[4], data[5] kullanılır)
        0, 10, 3,    // SET 10 3 → data[3] = 10
        0, 0, 4,     // SET 0 4 → data[4] = 0
        6, 3, 2,     // JIF 3 2 → data[3] > 0, 2 talimat ileri atla
        0, 99, 5,    // SET 99 5 → Atlanacak (data[5] = 99 olmamalı)
        0, 20, 5,    // SET 20 5 → data[5] = 20
        13, 5, 0,    // SYSCALL_PRN 5 → 20 yazdır

        // Test 4: SUBI (data[6], data[7] kullanılır)
        0, 15, 6,    // SET 15 6 → data[6] = 15
        0, 5, 7,     // SET 5 7 → data[7] = 5
        5, 6, 7,     // SUBI 6 7 → data[6] = data[6] - data[7] = 15 - 5 = 10
        13, 7, 0,    // SYSCALL_PRN 6 → 10 yazdır

        // Test 5: Çoklu PUSH/POP (data[8], data[9], data[10], data[11] kullanılır)
        0, 10, 8,    // SET 10 8 → data[8] = 10
        0, 20, 9,    // SET 20 9 → data[9] = 20
        7, 8, 0,     // PUSH 8 → data[8]’i (10) stack’e
        7, 9, 0,     // PUSH 9 → data[9]’u (20) stack’e
        8, 10, 0,    // POP 10 → stack’ten data[10]’a (20 yazılmalı)
        8, 11, 0,    // POP 11 → stack’ten data[11]’e (10 yazılmalı)
        13, 10, 0,   // SYSCALL_PRN 10 → 20 yazdır
        13, 11, 0,   // SYSCALL_PRN 11 → 10 yazdır

        11, 0, 0,    // HLT
        -1,          // Ayırıcı
        0            // Kalan adresler
    };

    Memory mem;
    mem_init(&mem);
    memcpy(mem.data, memory, MEM_SIZE * sizeof(long int));
    mem.size = MEM_SIZE;

    // 3. CPU’yu başlat
    CPU cpu;
    cpu_init(&cpu, &mem);

    // 4. Talimatları çalıştır
    while (!cpu_is_halted(&cpu)) {
        cpu_execute_instruction(&cpu);
    }

    // 5. Sonuçları kontrol et
    int failed = 0;

    // Test 1: ADDI kontrolü (data[0] = 30 olmalı)
    long int value = mem_read(&mem, OS_DATA_START_ADDR + 0, KERNEL);

    if (value != 30) {
        fprintf(stderr, "Test 1 FAILED: Expected data[0] = 30, got %ld\n", value);
        failed++;
    }

    else {
        printf("Test 1 PASSED: ADDI operation successful\n");
    }

    // Test 2: PUSH/POP kontrolü (data[2] = 30 olmalı)
    value = mem_read(&mem, OS_DATA_START_ADDR + 2, KERNEL);

    if (value != 30) {
        fprintf(stderr, "Test 2 FAILED: Expected data[2] = 30, got %ld\n", value);
        failed++;
    }

    else {
        printf("Test 2 PASSED: PUSH/POP operation successful\n");
    }

    // Test 3: JIF kontrolü (data[3] = 10, data[5] = 20 olmalı)
    long int value3 = mem_read(&mem, OS_DATA_START_ADDR + 3, KERNEL);
    long int value5 = mem_read(&mem, OS_DATA_START_ADDR + 5, KERNEL);

    if (value3 == 10 && value5 == 20) {
        printf("Test 3 PASSED: JIF operation successful\n");
    }

    else {
        fprintf(stderr, "Test 3 FAILED: Expected data[3] = 10, data[5] = 20, got data[3] = %ld, data[5] = %ld\n", value3, value5);
        failed++;
    }

    // Test 4: SUBI kontrolü (data[6] = 10 olmalı)
    long int value6 = mem_read(&mem, OS_DATA_START_ADDR + 7, KERNEL);

    if (value6 == 10) {
        printf("Test 4 PASSED: SUBI operation successful\n");
    }

    else {
        fprintf(stderr, "Test 4 FAILED: Expected data[6] = 10, got %ld\n", value6);
        failed++;
    }

    // Test 5: Çoklu PUSH/POP kontrolü (data[10] = 20, data[11] = 10 olmalı)
    long int value10 = mem_read(&mem, OS_DATA_START_ADDR + 10, KERNEL);
    long int value11 = mem_read(&mem, OS_DATA_START_ADDR + 11, KERNEL);

    if (value10 == 20 && value11 == 10) {
        printf("Test 5 PASSED: Multiple PUSH/POP operation successful\n");
    }

    else {
        fprintf(stderr, "Test 5 FAILED: Expected data[10] = 20, data[11] = 10, got data[10] = %ld, data[11] = %ld\n", value10, value11);
        failed++;
    }

    // 6. Genel sonuç
    if (failed == 0) {
        printf("All CPU tests PASSED!\n");
    }

    else {
        printf("CPU tests FAILED: %d test(s) failed.\n", failed);
    }
    mem_free(&mem);

    return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
