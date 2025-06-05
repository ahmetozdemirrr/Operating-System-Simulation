#include <stdio.h>
#include <stdbool.h>

#define CURRENT_RUNNING_THREAD_ID 100
#define PRINT_BLOCK_DURATION 101

long get_thread_table_addr(int thread_id, int field)
{
	return 200 + (thread_id * 8) + field;
}

void os_boot()
{
	memory[CURRENT_RUNNING_THREAD_ID] = 0;  // M[100] = 0
	scheduler();
}

void scheduler()
{
	long current_thread_id = memory[CURRENT_RUNNING_THREAD_ID];  // M[100]
	long selected_thread = 0;

	/* find first READY thread using Round-robin */
	for (int count = MAX_THREADS; count > 0; count--) {

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

void update_current_thread_in_table()
{
	long current_thread_id = memory[CURRENT_RUNNING_THREAD_ID];  // M[100]
	long syscall_id = memory[REG_SYSCALL_RESULT];  // Register 2

	long state_addr = get_thread_table_addr(current_thread_id, 1);
	long pc_addr = get_thread_table_addr(current_thread_id, 2);
	long sp_addr = get_thread_table_addr(current_thread_id, 3);
	long instr_count_addr = get_thread_table_addr(current_thread_id, 6);
	long wakeup_addr = get_thread_table_addr(current_thread_id, 7);

	// Thread state'ini syscall türüne göre güncelle
	if (syscall_id == SYSCALL_PRN_ID) {
		memory[state_addr] = THREAD_STATE_BLOCKED;
		// PRN syscall için thread'i PRINT_BLOCK_DURATION kadar blokla
		memory[wakeup_addr] = memory[PRINT_BLOCK_DURATION];  // M[101] = 100
	} else if (syscall_id == SYSCALL_HLT_ID) {
		memory[state_addr] = THREAD_STATE_TERMINATED;
	} else if (syscall_id == SYSCALL_YIELD_ID) {
		memory[state_addr] = THREAD_STATE_READY;
	}

	// Thread context'ini syscall sırasında register 5, 6, 7'ye kaydedilen değerlerden oku
	memory[pc_addr] = memory[5];           // Thread PC (syscall'da register 5'e kaydedildi)
	memory[sp_addr] = memory[6];           // Thread SP (syscall'da register 6'ya kaydedildi)
	memory[instr_count_addr] = memory[7];  // Thread IC (syscall'da register 7'ye kaydedildi)
	// wakeup_addr zaten yukarıda PRN syscall için set edildi, diğer syscall'lar için değişmez
}

void prepare_context_switch(long thread_id)
{
	long base_addr = get_thread_table_addr(thread_id, 0);

	// Thread table'dan mailbox'a context bilgilerini kopyala
	memory[MAILBOX_THREAD_ID] = memory[base_addr + 0];      // Thread ID
	memory[MAILBOX_STATE] = memory[base_addr + 1];          // State
	memory[MAILBOX_PC] = memory[base_addr + 2];             // PC
	memory[MAILBOX_SP] = memory[base_addr + 3];             // SP
	memory[MAILBOX_DATA_BASE] = memory[base_addr + 4];      // Data Base
	memory[MAILBOX_INSTR_BASE] = memory[base_addr + 5];     // Instruction Base
	memory[MAILBOX_INSTR_COUNT] = memory[base_addr + 6];    // Instruction Count
	memory[MAILBOX_WAKEUP_COUNT] = memory[base_addr + 7];   // Wakeup Count

	// Seçilen thread'in state'ini RUNNING yap
	memory[base_addr + 1] = THREAD_STATE_RUNNING;

	// Context switch sinyalini gönder
	memory[REG_CONTEXT_SWITCH_SIGNAL] = CTX_SWITCH_REQUEST;  // -999

	// Current running thread ID'yi güncelle
	memory[CURRENT_RUNNING_THREAD_ID] = thread_id;  // M[100] = thread_id
}

void syscall_handler()
{
	long syscall_id = memory[REG_SYSCALL_RESULT];  // Register 2

	switch (syscall_id) {
		case SYSCALL_PRN_ID:
			// PRN syscall için özel bir işlem yok,
			// wakeup count update_current_thread_in_table()'da yapılacak
			scheduler();
			break;

		case SYSCALL_HLT_ID:
			// Thread terminate edilecek
			scheduler();
			break;

		case SYSCALL_YIELD_ID:
			// Thread CPU'yu gönüllü olarak bırakıyor
			scheduler();
			break;
	}
}

int main()
{
	os_boot();
	syscall_handler();
	return 0;
}
