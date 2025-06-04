/* tests/os_debug_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpu.h>
#include <memory.h>
#include <parser.h>

int main(void) {
	printf("=== OS Debug Test ===\n\n");
	
	// Initialize memory
	Memory mem;
	mem_init(&mem);
	
	// Load OS and threads from file
	const char *asm_file = "programs/os.asm";
	printf("Loading program from: %s\n", asm_file);
	
	if (load_program_from_file(asm_file, &mem) != 0) {
		fprintf(stderr, "Failed to load assembly file: %s\n", asm_file);
		mem_free(&mem);
		return EXIT_FAILURE;
	}
	
	printf("Program loaded successfully!\n\n");
	
	// Check OS data section
	printf("=== OS Data Section Check ===\n");
	printf("M[%d] (offset 100, current_thread_id): %ld\n", 
		   OS_DATA_START_ADDR + 100, 
		   mem_read(&mem, OS_DATA_START_ADDR + 100, KERNEL));
	
	// Check thread table at offset 200
	printf("\n=== Thread Table (at OS data offset 200) ===\n");
	for (int i = 0; i <= 4; i++) {
		long int base = OS_DATA_START_ADDR + 200 + (i * 8);
		printf("Thread %d (addr %ld-%ld):\n", i, base, base + 7);
		for (int j = 0; j < 8; j++) {
			printf("  [%ld]: %ld\n", base + j, mem_read(&mem, base + j, KERNEL));
		}
	}
	
	// Check OS instructions
	printf("\n=== OS Instructions (first 5) ===\n");
	for (int i = 0; i < 5; i++) {
		long int addr = OS_INSTRUCTION_START_ADDR + (i * 3);
		long int opcode = mem_read(&mem, addr, KERNEL);
		long int op1 = mem_read(&mem, addr + 1, KERNEL);
		long int op2 = mem_read(&mem, addr + 2, KERNEL);
		printf("Instr %d (addr %ld): opcode=%ld, op1=%ld, op2=%ld\n", 
			   i, addr, opcode, op1, op2);
	}
	
	// Initialize CPU
	CPU cpu;
	cpu_init(&cpu, &mem);
	
	printf("\n=== Initial CPU State ===\n");
	printf("Mode: %s\n", cpu.mode == KERNEL ? "KERNEL" : "USER");
	printf("Thread ID: %d\n", cpu.curr_thread_id);
	printf("Data Base: %ld\n", cpu.curr_data_base_for_active_entity);
	printf("Instruction Base: %ld\n", cpu.curr_instruction_base_for_active_entity);
	cpu_dump_registers(&cpu);
	
	// Execute first few instructions with detailed debug
	printf("\n=== Executing First Instructions ===\n");
	for (int i = 0; i < 3 && !cpu_is_halted(&cpu); i++) {
		long int pc = mem_read(&mem, REG_PC, KERNEL);
		long int sp = mem_read(&mem, REG_SP, KERNEL);
		printf("\nBefore instruction %d: PC=%ld, SP=%ld\n", i, pc, sp);
		
		// Check what instruction we're about to execute
		long int opcode = mem_read(&mem, pc, KERNEL);
		long int op1 = mem_read(&mem, pc + 1, KERNEL);
		long int op2 = mem_read(&mem, pc + 2, KERNEL);
		printf("About to execute: opcode=%ld, op1=%ld, op2=%ld\n", opcode, op1, op2);
		
		// Special check for CALL
		if (opcode == OPCODE_CALL) {
			printf("CALL instruction detected. Target: instruction %ld (address %ld)\n", 
				   op1, OS_INSTRUCTION_START_ADDR + op1 * 3);
			printf("Stack check: SP=%ld, Stack bound=%d\n", sp, OS_BLOCK_END_ADDR);
		}
		
		cpu_execute_instruction(&cpu);
		
		pc = mem_read(&mem, REG_PC, KERNEL);
		sp = mem_read(&mem, REG_SP, KERNEL);
		printf("After instruction %d: PC=%ld, SP=%ld\n", i, pc, sp);
	}
	
	// Cleanup
	mem_free(&mem);
	return EXIT_SUCCESS;
}