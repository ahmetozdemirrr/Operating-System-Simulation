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

        printf("\nMemory Dump (OS Data Section, 20-29):\n");
        mem_dump(&test_memory, OS_DATA_START_ADDR, 10);

        printf("\nMemory Dump (Thread 1 Data Section, 1000-1059):\n");
        mem_dump(&test_memory, THREAD_DATA_START(1), 60);

        printf("\nMemory Dump (Thread 1 Instruction Section, 1256-1270):\n");
        mem_dump(&test_memory, THREAD_INSTR_START(1), 15);

        // --- Specific Value Checks for OS ---
        bool data_check_pc = (mem_read(&test_memory, REG_PC, KERNEL) == 0);
        print_test_result("OS Data Section: REG_PC value (0)", data_check_pc);
        if (!data_check_pc) overall_test_status = false;

        bool data_check_sp = (mem_read(&test_memory, REG_SP, KERNEL) == (long int)MEM_SIZE - 1);
        print_test_result("OS Data Section: REG_SP value (MEM_SIZE-1)", data_check_sp);
        if (!data_check_sp) overall_test_status = false;

        // --- Specific Value Checks for Thread 1 ---
        bool data_check_1050 = (mem_read(&test_memory, 1000 + 50, KERNEL) == 100);
        print_test_result("Thread 1 Data Section: Address 1050 value (100)", data_check_1050);
        if (!data_check_1050) overall_test_status = false;

        bool data_check_1051 = (mem_read(&test_memory, 1000 + 51, KERNEL) == 200);
        print_test_result("Thread 1 Data Section: Address 1051 value (200)", data_check_1051);
        if (!data_check_1051) overall_test_status = false;

        bool data_check_1052 = (mem_read(&test_memory, 1000 + 52, KERNEL) == -5);
        print_test_result("Thread 1 Data Section: Address 1052 value (-5)", data_check_1052);
        if (!data_check_1052) overall_test_status = false;

        long int instr_base = THREAD_INSTR_START(1); // 1256 for Thread 1
        long int instr_size = INSTR_SIZE; // 1 opcode + 2 operands = 3

        // Command 0: CPY 50 600 (Thread 1 relative addresses: 1050 to 1600)
        bool instr0_op = (mem_read(&test_memory, instr_base + 0 * instr_size, KERNEL) == (long int)OPCODE_CPY);
        bool instr0_p1 = (mem_read(&test_memory, instr_base + 0 * instr_size + 1, KERNEL) == 50);
        bool instr0_p2 = (mem_read(&test_memory, instr_base + 0 * instr_size + 2, KERNEL) == 600);
        print_test_result("Thread 1 Command 0: CPY 50 600", instr0_op && instr0_p1 && instr0_p2);
        if (!(instr0_op && instr0_p1 && instr0_p2)) overall_test_status = false;

        // Command 1: ADDI 600 51 (Thread 1 relative addresses: 1600 and 1051)
        bool instr1_op = (mem_read(&test_memory, instr_base + 1 * instr_size, KERNEL) == (long int)OPCODE_ADDI);
        bool instr1_p1 = (mem_read(&test_memory, instr_base + 1 * instr_size + 1, KERNEL) == 600);
        bool instr1_p2 = (mem_read(&test_memory, instr_base + 1 * instr_size + 2, KERNEL) == 51);
        print_test_result("Thread 1 Command 1: ADDI 600 51", instr1_op && instr1_p1 && instr1_p2);
        if (!(instr1_op && instr1_p1 && instr1_p2)) overall_test_status = false;

        // Command 2: SYSCALL PRN 600 (Thread 1 relative address: 1600)
        bool instr2_op = (mem_read(&test_memory, instr_base + 2 * instr_size, KERNEL) == (long int)OPCODE_SYSCALL_PRN);
        bool instr2_p1 = (mem_read(&test_memory, instr_base + 2 * instr_size + 1, KERNEL) == 600);
        bool instr2_p2 = (mem_read(&test_memory, instr_base + 2 * instr_size + 2, KERNEL) == 0);
        print_test_result("Thread 1 Command 2: SYSCALL PRN 600", instr2_op && instr2_p1 && instr2_p2);
        if (!(instr2_op && instr2_p1 && instr2_p2)) overall_test_status = false;

        // Command 3: HLT
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