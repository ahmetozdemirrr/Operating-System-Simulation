/* src/cpu.c */

#include <cpu.h>
#include <memory.h>

#define CX_DEBUG_FLAG

/**
 * @brief Calculates the memory address for a specific thread table field.
 *
 * This function computes the absolute memory address for a given thread ID and field offset
 * in the thread table, which starts at address 220. Each thread entry occupies 8 long integers.
 *
 * @param thread_id The ID of the thread.
 * @param field The field offset within the thread table entry (0 to 7).
 * @return The absolute memory address for the specified thread table field.
 */
static long int
get_thread_table_addr(int thread_id, int field)
{
    return 220 + (thread_id * 8) + field;  /* Thread table starts from 220 (absolute) */
}

/**
 * @brief Dumps the thread table contents to stderr for debugging.
 *
 * This function prints the thread table starting at address 220, showing each thread's
 * ID, state, program counter (PC), stack pointer (SP), data base, instruction base,
 * instruction count (IC), and wakeup count. It is only executed if the CPU debug level
 * is 3 or higher.
 *
 * @param cpu Pointer to the CPU structure containing the thread table.
 */
static void
dump_thread_table(const CPU * cpu)
{
    if (cpu->debug_level < 3) return; /* Only dump if debug level 3+ */

    fprintf(stderr, "\n=== THREAD TABLE DUMP ===\n");
    fprintf(stderr, "Thread Table Base Address: 220\n");
    fprintf(stderr, "Format: [ID] [State] [PC] [SP] [DataBase] [InstrBase] [IC] [WakeupCount]\n");
    fprintf(stderr, "States: 0=READY, 1=RUNNING, 2=BLOCKED, 3=TERMINATED\n\n");

    for (int tid = 0; tid <= MAX_THREADS; tid++) {
        long int id_addr = get_thread_table_addr(tid, 0);
        long int state_addr = get_thread_table_addr(tid, 1);
        long int pc_addr = get_thread_table_addr(tid, 2);
        long int sp_addr = get_thread_table_addr(tid, 3);
        long int db_addr = get_thread_table_addr(tid, 4);
        long int ib_addr = get_thread_table_addr(tid, 5);
        long int ic_addr = get_thread_table_addr(tid, 6);
        long int wc_addr = get_thread_table_addr(tid, 7);

        long int id = mem_read(cpu->mem, id_addr, KERNEL);
        long int state = mem_read(cpu->mem, state_addr, KERNEL);
        long int pc = mem_read(cpu->mem, pc_addr, KERNEL);
        long int sp = mem_read(cpu->mem, sp_addr, KERNEL);
        long int db = mem_read(cpu->mem, db_addr, KERNEL);
        long int ib = mem_read(cpu->mem, ib_addr, KERNEL);
        long int ic = mem_read(cpu->mem, ic_addr, KERNEL);
        long int wc = mem_read(cpu->mem, wc_addr, KERNEL);

        const char * state_names[] = {"READY", "RUNNING", "BLOCKED", "TERMINATED"};
        const char * state_name = (state >= 0 && state <= 3) ? state_names[state] : "UNKNOWN";

        fprintf(stderr, "Thread %d: [%ld] [%ld:%s] [%ld] [%ld] [%ld] [%ld] [%ld] [%ld]\n",
               tid, id, state, state_name, pc, sp, db, ib, ic, wc);
    }
    fprintf(stderr, "=== END THREAD TABLE ===\n\n");
}

/**
 * @brief Validates the CPU structure and its associated memory.
 *
 * This function performs sanity checks on the CPU structure, ensuring that the CPU pointer,
 * memory pointer, memory initialization, CPU mode, and current thread ID are valid.
 * If any check fails, it prints an error and exits the program.
 *
 * @param cpu Pointer to the CPU structure to validate.
 * @param caller Name of the calling function for error reporting.
 */
static void
check_cpu(const CPU * cpu, const char * caller)
{
	if (cpu == NULL) {
		fprintf(stderr, "FATAL ERROR: CPU pointer is NULL, from %s\n", caller);
		exit(EXIT_FAILURE);
	}

	if (cpu->mem == NULL) {
		fprintf(stderr, "FATAL ERROR: CPU's Memory pointer is NULL, from %s\n", caller);
		exit(EXIT_FAILURE);
	}

	if (!cpu->mem->is_initialized) {
		fprintf(stderr, "FATAL ERROR: Memory (via CPU) is not initialized, from %s\n", caller);
		exit(EXIT_FAILURE);
	}

	if (cpu->mode != USER && cpu->mode != KERNEL) {
		fprintf(stderr, "FATAL ERROR: Invalid CPU mode (%d), from %s\n", cpu->mode, caller);
		exit(EXIT_FAILURE);
	}

	if (cpu->curr_thread_id < OS_ID || cpu->curr_thread_id >= MAX_PROGRAM_ENTITIES) {
		fprintf(stderr,
				"FATAL ERROR: Invalid thread id (%d), from %s. Valid range: %d-%d\n",
				cpu->curr_thread_id, caller,
				OS_ID, MAX_PROGRAM_ENTITIES -1);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Validates a relative data address for the current thread.
 *
 * This function checks if the provided relative data address is within the valid range
 * for the current thread or OS. For the OS, it allows access to registers 0-19 or a larger
 * data range. If the address is invalid, it prints an error and exits.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_address The relative data address to validate.
 * @param context Description of the operation for error reporting.
 */
static void
check_data_address(const CPU * cpu, const long int relative_address, const char * context)
{
	long int max_data_size = ENTITY_DATA_SIZE;

    if (cpu->curr_thread_id == OS_ID) {
        max_data_size = (relative_address < 20) ? 20 : OS_USABLE_DATA_SIZE;
    }

	if (relative_address < 0 || relative_address >= max_data_size) {
		fprintf(stderr,
				"FATAL ERROR: Entity %d (mode %d) %s: Relative source address %ld is out of bounds (0-%ld).\n",
				cpu->curr_thread_id,
				cpu->mode,
				context,
				relative_address,
				max_data_size - 1);
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Validates a relative instruction address for the current thread.
 *
 * This function ensures that the provided relative instruction address is within the valid
 * range for the current thread or OS. If the address is out of bounds, it prints an error
 * and exits the program.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_address The relative instruction address to validate.
 * @param context Description of the operation for error reporting.
 */
static void
check_instruction_address(const CPU * cpu, const long int relative_address, const char * context)
{
	long int max_instr_size = ENTITY_INSTR_SIZE;

    if (cpu->curr_thread_id == OS_ID) {
        max_instr_size = OS_BLOCK_END_ADDR - OS_INSTRUCTION_START_ADDR + 1;
    }
    if (relative_address < 0 || relative_address >= max_instr_size) {
        fprintf(stderr,
                "FATAL ERROR: Entity %d (mode %d) %s: Relative instruction address %ld is out of bounds (0-%ld).\n",
                cpu->curr_thread_id,
                cpu->mode,
                context,
                relative_address,
                max_instr_size - 1);
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Executes the SET instruction to store a value in memory.
 *
 * This function writes a specified value to a relative data address, performing validation
 * checks for the CPU state and address bounds. For the OS, it allows direct access to
 * registers 0-19. Debug information is printed if enabled.
 *
 * @param cpu Pointer to the CPU structure.
 * @param value The value to write.
 * @param relative_address The relative memory address to write to.
 */
static void
exec_set(CPU * cpu, long int value, long int relative_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("SET: %ld - %ld\n", value, relative_address);
	#endif

	#ifdef DEBUG_FLAG
	    if (cpu->curr_thread_id == OS_ID && cpu->mode == KERNEL) {
	        long int absolute_addr = (relative_address < 20) ? relative_address : cpu->curr_data_base_for_active_entity + relative_address;

	        if (absolute_addr >= 200 && absolute_addr <= 287) {
	            int thread_id = (absolute_addr - 200) / 8;
	            int field = (absolute_addr - 200) % 8;
	            const char * field_names[] = {"ID", "State", "PC", "SP", "DataBase", "InstrBase", "IC", "WakeupCount"};

	            printf("DEBUG_SET_THREAD_TABLE: Thread %d, Field %d (%s), Address %ld, Value %ld\n",
	                   thread_id, field, field_names[field], absolute_addr, value);
	        }
	    }
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_address, "SET");

	long int absolute_addr = (cpu->curr_thread_id == OS_ID && relative_address < 20) ? relative_address : cpu->curr_data_base_for_active_entity + relative_address;
	mem_write(cpu->mem, absolute_addr, value, cpu->mode);
}

/**
 * @brief Executes the CPY instruction to copy a value between memory locations.
 *
 * This function copies a value from a source relative address to a destination relative
 * address, performing validation checks for CPU state and address bounds. For the OS,
 * it allows direct access to registers 0-19.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_src_address The relative source address to read from.
 * @param relative_dest_address The relative destination address to write to.
 */
static void
exec_cpy(CPU * cpu, long int relative_src_address, long int relative_dest_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("CPY: %ld - %ld\n", relative_src_address, relative_dest_address);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_src_address,  "CPY");
	check_data_address(cpu, relative_dest_address, "CPY");

	long int absolute_src_address  = (cpu->curr_thread_id == OS_ID && relative_src_address < 20) ? relative_src_address : cpu->curr_data_base_for_active_entity + relative_src_address;
	long int absolute_dest_address = (cpu->curr_thread_id == OS_ID && relative_dest_address < 20) ? relative_dest_address : cpu->curr_data_base_for_active_entity + relative_dest_address;

	mem_write(cpu->mem, absolute_dest_address, mem_read(cpu->mem, absolute_src_address, cpu->mode), cpu->mode);
}

/**
 * @brief Executes the CPYI instruction for indirect memory copy.
 *
 * This function reads a source address from a relative pointer address, then copies the
 * value at that source address to a destination relative address. It performs validation
 * checks for CPU state and address bounds, with special handling for OS register access.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_address_of_ptr The relative address containing the source pointer.
 * @param relative_dest_address The relative destination address to write to.
 */
static void
exec_cpyi(CPU * cpu, long int relative_address_of_ptr, long int relative_dest_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("CPYI: %ld - %ld\n", relative_address_of_ptr, relative_dest_address);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_address_of_ptr, "CPYI");
	check_data_address(cpu, relative_dest_address,   "CPYI");

	long int pointer_absolute_address  = (cpu->curr_thread_id == OS_ID && relative_address_of_ptr < 20) ? relative_address_of_ptr : cpu->curr_data_base_for_active_entity + relative_address_of_ptr;
	long int absolute_dest_address     = (cpu->curr_thread_id == OS_ID && relative_dest_address < 20) ? relative_dest_address : cpu->curr_data_base_for_active_entity + relative_dest_address;

	long int src_offset_value = mem_read(cpu->mem, pointer_absolute_address, cpu->mode);
	check_data_address(cpu, src_offset_value, "CPYI (indirect_src_offset_as_data_addr)");

	long int indirect_src_absolute_address = (cpu->curr_thread_id == OS_ID && src_offset_value < 20) ? src_offset_value : cpu->curr_data_base_for_active_entity + src_offset_value;

	if (indirect_src_absolute_address < 0 || indirect_src_absolute_address >= MEM_SIZE) {
		fprintf(stderr,
				"ERROR: Entity %d CPYI: Indirect source address %ld (read from %ld) is out of MEMORY bounds (0-%d)\n",
				cpu->curr_thread_id,
				pointer_absolute_address,
				indirect_src_absolute_address,
				MEM_SIZE - 1);
		exit(EXIT_FAILURE);
	}
	mem_write(cpu->mem, absolute_dest_address, mem_read(cpu->mem, indirect_src_absolute_address, cpu->mode), cpu->mode);
}

/**
 * @brief Executes the CPYI2 instruction for double-indirect memory copy.
 *
 * This function performs a double-indirect copy, reading pointers from both source and
 * destination relative addresses, then copying the value from the source's pointed address
 * to the destination's pointed address. It includes validation checks and debug output.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_src_ptr_address The relative address containing the source pointer.
 * @param relative_dest_ptr_address The relative address containing the destination pointer.
 */
static void
exec_cpyi2(CPU * cpu, long int relative_src_ptr_address, long int relative_dest_ptr_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("CPYI2: %ld - %ld\n", relative_src_ptr_address, relative_dest_ptr_address);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_src_ptr_address, "CPYI");
	check_data_address(cpu, relative_dest_ptr_address,   "CPYI");

	/* A1 -> M[A1] */
    long int absolute_src_ptr_address = (cpu->curr_thread_id == OS_ID && relative_src_ptr_address < 21) ?
        relative_src_ptr_address : cpu->curr_data_base_for_active_entity + relative_src_ptr_address;
    long int src_value = mem_read(cpu->mem, absolute_src_ptr_address, cpu->mode);

    long int actual_src_data_address = (cpu->curr_thread_id == OS_ID && src_value < 21) ?
        src_value : cpu->curr_data_base_for_active_entity + src_value;

    /* A2 -> M[A2] */
    long int absolute_dest_ptr_address = (cpu->curr_thread_id == OS_ID && relative_dest_ptr_address < 21) ?
        relative_dest_ptr_address : cpu->curr_data_base_for_active_entity + relative_dest_ptr_address;
    long int dest_value = mem_read(cpu->mem, absolute_dest_ptr_address, cpu->mode);

    long int actual_dest_data_address = (cpu->curr_thread_id == OS_ID && dest_value < 21) ?
        dest_value : cpu->curr_data_base_for_active_entity + dest_value;

    #ifdef DEBUG_FLAG
	    if (cpu->curr_thread_id == OS_ID && cpu->mode == KERNEL) {

	        if (actual_dest_data_address >= 200 && actual_dest_data_address <= 287) {
	            long int value_to_copy = mem_read(cpu->mem, actual_src_data_address, cpu->mode);
	            int thread_id = (actual_dest_data_address - 200) / 8;
	            int field = (actual_dest_data_address - 200) % 8;

	            printf("DEBUG_THREAD_TABLE_UPDATE: Thread %d, Field %d, Address %ld, Value %ld\n",
	                   thread_id, field, actual_dest_data_address, value_to_copy);

	            const char * field_names[] = {"ID", "State", "PC", "SP", "DataBase", "InstrBase", "IC", "WakeupCount"};
	            printf("  -> Updating %s to %ld\n", field_names[field], value_to_copy);

	            if (field == 1) {
	                const char * state_names[] = {"READY", "RUNNING", "BLOCKED", "TERMINATED"};
	                if (value_to_copy >= 0 && value_to_copy <= 3) {
	                    printf("  -> Thread %d state changing to: %s\n", thread_id, state_names[value_to_copy]);
	                }
	            }
	        }
	    }
	#endif

	if (actual_src_data_address < 0 || actual_src_data_address >= MEM_SIZE) {
		fprintf(stderr,
				"ERROR: Entity %d CPYI2: Indirect source data address %ld (read from M[%ld]) is out of MEMORY bounds (0-%d).\n",
				cpu->curr_thread_id,
				actual_src_data_address,
				absolute_src_ptr_address,
				MEM_SIZE - 1);
		exit(EXIT_FAILURE);
	}

	if (actual_dest_data_address < 0 || actual_dest_data_address >= MEM_SIZE) {
		fprintf(stderr,
				"ERROR: Entity %d CPYI2: Indirect destination data address %ld (read from M[%ld]) is out of MEMORY bounds (0-%d).\n",
				cpu->curr_thread_id,
				actual_dest_data_address,
				absolute_dest_ptr_address,
				MEM_SIZE - 1);
		exit(EXIT_FAILURE);
	}
	/* M[M[A1]]: */
	long int value_to_copy = mem_read(cpu->mem, actual_src_data_address, cpu->mode);
	/* M[M[A2]] = M[M[A1]] */
	mem_write(cpu->mem, actual_dest_data_address, value_to_copy, cpu->mode);
}

/**
 * @brief Executes the ADD instruction to add a value to a memory location.
 *
 * This function adds a specified value to the contents of a relative memory address,
 * performing validation checks for CPU state, address bounds, and overflow conditions.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_dest_address The relative address to modify.
 * @param value_to_add The value to add to the memory location.
 */
static void
exec_add(CPU * cpu, long int relative_dest_address, long int value_to_add)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("ADD: %ld - %ld\n",relative_dest_address, value_to_add);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_dest_address, "ADD");

	long int absolute_dest_address = (cpu->curr_thread_id == OS_ID && relative_dest_address < 20) ? relative_dest_address : cpu->curr_data_base_for_active_entity + relative_dest_address;
	long int dest_value = mem_read(cpu->mem, absolute_dest_address, cpu->mode);

	/* Overflow check */
	if ((value_to_add > 0 && dest_value > LONG_MAX - value_to_add) ||
		(value_to_add < 0 && dest_value < LONG_MIN - value_to_add)) {
		fprintf(stderr,
				"ERROR: Overflow in ADD operation for entity %d at address %ld\n",
				cpu->curr_thread_id,
				absolute_dest_address);
		exit(EXIT_FAILURE);
	}
	mem_write(cpu->mem, absolute_dest_address, value_to_add + dest_value, cpu->mode);
}

/**
 * @brief Executes the ADDI instruction to add two memory values.
 *
 * This function adds the value at a source relative address to the value at a destination
 * relative address, storing the result in the destination. It includes validation and
 * overflow checks.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_dest_address The relative destination address.
 * @param relative_src_address The relative source address.
 */
static void
exec_addi(CPU * cpu, long int relative_dest_address, long int relative_src_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("ADDI: %ld - %ld\n", relative_dest_address, relative_src_address);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_dest_address, "ADDI");
	check_data_address(cpu, relative_src_address,  "ADDI");

	long int absolute_dest_address = (cpu->curr_thread_id == OS_ID && relative_dest_address < 20) ? relative_dest_address : cpu->curr_data_base_for_active_entity + relative_dest_address;
	long int absolute_src_address  = (cpu->curr_thread_id == OS_ID && relative_src_address < 20) ? relative_src_address : cpu->curr_data_base_for_active_entity + relative_src_address;

	long int src_value  = mem_read(cpu->mem, absolute_src_address,  cpu->mode);
	long int dest_value = mem_read(cpu->mem, absolute_dest_address, cpu->mode);

	/* Overflow check */
	if ((src_value > 0 && dest_value > LONG_MAX - src_value) ||
		(src_value < 0 && dest_value < LONG_MIN - src_value)) {
		fprintf(stderr,
				"ERROR: Overflow in ADDI operation for entity %d at address %ld\n",
				cpu->curr_thread_id,
				absolute_dest_address);
		exit(EXIT_FAILURE);
	}
	mem_write(cpu->mem, absolute_dest_address, src_value + dest_value, cpu->mode);
}

/**
 * @brief Executes the SUBI instruction to subtract memory values.
 *
 * This function subtracts the value at a destination relative address from the value at
 * a source relative address, storing the result in the destination. It includes validation
 * and overflow checks.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_src_address The relative source address.
 * @param relative_dest_address The relative destination address.
 */
static void
exec_subi(CPU * cpu, long int relative_src_address, long int relative_dest_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("SUBI: %ld - %ld\n", relative_src_address, relative_dest_address);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_dest_address, "SUBI");
	check_data_address(cpu, relative_src_address,  "SUBI");

	long int absolute_dest_address = (cpu->curr_thread_id == OS_ID && relative_dest_address < 20) ? relative_dest_address : cpu->curr_data_base_for_active_entity + relative_dest_address;
	long int absolute_src_address  = (cpu->curr_thread_id == OS_ID && relative_src_address < 20) ? relative_src_address : cpu->curr_data_base_for_active_entity + relative_src_address;

	long int src_value  = mem_read(cpu->mem, absolute_src_address,  cpu->mode);
	long int dest_value = mem_read(cpu->mem, absolute_dest_address, cpu->mode);

	/* Overflow check */
	if ((dest_value > 0 && src_value < LONG_MIN + dest_value) ||
		(dest_value < 0 && src_value > LONG_MAX + dest_value)) {
		fprintf(stderr,
				"ERROR: Overflow in SUBI operation for entity %d at address %ld\n",
				cpu->curr_thread_id,
				absolute_dest_address);
		exit(EXIT_FAILURE);
	}
	mem_write(cpu->mem, absolute_dest_address, src_value - dest_value, cpu->mode);
}

/**
 * @brief Executes the JIF instruction for conditional jumps.
 *
 * This function checks a condition value at a relative address and, if the value is less
 * than or equal to zero, updates the program counter to a new instruction address.
 * Validation checks are performed for CPU state and address bounds.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_condition_address The relative address containing the condition value.
 * @param relative_new_pc_address The relative instruction address to jump to.
 * @param next_pc_address Pointer to the next program counter address to update.
 */
static void
exec_jif(CPU * cpu, long int relative_condition_address, long int relative_new_pc_address, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("JIF: %ld - %ld\n", relative_condition_address, relative_new_pc_address);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_condition_address, "JIF");
	check_instruction_address(cpu, relative_new_pc_address, "JIF");

	long int absolute_condition_address = (cpu->curr_thread_id == OS_ID && relative_condition_address < 20) ? relative_condition_address : cpu->curr_data_base_for_active_entity + relative_condition_address;
	long int condition_value = mem_read(cpu->mem, absolute_condition_address, cpu->mode);

	if (condition_value <= 0) {
		/* Each instruction takes INSTR_SIZE (3) bytes, so multiply relative_new_pc_address by INSTR_SIZE */
		long int absolute_instruction_address = cpu->curr_instruction_base_for_active_entity + (relative_new_pc_address * INSTR_SIZE);
		*next_pc_address = absolute_instruction_address;

		#ifdef DEBUG_FLAG
			printf("Jump to: %ld\n", absolute_instruction_address);
		#endif
	}

}

/**
 * @brief Executes the PUSH instruction to push a value onto the stack.
 *
 * This function pushes a value from a relative address onto the stack, updating the stack
 * pointer. It includes validation checks for CPU state, address bounds, and stack overflow.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_element_address The relative address of the value to push.
 */
static void
exec_push(CPU * cpu, long int relative_element_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("PUSH: %ld\n", relative_element_address);
	#endif

	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_element_address, "PUSH");

	long int absolute_element_address = (cpu->curr_thread_id == OS_ID && relative_element_address < 20) ? relative_element_address : cpu->curr_data_base_for_active_entity + relative_element_address;
	long int element_value = mem_read(cpu->mem, absolute_element_address, cpu->mode);
	long int current_sp_value = mem_read(cpu->mem, REG_SP, cpu->mode);

	long int stack_upper_bound = (cpu->curr_thread_id == OS_ID) ? OS_BLOCK_END_ADDR : THREAD_BLOCK_END(cpu->curr_thread_id);

	if (current_sp_value == 0 || current_sp_value > stack_upper_bound) {
        current_sp_value = stack_upper_bound;
        mem_write(cpu->mem, REG_SP, current_sp_value, cpu->mode);
    }

    long int new_sp_value = current_sp_value - 1;
    if (mem_read(cpu->mem, new_sp_value, cpu->mode) == -1) {
        fprintf(stderr, "FATAL ERROR: Stack overflow in PUSH for entity %d. SP %ld encountered -1.\n",
                cpu->curr_thread_id,
                new_sp_value);
        exit(EXIT_FAILURE);
    }
    if (new_sp_value < 0) {
        fprintf(stderr, "FATAL ERROR: Stack overflow in PUSH for entity %d. SP became negative (%ld).\n",
                cpu->curr_thread_id,
                new_sp_value);
        exit(EXIT_FAILURE);
    }
	mem_write(cpu->mem, current_sp_value, element_value, cpu->mode);
    mem_write(cpu->mem, REG_SP, new_sp_value, cpu->mode);
}

/**
 * @brief Executes the POP instruction to pop a value from the stack.
 *
 * This function pops a value from the stack (at SP + 1) and stores it at a relative
 * destination address, updating the stack pointer. It includes validation checks for CPU
 * state, address bounds, and stack underflow.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_dest_address The relative address to store the popped value.
 */
static void
exec_pop(CPU * cpu, long int relative_dest_address) /* POP bir üstü okumalı o anki SP değerini değil */
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("POP: %ld\n", relative_dest_address);
	#endif

	check_cpu(cpu, __func__);
    check_data_address(cpu, relative_dest_address, "POP");

    long int current_sp_value = mem_read(cpu->mem, REG_SP, cpu->mode); // Şu anki SP (örneğin 998)
    long int stack_upper_bound = (cpu->curr_thread_id == OS_ID) ? OS_BLOCK_END_ADDR : THREAD_BLOCK_END(cpu->curr_thread_id);

    if (current_sp_value >= stack_upper_bound) {
        fprintf(stderr, "FATAL ERROR: Stack underflow in POP for entity %d. SP %ld at or beyond upper bound %ld.\n",
                cpu->curr_thread_id, current_sp_value, stack_upper_bound);
        exit(EXIT_FAILURE);
    }

    /* Stack’ten değeri oku (SP + 1, çünkü PUSH SP’ye yazıp azaltmıştı) */
    long int element_address = current_sp_value + 1; /* 998 + 1 = 999 */
    long int element = mem_read(cpu->mem, element_address, cpu->mode);
    long int absolute_dest_address = (cpu->curr_thread_id == OS_ID && relative_dest_address < 20) ? relative_dest_address : cpu->curr_data_base_for_active_entity + relative_dest_address; // 21 + 2 = 23

    mem_write(cpu->mem, absolute_dest_address, element, cpu->mode);
    mem_write(cpu->mem, REG_SP, current_sp_value + 1, cpu->mode);
}

/**
 * @brief Executes the CALL instruction for subroutine calls.
 *
 * This function saves the next program counter address on the stack, updates the stack
 * pointer, and jumps to a relative instruction address. It includes validation checks for
 * CPU state, address bounds, and stack overflow.
 *
 * @param cpu Pointer to the CPU structure.
 * @param relative_jump_address The relative instruction address to jump to.
 * @param next_pc_address Pointer to the next program counter address to update.
 */
static void
exec_call(CPU * cpu, long int relative_jump_address, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("CALL: %ld\n", relative_jump_address);
	#endif

	#ifdef DEBUG_FLAG
	    if (cpu->curr_thread_id == OS_ID && relative_jump_address == 90) {
	        printf("DEBUG_CALL_UPDATE: Calling update_current_thread_in_table\n");
	        printf("  -> Current thread ID (M[100]): %ld\n",
	               mem_read(cpu->mem, OS_DATA_START_ADDR + 100, KERNEL));
	        printf("  -> Syscall ID (Register 2): %ld\n",
	               mem_read(cpu->mem, 2, KERNEL));
	    }
    #endif

	check_cpu(cpu, __func__);
    check_instruction_address(cpu, relative_jump_address, "CALL");

    long int current_sp_value = mem_read(cpu->mem, REG_SP, cpu->mode);
    long int stack_upper_bound = (cpu->curr_thread_id == OS_ID) ? OS_BLOCK_END_ADDR : THREAD_BLOCK_END(cpu->curr_thread_id);

    if (current_sp_value == 0 || current_sp_value > stack_upper_bound) {
        current_sp_value = stack_upper_bound;
        mem_write(cpu->mem, REG_SP, current_sp_value, cpu->mode);
    }

    long int new_sp_value = current_sp_value - 1;
    if (mem_read(cpu->mem, new_sp_value, cpu->mode) == -1) {
        fprintf(stderr, "FATAL ERROR: Stack overflow in CALL for entity %d. SP %ld encountered -1.\n",
                cpu->curr_thread_id,
                new_sp_value);
        exit(EXIT_FAILURE);
    }

    long int absolute_jump_address = cpu->curr_instruction_base_for_active_entity + (relative_jump_address * INSTR_SIZE);
    if (absolute_jump_address < 0 || absolute_jump_address >= MEM_SIZE) {
        fprintf(stderr,
                "FATAL ERROR: Jump address %ld is out of memory bounds (0-%d) in CALL for entity %d\n",
                absolute_jump_address,
                MEM_SIZE - 1,
                cpu->curr_thread_id);
        exit(EXIT_FAILURE);
    }
    mem_write(cpu->mem, current_sp_value, *next_pc_address, cpu->mode);
    mem_write(cpu->mem, REG_SP, new_sp_value, cpu->mode);
    *next_pc_address = absolute_jump_address;
}

/**
 * @brief Executes the RET instruction to return from a subroutine.
 *
 * This function pops the return address from the stack (at SP + 1), updates the stack
 * pointer, and sets the program counter to the return address. It includes validation
 * checks for CPU state, address bounds, and stack underflow.
 *
 * @param cpu Pointer to the CPU structure.
 * @param next_pc_address Pointer to the next program counter address to update.
 */
static void
exec_ret(CPU * cpu, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("RET\n");
	#endif

	check_cpu(cpu, __func__);

    long int current_sp_value = mem_read(cpu->mem, REG_SP, cpu->mode);
    long int stack_upper_bound = (cpu->curr_thread_id == OS_ID) ? OS_BLOCK_END_ADDR : THREAD_BLOCK_END(cpu->curr_thread_id);

    if (current_sp_value >= stack_upper_bound) {
        fprintf(stderr, "FATAL ERROR: Stack underflow in RET for entity %d. SP %ld at or beyond upper bound %ld.\n",
                cpu->curr_thread_id,
                current_sp_value,
                stack_upper_bound);
        exit(EXIT_FAILURE);
    }
    /* return address'i SP+1 adresinden oku (CALL'da yazıldığı yer) */
    long int return_address = mem_read(cpu->mem, current_sp_value + 1, cpu->mode);
    long int new_sp_value = current_sp_value + 1;

    if (return_address < 0 || return_address >= MEM_SIZE) {
        fprintf(stderr,
                "FATAL ERROR: Invalid return address %ld in RET for entity %d\n",
                return_address,
                cpu->curr_thread_id);
        exit(EXIT_FAILURE);
    }
    *next_pc_address = return_address;
    mem_write(cpu->mem, REG_SP, new_sp_value, cpu->mode);
}

/**
 * @brief Executes the HLT instruction to halt the CPU.
 *
 * This function sets the CPU's halted state to true, stopping further instruction execution.
 * It includes validation checks for the CPU state.
 *
 * @param cpu Pointer to the CPU structure.
 */
static void
exec_hlt(CPU * cpu)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("HLT\n");
	#endif

	check_cpu(cpu, __func__);
	cpu->is_halted = true;
}

/**
 * @brief Executes the USER instruction to switch to user mode.
 *
 * This function switches the CPU to user mode, updating the thread ID, data base,
 * instruction base, and stack pointer based on values in a mailbox. It supports context
 * switching for OS programs and handles pending print operations.
 *
 * @param cpu Pointer to the CPU structure.
 * @param pt_jump_address_offset The relative offset for the mailbox containing context data.
 * @param next_pc_address Pointer to the next program counter address to update.
 */
static void
exec_user(CPU * cpu, long int pt_jump_address_offset, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("USER\n");
	#endif

	check_cpu(cpu, __func__);

    long int mailbox_pc_abs_addr = cpu->curr_data_base_for_active_entity + pt_jump_address_offset;
    long int target_user_pc = mem_read(cpu->mem, mailbox_pc_abs_addr, cpu->mode);

	if (target_user_pc < 0 || target_user_pc >= MEM_SIZE) {
        fprintf(stderr,
                "FATAL ERROR: USER instr: Target PC %ld (from M[%ld]) is out of MEMORY bounds (0-%d).\n",
                target_user_pc,
                mailbox_pc_abs_addr,
                MEM_SIZE - 1);
        cpu->is_halted = true;
        return;
    }
    if (target_user_pc < USER_THREAD_START_BASE_ADDR) {
        fprintf(stderr,
                "FATAL ERROR: USER instr: Target PC %ld (from M[%ld]) is below USER mode boundary (%d).\n",
                target_user_pc,
                mailbox_pc_abs_addr,
                USER_THREAD_START_BASE_ADDR);
        cpu->is_halted = true;
        return;
    }

    /* OS, USER komutuyla bir kullanıcı thread'ine geçiş yapıyor */
	if (cpu->curr_thread_id == OS_ID) {
        /* Mailbox'tan diğer context bilgilerini oku */
        long int tid_relative_offset = pt_jump_address_offset - 2; /* (602 - 2 = 600 for ID) */
        long int db_relative_offset  = pt_jump_address_offset + 2; /* (602 + 2 = 604 for Data Base) */
        long int ib_relative_offset  = pt_jump_address_offset + 3; /* (602 + 3 = 605 for Instr Base) */
        long int sp_relative_offset  = pt_jump_address_offset + 1; /* (602 + 1 = 603 for SP) */

        int target_thread_id = (int)mem_read(cpu->mem, cpu->curr_data_base_for_active_entity + tid_relative_offset, cpu->mode);
        long int target_db     = mem_read(cpu->mem, cpu->curr_data_base_for_active_entity + db_relative_offset, cpu->mode);
        long int target_ib     = mem_read(cpu->mem, cpu->curr_data_base_for_active_entity + ib_relative_offset, cpu->mode);
        long int target_sp     = mem_read(cpu->mem, cpu->curr_data_base_for_active_entity + sp_relative_offset, cpu->mode);

        #ifdef DEBUG_FLAG
        	printf("OS (Thread %d) is executing USER. Performing context switch to Thread %d.\n", OS_ID, target_thread_id);
        #endif

        cpu->curr_thread_id = target_thread_id;
        cpu->curr_data_base_for_active_entity = target_db;
        cpu->curr_instruction_base_for_active_entity = target_ib;

        /*  pek olsaı değil */
	    if (target_thread_id == OS_ID) {
	        cpu->mode = KERNEL;
	    } else {
	        cpu->mode = USER;
	    }
        /* stack Pointer'ı ayarla */
        mem_write(cpu->mem, REG_SP, target_sp, KERNEL);

        /* Check if this thread has a pending print from SYSCALL PRN */
        if (cpu->has_pending_print[target_thread_id]) {
            printf("\nRequested SYSCALL PRN output (Thread %d): %ld\n", target_thread_id, cpu->pending_print_values[target_thread_id]);
            cpu->has_pending_print[target_thread_id] = false;
            cpu->pending_print_values[target_thread_id] = 0;
        }

        #ifdef DEBUG_FLAG
	        printf("Context switched to Thread %d: Mode=USER, DataBase=%ld, InstrBase=%ld, SP=%ld\n",
	               cpu->curr_thread_id, cpu->curr_data_base_for_active_entity,
	               cpu->curr_instruction_base_for_active_entity, mem_read(cpu->mem, REG_SP, cpu->mode));
        #endif
    }

    else {
        printf("Warning: USER instruction executed by non-OS thread_id %d. Only mode and PC will change.\n", cpu->curr_thread_id);
        cpu->mode = USER;
    }
    *next_pc_address = target_user_pc;
}

/**
 * @brief Executes the SYSCALL PRN instruction to schedule a print operation.
 *
 * This function schedules a print operation for a value at a relative address, storing
 * the value in the CPU structure for deferred printing. It saves the thread's context,
 * switches to the OS, and updates the program counter to the syscall handler.
 *
 * @param cpu Pointer to the CPU structure.
 * @param source_address The relative address of the value to print.
 * @param next_pc_address Pointer to the next program counter address to update.
 */
static void
exec_syscall_prn(CPU * cpu, long int source_address, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("SYSCALL PRN: %ld\n", source_address);
	#endif

    check_cpu(cpu, __func__);

    /* SYSCALL tespit edildiğinde thread table dump et */
    if (cpu->debug_level >= 3) {
        long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL);
        long int current_pc = mem_read(cpu->mem, REG_PC, cpu->mode);
        fprintf(stderr, "\n[INSTRUCTION %ld] SYSCALL DETECTED at PC=%ld, Thread=%d\n",
               current_instr_count, current_pc, cpu->curr_thread_id);
        dump_thread_table(cpu);
    }
    /* thread'in mevcut context değerlerini sakla */
    long int thread_pc = mem_read(cpu->mem, REG_PC, cpu->mode) + INSTR_SIZE;
    long int thread_sp = mem_read(cpu->mem, REG_SP, cpu->mode);

    /* thread'in kendi instruction count'unu thread table'dan al */
    long int thread_ic_addr = 200 + (cpu->curr_thread_id * 8) + 6;
    long int thread_ic = mem_read(cpu->mem, thread_ic_addr, KERNEL);

    /* print değeri */
    long int absolute_source_address = cpu->curr_data_base_for_active_entity + source_address;
    if (absolute_source_address < 0 || absolute_source_address >= MEM_SIZE) {
        fprintf(stderr, "FATAL ERROR: Memory address %ld is out of bounds in SYSCALL PRN for entity %d\n",
                absolute_source_address, cpu->curr_thread_id);
        exit(EXIT_FAILURE);
    }
    long int value = mem_read(cpu->mem, absolute_source_address, cpu->mode);

    /* Print değerini CPU struct'ında sakla - thread tekrar çalıştığında basılacak */
    cpu->pending_print_values[cpu->curr_thread_id] = value;
    cpu->has_pending_print[cpu->curr_thread_id] = true;

    /* Debug message - actual print will happen after 100 cycles when thread resumes */
    #ifdef DEBUG_FLAG
        printf("SYSCALL PRN: Thread %d scheduled to print %ld after 100 cycles\n", cpu->curr_thread_id, value);
    #endif

    /* syscall bilgisini kaydet */
    mem_write(cpu->mem, REG_SYSCALL_RESULT, SYSCALL_PRN_ID, KERNEL);

    /* thread context'ini geçici register'lara yaz (OS bunları okuyacak) */
    /* register 4: Wakeup count (REG_WAKEUP_INSTR_COUNT'dan oku) */
	long int wakeup_count = mem_read(cpu->mem, REG_WAKEUP_INSTR_COUNT, cpu->mode);
	mem_write(cpu->mem, 4, wakeup_count, KERNEL);  /* Register 4: Wakeup Count */
    mem_write(cpu->mem, 5, thread_pc,    KERNEL);  /* Register 5: Thread PC */
    mem_write(cpu->mem, 6, thread_sp,    KERNEL);  /* Register 6: Thread SP */
    mem_write(cpu->mem, 7, thread_ic,    KERNEL);  /* Register 7: Thread IC */

    /* switching to OS */
    cpu->mode = KERNEL;
    *next_pc_address = OS_SYSCALL_HANDLER_ADDR;
    cpu->curr_thread_id = OS_ID;
    cpu->curr_data_base_for_active_entity = OS_DATA_START_ADDR;
    cpu->curr_instruction_base_for_active_entity = OS_INSTRUCTION_START_ADDR;

    /* SYSCALL sonrası thread table dump et */
    if (cpu->debug_level >= 3) {
        long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL) + 1;
        fprintf(stderr, "\n[INSTRUCTION %ld] THREAD TABLE AFTER SYSCALL:\n", current_instr_count);
        dump_thread_table(cpu);
    }
}

/**
 * @brief Executes the SYSCALL HLT instruction to halt a thread.
 *
 * This function saves the thread's context, records the syscall, and switches to the OS
 * for handling. It updates the program counter to the syscall handler address.
 *
 * @param cpu Pointer to the CPU structure.
 * @param next_pc_address Pointer to the next program counter address to update.
 */
static void
exec_syscall_hlt(CPU * cpu, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
    	printf("SYSCALL HLT\n");
    #endif

    check_cpu(cpu, __func__);

    /* SYSCALL tespit edildiğinde thread table dump et */
    if (cpu->debug_level >= 3) {
        long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL);
        long int current_pc = mem_read(cpu->mem, REG_PC, cpu->mode);
        fprintf(stderr, "\n[INSTRUCTION %ld] SYSCALL DETECTED at PC=%ld, Thread=%d\n",
               current_instr_count, current_pc, cpu->curr_thread_id);
        dump_thread_table(cpu);
    }
    /* thread'in mevcut context değerlerini sakla */
    long int thread_pc = mem_read(cpu->mem, REG_PC, cpu->mode) + INSTR_SIZE;
    long int thread_sp = mem_read(cpu->mem, REG_SP, cpu->mode);

    /* thread'in kendi instruction count'unu thread table'dan al */
    long int thread_ic_addr = 200 + (cpu->curr_thread_id * 8) + 6;
    long int thread_ic = mem_read(cpu->mem, thread_ic_addr, KERNEL);

    /* syscall bilgisini kaydet */
    mem_write(cpu->mem, REG_SYSCALL_RESULT, SYSCALL_HLT_ID, KERNEL);

    /* Thread context'ini geçici register'lara yaz */
    /* register 4: Wakeup count (REG_WAKEUP_INSTR_COUNT'dan oku) */
	long int wakeup_count = mem_read(cpu->mem, REG_WAKEUP_INSTR_COUNT, cpu->mode);
	mem_write(cpu->mem, 4, wakeup_count, KERNEL);
    mem_write(cpu->mem, 5, thread_pc,    KERNEL);
    mem_write(cpu->mem, 6, thread_sp,    KERNEL);
    mem_write(cpu->mem, 7, thread_ic,    KERNEL);

    /* switching to OS */
    cpu->mode = KERNEL;
    *next_pc_address = OS_SYSCALL_HANDLER_ADDR;
    cpu->curr_thread_id = OS_ID;
    cpu->curr_data_base_for_active_entity = OS_DATA_START_ADDR;
    cpu->curr_instruction_base_for_active_entity = OS_INSTRUCTION_START_ADDR;

    /* SYSCALL sonrası thread table dump et */
    if (cpu->debug_level >= 3) {
        long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL) + 1;
        fprintf(stderr, "\n[INSTRUCTION %ld] THREAD TABLE AFTER SYSCALL:\n", current_instr_count);
        dump_thread_table(cpu);
    }
}

/**
 * @brief Executes the SYSCALL YIELD instruction to yield control to the OS.
 *
 * This function saves the thread's context, records the syscall, and switches to the OS
 * for scheduling. It updates the program counter to the syscall handler address.
 *
 * @param cpu Pointer to the CPU structure.
 * @param next_pc_address Pointer to the next program counter address to update.
 */
static void
exec_syscall_yield(CPU * cpu, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("SYSCALL YIELD\n");
	#endif

    check_cpu(cpu, __func__);

    /* SYSCALL tespit edildiğinde thread table dump et */
    if (cpu->debug_level >= 3) {
        long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL);
        long int current_pc = mem_read(cpu->mem, REG_PC, cpu->mode);
        fprintf(stderr, "\n[INSTRUCTION %ld] SYSCALL DETECTED at PC=%ld, Thread=%d\n",
               current_instr_count, current_pc, cpu->curr_thread_id);
        dump_thread_table(cpu);
    }
    /* thread'in mevcut context değerlerini sakla */
    long int thread_pc = mem_read(cpu->mem, REG_PC, cpu->mode) + INSTR_SIZE;
    long int thread_sp = mem_read(cpu->mem, REG_SP, cpu->mode);

    /* thread'in kendi instruction count'unu thread table'dan al */
    long int thread_ic_addr = 200 + (cpu->curr_thread_id * 8) + 6;
    long int thread_ic = mem_read(cpu->mem, thread_ic_addr, KERNEL);

    /* syscall bilgisini kaydet */
    mem_write(cpu->mem, REG_SYSCALL_RESULT, SYSCALL_YIELD_ID, KERNEL);

    /* thread context'ini geçici register'lara yaz */
	long int wakeup_count = mem_read(cpu->mem, REG_WAKEUP_INSTR_COUNT, cpu->mode);
	mem_write(cpu->mem, 4, wakeup_count, KERNEL);
    mem_write(cpu->mem, 5, thread_pc,    KERNEL);
    mem_write(cpu->mem, 6, thread_sp,    KERNEL);
    mem_write(cpu->mem, 7, thread_ic,    KERNEL);

    /* switching to OS */
    cpu->mode = KERNEL;
    *next_pc_address = OS_SYSCALL_HANDLER_ADDR;
    cpu->curr_thread_id = OS_ID;
    cpu->curr_data_base_for_active_entity = OS_DATA_START_ADDR;
    cpu->curr_instruction_base_for_active_entity = OS_INSTRUCTION_START_ADDR;

    /* SYSCALL sonrası thread table dump et */
    if (cpu->debug_level >= 3) {
        long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL) + 1;
        fprintf(stderr, "\n[INSTRUCTION %ld] THREAD TABLE AFTER SYSCALL:\n", current_instr_count);
        dump_thread_table(cpu);
    }
}

/**
 * @brief Initializes the CPU structure and sets up execution context.
 *
 * This function initializes the CPU with a given memory structure and debug level,
 * setting initial values for mode, thread ID, data base, instruction base, and registers.
 * It supports both single-threaded and OS program modes, with appropriate context setup.
 *
 * @param cpu Pointer to the CPU structure to initialize.
 * @param mem Pointer to the initialized Memory structure.
 * @param debug_level The debug level for logging (0-3).
 */
void
cpu_init(CPU * cpu, Memory * mem, int debug_level)
{
	if (cpu == NULL) {
		fprintf(stderr, "FATAL ERROR: CPU is not initialized!\n");
		exit(EXIT_FAILURE);
	}
	if (mem == NULL || !mem->is_initialized) {
		fprintf(stderr, "FATAL ERROR: Memory is not initialized!\n");
		exit(EXIT_FAILURE);
	}
	cpu->mem  = mem;
	cpu->mode = KERNEL;
	cpu->is_halted = false;
	cpu->curr_thread_id = OS_ID;
	cpu->curr_data_base_for_active_entity = OS_DATA_START_ADDR;
	cpu->curr_instruction_base_for_active_entity = OS_INSTRUCTION_START_ADDR;
	cpu->debug_level = debug_level;

	/* Initialize pending print arrays */
	for (int i = 0; i < MAX_PROGRAM_ENTITIES; i++) {
		cpu->pending_print_values[i] = 0;
		cpu->has_pending_print[i] = false;
	}

	/* Check if this is a single thread program */
	if (is_single_thread_program) {
		/* Single Thread Program Mode - Start as Thread 2 in USER mode */
		printf("CPU initialized for single thread program (USER mode)\n");
		cpu->mode = USER;
		cpu->curr_thread_id = 2;  /* Use Thread 2 to avoid register/OS conflicts */
		cpu->curr_data_base_for_active_entity = THREAD_DATA_START(2);
		cpu->curr_instruction_base_for_active_entity = THREAD_INSTR_START(2);

		/* Single thread için PC ve SP'yi thread 2'nin adreslerine set et */
		mem_write(mem, REG_PC, cpu->curr_instruction_base_for_active_entity, KERNEL);
		mem_write(mem, REG_SP, THREAD_BLOCK_END(2), KERNEL);

		printf("Single Thread Debug Info:\n");
		printf("  - Thread ID: %d\n", cpu->curr_thread_id);
		printf("  - Data Base: %ld\n", cpu->curr_data_base_for_active_entity);
		printf("  - Instruction Base: %ld\n", cpu->curr_instruction_base_for_active_entity);
		printf("  - Initial PC: %ld\n", cpu->curr_instruction_base_for_active_entity);
		printf("  - Initial SP: %d\n", THREAD_BLOCK_END(2));
	}
	else if (is_os_program) {
		/* OS Program Mode - Start as Thread 0 (OS) in KERNEL mode */
		printf("CPU initialized for OS program (KERNEL mode) - Detected %s sections\n",
               is_os_program ? "multiple" : "single");
		cpu->mode = KERNEL;
		cpu->curr_thread_id = OS_ID;
		cpu->curr_data_base_for_active_entity = OS_DATA_START_ADDR;
		cpu->curr_instruction_base_for_active_entity = OS_INSTRUCTION_START_ADDR;

		mem_write(mem, REG_PC, cpu->curr_instruction_base_for_active_entity, cpu->mode);
		mem_write(mem, REG_SP, OS_BLOCK_END_ADDR, cpu->mode);

		/* Context switch sinyalini başlat */
		mem_write(mem, REG_CONTEXT_SWITCH_SIGNAL, CTX_SWITCH_DONE, KERNEL);

		/* Thread table dump et */
		if (cpu->debug_level >= 3) {
			dump_thread_table(cpu);
		}
	} else {
		/* Fallback - should not normally happen */
		fprintf(stderr, "WARNING: Unknown program type detected, using default initialization\n");
		cpu->mode = KERNEL;
		cpu->curr_thread_id = OS_ID;
		cpu->curr_data_base_for_active_entity = OS_DATA_START_ADDR;
		cpu->curr_instruction_base_for_active_entity = OS_INSTRUCTION_START_ADDR;

		mem_write(mem, REG_PC, cpu->curr_instruction_base_for_active_entity, cpu->mode);
		mem_write(mem, REG_SP, OS_BLOCK_END_ADDR, cpu->mode);
	}

	mem_write(mem, REG_INSTR_COUNT, 0, KERNEL);
}

/**
 * @brief Executes a single CPU instruction.
 *
 * This function handles the fetch-decode-execute cycle for one instruction, including
 * context switching for OS programs, instruction fetching, decoding, and execution.
 * It updates the program counter and instruction count, and manages thread wakeups.
 *
 * @param cpu Pointer to the CPU structure.
 */
void
cpu_execute_instruction(CPU * cpu)
{
	/* Only execute OS context switch logic for multi-threaded programs */
	if (is_os_program && !is_single_thread_program)
	{
		/*
			Context Switch Sinyalini Kontrol Et (Her komut döngüsünün başında): Bu register OS tarafından KERNEL modunda
			set edildiği için KERNEL modunda okunmalı. Eğer o an USER modda bir thread çalışıyorsa bile, bu kontrol CPU
			simülatörünün kendi iç mantığıdır ve OS'nin bıraktığı bir sinyali okur. Güvenlik açısından, bu okuma ve sonraki
			işlemlerin KERNEL ayrıcalığıyla yapıldığını varsayabiliriz.
	    */
	    volatile long int ctx_signal = mem_read(cpu->mem, REG_CONTEXT_SWITCH_SIGNAL, KERNEL);

	    if (ctx_signal == CTX_SWITCH_REQUEST) {
	    	dump_thread_table(cpu);
	        /*
		        OS'nin context switch isteğinde bulunduğunu logla.
		        Bu loglama, cpu->curr_thread_id OS iken daha anlamlı olur,
		        çünkü scheduler OS'nin bir parçasıdır. Ancak sinyal herhangi bir anda tespit edilebilir.
	        */
	        #ifdef CX_DEBUG_FLAG
	        	printf("CPU: Context Switch Request DETECTED (Signal: %ld) by OS (presumably).\n", ctx_signal);
	        #endif
	        /* posta kutusundan yeni context bilgilerini KERNEL modunda oku, çünkü OS bunları KERNEL modunda yazdı. */
	        int next_tid =    (int)mem_read(cpu->mem, ADDR_MAILBOX_NEXT_THREAD_ID,   KERNEL);
	        long int next_state  = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_STATE,       KERNEL);
	        long int next_pc_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_PC,          KERNEL);
	        long int next_sp_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_SP,          KERNEL);
	        long int next_db_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_DATA_BASE,   KERNEL);
	        long int next_ib_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_INSTR_BASE,  KERNEL);
	        long int next_ic_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_INSTR_COUNT, KERNEL);
	        long int next_wc_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_WAKEUP_CNT,  KERNEL);

	        /* Thread table'ı context switch öncesi dump et */
	        if (cpu->debug_level >= 3) {
	            long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL);
	            fprintf(stderr, "\n[INSTRUCTION %ld] CONTEXT SWITCH DETECTED: Thread %d -> Thread %d\n",
	                   current_instr_count, cpu->curr_thread_id, next_tid);
	        }

	        char state[15];

	        switch (next_state) {
	        	case 0:
	        		strcpy(state, "READY");
	        		break;
	        	case 1:
	        		strcpy(state, "RUNNING");
	        		break;
	        	case 2:
	        		strcpy(state, "BLOCKED");
	        		break;
	        	case 3:
	        		strcpy(state, "TERMINATED");
	        		break;
	        }
	        #ifdef CX_DEBUG_FLAG
		        printf("CPU: Switching to Thread ID: %d, State: %s, PC: %ld, SP: %ld, DB: %ld, IB: %ld, IC: %ld, WC: %ld\n",
		               next_tid,
		               state,
		               next_pc_val,
		               next_sp_val,
		               next_db_val,
		               next_ib_val,
		               next_ic_val,
		               next_wc_val);
	        #endif
	        /* CPU context'ini doğrudan güncelle */
	        cpu->curr_thread_id = next_tid;
	        cpu->curr_data_base_for_active_entity = next_db_val;
	        cpu->curr_instruction_base_for_active_entity = next_ib_val;

	        if (next_tid == OS_ID) {
			    cpu->mode = KERNEL;
			} else {
			    cpu->mode = USER;

			    /* Check if this thread has a pending print from SYSCALL PRN */
			    if (cpu->has_pending_print[next_tid]) {
			        printf("\nRequested SYSCALL PRN output (Thread %d): %ld\n", next_tid, cpu->pending_print_values[next_tid]);
			        cpu->has_pending_print[next_tid] = false;
			        cpu->pending_print_values[next_tid] = 0;
			    }
			}
	        mem_write(cpu->mem, REG_PC, next_pc_val, KERNEL);
	        mem_write(cpu->mem, REG_SP, next_sp_val, KERNEL);

	        /* sinyali "işlendi" olarak işaretle */
	        mem_write(cpu->mem, REG_CONTEXT_SWITCH_SIGNAL, CTX_SWITCH_DONE, KERNEL);
	    }
	}

	if (cpu->is_halted) {
		return;
	}
	check_cpu(cpu, __func__);
	/*
		1 - now we are in KERNEL mode and we want to read
		PC value from program counter register...

		2 - reading opcode and converting to the Opcode (enum)

		3 - reading first operand (even it is garbage (zero from memset))

		4 - reading second operand (even it is garbage (zero from memset))
	*/
	/*********************** FETCHING PART ***********************/
	long int current_pc_address = mem_read(cpu->mem, REG_PC, cpu->mode);

	if (current_pc_address < 0 || current_pc_address + INSTR_SIZE > MEM_SIZE) {
		 fprintf(stderr,
				"FATAL ERROR: Program Counter (REG_PC: %ld) is out of valid memory bounds or too close to end.\n",
				current_pc_address);
		 cpu->is_halted = true;
		 return;
	}
	/*********************** FETCHING PART ***********************/
	/* Ayırıcı kontrolü: Bir sonraki talimat adresinde -1 varsa dur: next_instr durdurucu case bu olacak */
    if (mem_read(cpu->mem, current_pc_address, cpu->mode) == -1) {
        fprintf(stderr, "Reached end of instructions (separator -1) at address %ld for entity %d\n",
                current_pc_address, cpu->curr_thread_id);
        cpu->is_halted = true;
        return;
    }

	/*********************** DECODE PART ***********************/
	long int opcode_addr = current_pc_address;
	long int operand_1_addr = current_pc_address + 1;
	long int operand_2_addr = current_pc_address + 2;

	long int opcode_numeric = mem_read(cpu->mem, opcode_addr,    cpu->mode);
	Opcode opcode = (Opcode)opcode_numeric;
	long int operand_1  = mem_read(cpu->mem, operand_1_addr, cpu->mode);
	long int operand_2  = mem_read(cpu->mem, operand_2_addr, cpu->mode);
	/*********************** DECODE PART ***********************/

	#ifdef DEBUG_FLAG
		printf("\n\n\nInstruction for Thread - %d \n- (Mode:%s) at PC=%ld: Opcode=%ld, Op1=%ld, Op2=%ld\n\n\n",
	           cpu->curr_thread_id,
	           (cpu->mode == KERNEL ? "K" : "U"),
	           current_pc_address,
	           opcode_numeric,
	           operand_1,
	           operand_2);
	#endif

	/* INSTR_SIZE: MAX_OPERANDS + 1 = 3 from common header... (MAX_OPERANDS: 2, Opcode: 1) */
	long int next_pc_address = current_pc_address + INSTR_SIZE;

	/*********************** EXECUTE PART ***********************/
	switch (opcode)
	{
		case OPCODE_SET:
			exec_set(cpu, operand_1, operand_2);
			break;

		case OPCODE_CPY:
			exec_cpy(cpu, operand_1, operand_2);
			break;

		case OPCODE_CPYI:
			exec_cpyi(cpu, operand_1, operand_2);
			break;

		case OPCODE_CPYI2:
			exec_cpyi2(cpu, operand_1, operand_2);
			break;

		case OPCODE_ADD:
			exec_add(cpu, operand_1, operand_2);
			break;

		case OPCODE_ADDI:
			exec_addi(cpu, operand_1, operand_2);
			break;

		case OPCODE_SUBI:
			exec_subi(cpu, operand_1, operand_2);
			break;

		case OPCODE_JIF:
			exec_jif(cpu, operand_1, operand_2, &next_pc_address);
			break;

		case OPCODE_PUSH:
			exec_push(cpu, operand_1);
			break;

		case OPCODE_POP:
			exec_pop(cpu, operand_1);
			break;

		case OPCODE_CALL:
			exec_call(cpu, operand_1, &next_pc_address);
			break;

		case OPCODE_RET:
			exec_ret(cpu, &next_pc_address);
			break;

		case OPCODE_HLT:
			exec_hlt(cpu);
			break;

		case OPCODE_USER:
			exec_user(cpu, operand_1, &next_pc_address);
			break;

		case OPCODE_SYSCALL_PRN:
			exec_syscall_prn(cpu, operand_1, &next_pc_address);
			break;

		case OPCODE_SYSCALL_HLT:
			exec_syscall_hlt(cpu, &next_pc_address);
			break;

		case OPCODE_SYSCALL_YIELD:
			exec_syscall_yield(cpu, &next_pc_address);
			break;

		default:
			fprintf(stderr,
					"FATAL ERROR: Invalid or Unknown opcode %ld encountered by entity %d at address %ld\n",
                    opcode_numeric,
                    cpu->curr_thread_id,
                    current_pc_address);
			exit(EXIT_FAILURE);
	}
	/*********************** EXECUTE PART ***********************/

	if (!cpu->is_halted) {
		mem_write(cpu->mem, REG_PC, next_pc_address, cpu->mode);
	}
	long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL);
	mem_write(cpu->mem, REG_INSTR_COUNT, current_instr_count + 1, KERNEL);

	/* WAKEUP COUNTDOWN - Her instruction sonrası BLOCKED thread'lerin wakeup değerlerini azalt */
	/* bu kod KERNEL modunda çalışmalı çünkü thread table'a erişiyor - ONLY for OS programs */
	if (is_os_program && !is_single_thread_program && (cpu->mode == KERNEL || cpu->mode == USER)) {
	    /* tüm thread'leri kontrol et */
	    for (int tid = 1; tid <= MAX_THREADS; tid++) {

	        long int state_addr = get_thread_table_addr(tid, 1);
	        long int state = mem_read(cpu->mem, state_addr, KERNEL);

	        if (state == THREAD_STATE_BLOCKED) {

	            long int wakeup_addr = get_thread_table_addr(tid, 7);
	            long int wakeup = mem_read(cpu->mem, wakeup_addr, KERNEL);

	            if (wakeup > 0) {
	                wakeup--;
	                mem_write(cpu->mem, wakeup_addr, wakeup, KERNEL);

	                #ifdef DEBUG_FLAG
	                	printf("Thread %d wakeup countdown: %ld\n", tid, wakeup);
	                #endif

	                if (wakeup == 0) {
	                    mem_write(cpu->mem, state_addr, THREAD_STATE_READY, KERNEL);
	                    printf("Thread %d unblocked (wakeup reached 0)\n", tid);
	                }
	            }
	        }
	    }
	}
}

/**
 * @brief Checks if the CPU is halted.
 *
 * This function returns the CPU's halted state after performing validation checks on the
 * CPU structure.
 *
 * @param cpu Pointer to the CPU structure.
 * @return True if the CPU is halted, false otherwise.
 */
bool
cpu_is_halted(const CPU * cpu)
{
	check_cpu(cpu, __func__);
	return cpu->is_halted;
}

/**
 * @brief Dumps the CPU register contents for debugging.
 *
 * This function prints the values of the program counter (PC), stack pointer (SP), and
 * instruction count registers for the current thread and mode.
 *
 * @param cpu Pointer to the CPU structure.
 */
void
cpu_dump_registers(const CPU * cpu)
{
	check_cpu(cpu, __func__);
    printf("CPU Registers for Thread %d (Mode: %s):\n",
           cpu->curr_thread_id, cpu->mode == KERNEL ? "KERNEL" : "USER");
    printf("REG_PC: %ld\n", mem_read(cpu->mem, REG_PC, cpu->mode));
    printf("REG_SP: %ld\n", mem_read(cpu->mem, REG_SP, cpu->mode));
    printf("REG_INSTR_COUNT: %ld\n", mem_read(cpu->mem, REG_INSTR_COUNT, cpu->mode));
}
