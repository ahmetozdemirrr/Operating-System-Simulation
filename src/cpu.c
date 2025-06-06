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

    /* src_value da relative bir adres olabilir, onu da dönüştür */
    long int actual_src_data_address = (cpu->curr_thread_id == OS_ID && src_value < 21) ?
        src_value : cpu->curr_data_base_for_active_entity + src_value;

    /* A2 -> M[A2] */
    long int absolute_dest_ptr_address = (cpu->curr_thread_id == OS_ID && relative_dest_ptr_address < 21) ?
        relative_dest_ptr_address : cpu->curr_data_base_for_active_entity + relative_dest_ptr_address;
    long int dest_value = mem_read(cpu->mem, absolute_dest_ptr_address, cpu->mode);

    /* dest_value da relative bir adres olabilir, onu da dönüştür */
    long int actual_dest_data_address = (cpu->curr_thread_id == OS_ID && dest_value < 21) ?
        dest_value : cpu->curr_data_base_for_active_entity + dest_value;

    #ifdef DEBUG_FLAG
	    if (cpu->curr_thread_id == OS_ID && cpu->mode == KERNEL) {
	        /* Thread table aralığını kontrol et (200-287 arası) */
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

static void
exec_hlt(CPU * cpu)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("HLT\n");
	#endif

	check_cpu(cpu, __func__);
	cpu->is_halted = true;
}

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

        /* Bu pek olası değil USER komutuyla ama kontrol edilebilir */
	    if (target_thread_id == OS_ID) {
	        cpu->mode = KERNEL;
	    } else {
	        cpu->mode = USER;
	    }
        /* stack Pointer'ı ayarla (KERNEL ayrıcalığıyla, çünkü REG_SP özel bir register) */
        mem_write(cpu->mem, REG_SP, target_sp, KERNEL);

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

static void
exec_syscall_prn(CPU * cpu, long int source_address, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("SYSCALL PRN: %ld\n", source_address);
	#endif

    check_cpu(cpu, __func__);

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
    printf("\n\n\n\n\n\nSYSCALL PRN output: %ld\n\n\n\n\n\n", value);

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
}

static void
exec_syscall_hlt(CPU * cpu, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
    	printf("SYSCALL HLT\n");
    #endif

    check_cpu(cpu, __func__);

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
}

static void
exec_syscall_yield(CPU * cpu, long int * next_pc_address)
{
	#ifdef INSTRUCTION_FLOW_DFLAG
		printf("SYSCALL YIELD\n");
	#endif

    check_cpu(cpu, __func__);

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
}

static long int get_thread_table_addr(int thread_id, int field)
{
    return 220 + (thread_id * 8) + field;  /* Thread table 220'den başlıyor (absolute) */
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

	mem_write(mem, REG_PC, cpu->curr_instruction_base_for_active_entity, cpu->mode); /* sistem ilk başladığında OS entry pointten çalışsın */
	mem_write(mem, REG_SP, OS_BLOCK_END_ADDR, cpu->mode); /* 1999: from up to bottom */
	mem_write(mem, REG_INSTR_COUNT, 0, cpu->mode);
	/*
		Context switch sinyalini başlangıçta "işlem tamamlandı" olarak ayarla.
    	Bu yazma işlemi KERNEL modunda yapılmalı, çünkü bu OS'nin kontrol edeceği bir register.
    */
    mem_write(mem, REG_CONTEXT_SWITCH_SIGNAL, CTX_SWITCH_DONE, KERNEL);
}

void
cpu_execute_instruction(CPU * cpu)
{
	/*
		Context Switch Sinyalini Kontrol Et (Her komut döngüsünün başında): Bu register OS tarafından KERNEL modunda
		set edildiği için KERNEL modunda okunmalı. Eğer o an USER modda bir thread çalışıyorsa bile, bu kontrol CPU
		simülatörünün kendi iç mantığıdır ve OS'nin bıraktığı bir sinyali okur. Güvenlik açısından, bu okuma ve sonraki
		işlemlerin KERNEL ayrıcalığıyla yapıldığını varsayabiliriz.
    */
    volatile long int ctx_signal = mem_read(cpu->mem, REG_CONTEXT_SWITCH_SIGNAL, KERNEL);

    if (ctx_signal == CTX_SWITCH_REQUEST) {
        /*
	        OS'nin context switch isteğinde bulunduğunu logla.
	        Bu loglama, cpu->curr_thread_id OS iken daha anlamlı olur,
	        çünkü scheduler OS'nin bir parçasıdır. Ancak sinyal herhangi bir anda tespit edilebilir.
        */
        printf("CPU: Context Switch Request DETECTED (Signal: %ld) by OS (presumably).\n", ctx_signal);

        /* posta kutusundan yeni context bilgilerini KERNEL modunda oku, çünkü OS bunları KERNEL modunda yazdı. */
        int next_tid =    (int)mem_read(cpu->mem, ADDR_MAILBOX_NEXT_THREAD_ID,   KERNEL);
        long int next_state  = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_STATE,       KERNEL);
        long int next_pc_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_PC,          KERNEL);
        long int next_sp_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_SP,          KERNEL);
        long int next_db_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_DATA_BASE,   KERNEL);
        long int next_ib_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_INSTR_BASE,  KERNEL);
        long int next_ic_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_INSTR_COUNT, KERNEL);
        long int next_wc_val = mem_read(cpu->mem, ADDR_MAILBOX_NEXT_WAKEUP_CNT,  KERNEL);

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
        printf("CPU: Switching to Thread ID: %d, State: %s, PC: %ld, SP: %ld, DB: %ld, IB: %ld, IC: %ld, WC: %ld\n",
               next_tid,
               state,
               next_pc_val,
               next_sp_val,
               next_db_val,
               next_ib_val,
               next_ic_val,
               next_wc_val);

        /* CPU context'ini doğrudan güncelle */
        cpu->curr_thread_id = next_tid;
        cpu->curr_data_base_for_active_entity = next_db_val;
        cpu->curr_instruction_base_for_active_entity = next_ib_val;

        if (next_tid == OS_ID) {
		    cpu->mode = KERNEL;
		} else {
		    cpu->mode = USER;
		}
        mem_write(cpu->mem, REG_PC, next_pc_val, KERNEL);
        mem_write(cpu->mem, REG_SP, next_sp_val, KERNEL);

        /* sinyali "işlendi" olarak işaretle */
        mem_write(cpu->mem, REG_CONTEXT_SWITCH_SIGNAL, CTX_SWITCH_DONE, KERNEL);
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
	/* bu kod KERNEL modunda çalışmalı çünkü thread table'a erişiyor */
	if (cpu->mode == KERNEL || cpu->mode == USER) {
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

bool
cpu_is_halted(const CPU * cpu)
{
	check_cpu(cpu, __func__);
	return cpu->is_halted;
}

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
