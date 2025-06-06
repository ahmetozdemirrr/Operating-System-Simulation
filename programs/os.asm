# programs/os.asm #

######################## OS THREAD ########################
#
# subroutines:
# 0   -> entry point of OS (booting)
# 5   -> thread table address calculator
# 30  -> scheduler
# 90  -> update_current_thread_in_table
# 160 -> prepare_context_switch
# 200 -> syscall_handler
# 210 -> equality checker
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
102 10   # max_thread_count

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
17  CPY   353 115  # sonucun alınacağı yere son değeri yaz
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
# kullanılan değişkenler: 356-363
#
# SYNOPSIS: Round-robin ile READY thread bul, MAX_THREADS kere dön:
#
# long current_thread_id = memory[CURRENT_RUNNING_THREAD_ID];  // M[100]
# long selected_thread = 0;
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
30  CPY   100 356  # current_thread_id
31  SET   0   357  # selected_thread = 0

32  CPY   102 358  # MAX_THREADS -> 358 (count)

# döngü başı
33  JIF   358 56   # count <= 0 ise döngüden çık
# offset = MAX_THREADS - count + 1
34  CPY   102 359  # MAX_THREADS -> 359
35  CPY   358 360  # count (temp)
36  SUBI  359 360  # MAX_THREADS - count -> 360
37  ADD   360 1    # offset
# tid = current_thread_id + offset
38  ADDI  360 356  # current_thread_id + offset -> 360
# tid'yi 360 tutuyor
# (tid > MAX_THREADS) kontrolü
39  CPY   102 364  # MAX_THREADS -> 364
40  SUBI  360 364  # tid - MAX_THREADS -> 364
41  JIF   364 45   # <= 0 ise state_addr = get_thread_table_addr(tid, 1), satırından devam et
# > 0 ise
42  CPY   102 361  # MAX_THREADS -> 361
43  SUBI  360 361  # tid - MAX_THREADS -> 361
44  CPY   361 360  # tid - MAX_THREADS -> 360 (tid)

45  CPY   360 112  # tid: 356
46  SET   1   113  # field: 1, for state
47  CALL  5        # table adresini hesapla
48  CPY   115 362  # state_address -> 362
49  CPYI  362 363  # M[state_address] -> 363 (state)

# (state == THREAD_STATE_READY) kontrolü, is_equal çağrısı
50  CPY   90  116  # M[90] = THREAD_STATE_READY, first param
51  CPY   363 117  # state -> 117, second param
52  CALL  210      # is_equal
53  ADD   358 -1   # count--
54  JIF   118 33   # eşit değiller, döngü başına (yani state READY değil)
55  CPY   360 357  # selected_thread = tid, buradan döngü başına dönmeyeceğiz, dolayısıyla break olacak

# (selected_thread == 0) kontrolü is_equal ile
56  CPY   357 116  # selected_thread -> 116 : first param
57  SET   0   117  # eşitlik kontrolü yapılacak sayı (0) : second param
58  CALL  210      # is_equal
59  JIF   118 61   # <= 0 ise eşit değiller, (current_thread_id > 0) kontrolüne git
# eşitler selected_thread = 1 atamasını yap
60  SET   1   357  # eşitlerse (selected bir thread yok demek idle threadi seç): selected_thread = 1
# (current_thread_id > 0) kontrolü
61  JIF   356 63   # <= 0 ise, update yapmaya gerek yok sadece prepare çağır
# > 0 ise
62  CALL  90       # update_current_thread_in_table subroutine çağrısı
63  CALL  160      # prepare_context_switch subroutine çağrısı
64  RET            # return to callee

65  SET   0   73   # dummy instruction
66  SET   0   73   # dummy instruction
67  SET   0   73   # dummy instruction
68  SET   0   73   # dummy instruction
69  SET   0   73   # dummy instruction
70  SET   0   73   # dummy instruction
71  SET   0   73   # dummy instruction
72  SET   0   73   # dummy instruction
73  SET   0   73   # dummy instruction
74  SET   0   73   # dummy instruction
75  SET   0   73   # dummy instruction
76  SET   0   73   # dummy instruction
77  SET   0   73   # dummy instruction
78  SET   0   73   # dummy instruction
79  SET   0   73   # dummy instruction
80  SET   0   73   # dummy instruction
81  SET   0   73   # dummy instruction
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
# void update_current_thread_in_table()
# {
# 	long current_thread_id = memory[CURRENT_RUNNING_THREAD_ID];  // M[100]
# 	long syscall_id = memory[REG_SYSCALL_RESULT];  // Register 2
#
# 	// Thread table adreslerini hesapla
# 	long state_addr = get_thread_table_addr(current_thread_id, 1);
# 	long pc_addr = get_thread_table_addr(current_thread_id, 2);
# 	long sp_addr = get_thread_table_addr(current_thread_id, 3);
# 	long instr_count_addr = get_thread_table_addr(current_thread_id, 6);
# 	long wakeup_addr = get_thread_table_addr(current_thread_id, 7);
#
# 	// Thread state'ini syscall türüne göre güncelle
# 	if (syscall_id == SYSCALL_PRN_ID) {
# 		memory[state_addr] = THREAD_STATE_BLOCKED;
# 		// ÖNEMLİ: PRN syscall için thread'i PRINT_BLOCK_DURATION kadar blokla
# 		memory[wakeup_addr] = memory[PRINT_BLOCK_DURATION];  // M[101] = 100
# 	} else if (syscall_id == SYSCALL_HLT_ID) {
# 		memory[state_addr] = THREAD_STATE_TERMINATED;
# 	} else if (syscall_id == SYSCALL_YIELD_ID) {
# 		memory[state_addr] = THREAD_STATE_READY;
# 	}
#
# 	// Thread context'ini syscall sırasında register'lara kaydedilen değerlerden oku
# 	memory[pc_addr] = memory[5];           // Thread PC (Register 5)
# 	memory[sp_addr] = memory[6];           // Thread SP (Register 6)
# 	memory[instr_count_addr] = memory[7];  // Thread IC (Register 7)
#
# 	// Wakeup count Register 4'ten oku (PRN dışında)
# 	if (syscall_id != SYSCALL_PRN_ID) {
# 		memory[wakeup_addr] = memory[4];   // Register 4
# 	}
# 	// PRN için wakeup count yukarıda zaten ayarlandı
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
#
# - 372: cuurent_thread_id
# - 373: syscall_id
# - 374: state_address
# - 375: pc_address
# - 376: sp_address
# - 377: instr_count_address
# - 378: wakeup_address
#

# 1 - syscall_id == syscall prn
112 CPY   373 379  # temp syscall_id
113 SET   1   116  # first parameter: 1 (syscall prn)
114 CPY   379 117  # second parameter: syscall_id
115 CALL  210      # is_equal(syscall prn, syscall_id)
116 JIF   118 124  # if result <= 0, sonraki checke git
117 SET   2   380  # M[380] = 2 (blocked = 2)
118 SET   380 381  # 380'i dolayla
119 CPYI2 381 374  # state_addressin tutulduğu yere 2 yaz (blocked)
120 SET   100 382  # M[382] = 100
121 SET   382 383  # 382'yi dolayla
122 CPYI2 383 378  # wakeup_address'e 100 yaz
123 JIF   80  139  # eğer buna eşitse, diğer eşitlikleri kontrol etme

# 2 - syscall_id == syscall hlt
124 SET   2   116  # first parameter: 2 (syscall hlt)
125 CPY   379 117  # second parameter: syscall_id
126 CALL  210      # is_equal(syscall hlt, syscall_id)
127 JIF   118 132  # if result <= 0, sonraki checke git
128 SET   3   384  # M[384] = 3 (terminated = 3)
129 SET   384 385  # 384'ü dolayla
130 CPYI2 385 374  # state_addressin tutulduğu yere 3 yaz (terminated)
131 JIF   80  139  # eğer buna eşitse, diğer eşitlikleri kontrol etme

# 3 - syscall_id == syscall yield
132 SET   3   116  # first parameter: 3 (syscall yield)
133 CPY   379 117  # seconde parameter: syscall_id
134 CALL  210      # is_equal(syscall yield, syscall_id)
135 JIF   118 153  # if result <= 0, eşit değilse çok kritik bir hata var çünkü syscall sadece bu üçü olabilir (HLT yap)
136 SET   0   386  # M[386] = 0 (ready = 0)
137 SET   386 387  # 386'yı dolayla
138 CPYI2 387 374  # state_addressin tutulduğu yere 0 yaz (ready)

# memory[pc_addr] = memory[5]
139 SET   5   388  # register 5'yi dolayla
140 CPYI2 388 375  # 375 pc_addr tutar, pc_addr da pc değerini: ikisi de 2 kere dolaylanmıştır

# memory[sp_addr] = memory[6]
141 SET   6   389  # register 6'yı dolayla
142 CPYI2 389 376  # 376 sp_addr tutar, sp_addr da sp değerini: ikisi de 2 kere dolaylanmıştır

# memory[instr_count_addr] = memory[7]
143 SET   7   390  # register 7'yi dolayla
144 CPYI2 390 377  # 377 instr_count_address tutar, instr_count_address da ic değerini: ikisi de 2 kere dolaylanmıştır

145 SET   1   116  # first parameter: syscall prn
146 CPY   379 117  # second parameter: syscall_id
147 CALL  210      # is_equal(syscall prn, syscall_id)
148 JIF   118 150  # eşit değiller memory[wakeup_addr] = memory[4] işlemini yapmaya git
149 RET            # eşitler return ile dön

# memory[wakeup_addr] = memory[4]
150 SET   4   391  # register 4'ü dolayla
151 CPYI2 391 378  # 378 wakeup_address tutar, wakeup_address de wc değerini: ikisi de 2 kere dolaylanmıştır

152 RET            # return to callee
153 HLT
## end of update_current_thread_in_table routine ##

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
#
# Update thread state to RUNNING in table
# memory[base_addr + 1] = THREAD_STATE_RUNNING;
#
180 SET   1   391  # (91) address of THREAD_STATE_RUNNING -> 391
181 SET   391 393  # 391'i dolayla
182 CPY   115 392  # base_address kopyala (dirty olduğu için)
183 ADD   392 1    # base_addr + 1
184 CPYI2 393 392  # 391->91->running_state_value : 1, 392->base_address+1->state, sonuç olarak state = 1
185 SET   0   112  # thread_id
186 SET   1   113  # field: 0 (state address)
187 CALL  5        # base_address hesapla OS state için onu şimdi ready yapacağız...
188 CPY   115 908  # kopyala
189 SET   0   909  # READY state
190 SET   909 910  # dolayla
191 CPYI2 910 908  # OS state2i ready yap
# ctx_signal
192 CPY   389  100 # current_running_thread ayarla (389 bu değeri tutuyordu)
193 SET   -999 17  # 17. registera -999 sinyalini koy
# return to callee
194 RET
## end of prepare_context_switch routine ##

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
# 	scheduler();
#	exit()
# }
#
200 SET   0   112  # thread_id
201 SET   1   113  # field: 0 (state address)
202 CALL  5        # base_address hesapla
203 CPY   115 907  # kopyala
204 SET   1   905  # RUNNING STATE
205 SET   905 906  # dolayla
206 CPYI2 906 907  # OS state'ini 1 yap
207 CALL  30       # scheduler çağrısı
208 HLT
## end of syscall_handler routine ##

209 SET   0   73   # dummy instruction

#
# equality checker subroutine:
#
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
###############################################################






################# USER THREAD 1 (idle thread) #################
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
1 JIF 80 0      # unconditional jump
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
32  SYSCALL HLT
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
20  SYSCALL HLT     # stop execution

End Instruction Section
###############################################################





######################## USER THREAD 4 ########################
# Multiplication Implementation
Begin Data Section
0   5       # mult operand 1
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
10  SYSCALL HLT

End Instruction Section
###############################################################
