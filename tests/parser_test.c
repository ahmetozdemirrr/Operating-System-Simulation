/* tests/parser_test.c */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <common.h>
#include <memory.h>
#include <parser.h>

void print_test_result(const char* test_name, bool success)
{
    if (success) {
        printf("TEST PASSED: %s\n", test_name);
    }
    else {
        fprintf(stderr, "TEST FAILED: %s\n", test_name);
    }
}

int main(void)
{
    printf("Parser Module Tests Starting...\n\n");

    Memory test_memory;
    bool overall_test_status = true;
    int parser_result;

    const char *test_asm_file = "tests/test_source/test_parser.asm";

    printf("--- Test 1: Program Loading and Basic Checks (%s) ---\n", test_asm_file);
    mem_init(&test_memory);

    parser_result = load_program_from_file(test_asm_file, &test_memory);

    if (parser_result != 0) {
        fprintf(stderr, "TEST FAILED: load_program_from_file expected 0 but returned %d.\n", parser_result);
        overall_test_status = false;
    }
    else {
        printf("INFO: load_program_from_file completed successfully.\n");

        printf("\nMemory Dump (OS Data Section, %ld-%ld):\n", (long int)OS_DATA_START_ADDR, (long int)OS_DATA_START_ADDR + 9);
        mem_dump(&test_memory, OS_DATA_START_ADDR, 10);

        printf("\nMemory Dump (Thread 1 Data Section, %ld-%ld):\n", THREAD_DATA_START(1), THREAD_DATA_START(1) + 59);
        mem_dump(&test_memory, THREAD_DATA_START(1), 60);

        printf("\nMemory Dump (Thread 1 Instruction Section, %ld-%ld):\n", THREAD_INSTR_START(1), THREAD_INSTR_START(1) + 14);
        mem_dump(&test_memory, THREAD_INSTR_START(1), 15);

        // --- Specific Value Checks for OS ---
        // test_parser.asm OS data:
        // 0 0    # Program Counter -> OS_DATA_START_ADDR + 0 = 0
        // 1 16383 # Stack Pointer   -> OS_DATA_START_ADDR + 1 = 16383
        // Physical REG_PC (addr 0) and REG_SP (addr 1) are not directly set by parser for OS data in this way.
        // They would be set by cpu_init. Here, after mem_init and parser, they should be 0.
        bool data_check_pc_phys = (mem_read(&test_memory, REG_PC, KERNEL) == 0);
        print_test_result("Physical REG_PC (addr 0) value after parse", data_check_pc_phys);
        if (!data_check_pc_phys) overall_test_status = false;

        // Physical REG_SP (addr 1) should be 0 as mem_init zeros it and parser doesn't write to physical addr 1.
        // The '1 16383' in test_parser.asm writes to OS_DATA_START_ADDR + 1.
        bool data_check_sp_phys = (mem_read(&test_memory, REG_SP, KERNEL) == 0);
        print_test_result("Physical REG_SP (addr 1) value after parse", data_check_sp_phys);
        if (!data_check_sp_phys) overall_test_status = false;

        // Check values loaded into OS data segment by parser
        bool os_data_val0 = (mem_read(&test_memory, OS_DATA_START_ADDR + 0, KERNEL) == 0);
        print_test_result("OS Data Segment: offset 0 value (0)", os_data_val0);
        if (!os_data_val0) overall_test_status = false;

        bool os_data_val1 = (mem_read(&test_memory, OS_DATA_START_ADDR + 1, KERNEL) == 16383);
        print_test_result("OS Data Segment: offset 1 value (16383)", os_data_val1);
        if (!os_data_val1) overall_test_status = false;


        // --- Specific Value Checks for Thread 1 ---
        // test_parser.asm Thread 1 data:
        // 0 100    -> THREAD_DATA_START(1) + 0  = 100
        // 50 100   -> THREAD_DATA_START(1) + 50 = 100
        // 51 200   -> THREAD_DATA_START(1) + 51 = 200
        // 52 -5    -> THREAD_DATA_START(1) + 52 = -5
        bool data_check_T1_d0 = (mem_read(&test_memory, THREAD_DATA_START(1) + 0, KERNEL) == 100);
        print_test_result("Thread 1 Data Section: offset 0 value (100)", data_check_T1_d0);
        if (!data_check_T1_d0) overall_test_status = false;

        bool data_check_T1_d50 = (mem_read(&test_memory, THREAD_DATA_START(1) + 50, KERNEL) == 100);
        print_test_result("Thread 1 Data Section: offset 50 value (100)", data_check_T1_d50);
        if (!data_check_T1_d50) overall_test_status = false;

        bool data_check_T1_d51 = (mem_read(&test_memory, THREAD_DATA_START(1) + 51, KERNEL) == 200);
        print_test_result("Thread 1 Data Section: offset 51 value (200)", data_check_T1_d51);
        if (!data_check_T1_d51) overall_test_status = false;

        bool data_check_T1_d52 = (mem_read(&test_memory, THREAD_DATA_START(1) + 52, KERNEL) == -5);
        print_test_result("Thread 1 Data Section: offset 52 value (-5)", data_check_T1_d52);
        if (!data_check_T1_d52) overall_test_status = false;

        long int instr_base = THREAD_INSTR_START(1); // Should be 3000
        long int instr_size = INSTR_SIZE; // 1 opcode + 2 operands = 3

        // Command 0: CPY 50 200 (from test_parser.asm)
        bool instr0_op = (mem_read(&test_memory, instr_base + 0 * instr_size, KERNEL) == (long int)OPCODE_CPY);
        bool instr0_p1 = (mem_read(&test_memory, instr_base + 0 * instr_size + 1, KERNEL) == 50);
        bool instr0_p2 = (mem_read(&test_memory, instr_base + 0 * instr_size + 2, KERNEL) == 200); // Corrected from 600
        print_test_result("Thread 1 Command 0: CPY 50 200", instr0_op && instr0_p1 && instr0_p2);
        if (!(instr0_op && instr0_p1 && instr0_p2)) overall_test_status = false;

        // Command 1: ADDI 200 51 (from test_parser.asm)
        bool instr1_op = (mem_read(&test_memory, instr_base + 1 * instr_size, KERNEL) == (long int)OPCODE_ADDI);
        bool instr1_p1 = (mem_read(&test_memory, instr_base + 1 * instr_size + 1, KERNEL) == 200); // Corrected from 600
        bool instr1_p2 = (mem_read(&test_memory, instr_base + 1 * instr_size + 2, KERNEL) == 51);
        print_test_result("Thread 1 Command 1: ADDI 200 51", instr1_op && instr1_p1 && instr1_p2);
        if (!(instr1_op && instr1_p1 && instr1_p2)) overall_test_status = false;

        // Command 2: SYSCALL PRN 200 (from test_parser.asm)
        bool instr2_op = (mem_read(&test_memory, instr_base + 2 * instr_size, KERNEL) == (long int)OPCODE_SYSCALL_PRN);
        bool instr2_p1 = (mem_read(&test_memory, instr_base + 2 * instr_size + 1, KERNEL) == 200); // Corrected from 600
        bool instr2_p2 = (mem_read(&test_memory, instr_base + 2 * instr_size + 2, KERNEL) == 0); // Operand 2 should be 0 for 1-operand instructions
        print_test_result("Thread 1 Command 2: SYSCALL PRN 200", instr2_op && instr2_p1 && instr2_p2);
        if (!(instr2_op && instr2_p1 && instr2_p2)) overall_test_status = false;

        // Command 3: HLT (from test_parser.asm)
        bool instr3_op = (mem_read(&test_memory, instr_base + 3 * instr_size, KERNEL) == (long int)OPCODE_HLT);
        bool instr3_p1 = (mem_read(&test_memory, instr_base + 3 * instr_size + 1, KERNEL) == 0);
        bool instr3_p2 = (mem_read(&test_memory, instr_base + 3 * instr_size + 2, KERNEL) == 0);
        print_test_result("Thread 1 Command 3: HLT", instr3_op && instr3_p1 && instr3_p2);
        if (!(instr3_op && instr3_p1 && instr3_p2)) overall_test_status = false;
    }

    printf("\nParser Module Tests Completed.\n");
    if (overall_test_status) {
        printf("ALL PARSER TESTS PASSED!\n");
        return 0;
    }
    else {
        fprintf(stderr, "SOME PARSER TESTS FAILED!\n");
        return 1;
    }
}
