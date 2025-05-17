/* src/parser.c */

#include <parser.h>

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
 * a format of "<address> <value>", where <address> is the memory location and <value>
 * is the data to be stored. The line is assumed to be pre-cleaned by remove_comment
 * and trim_whitespace.
 *
 * @param line Pointer to the null-terminated string containing the data line.
 * @param mem Pointer to the Memory structure where the data will be written.
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
parse_data_line(const char * line, Memory * mem)
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
        *comment_start = '\0'; /* prtotect the before # part, remain is comment */
        trim_whitespace(copy_line);
    }

    char * token = strtok(copy_line, " \t");
    if (token == NULL) {
        fprintf(stderr, "ERROR: Missing address for data definition\n");
        free(copy_line);
        return -1;
    }

    char * endptr;
    long int address = strtol(token, &endptr, 10);
    if (endptr == token || *endptr != '\0') {
        fprintf(stderr, "ERROR: Invalid address: %s\n", token);
        free(copy_line);
        return -1;
    }

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
 * expecting a format of "<index> <MNEMONIC> [OPERAND1] [OPERAND2]", where <index> is
 * the logical instruction index, <MNEMONIC> is the operation code (e.g., SET, SYSCALL),
 * and [OPERAND1] [OPERAND2] are optional operands. The line is assumed to be pre-cleaned
 * by remove_comment and trim_whitespace. The instruction is written to the memory
 * starting at INSTRUCTION_AREA_START with an offset based on curr_insr_index.
 *
 * @param line Pointer to the null-terminated string containing the instruction line.
 * @param mem Pointer to the Memory structure where the instruction will be written.
 * @param curr_insr_index The logical index of the current instruction (0, 1, 2, ...),
 *                        used to calculate the memory offset.
 *
 * @return 0 on success, -1 on failure (e.g., NULL pointers, invalid index/mnemonic/operands,
 *         memory allocation failure, or mismatched index).
 *
 * @note The function allocates memory for a copy of the input line using strdup,
 *       which must be freed before returning. The index is validated against
 *       curr_insr_index (warning if mismatched). SYSCALL instructions require a
 *       sub-instruction (e.g., PRN, HLT). Operand count is validated based on the
 *       mnemonic using the Opcode enum.
 */
static int 
parse_insr_line(const char * line, Memory * mem, int curr_insr_index)
{
    if (line == NULL || mem == NULL) {
        fprintf(stderr, "ERROR: NULL line or memory pointer\n");
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
        *comment_start = '\0'; /* prtotect the before # part, remain is comment */
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
 * @brief Loads an assembly program from a file into memory using a DFA-based parser.
 *
 * This function reads an assembly file and parses its contents into instructions 
 * and data using a DFA. The DFA starts in INITIAL_CONTEXT and transitions to 
 * INSTRUCTION_CONTEXT or DATA_CONTEXT based on the file content. Data lines are 
 * limited to 255, while instruction lines have no limit. Comments and empty lines 
 * are skipped without modifying the original file.
 *
 * @param filename Path to the assembly file.
 * @param mem Pointer to the Memory structure where the program will be loaded.
 *
 * @return 0 on success, -1 on failure (e.g., file errors, parsing failures, or 
 * data line limits exceeded).
 */
int
load_program_from_file(const char * filename, Memory * mem)
{
    if (filename == NULL || mem == NULL) {
        fprintf(stderr, "ERROR: NULL filename or memory pointer\n");
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

    Parser_State state = INITIAL_CONTEXT;
    int curr_insr_index = 0;
    int data_count = 0;
    int line_number = 0;

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
                } 
                else if (strcmp(clean_line, "Begin Instruction Section") == 0) {
                    state = INSTRUCTION_CONTEXT;
                } 
                else {
                    fprintf(stderr, "ERROR at line %d: Invalid initial line: %s\n", line_number, clean_line);
                    state = ERROR_CONTEXT;
                }
                break;

            case INSTRUCTION_CONTEXT:
                if (strcmp(clean_line, "End Instruction Section") == 0) {
                    state = INITIAL_CONTEXT;
                } 
                else if (strcmp(clean_line, "Begin Data Section") == 0 || 
                         strcmp(clean_line, "End Data Section") == 0) {
                    fprintf(stderr, "ERROR at line %d: Unexpected section marker %s before End Instruction Section\n", line_number, clean_line);
                    state = ERROR_CONTEXT;
                }
                else if (parse_insr_line(clean_line, mem, curr_insr_index) != 0) {
                    fprintf(stderr, "ERROR at line %d: Failed to parse instruction line: %s\n", line_number, clean_line);
                    free(line_buffer);
                    fclose(file);
                    return -1;
                }
                else {
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
                else if (parse_data_line(clean_line, mem) != 0) {
                    fprintf(stderr, "ERROR at line %d: Failed to parse data line: %s\n", line_number, clean_line);
                    free(line_buffer);
                    fclose(file);
                    return -1;
                }
                else {
                    data_count++;
                    
                    if (data_count > MAX_DATA_LINES) {
                        fprintf(stderr, "ERROR at line %d: Maximum data lines (%d) exceeded\n", line_number, MAX_DATA_LINES);
                        free(line_buffer);
                        fclose(file);
                        return -1;
                    }
                }
                break;

            case ERROR_CONTEXT:
                fprintf(stderr, "ERROR: Parsing stopped due to previous errors at line %d\n", line_number);
                free(line_buffer);
                fclose(file);
                return -1;
        }
    }
    free(line_buffer);
    fclose(file);
    return 0;
}
