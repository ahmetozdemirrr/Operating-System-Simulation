/* src/parser.c */

#include <parser.h>

int 
load_program_from_file(const char * filename, Memory * mem)
{

}

static int 
parse_data_line(const char * line, Memory * mem)
{

}

static int 
parse_insr_line(const char * line, Memory * mem, int curr_insr_index)
{
	if (line == NULL || mem == NULL) {
		fprintf(stderr, "ERROR: NULL line or memory pointer\n");
        return -1;
	}
	char * copy_line = strdup(line);
	
	if (copy_line == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        return -1;
    }

    char * token = strtok(copy_line, " \t");

    if (token == NULL) {
        free(copy_line);
        return -1;
    }
    /* 1 - index of instruction */
    long int instruction_label_val = strtol(token, NULL, 10);

    if (instruction_label_val != curr_insr_index) {
        fprintf(stderr, "WARNING: Instruction index %ld does not match expected %d\n",
                instruction_label_val, curr_insr_index);
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
        case OPCODE_ADD:
        case OPCODE_ADDI:
        case OPCODE_SUBI:
        case OPCODE_JIF:
            expected_operands = 2;
            break;
        case OPCODE_PUSH:
        case OPCODE_POP:
        case OPCODE_CALL:
        case OPCODE_SYSCALL_PRN:
            expected_operands = 1;
            break;
        case OPCODE_RET:
        case OPCODE_HLT:
        case OPCODE_USER:
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
    long int addr = INSTRUCTION_AREA_START + curr_insr_index * (1 + MAX_OPERANDS);
    mem_write(mem, addr, (long int)opcode, KERNEL);
    
    for (int i = 0; i < num_operands; i++) {
        mem_write(mem, addr + 1 + i, operands[i], KERNEL);
    }
    free(copy_line);
    return 0;
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
	
	if (iter_end >= iter_begin) {
        str[iter_end + 1] = '\0';
    } 

    else {
        str[iter_begin] = '\0';
    }
	return str + iter_begin;
}

/**
 * @brief Removes comment lines (starting with '#') from an assembly file.
 *
 * Reads the input file, skips lines starting with '#', and writes the remaining
 * lines to a temporary file, then replaces the original file.
 *
 * @param filename Path to the assembly file.
 */
static void 
remove_comment(const char * filename)
{
	FILE * file = fopen(filename, "r");

	if (file == NULL) {
		fprintf(stderr, "ERROR: Could not open file: %s\n", filename);
		return;
	}
	/* create a temp file for new version */
	char temp_filename[] = "tempXXXXXX";
	int temp_fd = mkstemp(temp_filename);
	
	if (temp_fd == -1) {
        fprintf(stderr, "ERROR: Could not create temporary file\n");
        fclose(file);
        return;
    }
    FILE * temp_file = fdopen(temp_fd, "w");

    if (temp_file == NULL) {
        fprintf(stderr, "ERROR: Could not open temporary file\n");
        fclose(file);
        close(temp_fd);
        return;
    }
	char * line_buffer = (char *)malloc(sizeof(char) * ASM_LINE_BUFFER_SIZE);

	if (line_buffer == NULL) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        fclose(file);
        fclose(temp_file);
        remove(temp_filename);
        return;
    }

	while (fgets(line_buffer, ASM_LINE_BUFFER_SIZE, file) != NULL) {
		char * original_buffer = line_buffer;
		char * clean_line = trim_whitespace(line_buffer);

		if (strlen(clean_line) > 0 && clean_line[0] != '#') {
            fprintf(temp_file, "%s\n", clean_line);
        }
        /* for free, we keep the original address of buffer to avoid "invalid pointer" */
        line_buffer = original_buffer;
	}
	fclose(file);
    fclose(temp_file);
    free(line_buffer);

    if (rename(temp_filename, filename) != 0) {
        fprintf(stderr, "ERROR: Could not replace file %s\n", filename);
        remove(temp_filename);
        return;
    }
}
