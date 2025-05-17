CC = gcc # compiler
CFLAGS = -g -Wall -Wextra -std=c11 -Iinclude

# directories
SRC_DIR = src
INCLUDE_DIR = include
TEST_DIR = tests
OBJ_DIR = obj
BIN_DIR = bin

# --- Bellek Testi Dosyaları ---

MEM_C_FILES = $(TEST_DIR)/mem_test.c $(SRC_DIR)/memory.c
# object files -> obj/ :
# - tests/mem_test.c -> obj/mem_test.o
# - src/memory.c -> obj/memory.o
MEM_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter $(TEST_DIR)/%, $(MEM_C_FILES))) \
           $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(filter $(SRC_DIR)/%, $(MEM_C_FILES)))

MEM_TEST_TARGET = $(BIN_DIR)/mem_test_runner

# === TARGETS ===

default: memtest

memtest: $(MEM_TEST_TARGET)
	@echo "Bellek testi çalıştırılıyor..."
	./$(MEM_TEST_TARGET)
	@echo "Bellek testi tamamlandı."

$(MEM_TEST_TARGET): $(MEM_OBJS)
	@mkdir -p $(BIN_DIR) # bin dizini yoksa oluştur
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	@echo "Bellek testi için çalıştırılabilir dosya oluşturuldu: $@"


# === GENERAL COMPİLE RULES ===

# src dizinindeki .c dosyalarından obj dizinine .o dosyası oluşturma kuralı
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR) # | $(OBJ_DIR) order-only prerequisite
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Derlendi: $< -> $@"

# tests dizinindeki .c dosyalarından obj dizinine .o dosyası oluşturma kuralı
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Derlendi: $< -> $@"

# obj dizinini oluşturmak için "order-only prerequisite"
# Bu kural, .o dosyaları oluşturulmadan önce $(OBJ_DIR)'in var olmasını sağlar.
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# Temizleme hedefi
clean:
	@echo "Temizleniyor..."
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
	@echo "Temizlik tamamlandı."

.PHONY: all default memtest clean