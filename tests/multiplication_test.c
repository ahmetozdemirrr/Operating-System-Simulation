/* tests/multiplication_test.c */

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
	const char * asm_file = "tests/test_source/mult_by_addition.asm";

	if (load_program_from_file(asm_file, &mem) != 0) {
		fprintf(stderr, "Failed to load assembly file: %s\n", asm_file);
		mem_free(&mem);
		return EXIT_FAILURE;
	}

	/* Initialize CPU */
	CPU cpu;
	cpu_init(&cpu, &mem);

	/* Starting in OS mode */
	printf("Starting Multiplication by Repeated Addition Test...\n");
	long int thread1_data_base = THREAD_DATA_START(1);

	long int operand1 = mem_read(&mem, thread1_data_base + 0, KERNEL); // M[0]
	long int operand2 = mem_read(&mem, thread1_data_base + 1, KERNEL); // M[1]
	long int expected_result = operand1 * operand2;

	printf("Multiplying: %ld * %ld\n", operand1, operand2);
	printf("Expected result: %ld\n\n", expected_result);

	/* Debug information */
	printf("Memory Layout:\n");
	printf("- OS Data: %d-%d\n", OS_DATA_START_ADDR, OS_DATA_END_ADDR);
	printf("- OS Instructions: %d-%d\n", OS_INSTRUCTION_START_ADDR, OS_INSTRUCTION_END_ADDR);
	printf("- Thread 1 Data: %ld-%ld\n", (long int)THREAD_DATA_START(1), (long int)THREAD_DATA_END(1));
	printf("- Thread 1 Instructions: %ld-%ld\n", (long int)THREAD_INSTR_START(1), (long int)THREAD_INSTR_END(1));
	printf("\n");

	/* Execution loop */
	int instruction_count = 0;
	int max_instructions = 500000000;
	int os_instructions = 0;
	int thread_instructions = 0;

	while (!cpu_is_halted(&cpu) && instruction_count < max_instructions)
	{
		/* Display current mode and relevant data for Thread 1 */
		if (cpu.curr_thread_id == 1 && (instruction_count % 10 == 0 || instruction_count < 20)) {
			printf("[%d] Mode: %s, Thread: %d, PC: %ld, ",
				   instruction_count,
				   cpu.mode == KERNEL ? "KERNEL" : "USER",
				   cpu.curr_thread_id,
				   mem_read(&mem, REG_PC, KERNEL));
			printf("i(M[2]): %ld, sum_buffer(M[3]): %ld, N(M[4]): %ld, temp_i(M[5]): %ld, N-i(M[101]): %ld\n",
				   mem_read(&mem, thread1_data_base + 2, cpu.mode),
				   mem_read(&mem, thread1_data_base + 3, cpu.mode),
				   mem_read(&mem, thread1_data_base + 4, cpu.mode),
				   mem_read(&mem, thread1_data_base + 5, cpu.mode),
				   mem_read(&mem, thread1_data_base + 101, cpu.mode));
		}

		else if (instruction_count < 5) {
			 printf("[%d] Mode: %s, Thread: %d, PC: %ld\n",
				   instruction_count,
				   cpu.mode == KERNEL ? "KERNEL" : "USER",
				   cpu.curr_thread_id,
				   mem_read(&mem, REG_PC, KERNEL));
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
	long int result = mem_read(&mem, thread1_data_base + 3, KERNEL);
	printf("\nCalculated result (sum_buffer M[3]): %ld\n", result);

	/* Verify result */
	int test_passed = 0;
	if (result == expected_result) {
		printf("TEST PASSED: Multiplication result is correct! (%ld)\n", result);
		test_passed = 1;
	} else {
		fprintf(stderr, "TEST FAILED: Expected result %ld, got %ld\n", expected_result, result);
	}
	/* Memory dumps */
	printf("\nMemory dump (Thread 1 Data Section - first 10 + temps):\n");
	mem_dump(&mem, thread1_data_base, 10); // İlk 10 (0-9)
	mem_dump(&mem, thread1_data_base + 100, 2); // temp değişkenler (100-101)

	/* Register state */
	printf("\nFinal CPU state:\n");
	cpu_dump_registers(&cpu);

	/* Free memory */
	mem_free(&mem);

	return test_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
