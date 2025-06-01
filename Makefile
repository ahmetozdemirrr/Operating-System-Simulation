CC = gcc # compiler
CFLAGS = -g -Wall -Wextra -std=c11 -Iinclude

# directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

# --- memory test files ---
MEM_C_FILES = $(TEST_DIR)/mem_test.c $(SRC_DIR)/memory.c
MEM_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter $(TEST_DIR)/%, $(MEM_C_FILES))) \
           $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter $(SRC_DIR)/%, $(MEM_C_FILES)))
MEM_TEST_TARGET = $(BIN_DIR)/mem_test_runner

# --- parser test files ---
PARSER_C_FILES = $(TEST_DIR)/parser_test.c $(SRC_DIR)/parser/parser.c $(SRC_DIR)/memory.c
PARSER_OBJS = $(OBJ_DIR)/parser_test.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/memory.o
PARSER_TEST_TARGET = $(BIN_DIR)/parser_test_runner

# --- cpu test files ---
CPU_C_FILES = $(TEST_DIR)/sort_test.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
CPU_OBJS = $(OBJ_DIR)/sort_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
CPU_TEST_TARGET = $(BIN_DIR)/cpu_test_runner

# --- linear search test files ---
SEARCH_C_FILES = $(TEST_DIR)/linear_search_test.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
SEARCH_OBJS = $(OBJ_DIR)/linear_search_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
SEARCH_TEST_TARGET = $(BIN_DIR)/search_test_runner

# --- multiplication by addition test files --- (YENİ EKLENEN BÖLÜM)
MULT_TEST_C_FILE = $(TEST_DIR)/multiplication_test.c # Test dosyanızın adı bu olmalı
MULT_C_DEPS = $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
MULT_OBJS = $(OBJ_DIR)/multiplication_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
MULT_TEST_TARGET = $(BIN_DIR)/mult_test_runner

# Scheduler test files
SCHEDULER_C_FILES = $(TEST_DIR)/scheduler_test.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c
SCHEDULER_OBJS = $(OBJ_DIR)/scheduler_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o
SCHEDULER_TEST_TARGET = $(BIN_DIR)/scheduler_test_runner

# === TARGETS ===
default: memtest parsetest searchtest multtest schedulertest

# Add searchtest to existing targets
searchtest: $(SEARCH_TEST_TARGET)
	@echo "Running Linear Search Test..."
	./$(SEARCH_TEST_TARGET)
	@echo "Linear Search Test is Complete."

memtest: $(MEM_TEST_TARGET)
	@echo "Running Memory Test..."
	./$(MEM_TEST_TARGET)
	@echo "Memory Test is Complete."

parsetest: $(PARSER_TEST_TARGET)
	@echo "Running Parser Test..."
	./$(PARSER_TEST_TARGET)
	@echo "Parser Test is Complete."

cputest: $(CPU_TEST_TARGET)
	@echo "Running CPU Test..."
	./$(CPU_TEST_TARGET)
	@echo "CPU Test is Complete."

multtest: $(MULT_TEST_TARGET)
	@echo "Running Multiplication by Addition Test..."
	./$(MULT_TEST_TARGET)
	@echo "Multiplication by Addition Test is Complete."

schedulertest: $(SCHEDULER_TEST_TARGET)
	@echo "Running Scheduler Test..."
	./$(SCHEDULER_TEST_TARGET)
	@echo "Scheduler Test is Complete."

$(MEM_TEST_TARGET): $(MEM_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(PARSER_TEST_TARGET): $(PARSER_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(CPU_TEST_TARGET): $(CPU_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SEARCH_TEST_TARGET): $(SEARCH_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(MULT_TEST_TARGET): $(MULT_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SCHEDULER_TEST_TARGET): $(SCHEDULER_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# === GENERAL COMPILE RULES ===
# rule for creating .o files in obj from .c files in src directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

# rule for creating .o files in obj from .c files in src/parser directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/parser/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

# rule for creating .o files in obj from .c files in tests directory
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# clean targter
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	@echo "Cleaning is complete."

.PHONY: all default memtest parsetest cputest searchtest multtest schedulertest clean
