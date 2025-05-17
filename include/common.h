/* include/common.h */

#ifndef COMMON_H
#define COMMON_H

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* memory macros */
#define MEM_SIZE 16384 
#define REG_PC 0 /* program counter */
#define REG_SP 1 /* stack pointer */
#define REG_SYSCALL_RESULT 2
#define REG_INSTR_COUNT 3
#define REG_RESERVED_START 4
#define REG_RESERVED_END 20

/* parser macros */
#define MAX_OPERANDS 2
#define MAX_MNEMONIC_LENGTH 16
#define ASM_LINE_BUFFER_SIZE 64
#define INSTRUCTION_AREA_START 200

typedef enum
{
	KERNEL,
	USER
}
CPU_mode;

typedef enum 
{
    OPCODE_SET = 0,
    OPCODE_CPY,
    OPCODE_CPYI,
    OPCODE_ADD,
    OPCODE_ADDI,
    OPCODE_SUBI,
    OPCODE_JIF,
    OPCODE_PUSH,
    OPCODE_POP,
    OPCODE_CALL,
    OPCODE_RET,
    OPCODE_HLT,
    OPCODE_USER,

    OPCODE_SYSCALL_PRN,
    OPCODE_SYSCALL_HLT,
    OPCODE_SYSCALL_YIELD,

    OPCODE_UNKNOWN
} 
Opcode;

#endif /* COMMON_H */