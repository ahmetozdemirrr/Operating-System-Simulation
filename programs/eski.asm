######################## OS THREAD ########################

Begin Data Section
# Thread States
90 0    # THREAD_STATE_READY
91 1    # THREAD_STATE_RUNNING
92 2    # THREAD_STATE_BLOCKED
93 3    # THREAD_STATE_TERMINATED






110 SET  0    829 # pc
111 SET  1    830 # sp
112 SET  2    831 # syscall_result (id)
113 SET  3    832 # register instruction count
114 SET  4    833 # wakeup instruction count








# Syscall IDs
94 1    # SYSCALL_PRN_ID
95 2    # SYSCALL_HLT_ID
96 3    # SYSCALL_YIELD_ID

# unconditional jif
99 -1

# Scheduler State
100 1   # current_running_thread_id (başlangıçta Thread 1)
101 1   # next_thread_id (başlangıçta Thread 1)
102 0   # print_block_counter (PRN syscall için)
103 100 # PRINT_BLOCK_DURATION (PRN syscall 100 instruction block eder)

# Round-robin scheduler variables
120 1   # scheduler_start_thread (round-robin başlangıç thread ID)
121 10  # max_thread_count

# Thread Table (500'den başlıyor)
# Her entry için 8 bilgi: thread_id, thread_state, PC, SP, data_base_addr,
# instr_base_addr, mode_to_restore, wake_up_instruction_count

# OS Thread Table Entry (500-507)
500 0    # thread_id = 0 (OS)
501 1    # thread_state = 1 (RUNNING)
502 1000 # PC = 1000 (OS instruction start)
503 1999 # SP = 1999 (OS stack top)
504 20   # data_base_addr = 20 (OS data start)
505 1000 # instr_base_addr = 1000 (OS instruction start)
506 0    # mode_to_restore = 0 (KERNEL)
507 0    # wake_up_instruction_count = 0

# User Thread 1 Table Entry (508-515)
508 1    # thread_id = 1
509 0    # thread_state = 0 (READY)
510 3000 # PC = 3000 (Thread 1 instruction start)
511 3999 # SP = 3999 (Thread 1 stack top)
512 2000 # data_base_addr = 2000
513 3000 # instr_base_addr = 3000
514 1    # mode_to_restore = 1 (USER)
515 0    # wake_up_instruction_count = 0

# User Thread 2 Table Entry (516-523)
516 2    # thread_id = 2
517 0    # thread_state = 0 (READY)
518 5000 # PC = 5000 (Thread 2 instruction start)
519 5999 # SP = 5999 (Thread 2 stack top)
520 4000 # data_base_addr = 4000
521 5000 # instr_base_addr = 5000
522 1    # mode_to_restore = 1 (USER)
523 0    # wake_up_instruction_count = 0

# User Thread 3 Table Entry (524-531)
524 3    # thread_id = 3
525 0    # thread_state = 0 (READY)
526 7000 # PC = 7000 (Thread 3 instruction start)
527 7999 # SP = 7999 (Thread 3 stack top)
528 6000 # data_base_addr = 6000
529 7000 # instr_base_addr = 7000
530 1    # mode_to_restore = 1 (USER)
531 0    # wake_up_instruction_count = 0

# User Thread 4 Table Entry (532-539)
532 4    # thread_id = 4
533 0    # thread_state = 0 (READY)
534 9000 # PC = 9000 (Thread 4 instruction start)
535 9999 # SP = 9999 (Thread 4 stack top)
536 8000 # data_base_addr = 8000
537 9000 # instr_base_addr = 9000
538 1    # mode_to_restore = 1 (USER)
539 0    # wake_up_instruction_count = 0

# User Thread 5 Table Entry (540-547)
540 5     # thread_id = 5
541 0     # thread_state = 0 (READY)
542 11000 # PC = 11000 (Thread 5 instruction start)
543 11999 # SP = 11999 (Thread 5 stack top)
544 10000 # data_base_addr = 10000
545 11000 # instr_base_addr = 11000
546 1     # mode_to_restore = 1 (USER)
547 0     # wake_up_instruction_count = 0

# User Thread 6 Table Entry (548-555)
548 6     # thread_id = 6
549 0     # thread_state = 0 (READY)
550 13000 # PC = 13000 (Thread 6 instruction start)
551 13999 # SP = 13999 (Thread 6 stack top)
552 12000 # data_base_addr = 12000
553 13000 # instr_base_addr = 13000
554 1     # mode_to_restore = 1 (USER)
555 0     # wake_up_instruction_count = 0

# User Thread 7 Table Entry (556-563)
556 7     # thread_id = 7
557 0     # thread_state = 0 (READY)
558 15000 # PC = 15000 (Thread 7 instruction start)
559 15999 # SP = 15999 (Thread 7 stack top)
560 14000 # data_base_addr = 14000
561 15000 # instr_base_addr = 15000
562 1     # mode_to_restore = 1 (USER)
563 0     # wake_up_instruction_count = 0

# User Thread 8 Table Entry (564-571)
564 8     # thread_id = 8
565 0     # thread_state = 0 (READY)
566 17000 # PC = 17000 (Thread 8 instruction start)
567 17999 # SP = 17999 (Thread 8 stack top)
568 16000 # data_base_addr = 16000
569 17000 # instr_base_addr = 17000
570 1     # mode_to_restore = 1 (USER)
571 0     # wake_up_instruction_count = 0

# User Thread 9 Table Entry (572-579)
572 9     # thread_id = 9
573 0     # thread_state = 0 (READY)
574 19000 # PC = 19000 (Thread 9 instruction start)
575 19999 # SP = 19999 (Thread 9 stack top)
576 18000 # data_base_addr = 18000
577 19000 # instr_base_addr = 19000
578 1     # mode_to_restore = 1 (USER)
579 0     # wake_up_instruction_count = 0

# User Thread 10 Table Entry (580-587)
580 10    # thread_id = 10
581 0     # thread_state = 0 (READY)
582 21000 # PC = 21000 (Thread 10 instruction start)
583 21999 # SP = 21999 (Thread 10 stack top)
584 20000 # data_base_addr = 20000
585 21000 # instr_base_addr = 21000
586 1     # mode_to_restore = 1 (USER)
587 0     # wake_up_instruction_count = 0


# Context Switch Target: The scheduler writes the entries of the thread it decides will work here.
# then a signal is written to a special register. The CPU simulator constantly checks this signal, if the
# signal indicates that the scheduler has finished its operation, the CPU should update the cpu struct by
# reading 600 - 607 (in KERNEL mode, offset: 0)
End Data Section

######################## OS THREAD ########################

Begin Data Section
# Thread States
90 0    # THREAD_STATE_READY
91 1    # THREAD_STATE_RUNNING
92 2    # THREAD_STATE_BLOCKED
93 3    # THREAD_STATE_TERMINATED

# Syscall IDs
94 1    # SYSCALL_PRN_ID
95 2    # SYSCALL_HLT_ID
96 3    # SYSCALL_YIELD_ID

# Unconditional JIF
99 -1   # Constant -1 for unconditional jumps

# Scheduler State
100 1   # current_running_thread_id (başlangıçta Thread 1)
101 1   # next_thread_id (başlangıçta Thread 1)
102 0   # print_block_counter (PRN syscall için)
103 100 # PRINT_BLOCK_DURATION (PRN syscall 100 instruction block eder)

# Temporary variables for syscall handler and scheduler
110 0   # temp_thread_id
111 0   # temp_thread_state
112 0   # temp_thread_pc
113 0   # temp_thread_sp
114 0   # temp_thread_data_base
115 0   # temp_thread_instr_base
116 0   # mode_to_restore
117 0   # wake_up_instr_count

# Round-robin scheduler variables
120 1   # scheduler_start_thread (round-robin başlangıç thread ID)
121 10  # max_thread_count

# Thread Table (500’den başlıyor)
# Her entry için 8 bilgi: thread_id, state, PC, SP, data_base_addr,
# instr_base_addr, mode_to_restore, wake_up_instruction_count

# OS Thread Table Entry (500–507)
500 0    # thread_id = 0 (OS)
501 1    # thread_state = 1 (RUNNING)
502 1000 # PC = 1000 (OS instruction start)
503 1999 # SP = 1999 (OS stack top)
504 20   # data_base_addr = 20 (OS data start)
505 1000 # instr_base_addr = 1000 (OS instruction start)
506 0    # mode_to_restore = 0 (KERNEL)
507 0    # wake_up_instruction_count = 0

# User Thread 1 Table Entry (508–515) - Idle Thread
508 1    # thread_id = 1
509 0    # thread_state = 0 (READY)
510 3000 # PC = 3000 (Thread 1 instruction start)
511 3999 # SP = 3999 (Thread 1 stack top)
512 2000 # data_base_addr = 2000
513 3000 # instr_base_addr = 3000
514 1    # mode_to_restore = 1 (USER)
515 0    # wake_up_instruction_count = 0

# User Thread 2 Table Entry (516–523)
516 2    # thread_id = 2
517 0    # thread_state = 0 (READY)
518 5000 # PC = 5000 (Thread 2 instruction start)
519 5999 # SP = 5999 (Thread 2 stack top)
520 4000 # data_base_addr = 4000
521 5000 # instr_base_addr = 5000
522 1    # mode_to_restore = 1 (USER)
523 0    # wake_up_instruction_count = 0

# User Thread 3 Table Entry (524–531)
524 3    # thread_id = 3
525 0    # thread_state = 0 (READY)
526 7000 # PC = 7000 (Thread 3 instruction start)
527 7999 # SP = 7999 (Thread 3 stack top)
528 6000 # data_base_addr = 6000
529 7000 # instr_base_addr = 7000
530 1    # mode_to_restore = 1 (USER)
531 0    # wake_up_instruction_count = 0

# User Thread 4 Table Entry (532–539)
532 4    # thread_id = 4
533 0    # thread_state = 0 (READY)
534 9000 # PC = 9000 (Thread 4 instruction start)
535 9999 # SP = 9999 (Thread 4 stack top)
536 8000 # data_base_addr = 8000
537 9000 # instr_base_addr = 9000
538 1    # mode_to_restore = 1 (USER)
539 0    # wake_up_instruction_count = 0

# User Thread 5 Table Entry (540–547)
540 5     # thread_id = 5
541 0     # thread_state = 0 (READY)
542 11000 # PC = 11000 (Thread 5 instruction start)
543 11999 # SP = 11999 (Thread 5 stack top)
544 10000 # data_base_addr = 10000
545 11000 # instr_base_addr = 11000
546 1     # mode_to_restore = 1 (USER)
547 0     # wake_up_instruction_count = 0

# User Thread 6 Table Entry (548–555)
548 6     # thread_id = 6
549 0     # thread_state = 0 (READY)
550 13000 # PC = 13000 (Thread 6 instruction start)
551 13999 # SP = 13999 (Thread 6 stack top)
552 12000 # data_base_addr = 12000
553 13000 # instr_base_addr = 13000
554 1     # mode_to_restore = 1 (USER)
555 0     # wake_up_instruction_count = 0

# User Thread 7 Table Entry (556–563)
556 7     # thread_id = 7
557 0     # thread_state = 0 (READY)
558 15000 # PC = 15000 (Thread 7 instruction start)
559 15999 # SP = 15999 (Thread 7 stack top)
560 14000 # data_base_addr = 14000
561 15000 # instr_base_addr = 15000
562 1     # mode_to_restore = 1 (USER)
563 0     # wake_up_instruction_count = 0

# User Thread 8 Table Entry (564–571)
564 8     # thread_id = 8
565 0     # thread_state = 0 (READY)
566 17000 # PC = 17000 (Thread 8 instruction start)
567 17999 # SP = 17999 (Thread 8 stack top)
568 16000 # data_base_addr = 16000
569 17000 # instr_base_addr = 17000
570 1     # mode_to_restore = 1 (USER)
571 0     # wake_up_instruction_count = 0

# User Thread 9 Table Entry (572–579)
572 9     # thread_id = 9
573 0     # thread_state = 0 (READY)
574 19000 # PC = 19000 (Thread 9 instruction start)
575 19999 # SP = 19999 (Thread 9 stack top)
576 18000 # data_base_addr = 18000
577 19000 # instr_base_addr = 19000
578 1    # mode_to_restore = 1 (USER)
579 0    # wake_up_instruction_count = 0

# User Thread 10 Table Entry (580–587)
580 10   # thread_id = 10
581 0    # thread_state = 0 (READY)
582 21000 # PC = 21000 (Thread 10 instruction start)
583 21999 # SP = 21999 (Thread 10 stack top)
584 20000 # data_base_addr = 20000
585 21000 # instr_base_addr = 21000
586 1    # mode_to_restore = 1 (USER)
587 0    # wake_up_instruction_count = 0

End Data Section

Begin Instruction Section
#subroutine# booting OS: entry point
0  SET  0 100      # OS başarıyla başlatıldı
1  CALL 30         # Scheduler’ı çağır
2  USER 112        # Scheduler’ın belirlediği thread PC’sine atla
#subroutine# booting OS end

#subroutine# scheduler start
# Round-robin: current_running_thread_id’den sonrakinden başla
30  CPY  100 801    # current_running_thread_id (M[100]) -> M[801], aramaya bundan sonra başlanacak
31  SET  8   802    # Sabit 8 (thread entry boyutu) -> M[802]
32  MUL  801 802    # thread_id * 8 -> M[802]
33  SET  500 803    # Thread tablosu başlangıcı (M[500]) -> M[803]
34  ADDI 802 803    # i = 500 + (thread_id * 8) -> M[803]
35  SET  1   804    # Sabit 1 -> M[804]
36  ADDI 801 804    # next_thread_id = current_thread_id + 1 -> M[804]
37  CPY  121 805    # max_thread_count (M[121]=10) -> M[805]
38  SUBI 804 805    # next_thread_id - max_thread_count -> M[805]
39  JIF  805 40     # next_thread_id > max_thread_count ise Thread 1’e sıfırla
40  SET  1   804    # next_thread_id = 1 (tablo başına dön)

# Tarama başlangıç adresini ayarla
41  CPY  804 806    # next_thread_id -> M[806]
42  SET  8   807    # Sabit 8 -> M[807]
43  MUL  806 807    # next_thread_id * 8 -> M[807]
44  SET  500 808    # Thread tablosu başlangıcı -> M[808]
45  ADDI 807 808    # i = 500 + (next_thread_id * 8) -> M[808]
46  SET  581 809    # N = 581 (Thread 10’un state adresi) -> M[809]
47  SET  0   810    # found_ready = 0 -> M[810]

# Ana döngü: Thread 1–10’da ready thread ara
48  CPY  808 811    # temp_i = i -> M[811]
49  CPY  809 812    # temp_N = N -> M[812]
50  SUBI 812 811    # N - i -> M[811]
51  JIF  811 88     # i >= N ise, başa dön veya idle thread’e geç

# Wake-up kontrolü
52  CPY  808 813    # i -> M[813]
53  SET  7   814    # Offset 7 (wake_up_instr_count) -> M[814]
54  ADDI 813 814    # i+7 (wake_up_count adresi) -> M[814]
55  CPYI 814 815    # M[i+7] -> M[815] (wake_up_count)
56  CPY  3   816    # REG_INSTR_COUNT (M[3]) -> M[816]
57  SUBI 815 816    # wake_up_count - instr_count -> M[816]
58  JIF  816 59     # wake_up_count <= instr_count ise READY yap
59  SET  0   817    # state = READY (0)
60  CPY  817 808    # M[817] -> M[i+1] (state’e yaz)

# Thread state kontrolü
61  CPYI 808 817    # M[i+1] -> M[817] (thread state)
62  JIF  817 64     # state <= 0 (READY) ise context copy’ye atla
63  JIF  99  84     # state > 0 (NOT READY) ise next thread’e git

# Ready thread bulundu, context bilgilerini kopyala
64  SET  1   810    # found_ready = 1 -> M[810]
65  CPY  808 818    # i değerini M[818]’e kopyala

# Thread ID (offset 0): M[i] -> temp_thread_id (110)
66  CPYI 818 110    # M[i] -> M[110] (temp_thread_id)

# Thread State (offset 1): M[i+1] -> temp_thread_state (111)
67  SET  1   819    # Sabit 1
68  CPY  818 820    # i’yi M[820]’ye kopyala
69  ADDI 820 819    # i+1 -> M[820]
70  CPYI 820 111    # M[i+1] -> M[111] (temp_thread_state)

# PC (offset 2): M[i+2] -> temp_thread_pc (112)
71  SET  2   821    # Sabit 2
72  CPY  818 822    # i’yi M[822]’ye kopyala
73  ADDI 822 821    # i+2 -> M[822]
74  CPYI 822 112    # M[i+2] -> M[112] (temp_thread_pc)

# SP (offset 3): M[i+3] -> temp_thread_sp (113)
75  SET  3   823    # Sabit 3
76  CPY  818 824    # i’yi M[824]’ye kopyala
77  ADDI 824 823    # i+3 -> M[824]
78  CPYI 824 113    # M[i+3] -> M[113] (temp_thread_sp)

# Data Base (offset 4): M[i+4] -> temp_thread_data_base (114)
79  SET  4   825    # Sabit 4
80  CPY  818 826    # i’yi M[826]’ya kopyala
81  ADDI 826 825    # i+4 -> M[826]
82  CPYI 826 114    # M[i+4] -> M[114] (temp_thread_data_base)

# Instruction Base (offset 5): M[i+5] -> temp_thread_instr_base (115)
83  SET  5   827    # Sabit 5
84  CPY  818 828    # i’yi M[828]’ya kopyala
85  ADDI 828 827    # i+5 -> M[828]
86  CPYI 828 115    # M[i+5] -> M[115] (temp_thread_instr_base)

# Mode to Restore (offset 6): M[i+6] -> mode_to_restore (116)
87  SET  6   829    # Sabit 6
88  CPY  818 830    # i’yi M[830]’ya kopyala
89  ADDI 830 829    # i+6 -> M[830]
90  CPYI 830 116    # M[i+6] -> M[116] (mode_to_restore)

# Wake-up Instruction Count (offset 7): M[i+7] -> wake_up_instr_count (117)
91  SET  7   831    # Sabit 7
92  CPY  818 832    # i’yi M[832]’ye kopyala
93  ADDI 832 831    # i+7 -> M[832]
94  CPYI 832 117    # M[i+7] -> M[117] (wake_up_instr_count)

# Context switch sinyali gönder ve current_running_thread_id güncelle
95  CPY  110 100    # temp_thread_id -> current_running_thread_id (M[100])
96  SET  -999 17    # REG_CONTEXT_SWITCH_SIGNAL = CTX_SWITCH_REQUEST
97  RET             # Çağıran yere geri dön

# Next thread
98  ADD  808 8      # i += 8, sonraki thread entry’sine bak
99  JIF  99  48     # Döngü başına geri dön

# Tablo sonuna ulaşıldı, baştan başla veya idle thread’e geç
100 CPYI 810 833    # found_ready -> M[833]
101 JIF  833 108    # found_ready = 1 ise RET
102 SET  509 808    # i = 509 (Thread 1 state) -> M[808]
103 CPYI 808 817    # M[i+1] -> M[817] (Thread 1 state)
104 JIF  817 64     # Thread 1 READY ise context copy’ye atla
105 SET  0   810    # found_ready = 0 -> M[810] (gereksiz, güvenlik için)

# Idle thread’e geç (Thread 1)
106 SET  508 110    # Thread 1 thread_id = 1
107 SET  509 111    # Thread 1 state = 0
108 SET  510 112    # Thread 1 PC = 3000
109 SET  511 113    # Thread 1 SP = 3999
110 SET  512 114    # Thread 1 data_base = 2000
111 SET  513 115    # Thread 1 instr_base = 3000
112 SET  514 116    # Thread 1 mode = 1 (USER)
113 SET  515 117    # Thread 1 wake_up_count = 0
114 SET  1   100    # current_running_thread_id = 1 (idle thread)
115 SET  -999 17    # REG_CONTEXT_SWITCH_SIGNAL = CTX_SWITCH_REQUEST
116 RET             # Geri dön
End Instruction Section
###############################################################






######################## USER THREAD 1 ########################
# idle thread, for efficient CPU:
Begin Data Section
# symbolic values for debugging
0 -73
1 -73
2 -73
3 -52
End Data Section

Begin Instruction Section
0 SYSCALL YIELD # scheduler'a kontrolü geri ver
1 JIF 99 0      # unconditional jump
End Instruction Section
###############################################################







######################## USER THREAD 2 ########################

# Working Bubble Sort for 5 numbers
# Performs 4 passes to ensure complete sorting
# Array: [5, 8579348734678, 999, 3, -98] -> [-98, 3, 5, 999, 8579348734678]

Begin Data Section
0   5               # arr[0]
1   8579348734678   # arr[1]
2   999             # arr[2]
3   3               # arr[3]
4   -98             # arr[4]
5   0               # comp_result: arr_x - arr_y
6   0               # swap_temp
7   4               # pass_counter (for N=5 element N-1=4 transition is enough)
8   -1              # const_minus_1 (for unconditional JIF)
End Data Section

Begin Instruction Section
# Bubble Sort for 5 elements. N-1 passes.
# Data offsets: arr[0]=0,... arr[4]=4. comp_result=5, swap_temp=6, pass_counter=7, const_minus_1=8

# --- Outer Loop (Pass Control) ---
0   CPY 7 5         # M[5] = pass_counter (content of M[7])
1   JIF 5 32        # if M[5] (pass_counter) <= 0, jump to HLT (instr 32).

# --- PASS Begin ---
# Compare: arr[0] (M[0]) and arr[1] (M[1])
# substract them and compare with JIF
2   CPY 1 5         # M[5] = arr[1] (tarfet for SUBI)
3   CPY 0 6         # M[6] = arr[0] (source for SUBI)
4   SUBI 6 5        # M[5] (target) = M[6](arr[0]) - M[5](arr[1]). Rseult: M[5] = arr[0] - arr[1]
5   JIF 5 9         # if M[5] (arr[0]-arr[1]) <= 0, do not swap, jump to instr 9.
6   CPY 0 6         # swap_temp (M[6]) = arr[0]
7   CPY 1 0         # arr[0] = arr[1]
8   CPY 6 1         # arr[1] = swap_temp

# Compare: arr[1] (M[1]) and arr[2] (M[2])
9   CPY 2 5         # M[5] = arr[2]
10  CPY 1 6         # M[6] = arr[1]
11  SUBI 6 5        # M[5] = arr[1] - arr[2]
12  JIF 5 16        # Eğer M[5] <= 0, do not swap, jump to instr 16.
13  CPY 1 6         # swap_temp (M[6]) = arr[1]
14  CPY 2 1         # arr[1] = arr[2]
15  CPY 6 2         # arr[2] = swap_temp

# Compare: arr[2] (M[2]) ve arr[3] (M[3])
16  CPY 3 5         # M[5] = arr[3]
17  CPY 2 6         # M[6] = arr[2]
18  SUBI 6 5        # M[5] = arr[2] - arr[3]
19  JIF 5 23        # Eğer M[5] <= 0, do not swap, jump to instr 23.
20  CPY 2 6         # swap_temp (M[6]) = arr[2]
21  CPY 3 2         # arr[2] = arr[3]
22  CPY 6 3         # arr[3] = swap_temp

# Compare: arr[3] (M[3]) ve arr[4] (M[4])
23  CPY 4 5         # M[5] = arr[4]
24  CPY 3 6         # M[6] = arr[3]
25  SUBI 6 5        # M[5] = arr[3] - arr[4]
26  JIF 5 30        # Eğer M[5] <= 0, do not swap, jump to instr 30.
27  CPY 3 6         # swap_temp (M[6]) = arr[3]
28  CPY 4 3         # arr[3] = arr[4]
29  CPY 6 4         # arr[4] = swap_temp

###############################################################
# for additional number
# Compare: arr[4] (M[5]) ve arr[5] (M[5])
#-  CPY 5 5         # M[5] = arr[5]
#-  CPY 4 6         # M[6] = arr[4]
#-  SUBI 6 5        # M[5] = arr[3] - arr[4]
#-  JIF 5 37        # if M[5] <= 0, do not swap, jump to instr 37.
#-  CPY 4 6         # swap_temp (M[6]) = arr[4]
#-  CPY 5 3         # arr[4] = arr[5]
#-  CPY 6 5         # arr[5] = swap_temp
###############################################################

# --- PASS End ---
30  ADD 7 -1        # pass_counter (M[7])--
31  JIF 8 0         # unconditional jump (because M[8]=-1) jump to instr 0.

# --- Sorting is ended ---
32  HLT
End Instruction Section
###############################################################





######################## USER THREAD 3 ########################
# Linear Search Implementation
Begin Data Section
# Array elements (indices 0-9)
0   32
1   12
2   998
3   -5
4   234
5   -345
6   1000
7   3
8   4
9   4
10  10   # array size
11  -345 # KEY to search for
12  -1   # result index (will be set to found index or -1)
13  0    # i (loop counter)

99  -1   # constant -1 for unconditional jumps
End Data Section

Begin Instruction Section
# Linear Search Algorithm
# for (i = 0; i < N; i++) {
#     if (((arr[i] - KEY) <= 0) && ((KEY - arr[i]) <= 0)) {
#         index = i;
#         break;
#     }
# }
# Döngü başlangıcı

0   CPY  10  14  # N -> 14
1   CPY  13  15  # i -> 15
2   CPY  14  100 # temp for N
3   CPY  15  101 # temp for i
4   SUBI 100 101 # i - N -> 15
5   JIF  101 18  # M[14] <= 0 ise i >= N olmuştur, bitir: HLT yap
6   CPY  11  16  # KEY -> 16
7   CPYI 13  17  # curr_elem: arr[M[13]] -> 17
8   SUBI 16  17  # KEY - curr_elem -> 17
9   JIF  17  12  # 17'deki sonuç 0'dan küçükse sonraki koşula bakalım (Y'de)
10  ADD  13  1   # eğer sonuç büyükse diğer iterasyona geçebiliriz: i++
11  JIF  99  0   # başa dön: koşulsuz

# KEY ve curr_elem dirty olabilir SUBI sonucunda, yeniden alıyorzu
12  CPY  11 16   # KEY -> 16
13  CPYI 13 17   # curr_elem: arr[M[13]] -> 17, dirty oldu yeniden alıyoruz
14  SUBI 17 16   # ters çıkarma
15  JIF  16 18   # bulundu
16  ADD  13 1    # bulunmadı: i++ sonraki iterasyona git
17  JIF  99 0    # başa dön: koşulsuz

18  CPY  13 12      # i -> result
19  SYSCALL PRN 12  # print result
20  HLT             # stop execution

End Instruction Section
###############################################################





######################## USER THREAD 4 ########################
# Multiplication Implementation
Begin Data Section
0   80000   # mult operand 1
1   -12     # mult operand 2
2   0       # counter i for loop
3   0       # summ_buffer: for, M[3] += M[1]
99  -1      # constant -1 for unconditional jumps
End Data Section

Begin Instruction Section
# Multiplication alg.
# for (i = 0; i < operand_1; i++) {
# 	sum_buffer += operand_2;
# }
# loop strat:

0   CPY  0 4       # operand_1 -> 4
1   CPY  2 5       # i -> 5
2   CPY  4 100     # opeand_1: temp
3   CPY  5 101     # i: temp
4   SUBI 100 101   # N - i
5   JIF  101 9     # döngü bitti bitir: HLT
6   ADDI 3 1       # M[3] += M[1] : sum_buffer += operand_2
7   ADD  2 1       # i++, next iteration
8   JIF  99 0      # unconditionl jump
9   SYSCALL PRN 3  # printing result
10  HLT

End Instruction Section
###############################################################
