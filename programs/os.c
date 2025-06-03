#include <stdio.h>
#include <stdbool.h>

// Thread states
#define THREAD_STATE_READY 0
#define THREAD_STATE_RUNNING 1
#define THREAD_STATE_BLOCKED 2
#define THREAD_STATE_TERMINATED 3

// Syscall IDs
#define SYSCALL_PRN_ID 1
#define SYSCALL_HLT_ID 2
#define SYSCALL_YIELD_ID 3

// Constants
#define MAX_THREADS 10
#define PRN_BLOCK_DURATION 100
#define CONTEXT_SWITCH_SIGNAL -999

// Memory map (absolute addresses)
long memory[24000];

// OS data area offsets (relative to OS_DATA_START_ADDR=20)
#define CURRENT_RUNNING_THREAD_ID 100  // M[100]
#define NEXT_THREAD_ID 101            // M[101] (not used anymore)
#define PRINT_BLOCK_COUNTER 102       // M[102] (not used anymore)
#define PRINT_BLOCK_DURATION 103      // M[103] = 100

// Thread table starts at 500 (absolute address)
// Each entry has 8 fields:
// 0: thread_id
// 1: thread_state
// 2: PC
// 3: SP
// 4: data_base_addr
// 5: instr_base_addr
// 6: register_instruction_count
// 7: wakeup_instruction_count

// Mailbox addresses (600-607)
#define MAILBOX_THREAD_ID 600
#define MAILBOX_STATE 601
#define MAILBOX_PC 602
#define MAILBOX_SP 603
#define MAILBOX_DATA_BASE 604
#define MAILBOX_INSTR_BASE 605
#define MAILBOX_INSTR_COUNT 606
#define MAILBOX_WAKEUP_COUNT 607

// Registers
#define REG_PC 0
#define REG_SP 1
#define REG_SYSCALL_RESULT 2
#define REG_INSTR_COUNT 3
#define REG_WAKEUP_COUNT 4
#define REG_CONTEXT_SWITCH_SIGNAL 17

// Helper functions
long get_thread_table_addr(int thread_id, int field) {
    return 500 + (thread_id * 8) + field;
}

void os_boot() {
    // OS Boot sequence
    // 1. Initialize OS data
    memory[20 + CURRENT_RUNNING_THREAD_ID] = 0;  // OS is running initially
    memory[20 + PRINT_BLOCK_DURATION] = 100;     // PRN block duration

    // 2. Call scheduler to select first thread
    scheduler();

    // 3. Jump to selected thread using USER instruction
    // USER will read PC from mailbox[MAILBOX_PC]
    // This is simulated - in assembly it would be: USER 112
}
Verdiğin C kodundaki değişken tanımlamalar hariç (onları sonradan modifiye ettik...), get_thread_table_addr, os_boot rutinlerini ve scheduler rutininin ADIM 2'den önceki kısmına kadar böyle yaptım. Bir bak yanlış veya hata falan var mı?
void scheduler() {
    long current_thread_id = memory[100];
    long selected_thread = 0;

    // ADIM 1: Tüm thread'lerin wakeup'ını kontrol et
    // 10'dan geriye say (10, 9, 8... 1)
    for (int tid = MAX_THREADS; tid > 0; tid--) {
        long state_addr = get_thread_table_addr(tid, 1);
        long state = memory[state_addr];

        if (state == THREAD_STATE_BLOCKED) {
            long wakeup_addr = get_thread_table_addr(tid, 7);
            long wakeup = memory[wakeup_addr];
            wakeup--;
            memory[wakeup_addr] = wakeup;

            if (wakeup <= 0) {
                memory[state_addr] = THREAD_STATE_READY;
            }
        }
    }

    // ADIM 2: Round-robin ile READY thread bul
    // MAX_THREADS kere dön
    for (int count = MAX_THREADS; count > 0; count--) {
        // Hangi thread'e bakacağız?
        int offset = MAX_THREADS - count + 1;
        int tid = current_thread_id + offset;
        if (tid > MAX_THREADS) {
            tid = tid - MAX_THREADS;
        }

        long state_addr = get_thread_table_addr(tid, 1);
        long state = memory[state_addr];

        if (state == THREAD_STATE_READY) {
            selected_thread = tid;
            break;
        }
    }

    if (selected_thread == 0) {
        selected_thread = 1;  // idle thread
    }

    if (current_thread_id > 0) {
        update_current_thread_in_table();
    }

    prepare_context_switch(selected_thread);
}

void update_current_thread_in_table() {
    // Update current thread's state based on syscall type
    long current_thread_id = memory[20 + CURRENT_RUNNING_THREAD_ID];
    long syscall_id = memory[REG_SYSCALL_RESULT];

    // Get thread table entry addresses
    long state_addr = get_thread_table_addr(current_thread_id, 1);
    long pc_addr = get_thread_table_addr(current_thread_id, 2);
    long sp_addr = get_thread_table_addr(current_thread_id, 3);
    long instr_count_addr = get_thread_table_addr(current_thread_id, 6);
    long wakeup_addr = get_thread_table_addr(current_thread_id, 7);

    // Update state based on syscall
    if (syscall_id == SYSCALL_PRN_ID) {
        memory[state_addr] = THREAD_STATE_BLOCKED;
    } else if (syscall_id == SYSCALL_HLT_ID) {
        memory[state_addr] = THREAD_STATE_TERMINATED;
    } else if (syscall_id == SYSCALL_YIELD_ID) {
        memory[state_addr] = THREAD_STATE_READY;
    }

    // Update other fields
    memory[pc_addr] = memory[REG_PC];
    memory[sp_addr] = memory[REG_SP];
    memory[instr_count_addr] = memory[REG_INSTR_COUNT];
    memory[wakeup_addr] = memory[REG_WAKEUP_COUNT];
}

void prepare_context_switch(long thread_id) {
    // Copy thread info from table to mailbox
    long base_addr = get_thread_table_addr(thread_id, 0);

    // Copy all 8 fields to mailbox
    memory[MAILBOX_THREAD_ID] = memory[base_addr + 0];
    memory[MAILBOX_STATE] = memory[base_addr + 1];
    memory[MAILBOX_PC] = memory[base_addr + 2];
    memory[MAILBOX_SP] = memory[base_addr + 3];
    memory[MAILBOX_DATA_BASE] = memory[base_addr + 4];
    memory[MAILBOX_INSTR_BASE] = memory[base_addr + 5];
    memory[MAILBOX_INSTR_COUNT] = memory[base_addr + 6];
    memory[MAILBOX_WAKEUP_COUNT] = memory[base_addr + 7];

    // Update thread state to RUNNING in table
    memory[base_addr + 1] = THREAD_STATE_RUNNING;

    // Set context switch signal
    memory[REG_CONTEXT_SWITCH_SIGNAL] = CONTEXT_SWITCH_SIGNAL;

    // Update current running thread ID
    memory[20 + CURRENT_RUNNING_THREAD_ID] = thread_id;
}

void syscall_handler() {
    // Read syscall ID from register 2
    long syscall_id = memory[REG_SYSCALL_RESULT];

    switch (syscall_id) {
        case SYSCALL_PRN_ID:
            // Set wakeup count for blocking
            memory[REG_WAKEUP_COUNT] = memory[20 + PRINT_BLOCK_DURATION];
            scheduler();
            break;

        case SYSCALL_HLT_ID:
            // Thread will be terminated
            scheduler();
            break;

        case SYSCALL_YIELD_ID:
            // Thread yields CPU
            scheduler();
            break;

        default:
            printf("Unknown syscall: %ld\n", syscall_id);
            break;
    }
}

// Main execution flow
int main() {
    // Initialize thread table (example)
    // OS (thread 0)
    memory[500] = 0;     // thread_id
    memory[501] = 1;     // RUNNING
    memory[502] = 1000;  // PC
    memory[503] = 1999;  // SP
    memory[504] = 20;    // data_base
    memory[505] = 1000;  // instr_base
    memory[506] = 0;     // instr_count
    memory[507] = 0;     // wakeup_count

    // Thread 1 (idle thread)
    memory[508] = 1;     // thread_id
    memory[509] = 0;     // READY
    memory[510] = 3000;  // PC
    memory[511] = 3999;  // SP
    memory[512] = 2000;  // data_base
    memory[513] = 3000;  // instr_base
    memory[514] = 0;     // instr_count
    memory[515] = 0;     // wakeup_count

    // Thread 2
    memory[516] = 2;     // thread_id
    memory[517] = 0;     // READY
    memory[518] = 5000;  // PC
    memory[519] = 5999;  // SP
    memory[520] = 4000;  // data_base
    memory[521] = 5000;  // instr_base
    memory[522] = 0;     // instr_count
    memory[523] = 0;     // wakeup_count

    // Simulate OS execution
    printf("=== OS BOOT ===\n");
    os_boot();

    printf("\n=== SYSCALL EXAMPLES ===\n");

    // Simulate SYSCALL PRN from Thread 2
    memory[20 + CURRENT_RUNNING_THREAD_ID] = 2;
    memory[REG_SYSCALL_RESULT] = SYSCALL_PRN_ID;
    memory[REG_PC] = 5010;
    memory[REG_SP] = 5998;
    memory[REG_INSTR_COUNT] = 50;
    printf("\nThread 2 calls SYSCALL PRN\n");
    syscall_handler();

    // Show mailbox after context switch
    printf("\nMailbox after context switch:\n");
    printf("Thread ID: %ld\n", memory[MAILBOX_THREAD_ID]);
    printf("State: %ld\n", memory[MAILBOX_STATE]);
    printf("PC: %ld\n", memory[MAILBOX_PC]);
    printf("SP: %ld\n", memory[MAILBOX_SP]);

    return 0;
}

/* ASSEMBLY TRANSLATION GUIDE:

1. OS_BOOT (instruction 0-2):
   - SET 0 100     # current_thread_id = 0
   - CALL scheduler
   - USER 112      # Jump to thread PC from mailbox

2. SCHEDULER (starts at instruction 30):
   - Get current_thread_id from M[100]
   - Calculate next thread to check (current + 1)
   - Loop through threads:
     * Check thread state
     * If BLOCKED, decrement wakeup count
     * If wakeup <= 0, make READY
     * If READY found, select it
   - If no READY thread, select idle (thread 1)
   - Update current thread in table
   - Prepare context switch
   - RET

3. UPDATE_CURRENT_THREAD:
   - Check syscall ID in register 2
   - Update thread state based on syscall type
   - Update PC, SP, instruction count, wakeup count

4. PREPARE_CONTEXT_SWITCH:
   - Copy thread data from table to mailbox (600-607)
   - Set thread state to RUNNING
   - Set context switch signal (-999) to register 17
   - Update current_thread_id

5. SYSCALL_HANDLER (starts at instruction 200):
   - Read syscall ID from register 2
   - For PRN: Set wakeup count, call scheduler
   - For HLT: Call scheduler
   - For YIELD: Call scheduler
   - RET

Key memory locations:
- M[100]: current_running_thread_id
- M[103]: PRN block duration (100)
- M[500-587]: Thread table (8 entries per thread)
- M[600-607]: Context switch mailbox
- Register 17: Context switch signal
*/
