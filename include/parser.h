/* include/parser.h */

#ifndef PARSER_H
#define PARSER_H

#include <memory.h>
#include <common.h>

#include <ctype.h>
/* #include <unistd.h> for strdup func. */

/**
 * mnemonics:
 * 
 * - SET	- ADDI 	- POP 	- USER
 * - CPY 	- SUBI 	- CALL 	- SYSCALL PRN
 * - CPYI 	- JIF 	- RET 	- SYSCALL HLT
 * - ADD 	- PUSH 	- HLT	- SYSCALL YIELD
 * 
 */

typedef enum
{
	INITIAL_CONTEXT,
	INSTRUCTION_CONTEXT,
	DATA_CONTEXT,
	ERROR_CONTEXT
}
Parser_State;

int load_program_from_file(const char * filename, Memory * mem);

#endif /* PARSER_H */
