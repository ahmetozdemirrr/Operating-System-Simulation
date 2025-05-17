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

        printf("\nMemory Dump (Data Section):\n");
        mem_dump(&test_memory, 0, 10);
        mem_dump(&test_memory, 48, 10);

        printf("\nMemory Dump (Instruction Section - INSTRUCTION_AREA_START assumption):\n");
        mem_dump(&test_memory, INSTRUCTION_AREA_START, 15);

        // --- Specific Value Checks ---
        bool data_check_pc = (mem_read(&test_memory, REG_PC, KERNEL) == 0);
        print_test_result("Data Section: REG_PC value (0)", data_check_pc);
        if (!data_check_pc) overall_test_status = false;

        bool data_check_sp = (mem_read(&test_memory, REG_SP, KERNEL) == (long int)MEM_SIZE - 1); // as in test_parser.asm
        print_test_result("Data Section: REG_SP value (MEM_SIZE-1)", data_check_sp);
        if (!data_check_sp) overall_test_status = false;
        
        bool data_check_50 = (mem_read(&test_memory, 50, KERNEL) == 100);
        print_test_result("Data Section: Address 50 value (100)", data_check_50);
        if (!data_check_50) overall_test_status = false;

        bool data_check_51 = (mem_read(&test_memory, 51, KERNEL) == 200);
        print_test_result("Data Section: Address 51 value (200)", data_check_51);
        if (!data_check_51) overall_test_status = false;

        bool data_check_52 = (mem_read(&test_memory, 52, KERNEL) == -5);
        print_test_result("Data Section: Address 52 value (-5)", data_check_52);
        if (!data_check_52) overall_test_status = false;

        long int instr_base = INSTRUCTION_AREA_START;
        long int instr_size = 1 + MAX_OPERANDS;

        // Command 0: CPY 50 600
        bool instr0_op = (mem_read(&test_memory, instr_base + 0 * instr_size, KERNEL) == (long int)OPCODE_CPY);
        bool instr0_p1 = (mem_read(&test_memory, instr_base + 0 * instr_size + 1, KERNEL) == 50);
        bool instr0_p2 = (mem_read(&test_memory, instr_base + 0 * instr_size + 2, KERNEL) == 600);
        print_test_result("Command 0: CPY 50 600", instr0_op && instr0_p1 && instr0_p2);
        if (!(instr0_op && instr0_p1 && instr0_p2)) overall_test_status = false;

        // Command 1: ADDI 600 51
        bool instr1_op = (mem_read(&test_memory, instr_base + 1 * instr_size, KERNEL) == (long int)OPCODE_ADDI);
        bool instr1_p1 = (mem_read(&test_memory, instr_base + 1 * instr_size + 1, KERNEL) == 600);
        bool instr1_p2 = (mem_read(&test_memory, instr_base + 1 * instr_size + 2, KERNEL) == 51);
        print_test_result("Command 1: ADDI 600 51", instr1_op && instr1_p1 && instr1_p2);
        if (!(instr1_op && instr1_p1 && instr1_p2)) overall_test_status = false;

        // Command 2: SYSCALL PRN 600
        bool instr2_op = (mem_read(&test_memory, instr_base + 2 * instr_size, KERNEL) == (long int)OPCODE_SYSCALL_PRN);
        bool instr2_p1 = (mem_read(&test_memory, instr_base + 2 * instr_size + 1, KERNEL) == 600);
        bool instr2_p2 = (mem_read(&test_memory, instr_base + 2 * instr_size + 2, KERNEL) == 0);
        print_test_result("Command 2: SYSCALL PRN 600", instr2_op && instr2_p1 && instr2_p2);
        if (!(instr2_op && instr2_p1 && instr2_p2)) overall_test_status = false;

        // Command 3: HLT
        bool instr3_op = (mem_read(&test_memory, instr_base + 3 * instr_size, KERNEL) == (long int)OPCODE_HLT);
        bool instr3_p1 = (mem_read(&test_memory, instr_base + 3 * instr_size + 1, KERNEL) == 0);
        bool instr3_p2 = (mem_read(&test_memory, instr_base + 3 * instr_size + 2, KERNEL) == 0);
        print_test_result("Command 3: HLT", instr3_op && instr3_p1 && instr3_p2);
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
