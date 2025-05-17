/* include/parser.h */

#ifndef PARSER_H
#define PARSER_H

#include <memory.h>
#include <common.h>

#include <ctype.h>

/**
 * mnemonics:
 * 
 * - SET	- ADDI 	- POP 	- USER
 * - CPY 	- SUBI 	- CALL 	- SYSCALL PRN
 * - CPYI 	- JIF 	- RET 	- SYSCALL HLT
 * - ADD 	- PUSH 	- HLT	- SYSCALL YIELD
 * 
 */
typedef struct 
{
    /**
     *  it is a data or a instruction, if data then:
     * 
     * - data_address
     * - data_value
     * 
     *  else:
     * 
     * - instruction_label
     * - operands[]
     * - mnemonic
     * - num_operands
     * 
     */
    bool is_data_definition;
    long int data_address;
    long int data_value;

    long int instruction_label;
    long int operands[MAX_OPERANDS];
    char mnemonic[MAX_MNEMONIC_LENGTH];
    int num_operands;
}
Parsed_line;

int load_program_from_file(const char * filename, Memory * mem);
static int parse_data_line(const char * line, Memory * mem);
static int parse_insr_line(const char * line, Memory * mem, int curr_insr_index);
static void remove_comment(const char * filename);
static char * trim_whitespace(char * str);

#endif /* PARSER_H */
