Begin Data Section
0 -1
1 -2
2 -3

# Thread States
90 0    # THREAD_STATE_READY
91 1    # THREAD_STATE_RUNNING
92 2    # THREAD_STATE_BLOCKED
93 3    # THREAD_STATE_TERMINATED

# Syscall IDs
94 1    # SYSCALL_PRN_ID
95 2    # SYSCALL_HLT_ID
96 3    # SYSCALL_YIELD_ID

# Scheduler State
100 1   # current_running_thread_id (başlangıçta Thread 1)
101 1   # next_thread_id (başlangıçta Thread 1)
102 0   # print_block_counter (PRN syscall için)
103 100 # PRINT_BLOCK_DURATION (PRN syscall 100 instruction block eder)

# Temporary variables for syscall handler
110 0   # temp_syscall_type
111 0   # temp_current_thread
112 0   # temp_return_address
113 0   # temp_old_sp
114 0   # temp_thread_entry_base
115 0   # temp_comparison_result
116 -1  # constant_minus_1 (for unconditional jumps)

# Round-robin scheduler variables
120 1   # scheduler_start_thread (round-robin başlangıç thread ID)
121 10  # max_thread_count

# Thread Table (500'den başlıyor)
# Her entry için 8 bilgi: thread_id, thread_state, PC, SP, data_base_addr, instr_base_addr, mode_to_restore, wake_up_instruction_count

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
600
601
602
603
604
605
606
607
End Data Section

Begin Instruction Section
# OS Syscall Handler (OS Instruction Section - 1000'den başlar)
# Supporting 3 threads (Thread 1, 2, 3) - can be extended
# Stack state: [old_sp], [return_address], ...

# =================== ENTRY AND CONTEXT SAVE ===================
0   POP 113         # temp_old_sp (OS data offset 113)
1   POP 112         # temp_return_address (OS data offset 112)
2   CPY 100 111     # temp_current_thread = current_running_thread_id (offset 100)

# Save current thread context immediately
3   CPY 111 114     # current_thread_id for comparison
4   SET 1 115       # check if thread 1
5   SUBI 114 115    # current_thread - 1
6   JIF 114 25      # if thread 1, goto save_thread1_context

7   SET 2 115       # check if thread 2
8   CPY 111 114     # reload current_thread
9   SUBI 114 115    # current_thread - 2
10  JIF 114 30      # if thread 2, goto save_thread2_context

11  JIF -1 35       # else thread 3, goto save_thread3_context

# =================== SYSCALL TYPE DISPATCH ===================
12  CPY 2 110       # temp_syscall_type = REG_SYSCALL_RESULT
13  SET 1 115       # SYSCALL_PRN_ID = 1
14  CPY 110 114     # copy syscall_type
15  SUBI 114 115    # syscall_type - SYSCALL_PRN_ID
16  JIF 114 40      # if <= 0, goto handle_prn

17  SET 2 115       # SYSCALL_HLT_ID = 2
18  CPY 110 114     # copy syscall_type
19  SUBI 114 115    # syscall_type - SYSCALL_HLT_ID
20  JIF 114 50      # if <= 0, goto handle_hlt

21  JIF -1 60       # else goto handle_yield
22  HLT             # padding
23  HLT             # padding
24  HLT             # padding

# Save Thread 1 context (PC and SP)
25  CPY 112 510     # Thread 1 PC = return_address (absolute addr 510)
26  CPY 1 511       # Thread 1 SP = current REG_SP (absolute addr 511)
27  JIF -1 12       # goto syscall_dispatch
28  HLT             # padding
29  HLT             # padding

# Save Thread 2 context
30  CPY 112 518     # Thread 2 PC = return_address (absolute addr 518)
31  CPY 1 519       # Thread 2 SP = current REG_SP (absolute addr 519)
32  JIF -1 12       # goto syscall_dispatch
33  HLT             # padding
34  HLT             # padding

# Save Thread 3 context
35  CPY 112 526     # Thread 3 PC = return_address (absolute addr 526)
36  CPY 1 527       # Thread 3 SP = current REG_SP (absolute addr 527)
37  JIF -1 12       # goto syscall_dispatch
38  HLT             # padding
39  HLT             # padding

# =================== HANDLE_PRN ===================
40  # Print syscall: Set current thread to BLOCKED
41  CPY 111 114     # current_thread_id
42  SET 1 115       # check thread 1
43  SUBI 114 115    # current_thread - 1
44  JIF 114 70      # if thread 1, set thread 1 to blocked

45  SET 2 115       # check thread 2
46  CPY 111 114     # reload current_thread
47  SUBI 114 115    # current_thread - 2
48  JIF 114 75      # if thread 2, set thread 2 to blocked

49  JIF -1 80       # else thread 3, set thread 3 to blocked

# =================== HANDLE_HLT ===================
50  # Halt syscall: Set current thread to TERMINATED
51  CPY 111 114     # current_thread_id
52  SET 1 115       # check thread 1
53  SUBI 114 115    # current_thread - 1
54  JIF 114 85      # if thread 1, set thread 1 to terminated

55  SET 2 115       # check thread 2
56  CPY 111 114     # reload current_thread
57  SUBI 114 115    # current_thread - 2
58  JIF 114 90      # if thread 2, set thread 2 to terminated

59  JIF -1 95       # else thread 3, set thread 3 to terminated

# =================== HANDLE_YIELD ===================
60  # Yield syscall: Set current thread to READY
61  CPY 111 114     # current_thread_id
62  SET 1 115       # check thread 1
63  SUBI 114 115    # current_thread - 1
64  JIF 114 100     # if thread 1, set thread 1 to ready

65  SET 2 115       # check thread 2
66  CPY 111 114     # reload current_thread
67  SUBI 114 115    # current_thread - 2
68  JIF 114 105     # if thread 2, set thread 2 to ready

69  JIF -1 110      # else thread 3, set thread 3 to ready

# =================== SET THREAD STATES ===================
# Set Thread 1 to BLOCKED (PRN)
70  SET 2 509       # Thread 1 state = BLOCKED (absolute addr 509)
71  SET 100 515     # Thread 1 wake_up_count = 100 (absolute addr 515)
72  JIF -1 115      # goto scheduler
73  HLT             # padding
74  HLT             # padding

# Set Thread 2 to BLOCKED (PRN)
75  SET 2 517       # Thread 2 state = BLOCKED (absolute addr 517)
76  SET 100 523     # Thread 2 wake_up_count = 100 (absolute addr 523)
77  JIF -1 115      # goto scheduler
78  HLT             # padding
79  HLT             # padding

# Set Thread 3 to BLOCKED (PRN)
80  SET 2 525       # Thread 3 state = BLOCKED (absolute addr 525)
81  SET 100 531     # Thread 3 wake_up_count = 100 (absolute addr 531)
82  JIF -1 115      # goto scheduler
83  HLT             # padding
84  HLT             # padding

# Set Thread 1 to TERMINATED (HLT)
85  SET 3 509       # Thread 1 state = TERMINATED (absolute addr 509)
86  JIF -1 115      # goto scheduler
87  HLT             # padding
88  HLT             # padding
89  HLT             # padding

# Set Thread 2 to TERMINATED (HLT)
90  SET 3 517       # Thread 2 state = TERMINATED (absolute addr 517)
91  JIF -1 115      # goto scheduler
92  HLT             # padding
93  HLT             # padding
94  HLT             # padding

# Set Thread 3 to TERMINATED (HLT)
95  SET 3 525       # Thread 3 state = TERMINATED (absolute addr 525)
96  JIF -1 115      # goto scheduler
97  HLT             # padding
98  HLT             # padding
99  HLT             # padding

# Set Thread 1 to READY (YIELD)
100 SET 0 509       # Thread 1 state = READY (absolute addr 509)
101 JIF -1 115      # goto scheduler
102 HLT             # padding
103 HLT             # padding
104 HLT             # padding

# Set Thread 2 to READY (YIELD)
105 SET 0 517       # Thread 2 state = READY (absolute addr 517)
106 JIF -1 115      # goto scheduler
107 HLT             # padding
108 HLT             # padding
109 HLT             # padding

# Set Thread 3 to READY (YIELD)
110 SET 0 525       # Thread 3 state = READY (absolute addr 525)
111 JIF -1 115      # goto scheduler
112 HLT             # padding
113 HLT             # padding
114 HLT             # padding

# =================== SIMPLE ROUND-ROBIN SCHEDULER ===================
115 CPY 100 101     # next_thread = current_running_thread_id
116 ADD 101 1       # next_thread++

# Check wrap around (supporting threads 1, 2, 3)
117 SET 3 115       # max_thread = 3
118 CPY 101 114     # copy next_thread
119 SUBI 114 115    # next_thread - max_thread
120 JIF 114 125     # if <= 0, continue
121 SET 1 101       # next_thread = 1 (wrap around)
122 JIF -1 125      # goto check_thread_ready
123 HLT             # padding
124 HLT             # padding

# Check if selected thread is READY (state = 0)
125 CPY 101 114     # next_thread
126 SET 1 115       # check thread 1
127 SUBI 114 115    # next_thread - 1
128 JIF 114 140     # if thread 1, check thread 1 state

129 SET 2 115       # check thread 2
130 CPY 101 114     # reload next_thread
131 SUBI 114 115    # next_thread - 2
132 JIF 114 150     # if thread 2, check thread 2 state

133 JIF -1 160      # else thread 3, check thread 3 state
134 HLT             # padding
135 HLT             # padding
136 HLT             # padding
137 HLT             # padding
138 HLT             # padding
139 HLT             # padding

# Check Thread 1 state
140 CPY 509 114     # load Thread 1 state
141 JIF 114 170     # if state <= 0 (READY), switch to Thread 1
142 ADD 101 1       # try next thread
143 JIF -1 117      # goto wrap around check
144 HLT             # padding
145 HLT             # padding
146 HLT             # padding
147 HLT             # padding
148 HLT             # padding
149 HLT             # padding

# Check Thread 2 state
150 CPY 517 114     # load Thread 2 state
151 JIF 114 185     # if state <= 0 (READY), switch to Thread 2
152 ADD 101 1       # try next thread
153 JIF -1 117      # goto wrap around check
154 HLT             # padding
155 HLT             # padding
156 HLT             # padding
157 HLT             # padding
158 HLT             # padding
159 HLT             # padding

# Check Thread 3 state
160 CPY 525 114     # load Thread 3 state
161 JIF 114 200     # if state <= 0 (READY), switch to Thread 3
162 ADD 101 1       # try next thread
163 JIF -1 117      # goto wrap around check
164 HLT             # padding
165 HLT             # padding
166 HLT             # padding
167 HLT             # padding
168 HLT             # padding
169 HLT             # padding

# =================== CONTEXT SWITCH TO THREAD 1 ===================
170 SET 1 509       # Set Thread 1 state to RUNNING
171 SET 1 100       # current_running_thread_id = 1
172 SET 1 600       # MAILBOX_NEXT_THREAD_ID = 1
173 CPY 510 601     # MAILBOX_NEXT_PC = Thread 1 PC
174 CPY 511 602     # MAILBOX_NEXT_SP = Thread 1 SP
175 CPY 512 603     # MAILBOX_NEXT_DATA_BASE = Thread 1 data_base
176 CPY 513 604     # MAILBOX_NEXT_INSTR_BASE = Thread 1 instr_base
177 CPY 514 605     # MAILBOX_NEXT_MODE = Thread 1 mode
178 JIF -1 215      # goto trigger_context_switch
179 HLT             # padding
180 HLT             # padding
181 HLT             # padding
182 HLT             # padding
183 HLT             # padding
184 HLT             # padding

# =================== CONTEXT SWITCH TO THREAD 2 ===================
185 SET 1 517       # Set Thread 2 state to RUNNING
186 SET 2 100       # current_running_thread_id = 2
187 SET 2 600       # MAILBOX_NEXT_THREAD_ID = 2
188 CPY 518 601     # MAILBOX_NEXT_PC = Thread 2 PC
189 CPY 519 602     # MAILBOX_NEXT_SP = Thread 2 SP
190 CPY 520 603     # MAILBOX_NEXT_DATA_BASE = Thread 2 data_base
191 CPY 521 604     # MAILBOX_NEXT_INSTR_BASE = Thread 2 instr_base
192 CPY 522 605     # MAILBOX_NEXT_MODE = Thread 2 mode
193 JIF -1 215      # goto trigger_context_switch
194 HLT             # padding
195 HLT             # padding
196 HLT             # padding
197 HLT             # padding
198 HLT             # padding
199 HLT             # padding

# =================== CONTEXT SWITCH TO THREAD 3 ===================
200 SET 1 525       # Set Thread 3 state to RUNNING
201 SET 3 100       # current_running_thread_id = 3
202 SET 3 600       # MAILBOX_NEXT_THREAD_ID = 3
203 CPY 526 601     # MAILBOX_NEXT_PC = Thread 3 PC
204 CPY 527 602     # MAILBOX_NEXT_SP = Thread 3 SP
205 CPY 528 603     # MAILBOX_NEXT_DATA_BASE = Thread 3 data_base
206 CPY 529 604     # MAILBOX_NEXT_INSTR_BASE = Thread 3 instr_base
207 CPY 530 605     # MAILBOX_NEXT_MODE = Thread 3 mode
208 JIF -1 215      # goto trigger_context_switch
209 HLT             # padding
210 HLT             # padding
211 HLT             # padding
212 HLT             # padding
213 HLT             # padding
214 HLT             # padding

# =================== TRIGGER CONTEXT SWITCH ===================
215 SET -999 17     # REG_CONTEXT_SWITCH_SIGNAL = CTX_SWITCH_REQUEST
216 HLT             # End syscall handler - CPU will detect signal and switch
End Instruction Section

######################## USER THREAD 1 ########################

# Working Bubble Sort for 5 numbers
# Performs 4 passes to ensure complete sorting
# Array: [5, 8579348734678, 999, 3, -98] -> [-98, 3, 5, 999, 8579348734678]

Begin Data Section
0   5       		# arr[0]
1   8579348734678   # arr[1]
2   999       		# arr[2]
3   3       		# arr[3]
4   -98 			# arr[4]
5   0 				# comp_result: arr_x - arr_y
6   0 				# swap_temp
7   4 				# pass_counter (for N=5 element N-1=4 transition is enough)
8   -1 				# const_minus_1 (for unconditional JIF)
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
###############################################################

######################## USER THREAD 3 ########################
###############################################################
