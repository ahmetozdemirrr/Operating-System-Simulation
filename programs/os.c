#include <stdio.h>
#include <stdbool.h>

long get_thread_table_addr(int thread_id, int field)
{
	return 200 + (thread_id * 8) + field;
}

void os_boot()
{
	memory[20 + CURRENT_RUNNING_THREAD_ID] = 0;
	scheduler();
}

void scheduler()
{
	long current_thread_id = memory[100];
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
	long current_thread_id = memory[20 + CURRENT_RUNNING_THREAD_ID];
	long syscall_id = memory[REG_SYSCALL_RESULT];

	long state_addr = get_thread_table_addr(current_thread_id, 1);
	long pc_addr = get_thread_table_addr(current_thread_id, 2);
	long sp_addr = get_thread_table_addr(current_thread_id, 3);
	long instr_count_addr = get_thread_table_addr(current_thread_id, 6);
	long wakeup_addr = get_thread_table_addr(current_thread_id, 7);

	if (syscall_id == SYSCALL_PRN_ID) {
		memory[state_addr] = THREAD_STATE_BLOCKED;
	} else if (syscall_id == SYSCALL_HLT_ID) {
		memory[state_addr] = THREAD_STATE_TERMINATED;
	} else if (syscall_id == SYSCALL_YIELD_ID) {
		memory[state_addr] = THREAD_STATE_READY;
	}
	memory[pc_addr] = memory[REG_PC];
	memory[sp_addr] = memory[REG_SP];
	memory[instr_count_addr] = memory[REG_INSTR_COUNT];
	memory[wakeup_addr] = memory[REG_WAKEUP_COUNT];
}

void prepare_context_switch(long thread_id)
{
	long base_addr = get_thread_table_addr(thread_id, 0);

	memory[MAILBOX_THREAD_ID] = memory[base_addr + 0];
	memory[MAILBOX_STATE] = memory[base_addr + 1];
	memory[MAILBOX_PC] = memory[base_addr + 2];
	memory[MAILBOX_SP] = memory[base_addr + 3];
	memory[MAILBOX_DATA_BASE] = memory[base_addr + 4];
	memory[MAILBOX_INSTR_BASE] = memory[base_addr + 5];
	memory[MAILBOX_INSTR_COUNT] = memory[base_addr + 6];
	memory[MAILBOX_WAKEUP_COUNT] = memory[base_addr + 7];

	memory[base_addr + 1] = THREAD_STATE_RUNNING;
	memory[REG_CONTEXT_SWITCH_SIGNAL] = CONTEXT_SWITCH_SIGNAL;
	memory[20 + CURRENT_RUNNING_THREAD_ID] = thread_id;
}

void syscall_handler()
{
	long syscall_id = memory[REG_SYSCALL_RESULT];

	switch (syscall_id) {
		case SYSCALL_PRN_ID:
			memory[REG_WAKEUP_COUNT] = memory[20 + PRINT_BLOCK_DURATION];
			scheduler();
			break;

		case SYSCALL_HLT_ID:
			scheduler();
			break;

		case SYSCALL_YIELD_ID:
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
