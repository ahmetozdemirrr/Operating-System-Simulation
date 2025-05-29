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
#define MEM_SIZE 22000 /* 2000 for each thread even OS (32768) */
#define REG_PC 0 /* program counter */
#define REG_SP 1 /* stack pointer */
#define REG_SYSCALL_RESULT 2
#define REG_INSTR_COUNT 3
#define REG_RESERVED_START 4
#define REG_RESERVED_END 20

/* OS and Thread Layout Macros */
#define OS_ID 0
#define MAX_THREADS 10
#define MAX_PROGRAM_ENTITIES 11

/* Entity size macros */
#define ENTITY_DATA_SIZE 1000
#define ENTITY_INSTRUCTION_SIZE 1000
#define ENTITY_TOTAL_SIZE 2000  /* data + instruction */

/* OS specific sizes */
#define OS_USABLE_DATA_SIZE 980  /* 1000 - 20 (registers) */
#define OS_TOTAL_SIZE 1980  /* 20 (registers) + 980 (data) + 1000 (instruction) */

/* OS specific layout */
#define OS_DATA_START_ADDR 20  /* Register'lardan hemen sonra */
#define OS_DATA_END_ADDR 999   /* 20 + 980 - 1 */
#define OS_INSTRUCTION_START_ADDR 1000  /* Data'dan hemen sonra */
#define OS_INSTRUCTION_END_ADDR 1999    /* 1000 + 1000 - 1 */
#define OS_BLOCK_END_ADDR OS_INSTRUCTION_END_ADDR
#define OS_SYSCALL_HANDLER_ADDR OS_INSTRUCTION_START_ADDR

/* User thread layout */
#define USER_THREAD_START_BASE_ADDR 2000  /* OS'tan hemen sonra */

/* Thread address calculation macros */
#define THREAD_BASE_ADDR(thread_id) (USER_THREAD_START_BASE_ADDR + ((thread_id) - 1) * ENTITY_TOTAL_SIZE)
#define THREAD_DATA_START(thread_id) (THREAD_BASE_ADDR(thread_id))
#define THREAD_DATA_END(thread_id) (THREAD_DATA_START(thread_id) + ENTITY_DATA_SIZE - 1)
#define THREAD_INSTR_START(thread_id) (THREAD_DATA_END(thread_id) + 1)
#define THREAD_INSTR_END(thread_id) (THREAD_INSTR_START(thread_id) + ENTITY_INSTRUCTION_SIZE - 1)
#define THREAD_BLOCK_END(thread_id) THREAD_INSTR_END(thread_id)

/* Backward compatibility */
#define ENTITY_INSTRUCTION_OFFSET ENTITY_DATA_SIZE
#define ENTITY_BLOCK_SIZE_EXCEPT_OS ENTITY_TOTAL_SIZE
#define ENTITY_INSTR_SIZE ENTITY_INSTRUCTION_SIZE

/* parser macros */
#define MAX_OPERANDS 2
#define ASM_LINE_BUFFER_SIZE 256
#define INSTR_SIZE (1 + MAX_OPERANDS) /* opcode + operands */

/* SYSCALL macros */
#define SYSCALL_PRN_ID 1
#define SYSCALL_HLT_ID 2
#define SYSCALL_YIELD_ID 3

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
	OPCODE_CPYI2,
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
