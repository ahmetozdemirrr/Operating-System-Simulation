/* tests/mem_test.c */

#include <common.h>
#include <memory.h>

void print_test_result(const char * test_name, bool success) 
{
    if (success) {
        printf("TEST PASSED: %s\n", test_name);
    } 

    else {
        fprintf(stderr, "TEST FAILED: %s\n", test_name);
    }
}

int main(void) 
{
    printf("Starting Memory Module Tests...\n\n");

    Memory test_memory;
    bool test_status;

    // 1. mem_init tests
    printf("\n--- mem_init Tests ---\n");
    mem_init(&test_memory, MEM_SIZE);
    test_status = (test_memory.is_initialized == true &&
                   test_memory.size == MEM_SIZE &&
                   test_memory.data[REG_SP] == (long int)MEM_SIZE - 1 &&
                   test_memory.data[0] == 0 &&
                   test_memory.data[MEM_SIZE / 2] == 0);
    print_test_result("mem_init basic initialization and zeroing", test_status);
    if (!test_status) exit(EXIT_FAILURE);

    printf("SP (Address %d) value after mem_init: %ld\n", REG_SP, test_memory.data[REG_SP]);
    mem_dump(&test_memory, 0, 5);
    mem_dump(&test_memory, MEM_SIZE - 5, 5);

    // 2. mem_write and mem_read tests (in KERNEL mode)
    printf("\n--- mem_write/mem_read Basic Tests (KERNEL Mode) ---\n");
    long int test_address1 = 100;
    long int test_value1 = 12345;

    mem_write(&test_memory, test_address1, test_value1, KERNEL);
    long int read_value1 = mem_read(&test_memory, test_address1, KERNEL);
    test_status = (read_value1 == test_value1);
    print_test_result("Simple write and read (KERNEL)", test_status);
    if (!test_status) exit(EXIT_FAILURE);

    long int test_address2 = REG_PC;
    long int test_value2 = 77;
    mem_write(&test_memory, test_address2, test_value2, KERNEL);
    long int read_value2 = mem_read(&test_memory, test_address2, KERNEL);
    test_status = (read_value2 == test_value2);
    print_test_result("Write and read to REG_PC (KERNEL)", test_status);
    if (!test_status) exit(EXIT_FAILURE);

    printf("Memory dump after write operations (Addresses 98-102 and 0-5):\n");
    mem_dump(&test_memory, 98, 5);
    mem_dump(&test_memory, 0, 5);

    printf("\n--- User Mode Access Tests ---\n");
    long int protected_addr = 50;       // A protected address (0-999)
    long int unprotected_addr = 1050;   // An unprotected address (1000 and above)
    long int user_value = 555;

    // Test writing to an UNPROTECTED area in USER mode (should succeed)
    printf("Attempting to WRITE to UNPROTECTED area (address %ld) in USER mode...\n", unprotected_addr); // Corrected printf message
    mem_write(&test_memory, unprotected_addr, user_value, USER);
    long int read_unprotected_value = mem_read(&test_memory, unprotected_addr, USER);
    test_status = (read_unprotected_value == user_value);
    print_test_result("Write/read to unprotected area in USER mode", test_status);
    if (!test_status) {
        fprintf(stderr, "DETAIL ERROR: Value read from unprotected area does not match written value! Expected: %ld, Read: %ld\n", user_value, read_unprotected_value);
        exit(EXIT_FAILURE);
    }

/*  This commented block is for testing writes to protected area, which should cause program termination.
    You would uncomment this spécifique test case to verify that behavior.
    
    printf("\nAttempting to WRITE to PROTECTED area (address %ld) in USER mode (program termination expected)...\n", protected_addr);
    mem_write(&test_memory, protected_addr, user_value, USER); 
    
    fprintf(stderr, "TEST FAILED: Write to protected area in USER mode did not terminate the program!\n");
    print_test_result("Write to protected area in USER mode (FAILURE - no termination)", false);
    exit(EXIT_FAILURE); 
*/

    printf("\nCalling mem_dump with various ranges:\n");
    mem_dump(&test_memory, 0, 25);
    mem_dump(&test_memory, MEM_SIZE - 10, 15);
    mem_dump(&test_memory, 500, 0);
    mem_dump(&test_memory, MEM_SIZE, 5);

    printf("\n--- mem_free Test ---\n");
    mem_free(&test_memory);
    test_status = (test_memory.is_initialized == false);
    print_test_result("is_initialized check after mem_free", test_status);

    printf("\nMemory Module Tests Completed.\n");
    return 0;
}
