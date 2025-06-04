######################## OS THREAD ########################
# subroutines:
# 0   -> entry point of OS (booting)
# 5   -> thread table address calculator
# 30  -> scheduler
# 90  -> update_current_thread_in_table
# 160 -> prepare_context_switch
# 200 -> syscall_handler
# 210 -> equality checker (bunu daha aşağıya taşı, stack için yer kalmıyor)
#
###########################################################

Begin Data Section

# Constants
80 -1   # unconditional jump etc.
81 0
82 1
83 2
84 3
85 4
86 5
87 6
88 7
89 8    # TABLE_ENTRY_SIZE

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
100 0   # current_running_thread_id (başlangıçta Thread 1)
101 100 # PRINT_BLOCK_DURATION (PRN syscall 100 instruction block eder)

# Round-robin scheduler variables
102 10  # max_thread_count

# thread table hesaplama rutini
112 0    # first parameter: thread_id
113 0    # second parametrer: field
114 200  # base adres for thread table
115 0    # result

# equality checker
116 0    # first number
117 0    # second number
118 0    # is_equal, 0:no, 1:yes

# Thread Table (200'den başlıyor)
# Her entry için 8 bilgi: thread_id, thread_state, PC, SP, data_base_addr,
# instr_base_addr, register_instruction_count, wake_up_instruction_count

# OS Thread Table Entry (200-207)
200 0    # thread_id = 0 (OS)
201 1    # thread_state = 1 (RUNNING)
202 1000 # PC = 1000 (OS instruction start)
203 1999 # SP = 1999 (OS stack top)
204 20   # data_base_addr = 20 (OS data start)
205 1000 # instr_base_addr = 1000 (OS instruction start)
206 0    # register_instruction_count = 0
207 0    # wake_up_instruction_count = 0

# OS Thread Table Entry (208-215) (idle thread)
208 1    # thread_id = 1
209 0    # thread_state = 0 (READY)
210 3000 # PC = 3000 (Thread 1 instruction start)
211 3999 # SP = 3999 (Thread 1 stack top)
212 2000 # data_base_addr = 2000
213 3000 # instr_base_addr = 3000
214 0    # register_instruction_count = 0
215 0    # wake_up_instruction_count = 0

# User Thread 2 Table Entry (216-223)
216 2    # thread_id = 2
217 0    # thread_state = 0 (READY)
218 5000 # PC = 5000 (Thread 2 instruction start)
219 5999 # SP = 5999 (Thread 2 stack top)
220 4000 # data_base_addr = 4000
221 5000 # instr_base_addr = 5000
222 0    # register_instruction_count = 0
223 0    # wake_up_instruction_count = 0

# User Thread 3 Table Entry (224-231)
224 3    # thread_id = 3
225 0    # thread_state = 0 (READY)
226 7000 # PC = 7000 (Thread 3 instruction start)
227 7999 # SP = 7999 (Thread 3 stack top)
228 6000 # data_base_addr = 6000
229 7000 # instr_base_addr = 7000
230 0    # register_instruction_count = 0
231 0    # wake_up_instruction_count = 0

# User Thread 4 Table Entry (232-239)
232 4    # thread_id = 4
233 0    # thread_state = 0 (READY)
234 9000 # PC = 9000 (Thread 4 instruction start)
235 9999 # SP = 9999 (Thread 4 stack top)
236 8000 # data_base_addr = 8000
237 9000 # instr_base_addr = 9000
238 0    # register_instruction_count = 0
239 0    # wake_up_instruction_count = 0

# User Thread 5 Table Entry (240-247)
240 5     # thread_id = 5
241 3     # thread_state = 3 (Terminated)
242 11000 # PC = 11000 (Thread 5 instruction start)
243 11999 # SP = 11999 (Thread 5 stack top)
244 10000 # data_base_addr = 10000
245 11000 # instr_base_addr = 11000
246 0     # register_instruction_count = 0
247 0     # wake_up_instruction_count = 0

# User Thread 6 Table Entry (248-255)
248 6     # thread_id = 6
249 3     # thread_state = 3 (Terminated)
250 13000 # PC = 13000 (Thread 6 instruction start)
251 13999 # SP = 13999 (Thread 6 stack top)
252 12000 # data_base_addr = 12000
253 13000 # instr_base_addr = 13000
254 0     # register_instruction_count = 0
255 0     # wake_up_instruction_count = 0

# User Thread 7 Table Entry (256-263)
256 7     # thread_id = 7
257 3     # thread_state = 3 (Terminated)
258 15000 # PC = 15000 (Thread 7 instruction start)
259 15999 # SP = 15999 (Thread 7 stack top)
260 14000 # data_base_addr = 14000
261 15000 # instr_base_addr = 15000
262 0     # register_instruction_count = 0
263 0     # wake_up_instruction_count = 0

# User Thread 8 Table Entry (264-271)
264 8     # thread_id = 8
265 3     # thread_state = 3 (Terminated)
266 17000 # PC = 17000 (Thread 8 instruction start)
267 17999 # SP = 17999 (Thread 8 stack top)
268 16000 # data_base_addr = 16000
269 17000 # instr_base_addr = 17000
270 0     # register_instruction_count = 0
271 0     # wake_up_instruction_count = 0

# User Thread 9 Table Entry (272-279)
272 9     # thread_id = 9
273 3     # thread_state = 3 (Terminated)
274 19000 # PC = 19000 (Thread 9 instruction start)
275 19999 # SP = 19999 (Thread 9 stack top)
276 18000 # data_base_addr = 18000
277 19000 # instr_base_addr = 19000
278 0     # register_instruction_count = 0
279 0     # wake_up_instruction_count = 0

# User Thread 10 Table Entry (280-287)
280 10    # thread_id = 10
281 3     # thread_state = 3 (Terminated)
282 21000 # PC = 21000 (Thread 10 instruction start)
283 21999 # SP = 21999 (Thread 10 stack top)
284 20000 # data_base_addr = 20000
285 21000 # instr_base_addr = 21000
286 0     # register_instruction_count = 0
287 0     # wake_up_instruction_count = 0

# Mailboxes
600 0     # MAILBOX_THREAD_ID
601 0     # MAILBOX_STATE
602 0     # MAILBOX_PC: booting kısmı buradan seçilen threadin PC değerini çeker ve USER ile çağırır
603 0     # MAILBOX_SP
604 0     # MAILBOX_DATA_BASE
605 0     # MAILBOX_INSTR_BASE
606 0     # MAILBOX_INSTR_COUNT
607 0     # MAILBOX_WAKEUP_COUNT

#
# Context Switch Target: The scheduler writes the entries of the thread it decides will work here.
# then a signal is written to a special register. The CPU simulator constantly checks this signal,
# if the signal indicates that the scheduler has finished its operation, the CPU should update the
# cpu struct by reading 600 - 607 (in KERNEL mode, offset: 0)
#
End Data Section

Begin Instruction Section
0   SET   0   100  # çalışan thread
1   CALL  30       # scheduler çağrısı
2   USER  602      # scheduler seçilen threaidn PC değerini buraya koyar
3   SET   0   73   # dummy instruction
4   SET   0   73   # dummy instruction
#
# get_table_adress (subroutine):
#
# thread table adresi getirmek için burada bir rutin yazacağız:
#
# long get_thread_table_addr(int thread_id, int field) {
#    return 200 + (thread_id * 8) + field;
# }
#
# parametrelerin 112 ve 113 adreslerine koyulmasını bekleriz, 114 de thread table base adresini tutar
# 115 adresi de sonucun koyulacağı yer olacak
#
# kullanılan değişkenler: 350-355
#
5   CPY   112 350  # 350. adrese ilk parametreyi al: thread_id
6   CPY   113 351  # 351. adrese ikinci parametreyi al: field
7   SET   0   352  # i = 0
8   CPY   89  355  # thread table entry size
9   CPY   114 353  # base adresi al (thread table için 200)
10  CPY   352 354  # temp i (SUBI için dirty olacak)
11  SUBI  350 354  # entry size - i <= 0 mı kontrolü için
12  JIF   354 16   # sonuç 0'dan küçükse döngü bitir
13  ADDI  353 355  # base adrese 8 ekle
14  ADD   352 1    # 352'de orijinal i tutuluyor, yani: i++
15  JIF   80  10   # döngü başına koşulsuz atla
16  ADDI  353 351  # son olarak field değerini ekle
17  SET   115 353  # sonucun alınacağı yere son değeri yaz
18  RET            # çağıran rutine geri dön
19  SET   0   73   # dummy instruction
20  SET   0   73   # dummy instruction
21  SET   0   73   # dummy instruction
22  SET   0   73   # dummy instruction
23  SET   0   73   # dummy instruction
24  SET   0   73   # dummy instruction
25  SET   0   73   # dummy instruction
26  SET   0   73   # dummy instruction
27  SET   0   73   # dummy instruction
28  SET   0   73   # dummy instruction
29  SET   0   73   # dummy instruction
## end of get_table_adress routine ##

#
# scheduler (subroutine)
#
# kullanılan değişkenler: 356-371
#
30  CPY   100 356  # current_thread_id al
31  SET   0   357  # selected_thread = 0: no selected yet
#
# SYNOPSIS: Round-robin ile READY thread bul, MAX_THREADS kere dön:
#
# for (int count = MAX_THREADS; count > 0; count--) {
#     // Hangi thread'e bakacağız?
#     int offset = MAX_THREADS - count + 1;
#     int tid = current_thread_id + offset;
#     if (tid > MAX_THREADS) {
#         tid = tid - MAX_THREADS;
#     }
#
#     long state_addr = get_thread_table_addr(tid, 1);
#     long state = memory[state_addr];
#
#     if (state == THREAD_STATE_READY) {
#         selected_thread = tid;
#         break;
#     }
# }
#
# round robin döngüsü: MAX_THREADS kere dön
#
32  CPY   102 358  # count = max_threads (10), döngü geriden dönecek (count)

# döngü başı
33  JIF   358 63   # count <= 0 ise döngü sonu

# offset heapla: offset = MAX_THREADS - count + 1
34  CPY   102 359  # MAX_THREADS -> 359
35  CPY   358 360  # count -> 360
36  SUBI  359 360  # MAX_THREADS - count -> 360
37  ADD   360 1    # MAX_THREADS - count + 1 -> 360
38  CPY   360 361  # offset -> 361

# tid = current_thread_id + offset
39  CPY   356 362  # current_thread_id -> 362
40  ADDI  362 361  # current_thread_id + offset -> 362 (tid)

# tid > MAX_THREADS kontrolü
41  CPY   362 363  # tid (copy)
42  CPY   102 364  # MAX_THREADS -> 364
43  SUBI  363 364  # tid - MAX_THREADS -> 364
44  JIF   364 46   # tid <= MAX_THREADS ise atla

# tid > MAX_THREADS, düzeltme gerekli: tid = tid - MAX_THREADS;
45  CPY   364 362  # tid - MAX_THREADS -> 362 (tid)'ye çıkarılmış değer atanır

# Thread state kontrolü
# state adresi hesapla
46  CPY   362 112  # 112: tid
47  SET   1   113  # 113: field
48  CALL  5        # table adresi hesapla
49  CPY   115 365  # state_address
50  CPYI  365 366  # 365->X->state : 366->state, burada 365 ikinci düzey gösterici, 366 da birinci düzey o yüzden CPYI kullanıyourz

# state == THREAD_STATE_READY kontrolü
51  CPY   366 367  # state copy
52  CPY   90  368  # THREAD_STATE_READY değeri 90. adreste tutulur
53  SUBI  367 368  # state - THREAD_STATE_READY
54  JIF   368 57   # sonuç <= 0 ise eşit olma potansiyelleri var ikinci çıkarmaya git
55  ADD   358 -1   # count--
56  JIF   80  33   # sonuç <= 0 değil count'u azalttık döngü başına dön

57  CPY   90  368  # 368 dirty olmuştu tekrar kopyala
58  SUBI  368 367  # THREAD_STATE_READY - state
59  JIF   367 62   # sonuç sıfırdan küçük yani iki sayı eşit
60  ADD   358 -1   # count--
61  JIF   80  33   # sonuç <= 0 değil count'u azalttık döngü başına dön

# selected_thread = tid
62  CPY   362 357  # 362 -> 357, yani selected_thread = tid ataması, döngüden çık, break olmuş oluyor
# burada zaten tekrar 33'e zıplatan bir satır olmadığı için döngü bitmiş oluyor, birnevi break

#
# döngü sonu:
# context;
# - 357: selected_thread,
# - 356: current_thread_id
#
# selected_thread == 0
63  CPY   357 369  # selected_thread -> 369
64  SET   0   370  # sabit sıfır, karşılaştırma için
65  SUBI  369 370  # selected_thread - 0
66  JIF   370 68   # selected_thread - 0, değeri 0'dan küçük ise seçilen thread 0 olabilir, ters çıakrma yap (68'de)
# şart sağlanmıyorsa sonraki if bloğuna gir
# çıkarma sonucu <= 0 değilse ilk if bloğu atlanır sonrakine geçilir
67  JIF   80  71   # şart sağlanmadıysa sonraki if bloğuna koşulsuz git

68  SET   0   370  # sabit sıfırla yine (önceki dirty oldu)
69  SUBI  370 369  # 0 - selected_thread
70  JIF   369 76   # sonuç <= 0 yani seçilen thread == 0 olur, önce idle ata sonra diğer blokları işle

# current_thread_id > 0 kontrolü
71  CPY   100 371  # cuurent_thread_id -> 371
72  JIF   371 74   # <= 0 ise
73  CALL  90       # CALL update_current_thread_in_table

74  CALL  160      # CALL prepare_context_switch
75  RET

# selected_thread = 1;  // idle thread
76  SET   1   357
# current_thread_id > 0 kontrolü
77  CPY   100 371  # cuurent_thread_id -> 371
78  JIF   371 74   # <= 0 ise
79  CALL  90       # CALL update_current_thread_in_table

80  CALL  160      # CALL prepare_context_switch
81  RET
82  SET   0   73   # dummy instruction
83  SET   0   73   # dummy instruction
84  SET   0   73   # dummy instruction
85  SET   0   73   # dummy instruction
86  SET   0   73   # dummy instruction
87  SET   0   73   # dummy instruction
88  SET   0   73   # dummy instruction
89  SET   0   73   # dummy instruction
## end of scheduler routine ##

#
# update_current_thread_in_table subroutine:
# kullanılan değişkenler: 372-388
#
# C impelemebtasyonu:
#
# void update_current_thread_in_table() {
# 	// Update current thread's state based on syscall type
# 	long current_thread_id = memory[20 + CURRENT_RUNNING_THREAD_ID];
# 	long syscall_id = memory[REG_SYSCALL_RESULT];
#
# 	// Get thread table entry addresses
# 	long state_addr = get_thread_table_addr(current_thread_id, 1);
# 	long pc_addr = get_thread_table_addr(current_thread_id, 2);
# 	long sp_addr = get_thread_table_addr(current_thread_id, 3);
# 	long instr_count_addr = get_thread_table_addr(current_thread_id, 6);
# 	long wakeup_addr = get_thread_table_addr(current_thread_id, 7);
#
# 	// Update state based on syscall
# 	if (syscall_id == SYSCALL_PRN_ID) {
# 		memory[state_addr] = THREAD_STATE_BLOCKED;
# 	} else if (syscall_id == SYSCALL_HLT_ID) {
# 		memory[state_addr] = THREAD_STATE_TERMINATED;
# 	} else if (syscall_id == SYSCALL_YIELD_ID) {
# 		memory[state_addr] = THREAD_STATE_READY;
# 	}
#
# 	// Update other fields
# 	memory[pc_addr] = memory[REG_PC];
# 	memory[sp_addr] = memory[REG_SP];
# 	memory[instr_count_addr] = memory[REG_INSTR_COUNT];
# 	memory[wakeup_addr] = memory[REG_WAKEUP_COUNT];
# }
#
90  CPY   100 372  # cuurent_thread_id -> 372
91  CPY   2   373  # syscall_id -> 373
# thread table address calculator çağrıları:
92  CPY   372 112  # tid
93  SET   1   113  # field
94  CALL  5        # table adresi hesapla
95  CPY   115 374  # 374: state_address

96  CPY   372 112  # tid
97  SET   2   113  # field
98  CALL  5        # table adresi hesapla
99  CPY   115 375  # 375: pc_address

100 CPY   372 112  # tid
101 SET   3   113  # field
102 CALL  5        # table adresi hesapla
103 CPY   115 376  # 376: sp_address

104 CPY   372 112  # tid
105 SET   6   113  # field
106 CALL  5        # table adresi hesapla
107 CPY   115 377  # 377: instr_count_address

108 CPY   372 112  # tid
109 SET   7   113  # field
110 CALL  5        # table adresi hesapla
111 CPY   115 378  # 378: wakeup_address

#
# context:
# - 374: state_address
# - 375: pc_address
# - 376: sp_address
# - 377: instr_count_address
# - 378: wakeup_address
#
# syscall id tespiti:
#
112 CPY   373 379  # temp for syscall_id
113 CPY   94  380  # syscall_prn_id = 1
114 CPY   95  381  # syscall_hlt_id = 2
115 CPY   96  382  # syscall_yield_id = 3

# eşitlik kontrolleri:
# 1 - syscall_id == SYSCALL_PRN_ID
116 CPY   379 116  # first_param
117 CPY   380 117  # second_param
118 CALL  210      # is_equal call
119 CPY   118 383  # result of is_equal
120 JIF   383 124  # eşit değiller sonraki eşitlik kontrolüne git
# eşitlerse
121 SET   92  384  # 92, blocked_state değerini tutar (2), dolaylama yapıyruz burada
122 CPYI2 384 374  # M[M[384]] -> M[M[374]], (M[384]:92, M[374]:state_address (M[92]:2, M[state_address]:target))
123 JIF   80  133  # koşulsuz atla diğer atamalara gerek yok

# 2 - syscall_id == SYSCALL_HLT_ID
124 CPY   381 117  # second_param
125 CALL  210      # is_equal call
126 CPY   118 383  # result of is_equal
127 JIF   383 131  # eşit değiller sonraki eşitlik kontrolüne git
# eşitlerse
128 SET   93  384  # 92, terminated_state değerini tutar (3), dolaylama yapıyruz burada
129 CPYI2 384 374  # M[M[384]] -> M[M[374]], (M[384]:93, M[374]:state_address (M[93]:3, M[state_address]:target))
130 JIF   80  133  # koşulsuz atla diğer atamalara gerek yok

# 3 - syscall_id == SYSCALL_YIELD_ID, burada kesinlikle eşit olmalı çünkü başka syscall yok
131 SET   90  384  # 90, ready_state değerini tutar (0), dolaylama yapıyruz burada
132 CPYI2 384 374  # M[M[384]] -> M[M[374]], (M[384]:90, M[374]:state_address (M[90]:0, M[state_address]:target))


# pc_address ataması:
133 SET   0   385  # 0. register adresini dolayla
134 CPYI2 385 375  # M[M[385]] PC'dir, onu 375'e (pc_address'e ata)
# sp_address ataması:
135 SET   1   386  # 1. register adresini dolayla
136 CPYI2 386 376  # M[M[386]] SP'dir, onu 376'a (sp_address'a ata)
# instr_count_address ataması:
137 SET   3   387  # 3. register adresini dolayla
138 CPYI2 387 377  # M[M[387]] IC'dir, onu 376'a (instr_count_address'e ata)
# wakeup_address ataması:
139 SET   4   388  # 4. register adresini dolayla
140 CPYI2 388 378  # M[M[388]] WC'dir, onu 376'a (wakeup_address'e ata)

141 RET            # çağırana geri dön
## end of update_current_thread_in_table routine ##

142 SET   0   73   # dummy instruction
143 SET   0   73   # dummy instruction
144 SET   0   73   # dummy instruction
145 SET   0   73   # dummy instruction
146 SET   0   73   # dummy instruction
147 SET   0   73   # dummy instruction
148 SET   0   73   # dummy instruction
149 SET   0   73   # dummy instruction
150 SET   0   73   # dummy instruction
151 SET   0   73   # dummy instruction
152 SET   0   73   # dummy instruction
153 SET   0   73   # dummy instruction
154 SET   0   73   # dummy instruction
155 SET   0   73   # dummy instruction
156 SET   0   73   # dummy instruction
157 SET   0   73   # dummy instruction
158 SET   0   73   # dummy instruction
159 SET   0   73   # dummy instruction

#
# prepare_context_switch routine:
#
# kullanılan değişkenler: 389-393
#
# context: 357 adresi scheduler'da selected_thread değerini tutar, dolaısıyla bu fonksiyon
# bu parametreyi bu adresten okumalıdır.
#
# C implementasyonu:
#
# void prepare_context_switch(long thread_id) {
# 	// Copy thread info from table to mailbox
# 	long base_addr = get_thread_table_addr(thread_id, 0);
#
# 	// Copy all 8 fields to mailbox
# 	memory[MAILBOX_THREAD_ID] = memory[base_addr + 0];
# 	memory[MAILBOX_STATE] = memory[base_addr + 1];
# 	memory[MAILBOX_PC] = memory[base_addr + 2];
# 	memory[MAILBOX_SP] = memory[base_addr + 3];
# 	memory[MAILBOX_DATA_BASE] = memory[base_addr + 4];
# 	memory[MAILBOX_INSTR_BASE] = memory[base_addr + 5];
# 	memory[MAILBOX_INSTR_COUNT] = memory[base_addr + 6];
# 	memory[MAILBOX_WAKEUP_COUNT] = memory[base_addr + 7];
#
# 	// Update thread state to RUNNING in table
# 	memory[base_addr + 1] = THREAD_STATE_RUNNING;
#
# 	// Set context switch signal
# 	memory[REG_CONTEXT_SWITCH_SIGNAL] = CONTEXT_SWITCH_SIGNAL;
#
# 	// Update current running thread ID
# 	memory[20 + CURRENT_RUNNING_THREAD_ID] = thread_id;
# }
#
160 CPY   357 389  # copying thread_id (selected by scheduler)
161 CPY   389 112  # tid
162 SET   0   113  # field (we want to getting base_address)
163 CALL  5        # table adresi hesapla
164 CPY   115 390  # hesaplanan base_address -> 390
#
# mailbox'a thread table entry değerlerini geçiriyoruz
# copy the mailbox addresses: 600-607
# 600 - id
# 601 - state
# 602 - pc
# 603 - sp
# 604 - data base addr
# 605 - instruction base addr
# 606 - register instruction count
# 607 - wakeuo instruction count
#
165 CPYI  390 600  # 390->base_addr->id_value => 600->id_value
166 ADD   390 1    # sonraki sütuna geç
167 CPYI  390 601  # 390->base_addr + 1->state_value => 601->state_value
168 ADD   390 1    # sonraki sütuna geç
169 CPYI  390 602  # 390->base_addr + 2->pc_value => 602->pc_value
170 ADD   390 1    # sonraki sütuna geç
171 CPYI  390 603  # 390->base_addr + 3->sp_value => 603->sp_value
172 ADD   390 1    # sonraki sütuna geç
173 CPYI  390 604  # 390->base_addr + 4->db_value => 604->db_value
174 ADD   390 1    # sonraki sütuna geç
175 CPYI  390 605  # 390->base_addr + 5->ib_value => 605->ib_value
176 ADD   390 1    # sonraki sütuna geç
177 CPYI  390 606  # 390->base_addr + 6->ic_value => 606->ic_value
178 ADD   390 1    # sonraki sütuna geç
179 CPYI  390 607  # 390->base_addr + 7->wc_value => 607->wc_value
# Update thread state to RUNNING in table
# memory[base_addr + 1] = THREAD_STATE_RUNNING;
180 SET   91  391  # (91) address of THREAD_STATE_RUNNING -> 391
181 CPY   115 392  # base_address kopyala (dirty olduğu için)
182 ADD   392 1    # base_addr + 1
183 CPYI  391 392  # 391->91->running_state_value : 1, 392->base_address+1->state, sonuç olarak state = 1
# ctx_signal
184 SET   -999 17  # 17. registera -999 sinyalini koy
185 CPY   389  100 # current_running_thread ayarla (389 bu değeri tutuyordu)
# return to calle
186 RET
## end of prepare_context_switch routine ##

187 SET   0   73   # dummy instruction
188 SET   0   73   # dummy instruction
189 SET   0   73   # dummy instruction
190 SET   0   73   # dummy instruction
191 SET   0   73   # dummy instruction
192 SET   0   73   # dummy instruction
193 SET   0   73   # dummy instruction
194 SET   0   73   # dummy instruction
195 SET   0   73   # dummy instruction
196 SET   0   73   # dummy instruction
197 SET   0   73   # dummy instruction
198 SET   0   73   # dummy instruction
199 SET   0   73   # dummy instruction

#
# syscall_handler subroutine:
#
# kullanılan değişkenler: 394-
#
# C impelementation:
#
# void syscall_handler() {
# 	// Read syscall ID from register 2
# 	long syscall_id = memory[REG_SYSCALL_RESULT];
#
# 	switch (syscall_id) {
# 		case SYSCALL_PRN_ID:
# 			// Set wakeup count for blocking
# 			memory[REG_WAKEUP_COUNT] = memory[20 + PRINT_BLOCK_DURATION];
# 			scheduler();
# 			break;
#
# 		case SYSCALL_HLT_ID:
# 			// Thread will be terminated
# 			scheduler();
# 			break;
#
# 		case SYSCALL_YIELD_ID:
# 			// Thread yields CPU
# 			scheduler();
# 			break;
# 	}
# }
#
200 CPY   2   394  # getting syscall_id from register 2
# using is_equal:
201 CPY   394 116  # first param: syscall_id
202 CPY   94  117  # second param: syscall_prn_id
203 CALL  210      # is_equal
204 CPY   118 395  # result -> 395
205 JIF   395 208    # <= 0 ise sonraki eşitliğe bak
# eşitse
206 CPY   101 4    # 101:REG_WAKEUP_COUNT, memory[101] = 100
207 CALL  30       # scheduler çağır
# eşit değilse zaten ya hlt ya da yield'dır. Onlar için özel bir işlem yapılmaz sadece scheduler çağrısı:
208 CALL  30       # scheduler çağrısı
209 RET            # return to calle
## end of syscall_handler routine ##

#
# equality checker subroutine:
# - 116: first param
# - 117: second param
# - 118: result (0-1)
#
210 CPY   116 900  # first_param  -> 900
211 CPY   117 901  # second_param -> 901
212 CPY   901 902  # second_param (temp)
213 SUBI  900 902  # first_param - second_param -> 902
214 JIF   902 217  # <= 0 ise ikinci çıkarmaya git
# değilse
215 SET   0   118  # sonuç 0, eşit değiller
216 RET

# ikinci çıkarma
217 SUBI  901 900  # second_param - first_param -> 900
218 JIF   900 221  # <= 0 ise sonucu 1 olarak yazmaya git
# değilse
219 SET   0   118  # sonuç 0, eşit değiller
220 RET

# iki sayı da eşitse:
221 SET   1   118  # sonuç 1, eşitler
222 RET
## end of is_equal routine ##

End Instruction Section

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
