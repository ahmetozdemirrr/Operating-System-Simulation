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
97 1    # constant 1
98 8    # TABLE_ENTRY_SIZE
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

Begin Instruction Section
#subroutine# booting OS: entry point
0   SET  0 100      # OS başarıyla başlatıldı
1   CALL 30         # Scheduler’ı çağır
2   USER 112        # Scheduler’ın belirlediği thread PC’sine atla
#subroutine# booting OS end

#subroutine# scheduler start
30  CPY  100 800 # şuan çalışan threadin id'sini al: M[100], her zaman bu bilgiyi tutar
31  ADDI 800 97  # next thread's id, 97 sabit '1' sayısını tutar
32  CPY  800 801 # temp next_thread_id
33  SUBI 121 801 # max_thread - next_id -> 801
34  JIF  801 45  # eğer max thread aşılıyorsa, başa dön
35  SET  501 802 # M[802] table sorguları için offset 500 (+1 table'ın ikinci sütunu anlamına gelir) değerini tutar

# çarpma işlemi: 500 + id * 8 + 1: next_thread'in state'ine id'si üzerinden ulaşmak için bu döngüyü kullanacağız
36  CPY  98  803 # operand_1 -> 803, 98 her zaman sabit '8' sayısını tutar
37  SET  0   804 # i -> 804
38  CPY  804 805 # i -> 805 (temp), subi'de dirty olacağı için kopyalıyoruz
39  CPY  801 806 # operand_1 -> 806 (temp), burada id kadar toplama işlemi yapacağız
40  SUBI 806 805 # N - i, o yüzden id - i <= 0 kontrolü yapıyoruz
41  JIF  805 Y   # Y devam edilecek yer (scheduler contextte)
42  ADDI 802 803 # 501 + 8 her seferinde (M[802]: 501 offsetini tutar, M[803]: 8 sabitini tutar)
43  ADDI 804 97  # i++
44  JIF  99  36  # çarpma döngüsün başına dön koşulsz
45  SET  1   800 # burayı 1 yap schedulerburadan aramaya devam etmeli, çünkü table'da sona gelindi başa dön

# context:
# - M[800]: Geçici thread ID, önce current_running_thread_id, sonra next_thread_id, sıfırlamada 1
# - M[802]: Hedef thread’in state adresi, tarama başlangıç noktası

Y
#subroutine# scheduler end

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





######################## USER THREAD 4 ########################
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
