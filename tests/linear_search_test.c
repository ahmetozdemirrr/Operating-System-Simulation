/* tests/linear_search_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpu.h>
#include <memory.h>
#include <parser.h>

int main(void)
{
	/* Initialize memory */
	Memory mem;
	mem_init(&mem);

	/* Read assembly file and load into memory */
	const char * asm_file = "tests/test_source/linear_search.asm";

	if (load_program_from_file(asm_file, &mem) != 0) {
		fprintf(stderr, "Failed to load assembly file: %s\n", asm_file);
		mem_free(&mem);
		return EXIT_FAILURE;
	}

	/* Initialize CPU */
	CPU cpu;
	cpu_init(&cpu, &mem);

	/* Starting in OS mode */
	printf("Starting Linear Search Test...\n");
	long int thread1_data_base = THREAD_DATA_START(1);

	printf("Initial array in Thread 1: ");
	for (int i = 0; i < 10; i++) {
		printf("%ld ", mem_read(&mem, thread1_data_base + i, KERNEL));
	}
	printf("\n");
	printf("Searching for: %ld\n", mem_read(&mem, thread1_data_base + 11, KERNEL));
	printf("Expected result: 5 (index of -345)\n\n");

	/* Debug information */
	printf("Memory Layout:\n");
	printf("- OS Data: %d-%d\n", OS_DATA_START_ADDR, OS_DATA_END_ADDR);
	printf("- OS Instructions: %d-%d\n", OS_INSTRUCTION_START_ADDR, OS_INSTRUCTION_END_ADDR);
	printf("- Thread 1 Data: %ld-%ld\n", THREAD_DATA_START(1), THREAD_DATA_END(1));
	printf("- Thread 1 Instructions: %ld-%ld\n", THREAD_INSTR_START(1), THREAD_INSTR_END(1));
	printf("\n");

	/* Execution loop */
	int instruction_count = 0;
	int max_instructions = 200;
	int os_instructions = 0;
	int thread_instructions = 0;

	while (!cpu_is_halted(&cpu) && instruction_count < max_instructions)
	{
		/* Display current mode */
		if (instruction_count < 10 || (instruction_count % 30) == 0) {
			printf("[%d] Mode: %s, Thread: %d, PC: %ld\n",
				   instruction_count,
				   cpu.mode == KERNEL ? "KERNEL" : "USER",
				   cpu.curr_thread_id,
				   mem_read(&mem, REG_PC, KERNEL));

			if (cpu.curr_thread_id == 1) {
				printf("Current i: %ld, result: %ld\n",
					   mem_read(&mem, thread1_data_base + 13, cpu.mode),
					   mem_read(&mem, thread1_data_base + 12, cpu.mode));
			}
		}

		/* Track counters based on mode */
		if (cpu.mode == KERNEL) {
			os_instructions++;
		} else {
			thread_instructions++;
		}
		cpu_execute_instruction(&cpu);
		instruction_count++;
	}

	if (instruction_count >= max_instructions) {
		fprintf(stderr, "\nWARNING: Execution stopped after %d instructions (possible infinite loop)\n", max_instructions);
	}
	printf("\nExecution Summary:\n");
	printf("- Total instructions: %d\n", instruction_count);
	printf("- OS instructions: %d\n", os_instructions);
	printf("- Thread instructions: %d\n", thread_instructions);

	/* Check results */
	long int result = mem_read(&mem, thread1_data_base + 12, KERNEL);
	printf("\nSearch result: %ld\n", result);

	/* Verify result */
	int test_passed = 0;
	if (result == 5) {
		printf("TEST PASSED: Found correct index (5) for value -345!\n");
		test_passed = 1;
	} else {
		fprintf(stderr, "TEST FAILED: Expected index 5, got %ld\n", result);
	}
	/* Memory dumps */
	printf("\nMemory dump (Thread 1 Data Section - first 20):\n");
	mem_dump(&mem, thread1_data_base, 20);

	/* Register state */
	printf("\nFinal CPU state:\n");
	cpu_dump_registers(&cpu);

	/* Free memory */
	mem_free(&mem);

	return test_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
