/* src/cpu.c */

#include <cpu.h>
#include <memory.h>

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

static void
check_data_address(const CPU * cpu, const long int relative_address, const char * context)
{
	long int max_data_size = ENTITY_DATA_SIZE;

    if (cpu->curr_thread_id == OS_ID) {
        max_data_size = OS_DATA_END_ADDR - OS_DATA_START_ADDR + 1;
    }

	if (relative_address < 0 || relative_address >= max_data_size) {
		fprintf(stderr,
				"FATAL ERROR: Entity %d (mode %d) %s: Relative source address %ld is out of bounds (0-%d).\n",
				cpu->curr_thread_id,
				cpu->mode,
				context,
				relative_address,
				max_data_size - 1);
		exit(EXIT_FAILURE);
	}
}

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

static void
exec_set(CPU * cpu, long int value, long int relative_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_address, "SET");

	long int absolute_addr = cpu->curr_data_base_for_active_entity + relative_address;
	mem_write(cpu->mem, absolute_addr, value, cpu->mode);
}

static void 
exec_cpy(CPU * cpu, long int relative_src_address, long int relative_dest_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_src_address,  "CPY");
	check_data_address(cpu, relative_dest_address, "CPY");

	long int absolute_src_address  = cpu->curr_data_base_for_active_entity +  relative_src_address;
	long int absolute_dest_address = cpu->curr_data_base_for_active_entity + relative_dest_address;

	mem_write(cpu->mem, absolute_dest_address, mem_read(cpu->mem, absolute_src_address, cpu->mode), cpu->mode);
}

static void 
exec_cpyi(CPU * cpu, long int relative_address_of_ptr, long int relative_dest_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_address_of_ptr, "CPYI");
	check_data_address(cpu, relative_dest_address,   "CPYI");

	long int pointer_absolute_address  = cpu->curr_data_base_for_active_entity + relative_address_of_ptr;
	long int absolute_dest_address     = cpu->curr_data_base_for_active_entity + relative_dest_address;

	long int indirect_src_absolute_address = mem_read(cpu->mem, pointer_absolute_address, cpu->mode);
	
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

static void
exec_add(CPU * cpu, long int relative_dest_address, long int value_to_add)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_dest_address, "ADD");

	long int absolute_dest_address = cpu->curr_data_base_for_active_entity + relative_dest_address;
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

static void
exec_addi(CPU * cpu, long int relative_dest_address, long int relative_src_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_dest_address, "ADDI");
	check_data_address(cpu, relative_src_address,  "ADDI");
	
	long int absolute_dest_address = cpu->curr_data_base_for_active_entity + relative_dest_address;
	long int absolute_src_address  = cpu->curr_data_base_for_active_entity + relative_src_address;

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

static void
exec_subi(CPU * cpu, long int relative_src_address, long int relative_dest_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_dest_address, "SUBI");
	check_data_address(cpu, relative_src_address,  "SUBI");
	
	long int absolute_dest_address = cpu->curr_data_base_for_active_entity + relative_dest_address;
	long int absolute_src_address  = cpu->curr_data_base_for_active_entity + relative_src_address;

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

static void
exec_jif(CPU * cpu, long int relative_condition_address, long int relative_new_pc_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_condition_address, "JIF");
	check_instruction_address(cpu, relative_new_pc_address, "JIF");

	long int absolute_condition_address = cpu->curr_data_base_for_active_entity + relative_condition_address;
	long int condition_value = mem_read(cpu->mem, absolute_condition_address, cpu->mode);

	if (condition_value <= 0) {
		/* Each instruction takes INSTR_SIZE (3) bytes, so multiply relative_new_pc_address by INSTR_SIZE */
		long int absolute_instruction_address = cpu->curr_instruction_base_for_active_entity + (relative_new_pc_address * INSTR_SIZE);
		mem_write(cpu->mem, REG_PC, absolute_instruction_address, cpu->mode);
	}
}

static void
exec_push(CPU * cpu, long int relative_element_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_element_address, "PUSH");

	long int absolute_element_address = cpu->curr_data_base_for_active_entity + relative_element_address;
	long int element_value = mem_read(cpu->mem, absolute_element_address, cpu->mode);

	long int target_address = mem_read(cpu->mem, REG_SP, cpu->mode);

	if (target_address == 0 || target_address > THREAD_BLOCK_END(cpu->curr_thread_id)) {
        target_address = THREAD_BLOCK_END(cpu->curr_thread_id);
        mem_write(cpu->mem, REG_SP, target_address, cpu->mode);
    }

    if (target_address - 1 < 0) {
        fprintf(stderr, "FATAL ERROR: Stack overflow in PUSH for entity %d\n", cpu->curr_thread_id);
        exit(EXIT_FAILURE);
    }
	mem_write(cpu->mem, target_address, element_value, cpu->mode);
	mem_write(cpu->mem, REG_SP, target_address - 1, cpu->mode);
}

static void
exec_pop(CPU * cpu, long int relative_dest_address)
{
	check_cpu(cpu, __func__);
	check_data_address(cpu, relative_dest_address, "POP");

	long int element_adress = mem_read(cpu->mem, REG_SP, cpu->mode);

	if (element_address >= THREAD_BLOCK_END(cpu->curr_thread_id)) {
        fprintf(stderr, "FATAL ERROR: Stack underflow in POP for entity %d\n", cpu->curr_thread_id);
        exit(EXIT_FAILURE);
    }
	long int element = mem_read(cpu->mem, element_adress, cpu->mode);

	long int absolute_dest_address = cpu->curr_data_base_for_active_entity + relative_dest_address;

	mem_write(cpu->mem, absolute_dest_address, element, cpu->mode);
	mem_write(cpu->mem, REG_SP, element_adress + 1, cpu->mode);
}

static void
exec_call()
{

}

static void 
exec_ret()
{

}

static void
exec_hlt()
{

}

static void
exec_user()
{

}

static void
exec_syscall_prn()
{

}

static void
exec_syscall_hlt()
{

}

static void
exec_syscall_yield()
{

}

void 
cpu_init(CPU * cpu, Memory * mem)
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

	mem_write(mem, REG_PC, cpu->curr_instruction_base_for_active_entity, cpu->mode);
	mem_write(mem, REG_SP, OS_BLOCK_END_ADDR, cpu->mode); /* 999: from up to bottom */
	mem_write(mem, REG_INSTR_COUNT, 0, cpu->mode);
}

void 
cpu_execute_instruction(CPU * cpu)
{
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

	/*********************** DECODE PART ***********************/
	long int opcode_addr = current_pc_address;
	long int operand_1_addr = current_pc_address + 1;
	long int operand_2_addr = current_pc_address + 2;

	long int opcode_numeric = mem_read(cpu->mem, opcode_addr,    cpu->mode);
	Opcode opcode = (Opcode)opcode_numeric;
	long int operand_1  = mem_read(cpu->mem, operand_1_addr, cpu->mode);
	long int operand_2  = mem_read(cpu->mem, operand_2_addr, cpu->mode);
	/*********************** DECODE PART ***********************/
	
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
			exec_jif(cpu, operand_1, operand_2);
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
			exec_user(cpu);
			break;

		case OPCODE_SYSCALL_PRN:
			exec_syscall_prn(cpu, operand_1);
			break;

		case OPCODE_SYSCALL_HLT:
			exec_syscall_hlt(cpu);
			break;

		case OPCODE_SYSCALL_YIELD:
			exec_syscall_yield(cpu);
			break;

		case OPCODE_UNKNOWN:
			fprintf(stderr, "FATAL ERROR: Invalid opcode reading from memory! %ld\n", opcode);
			cpu->is_halted = true;
			exit(EXIT_FAILURE);
	}
	/*********************** EXECUTE PART ***********************/

	if (!cpu->is_halted) {
		mem_write(cpu->mem, REG_PC, next_pc_address, KERNEL);
	}
	long int current_instr_count = mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL);
	mem_write(cpu->mem, REG_INSTR_COUNT, mem_read(cpu->mem, REG_INSTR_COUNT, KERNEL) + 1, KERNEL);
}

bool 
cpu_is_halted(const CPU * cpu)
{

}

void 
cpu_dump_registers(const CPU * cpu)
{

}

void 
cpu_set_context(CPU * cpu, int thread_id, long int data_base, long int instruction_base, CPU_mode initial_mode)
{

}
