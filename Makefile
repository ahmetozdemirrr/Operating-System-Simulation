# Makefile

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
SORT_FILES = $(TEST_DIR)/sort_test.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
SORT_OBJS = $(OBJ_DIR)/sort_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
SORT_TEST_TARGET = $(BIN_DIR)/cpu_test_runner

# --- linear search test files ---
SEARCH_C_FILES = $(TEST_DIR)/linear_search_test.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
SEARCH_OBJS = $(OBJ_DIR)/linear_search_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
SEARCH_TEST_TARGET = $(BIN_DIR)/search_test_runner

# --- multiplication by addition test files --- (YENİ EKLENEN BÖLÜM)
MULT_TEST_C_FILE = $(TEST_DIR)/multiplication_test.c # Test dosyanızın adı bu olmalı
MULT_C_DEPS = $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
MULT_OBJS = $(OBJ_DIR)/multiplication_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
MULT_TEST_TARGET = $(BIN_DIR)/mult_test_runner

# OS System Test files
OS_SYSTEM_C_FILES = $(TEST_DIR)/os_system_test.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
OS_SYSTEM_OBJS = $(OBJ_DIR)/os_system_test.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
OS_SYSTEM_TEST_TARGET = $(BIN_DIR)/os_system_test_runner

# Simulator files
SIMULATOR_C_FILES = simulation/simulator.c $(SRC_DIR)/cpu.c $(SRC_DIR)/memory.c $(SRC_DIR)/parser/parser.c
SIMULATOR_OBJS = $(OBJ_DIR)/simulator.o $(OBJ_DIR)/cpu.o $(OBJ_DIR)/memory.o $(OBJ_DIR)/parser.o
SIMULATOR_TARGET = $(BIN_DIR)/simulator

# === TARGETS ===
default: memtest parsetest sorttest searchtest multtest ossystemtest simulator

# Simulator target
simulator: $(SIMULATOR_TARGET)
	@echo "Simulator built successfully!"

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

sorttest: $(SORT_TEST_TARGET)
	@echo "Running CPU Test..."
	./$(SORT_TEST_TARGET)
	@echo "CPU Test is Complete."

multtest: $(MULT_TEST_TARGET)
	@echo "Running Multiplication by Addition Test..."
	./$(MULT_TEST_TARGET)
	@echo "Multiplication by Addition Test is Complete."

# OS System test target
ossystemtest: $(OS_SYSTEM_TEST_TARGET)
	@echo "Running OS System Test..."
	./$(OS_SYSTEM_TEST_TARGET)
	@echo "OS System Test is Complete."

$(SIMULATOR_TARGET): $(SIMULATOR_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(MEM_TEST_TARGET): $(MEM_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(PARSER_TEST_TARGET): $(PARSER_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SORT_TEST_TARGET): $(SORT_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(SEARCH_TEST_TARGET): $(SEARCH_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(MULT_TEST_TARGET): $(MULT_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(OS_SYSTEM_TEST_TARGET): $(OS_SYSTEM_OBJS)
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

# rule for creating .o files in obj from .c files in simulation directory
$(OBJ_DIR)/%.o: simulation/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# clean targter
clean:
	@echo "Cleaning..."
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	rm -rf *.txt
	@echo "Cleaning is complete."

.PHONY: all default memtest parsetest cputest searchtest multtest schedulertest multithreadtest ossystemtest osdebugtest simulator clean
