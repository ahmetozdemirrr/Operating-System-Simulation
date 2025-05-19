/* include/common.h */

#ifndef COMMON_H
#define COMMON_H

#define _POSIX_C_SOURCE 200809L /* for POSIX calls */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

/* memory macros */
#define MEM_SIZE 16384 
#define REG_PC 0 /* program counter */
#define REG_SP 1 /* stack pointer */
#define REG_SYSCALL_RESULT 2
#define REG_INSTR_COUNT 3
#define REG_RESERVED_START 4
#define REG_RESERVED_END 19

/* OS and Thread Layout Macros */
#define OS_ID 0
#define MAX_THREADS 10
#define MAX_PROGRAM_ENTITIES 11
#define ENTITY_DATA_SIZE 256
#define ENTITY_INSTRUCTION_OFFSET ENTITY_DATA_SIZE
#define ENTITY_BLOCK_SIZE_EXCEPT_OS 1000

/* OS specific layout */
#define OS_DATA_START_ADDR (REG_RESERVED_END + 1)
#define OS_DATA_END_ADDR (OS_DATA_START_ADDR + ENTITY_DATA_SIZE - 1)
#define OS_INSTRUCTION_START_ADDR (OS_DATA_START_ADDR + ENTITY_DATA_SIZE)
#define OS_BLOCK_END_ADDR 999

/* user thread spscific layout base */
#define USER_THREAD_START_BASE_ADDR 1000

/* Thread address calculation macros */
#define THREAD_BASE_ADDR(thread_id) (USER_THREAD_START_BASE_ADDR + ((thread_id) - 1) * ENTITY_BLOCK_SIZE_EXCEPT_OS)
#define THREAD_DATA_START(thread_id) (THREAD_BASE_ADDR(thread_id))
#define THREAD_DATA_END(thread_id) (THREAD_BASE_ADDR(thread_id) + ENTITY_DATA_SIZE - 1)
#define THREAD_INSTR_START(thread_id) (THREAD_BASE_ADDR(thread_id) + ENTITY_INSTRUCTION_OFFSET)
#define THREAD_BLOCK_END(thread_id) (THREAD_BASE_ADDR(thread_id) + ENTITY_BLOCK_SIZE_EXCEPT_OS - 1)

/* parser macros */
#define MAX_OPERANDS 2
#define ASM_LINE_BUFFER_SIZE 64
#define INSTR_SIZE (1 + MAX_OPERANDS) /* opcode + operands */

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
