/* tests/sort_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpu.h>
#include <memory.h>
#include <parser.h>

int main() {
	// 1. Belleği başlat
	Memory mem;
	mem_init(&mem);

	// 2. Assembly dosyasını oku ve belleğe yükle
	const char * asm_file = "tests/test_source/sort_test.asm";
	int thread_id = 0; // User thread 1
	long int data_base  = 21;
	long int instr_base = 277;

	if (load_program_from_file(asm_file, &mem) != 0) {
		fprintf(stderr, "Failed to load assembly file: %s\n", asm_file);
		mem_free(&mem);
		return EXIT_FAILURE;
	}

	// 3. CPU’yu başlat
	CPU cpu;
	cpu_init(&cpu, &mem);
	cpu_set_context(&cpu, thread_id, data_base, instr_base, KERNEL);

	// 4. Talimatları çalıştır
	printf("Running sort_test.asm...\n");
	while (!cpu_is_halted(&cpu)) {
		cpu_execute_instruction(&cpu);
	}

	// 5. Sonuçları kontrol et
	int failed = 0;

	// Test 1: Sıralama kontrolü (data[0]’dan data[4]’e kadar sıralı olmalı)
	// Varsayım: sort_test.asm, 5 elemanlık bir diziyi sıralıyor (örneğin, [5, 3, 8, 1, 4])
	long int expected_data[10] = {-999,-3,0,3,5,12,45,89,100,1999999}; // Beklenen sıralı dizi
	for (int i = 1000; i < 1021; i++) {
		long int value = mem_read(&mem, data_base + i, KERNEL);
		if (value != expected_data[i]) {
			fprintf(stderr, "Test 1 FAILED: Expected data[%d] = %ld, got %ld\n", i, expected_data[i-1000], value);
			failed++;
		}
	}
	if (failed == 0) {
		printf("Test 1 PASSED: Sorting operation successful\n");
		printf("Sorted array: ");
		for (int i = 0; i < 5; i++) {
			printf("%ld ", mem_read(&mem, data_base + i, KERNEL));
		}
		printf("\n");
	}

	// Test 2: SYSCALL PRN zaten sıralı diziyi yazdırmış olmalı, manuel kontrol
	printf("Check the output above for SYSCALL PRN results (should print sorted array).\n");

	// 6. Genel sonuç
	if (failed == 0) {
		printf("All tests PASSED!\n");
	} else {
		printf("Tests FAILED: %d test(s) failed.\n", failed);
	}
	mem_dump(&mem, 0, 2000);
	// 7. Belleği temizle
	mem_free(&mem);

	return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
