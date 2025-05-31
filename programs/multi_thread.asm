######################## OS THREAD ########################

Begin Data Section

# scheduler seçtiği threadin bilgilerini bruaya yazar
50 1    # scheduled_thread_id
51 1    # scheduled_thread_pc

# Thread States
90 0    # THREAD_STATE_READY
91 1    # THREAD_STATE_RUNNING
92 2    # THREAD_STATE_BLOCKED
93 3    # THREAD_STATE_TERMINATED

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

# Temporary variables for syscall handler, cpu buraya hangi threadin syscall yaptığını
# belirmtk için dolduracak, :

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

Begin Instruction Section

# Booting OS: entry point, setting some values like current_thread, last_scheduled etc.
0 	SET  0 100 # to show, OS is succesfuly init
1   CALL 30    # scheduler
2   USER 50    # scheduler tarafından belirlenen 50. adresteki yere git

# scheduler
30  SET  509 801 # index: i -> 509
31  SET  581 802 # N: toplam thread block sayısı (8*10)'dan, döngü için
# loop i to N:
32  CPY  801 803 # temp for i
33  CPY  802 804 # temp for N
34  SUBI 804 803 # N - i <= 0 -> 803
35  JIF  803 49  # N - i <= 0 mı? O halde scheduler baştan çalışsın (cpu idle state)
36  CPYI 801 805 # M[M[801]] -> 805
37  JIF  805 38  # o anki state ready ise bilgileri ver
38  ADDI 801 806 # M[M[801]] -> 806
39  SET  1   807 # for substracting
40  CPY  807 808 #
41  SUBI 806 808 # current - 1: thread_id verir, M[M[801] - 1] -> 808
42  CPYI 808 50  # M[M[801] - 1] -> M[50]
43  CPY  807 809 # for adding
44  ADD  1   809 # current + 1: pc verir, M[M[801] + 1] -> 809
45  CPYI 809 51  # M[M[801] + 1] -> M[50]
46  ADD  801 8   # i += 8, sonraki thread entry'sine bak
47  JIF  99  32  # döngü başına geri dön
48  RET 		 # çağıran yere geri dön
49  HLT






# OS Syscall Handler (OS Instruction Section - 1000'den başlar)
# Supporting up to 10 threads (Thread 1-10) with proper context management
# Stack state when entering: [old_sp], [return_address], ...

# =================== ENTRY AND CONTEXT SAVE ===================
0   POP 113         # temp_old_sp = pop old SP from stack
1   POP 112         # temp_return_address = pop return address from stack
2   CPY 100 111     # temp_current_thread = current_running_thread_id

# =================== SAVE CURRENT THREAD CONTEXT ===================
# Calculate thread table entry address: 500 + (thread_id * 8)
3   CPY 111 114     # thread_id for calculation
4   SET 8 115       # multiplier for table entry size
5   SET 0 116       # result accumulator = 0

# Multiply thread_id * 8 using addition loop
6   CPY 115 117     # counter = 8
7   JIF 117 11      # if counter <= 0, jump to after loop
8   ADDI 116 114    # result += thread_id
9   ADD 117 -1      # counter--
10  JIF -1 7        # unconditional jump back to loop

# Calculate absolute thread table address: 500 + (thread_id * 8)
11  ADD 116 500     # result = 500 + (thread_id * 8)
12  CPY 116 118     # thread_table_base = result

# Save PC and SP to thread table
13  ADD 118 2       # PC offset in thread table
14  CPY 112 118     # save return_address to thread_table[thread_id].PC
15  CPY 118 119     # backup PC address
16  ADD 119 1       # SP offset in thread table
17  CPY 1 119       # save current REG_SP to thread_table[thread_id].SP

# =================== SYSCALL TYPE DISPATCH ===================
18  CPY 2 110       # temp_syscall_type = REG_SYSCALL_RESULT
19  SET 1 115       # SYSCALL_PRN_ID = 1
20  CPY 110 114     # copy syscall_type
21  SUBI 114 115    # syscall_type - SYSCALL_PRN_ID
22  JIF 114 35      # if <= 0, goto handle_prn

23  SET 2 115       # SYSCALL_HLT_ID = 2
24  CPY 110 114     # copy syscall_type
25  SUBI 114 115    # syscall_type - SYSCALL_HLT_ID
26  JIF 114 45      # if <= 0, goto handle_hlt

27  JIF -1 55       # else goto handle_yield (SYSCALL_YIELD_ID = 3)

# Padding for alignment
28  HLT
29  HLT
30  HLT
31  HLT
32  HLT
33  HLT
34  HLT

# =================== HANDLE_PRN ===================
35  # Set current thread to BLOCKED state
36  CPY 111 114     # current_thread_id
37  CPY 118 119     # thread_table_base
38  ADD 119 1       # state offset in thread table
39  SET 2 119       # set state to BLOCKED (2)
40
41  # Set wake_up_instruction_count = 100
42  CPY 118 119     # thread_table_base
43  ADD 119 7       # wake_up_count offset
44  SET 100 119     # set wake_up_count = 100
45  JIF -1 65       # goto scheduler

# =================== HANDLE_HLT ===================
46  # Set current thread to TERMINATED state
47  CPY 111 114     # current_thread_id
48  CPY 118 119     # thread_table_base
49  ADD 119 1       # state offset in thread table
50  SET 3 119       # set state to TERMINATED (3)
51  JIF -1 65       # goto scheduler

# Padding
52  HLT
53  HLT
54  HLT

# =================== HANDLE_YIELD ===================
55  # Set current thread to READY state
56  CPY 111 114     # current_thread_id
57  CPY 118 119     # thread_table_base
58  ADD 119 1       # state offset in thread table
59  SET 0 119       # set state to READY (0)
60  JIF -1 65       # goto scheduler

# Padding
61  HLT
62  HLT
63  HLT
64  HLT

# =================== ROUND-ROBIN SCHEDULER ===================
65  SET 1 120       # next_thread_candidate = 1 (start from thread 1)
66  SET 10 121      # max_threads = 10

# Search for next ready thread
67  CPY 120 114     # candidate_thread_id
68  SET 8 115       # table entry size
69  SET 0 116       # multiply result = 0

# Calculate thread table address for candidate
70  CPY 115 117     # counter = 8
71  JIF 117 75      # if counter <= 0, continue
72  ADDI 116 114    # result += candidate_thread_id
73  ADD 117 -1      # counter--
74  JIF -1 71       # jump back to loop

75  ADD 116 500     # thread_table_addr = 500 + (candidate * 8)
76  ADD 116 1       # state_addr = thread_table_addr + 1
77  CPY 116 117     # load thread state

# Check if thread is READY (state = 0)
78  JIF 117 85      # if state <= 0, thread is READY, switch to it

# Try next thread
79  ADD 120 1       # next_thread_candidate++
80  CPY 121 114     # max_threads
81  CPY 120 115     # candidate_thread_id
82  SUBI 115 114    # candidate - max_threads
83  JIF 115 88      # if candidate <= max_threads, continue search
84  SET 1 120       # wrap around to thread 1

# Continue search if we haven't checked all threads
85  CPY 120 114     # Check if we've looped back to starting point
86  CPY 111 115     # original current_thread
87  SUBI 114 115    # candidate - original
88  JIF 114 67      # if different, continue search
89  JIF -1 120      # No ready threads found, halt system

# =================== CONTEXT SWITCH TO SELECTED THREAD ===================
90  # Selected thread is in 120 (next_thread_candidate)
91  CPY 120 100     # current_running_thread_id = selected_thread
92
93  # Calculate selected thread's table address
94  CPY 120 114     # selected_thread_id
95  SET 8 115       # table entry size
96  SET 0 116       # multiply result = 0

97  CPY 115 117     # counter = 8
98  JIF 117 102     # if counter <= 0, continue
99  ADDI 116 114    # result += selected_thread_id
100 ADD 117 -1      # counter--
101 JIF -1 98       # jump back

102 ADD 116 500     # selected_table_addr = 500 + (selected_id * 8)

# Set selected thread state to RUNNING
103 CPY 116 117     # table_addr
104 ADD 117 1       # state offset
105 SET 1 117       # set state to RUNNING (1)

# Load thread context to mailbox
106 SET 120 600     # MAILBOX_NEXT_THREAD_ID = selected_thread
107
108 CPY 116 117     # table_addr
109 ADD 117 2       # PC offset
110 CPY 117 601     # MAILBOX_NEXT_PC = thread.PC

111 CPY 116 117     # table_addr
112 ADD 117 3       # SP offset
113 CPY 117 602     # MAILBOX_NEXT_SP = thread.SP

114 CPY 116 117     # table_addr
115 ADD 117 4       # data_base offset
116 CPY 117 603     # MAILBOX_NEXT_DATA_BASE = thread.data_base

117 CPY 116 117     # table_addr
118 ADD 117 5       # instr_base offset
119 CPY 117 604     # MAILBOX_NEXT_INSTR_BASE = thread.instr_base

120 CPY 116 117     # table_addr
121 ADD 117 6       # mode offset
122 CPY 117 605     # MAILBOX_NEXT_MODE = thread.mode

# =================== TRIGGER CONTEXT SWITCH ===================
123 SET -999 17     # REG_CONTEXT_SWITCH_SIGNAL = CTX_SWITCH_REQUEST
124 HLT             # End syscall handler - CPU will detect signal and switch

# =================== SYSTEM HALT (No Ready Threads) ===================
125 # If no threads are ready, halt the system
126 HLT
End Instruction Section
###############################################################






######################## USER THREAD 1 ########################

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





######################## USER THREAD 2 ########################
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





######################## USER THREAD 3 ########################
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
