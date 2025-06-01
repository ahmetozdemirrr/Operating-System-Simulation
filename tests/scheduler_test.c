/* tests/exact_scheduler_test.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cpu.h>
#include <memory.h>

void print_thread_table(Memory *mem, int thread_id) {
    long int base = 500 + (thread_id * 8);
    printf("Thread %d Table Entry (addr %ld):\n", thread_id, base);
    printf("  ID: %ld, State: %ld, PC: %ld, SP: %ld\n",
           mem_read(mem, base, KERNEL),
           mem_read(mem, base + 1, KERNEL),
           mem_read(mem, base + 2, KERNEL),
           mem_read(mem, base + 3, KERNEL));
    printf("  DataBase: %ld, InstrBase: %ld, Mode: %ld, WakeUp: %ld\n",
           mem_read(mem, base + 4, KERNEL),
           mem_read(mem, base + 5, KERNEL),
           mem_read(mem, base + 6, KERNEL),
           mem_read(mem, base + 7, KERNEL));
}

void print_temp_variables(Memory *mem) {
    printf("\nTemp Variables (110-117):\n");
    printf("  temp_thread_id (110): %ld\n", mem_read(mem, 110, KERNEL));
    printf("  temp_thread_state (111): %ld\n", mem_read(mem, 111, KERNEL));
    printf("  temp_thread_pc (112): %ld\n", mem_read(mem, 112, KERNEL));
    printf("  temp_thread_sp (113): %ld\n", mem_read(mem, 113, KERNEL));
    printf("  temp_thread_data_base (114): %ld\n", mem_read(mem, 114, KERNEL));
    printf("  temp_thread_instr_base (115): %ld\n", mem_read(mem, 115, KERNEL));
    printf("  mode_to_restore (116): %ld\n", mem_read(mem, 116, KERNEL));
    printf("  wake_up_instr_count (117): %ld\n", mem_read(mem, 117, KERNEL));
}

void print_working_variables(Memory *mem) {
    printf("\nWorking Variables (801-823):\n");
    printf("  i (801): %ld\n", mem_read(mem, OS_DATA_START_ADDR + 801, KERNEL));
    printf("  N (802): %ld\n", mem_read(mem, OS_DATA_START_ADDR + 802, KERNEL));
    printf("  temp_i (803): %ld\n", mem_read(mem, OS_DATA_START_ADDR + 803, KERNEL));
    printf("  temp_N (804): %ld\n", mem_read(mem, OS_DATA_START_ADDR + 804, KERNEL));
    printf("  state (805): %ld\n", mem_read(mem, OS_DATA_START_ADDR + 805, KERNEL));
    printf("  base (810): %ld\n", mem_read(mem, OS_DATA_START_ADDR + 810, KERNEL));
}

int main(void)
{
    printf("=== EXACT SCHEDULER TEST ===\n\n");

    Memory mem;
    mem_init(&mem);

    // SENIN EXACT assembly kodunu instruction by instruction yaz
    // Instruction 30-71 = address 1090-1213
    long int exact_scheduler[] = {
        // 30: SET 509 801
        0, 509, 801,
        // 31: SET 581 802
        0, 581, 802,
        // 32: CPY 801 803
        1, 801, 803,
        // 33: CPY 802 804
        1, 802, 804,
        // 34: SUBI 804 803
        6, 804, 803,
        // 35: JIF 803 72
        7, 803, 72,
        // 36: CPYI 801 805
        2, 801, 805,
        // 37: JIF 805 69 (DÜZELTME: state > 0 ise next thread'e git!)
        7, 805, 69,
        // 38: CPY 801 810 (context copy başlıyor)
        1, 801, 810,
        // 39: SET 1 811
        0, 1, 811,
        // 40: CPY 810 812
        1, 810, 812,
        // 41: SUBI 812 811
        6, 812, 811,
        // 42: CPYI 812 110
        2, 812, 110,
        // 43: CPYI 810 111
        2, 810, 111,
        // 44: CPY 810 813
        1, 810, 813,
        // 45: ADDI 813 811
        5, 813, 811,
        // 46: CPYI 813 112
        2, 813, 112,
        // 47: CPY 810 814
        1, 810, 814,
        // 48: SET 2 815
        0, 2, 815,
        // 49: ADDI 814 815
        5, 814, 815,
        // 50: CPYI 814 113
        2, 814, 113,
        // 51: CPY 810 816
        1, 810, 816,
        // 52: SET 3 817
        0, 3, 817,
        // 53: ADDI 816 817
        5, 816, 817,
        // 54: CPYI 816 114
        2, 816, 114,
        // 55: CPY 810 818
        1, 810, 818,
        // 56: SET 4 819
        0, 4, 819,
        // 57: ADDI 818 819
        5, 818, 819,
        // 58: CPYI 818 115
        2, 818, 115,
        // 59: CPY 810 820
        1, 810, 820,
        // 60: SET 5 821
        0, 5, 821,
        // 61: ADDI 820 821
        5, 820, 821,
        // 62: CPYI 820 116
        2, 820, 116,
        // 63: CPY 810 822
        1, 810, 822,
        // 64: SET 6 823
        0, 6, 823,
        // 65: ADDI 822 823
        5, 822, 823,
        // 66: CPYI 822 117
        2, 822, 117,
        // 67: SET -999 17
        0, -999, 17,
        // 68: RET
        11, 0, 0,
        // 69: ADD 801 8 (next thread)
        4, 801, 8,
        // 70: JIF 99 32
        7, 99, 32,
        // 71: HLT
        12, 0, 0
    };

    // Scheduler'ı address 1090'dan itibaren yaz (instruction 30)
    int base_addr = 1090;
    for (size_t i = 0; i < sizeof(exact_scheduler)/sizeof(long int); i++) {
        mem_write(&mem, base_addr + i, exact_scheduler[i], KERNEL);
    }

    // Constant -1 for unconditional JIF (OS data offset 99)
    mem_write(&mem, OS_DATA_START_ADDR + 99, -1, KERNEL);

    // === Thread Table Setup ===
    printf("Setting up Thread Table at ABSOLUTE addresses...\n\n");

    // Thread 1 (READY) - Absolute addresses 508-515
    mem_write(&mem, 508, 1, KERNEL);    // thread_id
    mem_write(&mem, 509, 0, KERNEL);    // READY state
    mem_write(&mem, 510, 3000, KERNEL); // PC
    mem_write(&mem, 511, 3999, KERNEL); // SP
    mem_write(&mem, 512, 2000, KERNEL); // data_base
    mem_write(&mem, 513, 3000, KERNEL); // instr_base
    mem_write(&mem, 514, 1, KERNEL);    // USER mode
    mem_write(&mem, 515, 0, KERNEL);    // wake_up_count

    // Thread 2 (BLOCKED) - Absolute addresses 516-523
    mem_write(&mem, 516, 2, KERNEL);    // thread_id
    mem_write(&mem, 517, 2, KERNEL);    // BLOCKED state
    mem_write(&mem, 518, 5000, KERNEL); // PC
    mem_write(&mem, 519, 5999, KERNEL); // SP
    mem_write(&mem, 520, 4000, KERNEL); // data_base
    mem_write(&mem, 521, 5000, KERNEL); // instr_base
    mem_write(&mem, 522, 1, KERNEL);    // USER mode
    mem_write(&mem, 523, 50, KERNEL);   // wake_up_count

    // Thread 3 (READY) - Absolute addresses 524-531
    mem_write(&mem, 524, 3, KERNEL);    // thread_id
    mem_write(&mem, 525, 0, KERNEL);    // READY state
    mem_write(&mem, 526, 7000, KERNEL); // PC
    mem_write(&mem, 527, 7999, KERNEL); // SP
    mem_write(&mem, 528, 6000, KERNEL); // data_base
    mem_write(&mem, 529, 7000, KERNEL); // instr_base
    mem_write(&mem, 530, 1, KERNEL);    // USER mode
    mem_write(&mem, 531, 0, KERNEL);    // wake_up_count

    printf("Thread Table Setup:\n");
    for (int i = 1; i <= 3; i++) {
        print_thread_table(&mem, i);
        printf("\n");
    }

    // Temp değişkenleri temizle
    for (int i = 110; i <= 117; i++) {
        mem_write(&mem, i, -999, KERNEL);
    }

    // Working variables temizle
    for (int i = 801; i <= 823; i++) {
        mem_write(&mem, OS_DATA_START_ADDR + i, 0, KERNEL);
    }

    printf("Initial temp variables (should be -999):\n");
    print_temp_variables(&mem);

    // Context switch sinyalini DONE olarak ayarla
    mem_write(&mem, REG_CONTEXT_SWITCH_SIGNAL, 999, KERNEL);

    // CPU'yu başlat
    CPU cpu;
    cpu_init(&cpu, &mem);

    // PC'yi scheduler başlangıcına ayarla (instruction 30 = address 1090)
    mem_write(&mem, REG_PC, 1090, KERNEL);

    printf("\nCalling scheduler starting at PC 1090...\n");
    printf("Expected: Thread 1 should be selected (first READY thread)\n\n");

    // Address mapping reference
    printf("Address Mapping Reference:\n");
    printf("  Instr 30 (SET 509 801): 1090\n");
    printf("  Instr 36 (CPYI 801 805): 1108\n");
    printf("  Instr 37 (JIF 805 69): 1111\n");
    printf("  Instr 38 (CPY 801 810): 1114 (context copy başlangıcı)\n");
    printf("  Instr 67 (SET -999 17): 1201\n");
    printf("  Instr 68 (RET): 1204\n");
    printf("  Instr 69 (ADD 801 8): 1207 (next thread)\n");
    printf("  Instr 71 (HLT): 1213\n\n");

    // Scheduler'ı çalıştır
    int instruction_count = 0;
    int max_instructions = 50;
    bool context_switch_signal_sent = false;
    bool ret_executed = false;

    while (!cpu_is_halted(&cpu) && instruction_count < max_instructions && !ret_executed) {
        long int current_pc = mem_read(&mem, REG_PC, KERNEL);

        printf("[%d] PC: %ld", instruction_count, current_pc);

        // Key checkpoint'lerde extra info göster
        if (current_pc == 1108) {
            long int i_val = mem_read(&mem, OS_DATA_START_ADDR + 801, KERNEL);
            printf(" (CPYI: Reading state from absolute addr %ld)", i_val);
        } else if (current_pc == 1111) {
            long int state_val = mem_read(&mem, OS_DATA_START_ADDR + 805, KERNEL);
            printf(" (JIF: State=%ld, %s)", state_val,
                   state_val <= 0 ? "will continue to context copy" : "will jump to next thread");
        } else if (current_pc == 1114) {
            printf(" (Context copy starts)");
        } else if (current_pc == 1201) {
            printf(" (Setting context switch signal)");
        } else if (current_pc == 1204) {
            printf(" (RET - scheduler finished)");
        } else if (current_pc == 1207) {
            printf(" (Next thread: i += 8)");
        }
        printf("\n");

        cpu_execute_instruction(&cpu);
        instruction_count++;

        // Context switch sinyali kontrol et
        long int signal = mem_read(&mem, REG_CONTEXT_SWITCH_SIGNAL, KERNEL);
        if (signal == -999 && !context_switch_signal_sent) {
            printf("✓ Context switch signal detected!\n");
            context_switch_signal_sent = true;
        }

        // RET çalıştırıldıysa dur
        if (current_pc == 1204) {
            printf("RET executed, scheduler finished.\n");
            ret_executed = true;
            break;
        }
    }

    printf("\nScheduler completed after %d instructions.\n", instruction_count);

    // Working variables durumu
    print_working_variables(&mem);

    // Sonuçları kontrol et
    print_temp_variables(&mem);

    // Beklenen değerler: Thread 1'in bilgileri
    long int selected_thread_id = mem_read(&mem, 110, KERNEL);
    long int selected_state = mem_read(&mem, 111, KERNEL);
    long int selected_pc = mem_read(&mem, 112, KERNEL);
    long int selected_sp = mem_read(&mem, 113, KERNEL);
    long int selected_data_base = mem_read(&mem, 114, KERNEL);
    long int selected_instr_base = mem_read(&mem, 115, KERNEL);
    long int selected_mode = mem_read(&mem, 116, KERNEL);
    long int selected_wake_up = mem_read(&mem, 117, KERNEL);

    printf("\n=== TEST SONUÇLARI ===\n");
    printf("Selected Thread ID: %ld (Expected: 1)\n", selected_thread_id);
    printf("Selected State: %ld (Expected: 0)\n", selected_state);
    printf("Selected PC: %ld (Expected: 3000)\n", selected_pc);
    printf("Selected SP: %ld (Expected: 3999)\n", selected_sp);
    printf("Selected Data Base: %ld (Expected: 2000)\n", selected_data_base);
    printf("Selected Instr Base: %ld (Expected: 3000)\n", selected_instr_base);
    printf("Selected Mode: %ld (Expected: 1)\n", selected_mode);
    printf("Selected Wake Up: %ld (Expected: 0)\n", selected_wake_up);

    bool test_passed = (selected_thread_id == 1 &&
                       selected_state == 0 &&
                       selected_pc == 3000 &&
                       selected_sp == 3999 &&
                       selected_data_base == 2000 &&
                       selected_instr_base == 3000 &&
                       selected_mode == 1 &&
                       selected_wake_up == 0 &&
                       context_switch_signal_sent &&
                       ret_executed);

    printf("\nContext Switch Signal Sent: %s\n", context_switch_signal_sent ? "YES" : "NO");
    printf("RET Executed: %s\n", ret_executed ? "YES" : "NO");
    printf("TEST RESULT: %s\n", test_passed ? "PASSED ✓" : "FAILED ✗");

    if (!test_passed) {
        printf("\nDEBUG INFO:\n");
        printf("Thread table verification:\n");
        printf("  M[509] (Thread 1 state): %ld\n", mem_read(&mem, 509, KERNEL));
        printf("  M[510] (Thread 1 PC): %ld\n", mem_read(&mem, 510, KERNEL));
    }

    mem_free(&mem);
    return test_passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
