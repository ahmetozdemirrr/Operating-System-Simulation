/* include/memory.h */

#ifndef MEMORY_H
#define MEMORY_H

#include <common.h>

/*
 * Fixed size Memory simulation:
 * 
 * - data: each memory cell
 * - sie: size of memory
 * - is_initialized: state of memory 
 * 
 */
typedef struct
{
	long int data[MEM_SIZE];
	int size;
	bool is_initialized;
}
Memory;

void mem_init(Memory * mem);
void mem_free(Memory * mem);
void mem_write(Memory * mem, long int address, long int value, CPU_mode mode);
long int mem_read(Memory * mem, long int address, CPU_mode mode);
void mem_dump(const Memory * mem, int start_addr, int count);

#endif /* MEMORY_H */
