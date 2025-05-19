/* src/memory.c */

#include <memory.h>
#include <common.h>

/**
 * @brief Initializes the memory structure with a given size.
 *
 * This function sets up the memory structure by allocating memory for the data array,
 * initializing it to zero, and setting the stack pointer to the top of the memory.
 * It also marks the memory as initialized and sets the kernel mode flag.
 * Note that the size parameter is currently ignored, and MEM_SIZE is used instead.
 *
 * @param mem Pointer to the Memory structure to initialize.
 * @param size Requested size of the memory (currently ignored, uses MEM_SIZE).
 */
void 
mem_init(Memory * mem)
{
	if (mem == NULL) {
		fprintf(stderr, "ERROR: Memory is not a valid pointer (mem_init)\n");
	}
	mem->size = MEM_SIZE;
	memset(mem->data, 0, MEM_SIZE * sizeof(long int));
	mem->is_initialized = true;
}

/**
 * @brief Deallocates the memory structure.
 *
 * This function marks the memory as uninitialized, effectively freeing it for future
 * reinitialization. It does not free the data array, assuming it is statically allocated
 * or managed externally. This is a lightweight cleanup operation.
 *
 * @param mem Pointer to the Memory structure to free.
 */
void 
mem_free(Memory * mem)
{
	if (mem == NULL) {
		fprintf(stderr, "ERROR: Memory not initialized, cannot free...\n");
	}
	mem->is_initialized = false;
}

/**
 * @brief Reads a value from a specified memory address.
 *
 * This function retrieves the value stored at the given memory address, performing
 * necessary checks for memory initialization, address validity, and mode-based access
 * restrictions. In user mode, access to addresses 0-999 is prohibited. If any check fails,
 * an error is printed, and the program exits.
 *
 * @param mem Pointer to the Memory structure.
 * @param address The memory address to read from.
 * @param curr_mode Current CPU mode (KERNEL or USER).
 * @return The value stored at the specified address.
 */
long int 
mem_read(Memory * mem, long int address, CPU_mode curr_mode)
{
	if (!mem->is_initialized || !mem) {
		fprintf(stderr, "ERROR: Memory not initialized\n");
		exit(EXIT_FAILURE);
	}

	if (address < 0 || address >= mem->size) {
		fprintf(stderr, "ERROR: Invalid memory read address: %ld\n", address);
		exit(EXIT_FAILURE);
	}

	if (address < 1000 && curr_mode == USER) {
		fprintf(stderr, "ERROR: User Mode Protection Violation! Address %ld (range 0-999) is not accessible for reading.\n", address);
		exit(EXIT_FAILURE);
	}
	return mem->data[address];
}

/**
 * @brief Writes a value to a specified memory address.
 *
 * This function stores the provided value at the given memory address, performing
 * checks for memory initialization, address validity, and mode-based access restrictions.
 * In user mode, access to addresses 0-999 is prohibited. If any check fails, an error
 * is printed, and the program exits.
 *
 * @param mem Pointer to the Memory structure.
 * @param address The memory address to write to.
 * @param value The value to write.
 * @param curr_mode Current CPU mode (KERNEL or USER).
 */
void 
mem_write(Memory * mem, long int address, long int value, CPU_mode curr_mode)
{
	if (!mem->is_initialized || !mem) {
		fprintf(stderr, "ERROR: Memory not initialized\n");
		exit(EXIT_FAILURE);
	}

	if (address < 0 || address >= mem->size) {
		fprintf(stderr, "ERROR: Invalid memory write address: %ld\n", address);
		exit(EXIT_FAILURE);
	}

	if (address < 1000 && curr_mode == USER) {
		fprintf(stderr, "ERROR: User Mode Protection Violation! Address %ld (range 0-999) is not accessible for writing.\n", address);
		exit(EXIT_FAILURE);
	}
	mem->data[address] = value;
}

/**
 * @brief Dumps the contents of memory to stderr for debugging.
 *
 * This function prints a specified range of memory contents starting from the given
 * address, with annotations for special registers (e.g., PC, SP). It performs validation
 * checks on the memory pointer, initialization status, start address, and count. If
 * the requested range exceeds memory limits, it adjusts the count to stay within bounds.
 * The output is formatted with 8 values per line, including special register labels.
 *
 * @param mem Pointer to the Memory structure.
 * @param start_addr Starting address for the dump.
 * @param count Number of memory locations to dump.
 */
void 
mem_dump(const Memory * mem, int start_addr, int count)
{
	if (mem == NULL) {
		fprintf(stderr, "ERROR (mem_dump): Invalid memory pointer (NULL)!\n");
		return;
	}

	if (!mem->is_initialized) {
		fprintf(stderr, "WARNING (mem_dump): Memory not initialized. Dump might be inaccurate.\n");
	}

	if (start_addr < 0) {
		fprintf(stderr, "WARNING (mem_dump): Start address (%d) cannot be negative. Setting to 0.\n", start_addr);
		start_addr = 0;
	}

	if (start_addr >= mem->size) {
		fprintf(stderr, "WARNING (mem_dump): Start address (%d) exceeds memory size (%d). Cannot dump.\n", start_addr, mem->size);
		return;
	}

	if (count <= 0) {
		fprintf(stderr, "WARNING (mem_dump): Element count for dump (%d) must be positive. Not dumping.\n", count);
		return;
	}

	if (start_addr + count > mem->size) {
		fprintf(stderr, "WARNING (mem_dump): Requested dump range exceeds memory limits. "
						"Dumping only the valid portion.\n");
		count = mem->size - start_addr;
	}
	fprintf(stderr, "\n--- Memory Dump (Starting from Address %d, %d elements) ---\n", start_addr, count);

	int values_per_line = 8;

	for (int i = 0; i < count; ++i)
	{
		int current_addr = start_addr + i;

		if (i % values_per_line == 0) {
			if (i > 0) {
				fprintf(stderr, "\n");
			}
			fprintf(stderr, "[%05d]: ", current_addr);
		}
		fprintf(stderr, "%11ld ", mem->data[current_addr]);

		if      (current_addr == REG_PC) fprintf(stderr, "(PC) ");
		else if (current_addr == REG_SP) fprintf(stderr, "(SP) ");
		else if (current_addr == REG_SYSCALL_RESULT) fprintf(stderr, "(SYS_RES) ");
		else if (current_addr == REG_INSTR_COUNT) fprintf(stderr, "(INSTR_CNT) ");
		else if (current_addr >= REG_RESERVED_START && current_addr <= REG_RESERVED_END) fprintf(stderr, "(RES) ");
	}
	fprintf(stderr, "\n--- End of Memory Dump ---\n\n");
}
