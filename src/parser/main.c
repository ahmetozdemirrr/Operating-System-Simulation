#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define ASM_LINE_BUFFER_SIZE 64

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

int main(int argc, char const *argv[])
{
	remove_comment("ahmet.asm");
	
	return 0;
}