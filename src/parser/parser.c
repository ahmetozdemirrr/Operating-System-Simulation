/* src/parser.c */

#include <parser.h>

bool is_multi_section_program = false; /* Global variable to track program type */
bool is_os_program = false;            /* Global variable for OS program detection */
bool is_single_thread_program = false; /* Global variable for single thread program detection */

static int total_sections_count = 0; /* Section counter */

/**
 * @brief Resets program type detection variables.
 *
 * This function should be called before parsing a new program file to reset
 * the global state for program type detection.
 */
void
parser_reset(void)
{
    is_multi_section_program = false;
    is_os_program = false;
    is_single_thread_program = false;
    total_sections_count = 0;
}

/**
 * @brief Trims leading and trailing whitespace from a string in-place.
 *
 * This function removes leading and trailing whitespace characters (as defined by
 * isspace) from the input string by modifying it in-place. It adjusts the string by
 * shifting the content to skip leading whitespace and null-terminating after the last
 * non-whitespace character. The function is static, so it is only visible within this
 * translation unit. If the input is NULL, NULL is returned.
 *
 * @param str Pointer to the null-terminated string to trim.
 * @return Pointer to the trimmed string (adjusted for leading whitespace), or NULL if
 * input is NULL.
 */
static char *
trim_whitespace(char * str)
{
	if (str == NULL) {
		return NULL;
	}
	int iter_begin = 0;

	while (isspace(str[iter_begin])) {
		iter_begin++;
	}
	int iter_end = strlen(str) - 1;

	while (iter_end >= iter_begin && isspace(str[iter_end])) {
		iter_end--;
	}

	str[iter_end + 1] = '\0';
	return str + iter_begin;
}

/**
 * @brief Parses a data definition line from the assembly file and writes it to memory.
 *
 * This function processes a line from the data section of the assembly file, expecting
 * a format of "<address> <value>", where <address> is the memory location (offset within
 * the entity) and <value> is the data to be stored. The line is assumed to be pre-cleaned
 * by trim_whitespace. The actual memory address is calculated as base_addr + address.
 *
 * @param line Pointer to the null-terminated string containing the data line.
 * @param mem Pointer to the Memory structure where the data will be written.
 * @param base_addr The base address of the current entity's data section.
 *
 * @return 0 on success, -1 on failure (e.g., NULL pointers, invalid address/value,
 *         memory allocation failure, or extra tokens).
 *
 * @note The function allocates memory for a copy of the input line using strdup,
 *       which must be freed before returning. Invalid numeric inputs are detected
 *       using strtol with endptr. Extra tokens beyond <address> <value> result in
 *       an error.
 */
static int
parse_data_line(const char * line, Memory * mem, long int base_addr, long int current_entity_id)
{
	if (line == NULL || mem == NULL) {
		fprintf(stderr, "ERROR: NULL line or memory pointer\n");
		return -1;
	}

	/* 1. address */
	char * copy_line = strdup(line);
	if (copy_line == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed at \"parse_data_line\"\n");
		return -1;
	}

	/* find the # character and cut the comment */
	char * comment_start = strchr(copy_line, '#');
	if (comment_start != NULL) {
		*comment_start = '\0'; /* protect the before # part, remain is comment */
		trim_whitespace(copy_line);
	}

	char * token = strtok(copy_line, " \t");
	if (token == NULL) {
		fprintf(stderr, "ERROR: Missing address for data definition\n");
		free(copy_line);
		return -1;
	}

	char * endptr;
	long int address_offset = strtol(token, &endptr, 10);
	if (endptr == token || *endptr != '\0') {
		fprintf(stderr, "ERROR: Invalid address offset: %s\n", token);
		free(copy_line);
		return -1;
	}
	long int max_data_size = (current_entity_id == OS_ID) ? OS_USABLE_DATA_SIZE : ENTITY_DATA_SIZE;

	/* Check if the offset is within bounds */
	if (address_offset < 0 || address_offset >= max_data_size) {
		fprintf(stderr, "ERROR: Data address offset %ld out of bounds (0 to %ld)\n",
				address_offset, max_data_size - 1);
		free(copy_line);
		return -1;
	}
	long int address = base_addr + address_offset;

	/* 2. value */
	token = strtok(NULL, " \t");
	if (token == NULL) {
		fprintf(stderr, "ERROR: Missing value for data definition\n");
		free(copy_line);
		return -1;
	}

	long int value = strtol(token, &endptr, 10);
	if (endptr == token || *endptr != '\0') {
		fprintf(stderr, "ERROR: Invalid value: %s\n", token);
		free(copy_line);
		return -1;
	}

	/* 3. checking extra token */
	token = strtok(NULL, " \t");
	if (token != NULL) {
		fprintf(stderr, "ERROR: Unexpected extra tokens: %s\n", token);
		free(copy_line);
		return -1;
	}
	/* write to mem */
	mem_write(mem, address, value, KERNEL);
	free(copy_line);

	return 0;
}

/**
 * @brief Parses an instruction line from the assembly file and writes it to memory.
 *
 * This function processes a line from the instruction section of the assembly file,
 * expecting a format of "<index> <MNEMONIC> [OPERAND1] [OPERAND2] [# comment]", where
 * <index> is the logical instruction index, <MNEMONIC> is the operation code (e.g., SET,
 * SYSCALL), [OPERAND1] [OPERAND2] are optional operands, and [# comment] is an optional
 * comment. The line is assumed to be pre-cleaned by trim_whitespace. The instruction
 * is written to the memory starting at instr_base_addr with an offset based on
 * curr_insr_index.
 *
 * @param line Pointer to the null-terminated string containing the instruction line.
 * @param mem Pointer to the Memory structure where the instruction will be written.
 * @param curr_insr_index The logical index of the current instruction (0, 1, 2, ...),
 *                        used to calculate the memory offset.
 * @param instr_base_addr The base address of the current entity's instruction section.
 * @param used_indices Pointer to an array tracking used instruction indices.
 * @param used_indices_size Size of the used_indices array.
 *
 * @return 0 on success, -1 on failure (e.g., NULL pointers, invalid index/mnemonic/operands,
 *         memory allocation failure, duplicate index, or non-sequential index).
 *
 * @note The function allocates memory for a copy of the input line using strdup,
 *       which must be freed before returning. The index is validated to ensure it
 *       matches curr_insr_index and is not a duplicate. SYSCALL instructions require a
 *       sub-instruction (e.g., PRN, HLT). Operand count is validated based on the
 *       mnemonic using the Opcode enum.
 */
static int
parse_insr_line(const char * line, Memory * mem, int curr_insr_index, long int instr_base_addr, bool * used_indices, int used_indices_size)
{
	if (line == NULL || mem == NULL || used_indices == NULL) {
		fprintf(stderr, "ERROR: NULL line, memory pointer, or used_indices array\n");
		return -1;
	}
	char * copy_line = strdup(line);

	if (copy_line == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed at \"parse_insr_line\"\n");
		return -1;
	}

	/* find the # character and cut the comment */
	char * comment_start = strchr(copy_line, '#');
	if (comment_start != NULL) {
		*comment_start = '\0'; /* protect the before # part, remain is comment */
		trim_whitespace(copy_line);
	}

	char * token = strtok(copy_line, " \t");
	if (token == NULL) {
		free(copy_line);
		return -1;
	}

	/* 1 - index of instruction */
	char * endptr;
	long int instruction_label_val = strtol(token, &endptr, 10);
	if (endptr == token || *endptr != '\0') {
		fprintf(stderr, "ERROR: Invalid instruction index: %s\n", token);
		free(copy_line);
		return -1;
	}

	/* Check if index is already used */
	if (instruction_label_val >= 0 && instruction_label_val < used_indices_size) {
		if (used_indices[instruction_label_val]) {
			fprintf(stderr, "ERROR: Duplicate instruction index %ld detected\n", instruction_label_val);
			free(copy_line);
			return -1;
		}
		used_indices[instruction_label_val] = true;
	} else {
		fprintf(stderr, "ERROR: Instruction index %ld out of valid range (0 to %d)\n",
				instruction_label_val, used_indices_size - 1);
		free(copy_line);
		return -1;
	}

	/* Check if index is equal to curr_insr_index (ensures sequential order) */
	if (instruction_label_val != curr_insr_index) {
		fprintf(stderr, "ERROR: Instruction index %ld is not in sequential order (expected %d)\n",
				instruction_label_val, curr_insr_index);
		free(copy_line);
		return -1;
	}

	/* 2 - mnemonic */
	token = strtok(NULL, " \t");

	if (token == NULL) {
		fprintf(stderr, "ERROR: Missing mnemonic\n");
		free(copy_line);
		return -1;
	}
	Opcode opcode = OPCODE_UNKNOWN;

	int is_syscall = 0;

	if (strcmp(token, "SET") == 0) {
		opcode = OPCODE_SET;
	} else if (strcmp(token, "CPY") == 0) {
		opcode = OPCODE_CPY;
	} else if (strcmp(token, "CPYI") == 0) {
		opcode = OPCODE_CPYI;
	} else if (strcmp(token, "CPYI2") == 0) {
		opcode = OPCODE_CPYI2;
	} else if (strcmp(token, "ADD") == 0) {
		opcode = OPCODE_ADD;
	} else if (strcmp(token, "ADDI") == 0) {
		opcode = OPCODE_ADDI;
	} else if (strcmp(token, "SUBI") == 0) {
		opcode = OPCODE_SUBI;
	} else if (strcmp(token, "JIF") == 0) {
		opcode = OPCODE_JIF;
	} else if (strcmp(token, "PUSH") == 0) {
		opcode = OPCODE_PUSH;
	} else if (strcmp(token, "POP") == 0) {
		opcode = OPCODE_POP;
	} else if (strcmp(token, "CALL") == 0) {
		opcode = OPCODE_CALL;
	} else if (strcmp(token, "RET") == 0) {
		opcode = OPCODE_RET;
	} else if (strcmp(token, "HLT") == 0) {
		opcode = OPCODE_HLT;
	} else if (strcmp(token, "USER") == 0) {
		opcode = OPCODE_USER;
	} else if (strcmp(token, "SYSCALL") == 0) {
		is_syscall = 1;
	} else {
		fprintf(stderr, "ERROR: Unknown mnemonic: %s\n", token);
		free(copy_line);
		return -1;
	}

	if (is_syscall) {
		token = strtok(NULL, " \t");

		if (token == NULL) {
			fprintf(stderr, "ERROR: Missing SYSCALL sub-instruction\n");
			free(copy_line);
			return -1;
		}
		if (strcmp(token, "PRN") == 0) {
			opcode = OPCODE_SYSCALL_PRN;
		} else if (strcmp(token, "HLT") == 0) {
			opcode = OPCODE_SYSCALL_HLT;
		} else if (strcmp(token, "YIELD") == 0) {
			opcode = OPCODE_SYSCALL_YIELD;
		} else {
			fprintf(stderr, "ERROR: Unknown SYSCALL sub-instruction: %s\n", token);
			free(copy_line);
			return -1;
		}
	}

	/* 3. Operands */
	long int operands[MAX_OPERANDS] = {0};
	int num_operands = 0;

	while ((token = strtok(NULL, " \t")) != NULL && num_operands < MAX_OPERANDS)
	{
		char * endptr;
		operands[num_operands] = strtol(token, &endptr, 10);

		if (endptr == token || *endptr != '\0') {
			fprintf(stderr, "ERROR: Invalid operand: %s\n", token);
			free(copy_line);
			return -1;
		}
		num_operands++;
	}

	int expected_operands;

	switch (opcode)
	{
		case OPCODE_SET:
		case OPCODE_CPY:
		case OPCODE_CPYI:
		case OPCODE_CPYI2:
		case OPCODE_ADD:
		case OPCODE_ADDI:
		case OPCODE_SUBI:
		case OPCODE_JIF:
			expected_operands = 2;
			break;
		case OPCODE_PUSH:
		case OPCODE_POP:
		case OPCODE_CALL:
		case OPCODE_USER:
		case OPCODE_SYSCALL_PRN:
			expected_operands = 1;
			break;
		case OPCODE_RET:
		case OPCODE_HLT:
		case OPCODE_SYSCALL_HLT:
		case OPCODE_SYSCALL_YIELD:
			expected_operands = 0;
			break;
		default:
			fprintf(stderr, "ERROR: Unknown opcode\n");
			free(copy_line);
			return -1;
	}

	if (num_operands != expected_operands) {
		fprintf(stderr, "ERROR: Expected %d operands, got %d for opcode %d\n",
				expected_operands, num_operands, opcode);
		free(copy_line);
		return -1;
	}
	/* Calculate the address to write the instruction */
	long int addr = instr_base_addr + curr_insr_index * INSTR_SIZE;
	mem_write(mem, addr, (long int)opcode, KERNEL);

	for (int i = 0; i < num_operands; i++) {
		mem_write(mem, addr + 1 + i, operands[i], KERNEL);
	}
	free(copy_line);
	return 0;
}

/**
 * @brief First pass: Detects program type by counting sections
 *
 * This function performs a first pass through the file to detect whether it's
 * a single-thread program or a multi-section OS program. Single-thread programs
 * have exactly 2 sections (one data, one instruction), while OS programs have
 * multiple sections for different entities.
 *
 * @param filename Path to the assembly file.
 * @return 0 on success, -1 on failure.
 */
static int
detect_program_type(const char * filename)
{
	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Could not open file for type detection: %s\n", filename);
		return -1;
	}

	char * line_buffer = (char *)malloc(sizeof(char) * ASM_LINE_BUFFER_SIZE);
	if (line_buffer == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed in detect_program_type\n");
		fclose(file);
		return -1;
	}

	int section_count = 0;

	while (fgets(line_buffer, ASM_LINE_BUFFER_SIZE, file) != NULL) {
		char * clean_line = trim_whitespace(line_buffer);
		if (strlen(clean_line) == 0 || clean_line[0] == '#') {
			continue;
		}

		if (strcmp(clean_line, "Begin Data Section") == 0 ||
		    strcmp(clean_line, "Begin Instruction Section") == 0) {
			section_count++;
		}
	}

	/* Program type detection based on section count */
	if (section_count == 2) {
		/* Single thread program: exactly 2 sections (1 data + 1 instruction) */
		is_single_thread_program = true;
		is_os_program = false;
		is_multi_section_program = false;
		printf("PARSER: Detected SINGLE THREAD program (2 sections)\n");
	} else if (section_count > 2) {
		/* Multi-section OS program */
		is_single_thread_program = false;
		is_os_program = true;
		is_multi_section_program = true;
		printf("PARSER: Detected OS program (%d sections)\n", section_count);
	} else {
		fprintf(stderr, "ERROR: Invalid program structure (%d sections). Expected at least 2.\n", section_count);
		free(line_buffer);
		fclose(file);
		return -1;
	}

	total_sections_count = section_count;

	free(line_buffer);
	fclose(file);
	return 0;
}

/**
 * @brief Loads an assembly program from a file into memory using a DFA-based parser.
 *
 * This function reads an assembly file containing OS and thread definitions in a single file.
 * It parses the contents into instructions and data using a DFA, tracking the current entity
 * (OS or thread) being processed. Each entity (OS or thread) has a data section (256 elements)
 * and an instruction section. The memory layout is as follows:
 * - Registers: 0-19
 * - OS Data: 20-275
 * - OS Instructions: 276-999
 * - Thread 1: base=1000, Data: 1000-1255, Instructions: 1256-1999
 * - Thread 2: base=2000, Data: 2000-2255, Instructions: 2256-2999
 * - ... up to Thread 10.
 *
 * For single thread programs, the program is loaded into Thread 2's space to avoid
 * conflicts with registers (0-19) and OS space.
 *
 * @param filename Path to the assembly file.
 * @param mem Pointer to the Memory structure where the program will be loaded.
 *
 * @return 0 on success, -1 on failure (e.g., file errors, parsing failures, or entity limits exceeded).
 */
int
load_program_from_file(const char * filename, Memory * mem)
{
	if (filename == NULL || mem == NULL) {
		fprintf(stderr, "ERROR: NULL filename or memory pointer\n");
		return -1;
	}

	/* Reset parser state */
	parser_reset();

	/* First pass: detect program type */
	if (detect_program_type(filename) != 0) {
		fprintf(stderr, "ERROR: Failed to detect program type\n");
		return -1;
	}

	FILE * file = fopen(filename, "r");
	if (file == NULL) {
		fprintf(stderr, "ERROR: Could not open file: %s\n", filename);
		return -1;
	}

	char * line_buffer = (char *)malloc(sizeof(char) * ASM_LINE_BUFFER_SIZE);
	if (line_buffer == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed at \"load_program_from_file\"\n");
		fclose(file);
		return -1;
	}

	/* Initialize array to track used instruction indices */
	bool * used_indices = (bool *)calloc(ENTITY_INSTRUCTION_SIZE, sizeof(bool));
	if (used_indices == NULL) {
		fprintf(stderr, "ERROR: Memory allocation failed for used_indices array\n");
		free(line_buffer);
		fclose(file);
		return -1;
	}

	Parser_State state = INITIAL_CONTEXT;
	int curr_insr_index = 0; /* Reset for each entity's instruction section */
	int data_count = 0; /* Reset for each entity's data section */
	int line_number = 0;

	/* Determine starting entity and addresses based on program type */
	int current_entity_id;
	long int current_base_addr;
	long int current_instr_base_addr;

	if (is_single_thread_program) {
		/* Single thread: use Thread 2's space to avoid register conflicts */
		current_entity_id = 2;
		current_base_addr = THREAD_DATA_START(2);
		current_instr_base_addr = THREAD_INSTR_START(2);
		printf("PARSER: Loading single thread program into Thread 2 space (base: %ld)\n", current_base_addr);
	} else {
		/* Multi-section OS program: start with OS */
		current_entity_id = OS_ID;
		current_base_addr = OS_DATA_START_ADDR;
		current_instr_base_addr = OS_INSTRUCTION_START_ADDR;
		printf("PARSER: Loading OS program starting with OS entity\n");
	}

	while (fgets(line_buffer, ASM_LINE_BUFFER_SIZE, file) != NULL)
	{
		line_number++;

		char * clean_line = trim_whitespace(line_buffer);
		if (strlen(clean_line) == 0 || clean_line[0] == '#') {
			continue;
		}

		switch (state)
		{
			case INITIAL_CONTEXT:
				if (strcmp(clean_line, "Begin Data Section") == 0) {
					state = DATA_CONTEXT;
					data_count = 0; /* Reset data count for new entity */
				}
				else if (strcmp(clean_line, "Begin Instruction Section") == 0) {
					state = INSTRUCTION_CONTEXT;
					curr_insr_index = 0; /* Reset instruction index for new entity */
					memset(used_indices, 0, ENTITY_INSTRUCTION_SIZE * sizeof(bool)); /* Reset used indices */
				}
				else {
					fprintf(stderr, "ERROR at line %d: Invalid initial line: %s\n", line_number, clean_line);
					state = ERROR_CONTEXT;
				}
				break;

			case INSTRUCTION_CONTEXT:
				if (strcmp(clean_line, "End Instruction Section") == 0) {
					long int end_addr = current_instr_base_addr + curr_insr_index * INSTR_SIZE;
                    long int max_end_addr = (current_entity_id == OS_ID) ? OS_BLOCK_END_ADDR : THREAD_BLOCK_END(current_entity_id);
                    if (end_addr <= max_end_addr) {
                        mem_write(mem, end_addr, -1, KERNEL); /* as a separator -1 */
                    }

					state = INITIAL_CONTEXT;

					/* For single thread programs, don't increment entity */
					if (!is_single_thread_program) {
						current_entity_id++; /* Move to the next entity */

						if (current_entity_id > MAX_PROGRAM_ENTITIES) {
							fprintf(stderr, "ERROR at line %d: Maximum number of entities (%d) exceeded\n",
									line_number, MAX_PROGRAM_ENTITIES);
							free(line_buffer);
							free(used_indices);
							fclose(file);
							return -1;
						}
						/* Update base addresses for the next entity */
						if (current_entity_id <= MAX_THREADS) {
							current_base_addr = THREAD_BASE_ADDR(current_entity_id);
							current_instr_base_addr = THREAD_INSTR_START(current_entity_id);
						}
					}
				}
				else if (strcmp(clean_line, "Begin Data Section") == 0 ||
						 strcmp(clean_line, "End Data Section") == 0) {
					fprintf(stderr, "ERROR at line %d: Unexpected section marker %s before End Instruction Section\n", line_number, clean_line);
					state = ERROR_CONTEXT;
				}
				else if (parse_insr_line(clean_line, mem, curr_insr_index, current_instr_base_addr, used_indices, ENTITY_INSTRUCTION_SIZE) != 0) {
					fprintf(stderr, "ERROR at line %d: Failed to parse instruction line: %s\n", line_number, clean_line);
					free(line_buffer);
					free(used_indices);
					fclose(file);
					return -1;
				}
				else {
					/* Check if the instruction exceeds the entity's instruction block */
					long int instr_end_addr = current_instr_base_addr + (curr_insr_index + 1) * INSTR_SIZE - 1;
					long int max_instr_end_addr = (current_entity_id == OS_ID) ? OS_BLOCK_END_ADDR : THREAD_BLOCK_END(current_entity_id);
					if (instr_end_addr > max_instr_end_addr) {
						fprintf(stderr, "ERROR at line %d: Instruction at index %d exceeds entity's instruction block (ends at %ld)\n",
								line_number, curr_insr_index, max_instr_end_addr);
						free(line_buffer);
						free(used_indices);
						fclose(file);
						return -1;
					}
					curr_insr_index++;
				}
				break;

			case DATA_CONTEXT:
				if (strcmp(clean_line, "End Data Section") == 0) {
					state = INITIAL_CONTEXT;
				}
				else if (strcmp(clean_line, "Begin Instruction Section") == 0 ||
						 strcmp(clean_line, "End Instruction Section") == 0) {
					fprintf(stderr, "ERROR at line %d: Unexpected section marker %s before End Data Section\n", line_number, clean_line);
					state = ERROR_CONTEXT;
				}
				else if (parse_data_line(clean_line, mem, current_base_addr, current_entity_id) != 0) {
					fprintf(stderr, "ERROR at line %d: Failed to parse data line: %s\n", line_number, clean_line);
					free(line_buffer);
					free(used_indices);
					fclose(file);
					return -1;
				}
				else {
					data_count++;
					long int max_data_count = (current_entity_id == OS_ID) ? OS_USABLE_DATA_SIZE : ENTITY_DATA_SIZE;

					if (data_count > max_data_count) {
						fprintf(stderr, "ERROR at line %d: Maximum data lines (%ld) exceeded\n", line_number, max_data_count);
						free(line_buffer);
						free(used_indices);
						fclose(file);
						return -1;
					}
				}
				break;

			case ERROR_CONTEXT:
				fprintf(stderr, "ERROR: Parsing stopped due to previous errors at line %d\n", line_number);
				free(line_buffer);
				free(used_indices);
				fclose(file);
				return -1;
		}
	}

	/* Check if parsing ended in a valid state */
	if (state != INITIAL_CONTEXT) {
		fprintf(stderr, "ERROR at line %d: File ended without closing the current section\n", line_number);
		free(line_buffer);
		free(used_indices);
		fclose(file);
		return -1;
	}

	/* Validation checks based on program type */
	if (is_single_thread_program) {
		printf("PARSER: Single thread program loaded successfully into Thread 2 space\n");
	} else {
		if (current_entity_id < 1) {
			fprintf(stderr, "ERROR: At least one entity (OS) is required for multi-section program, but none were processed\n");
			free(line_buffer);
			free(used_indices);
			fclose(file);
			return -1;
		}
		printf("PARSER: OS program loaded successfully with %d entities\n", current_entity_id);
	}

	/* Debug: Program type detection result */
	#ifdef DEBUG_FLAG
		printf("PARSER: Program type detection complete:\n");
		printf("  - Single thread: %s\n", is_single_thread_program ? "YES" : "NO");
		printf("  - OS program: %s\n", is_os_program ? "YES" : "NO");
		printf("  - Multi-section: %s\n", is_multi_section_program ? "YES" : "NO");
		printf("  - Total sections: %d\n", total_sections_count);
	#endif

	free(line_buffer);
	free(used_indices);
	fclose(file);
	return 0;
}
