/* include/cpu.h */

#ifndef CPU_H
#define CPU_H

#include <common.h>
#include <memory.h>
#include <limits.h>

typedef struct
{
	Memory * mem;
	CPU_mode mode; /* USER - KERNEL */
	bool is_halted; /* is halted with HLT instruction */
	int curr_thread_id;
	long int curr_data_base_for_active_entity;
	long int curr_instruction_base_for_active_entity;
}
CPU;

void cpu_init(CPU * cpu, Memory * mem);
void cpu_execute_instruction(CPU * cpu);
bool cpu_is_halted(const CPU * cpu);
void cpu_dump_registers(const CPU * cpu);
void cpu_set_context(CPU * cpu, int thread_id, 
					long int data_base, 
					long int instruction_base, 
					CPU_mode initial_mode);

#endif /* CPU_H */
