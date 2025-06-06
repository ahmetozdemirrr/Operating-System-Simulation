/* include/cpu.h */

#ifndef CPU_H
#define CPU_H

#include <common.h>
#include <memory.h>
#include <limits.h>

/* External variable declarations for program type detection */
extern bool is_multi_section_program;
extern bool is_os_program;
extern bool is_single_thread_program;

typedef struct
{
	Memory * mem;
	CPU_mode mode; /* USER - KERNEL */
	bool is_halted; /* is halted with HLT instruction */
	int curr_thread_id;
	long int curr_data_base_for_active_entity;
	long int curr_instruction_base_for_active_entity;
	/* Pending print values for delayed SYSCALL PRN */
	long int pending_print_values[MAX_PROGRAM_ENTITIES];
	bool has_pending_print[MAX_PROGRAM_ENTITIES];
	/* Debug level for logging */
	int debug_level;
	/* Program type - true if OS, false if single thread */
	bool is_os_program;
}
CPU;

void cpu_init(CPU * cpu, Memory * mem, int debug_level);
void cpu_execute_instruction(CPU * cpu);
bool cpu_is_halted(const CPU * cpu);
void cpu_dump_registers(const CPU * cpu);

#endif /* CPU_H */
