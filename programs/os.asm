######################## OS THREAD ########################
# subroutines:
# 0  -> entry point of OS (booting)
# 5  -> thread table address calculator
# 30 -> scheduler
#
#
#

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
300 0     # MAILBOX_THREAD_ID
301 0     # MAILBOX_STATE
302 0     # MAILBOX_PC: booting kısmı buradan seçilen threadin PC değerini çeker ve USER ile çağırır
303 0     # MAILBOX_SP
304 0     # MAILBOX_DATA_BASE
305 0     # MAILBOX_INSTR_BASE
306 0     # MAILBOX_INSTR_COUNT
307 0     # MAILBOX_WAKEUP_COUNT

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
2   USER  302      # scheduler seçilen threaidn PC değerini buraya koyar
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
#
# scheduler (subroutine)
#
# kullanılan değişkenler: 356-
#
# for (int tid = 1; tid <= MAX_THREADS; tid++) {
#     long state_addr = get_thread_table_addr(tid, 1);
#     long state = memory[state_addr];
#
#     if (state == THREAD_STATE_BLOCKED) {
#         long wakeup_addr = get_thread_table_addr(tid, 7);
#         long wakeup = memory[wakeup_addr];
#         wakeup--;
#         memory[wakeup_addr] = wakeup;
#
#         if (wakeup <= 0) {
#             memory[state_addr] = THREAD_STATE_READY;
#         }
#     }
# }
#
30  CPY   100 356  # current_thread_id al
31  SET   0   357  # selected_thread = 0: no selected yet

# İlk döngü: Wakeup kontrolü (tid = 10'dan 1'e)
32  CPY   102 358  # i = max_threads (10), tid
33  CPY   358 359  # i, temp
34  JIF   359 67   # max_thread <= 0 ise atla, ikinci döngye gidilecek
35  CPY   358 112  # 112: tid
36  SET   1   113  # 113: field = 1, state ikinci sütunda
37  CALL  5        # table adresi hesapla
38  CPY   115 360  # M[115] = M[state_address], M[state_address] => 360
39  CPY   360 361  # subi için dirty olmasın
40  CPY   92  362  # thread_state_blocked -> 362 (2)
41  SUBI  361 362  # state - thread_state_blocked
42  JIF   362 45   # sonuç sıfır veya küçükse eşit olma potansiyeli var, ikinci çıkarmaya git
# değilse sayaç azalt ve dmngü başına dön
43  ADD   358 -1   # i--
44  JIF   80  33   # koşulsuz döngü başına dön

45  CPY   92  362  # 362 dirty olmuştu, yeniden kopyala thread_state_blocked'ı
46  SUBI  362 361  # thread_state_blocked - state
47  JIF   361 50   # bu sonuç da sıfırsa sayılar eşittir yani (state == THREAD_STATE_BLOCKED), wakeup güncellemeye git
# değilse sayaç azalt ve döngü başına dön
48  ADD   358 -1   # i--
49  JIF   80  33   # koşulsuz döngü başına dön

50  CPY   358 112  # 112: tid
51  SET   7   113  # 113: field = 7, wakeup count son sütunda (8.)
52  CALL  5        # table adresi hesapla
53  CPYI  115 363  # M[115] = M[wakeup_cnt_address], M[wakeup_cnt_address] => 363
54  ADD   363 -1   # wakeup değerini bir azalt
55  SET   363 364  # dolaylı adresleme için azaltılmış wakeup'ı tutan adresi 364 adresine yaz
56  CPYI2 364 115  # (364->363->azaltılmış wakeup cnt) => (115->wakeup_column_address->value), value = wakeup cnt
57  JIF   363 60   # wakeup değeri 0 veya daha azsa artık block durumundan ready durumuna geçmeli
# değilse sayaç azalt ve döngü başına dön
58  ADD   358 -1   # i--
59  JIF   80  33   # unconditional jump to loop

60  CPY   358 112  # 112: tid
61  SET   1   113  # 113: field = 1, state ikinci sütunda
62  CALL  5        # table adresi hesapla
63  SET   0   366  # READY: 0
64  SET   366 367  # dolaylı address
65  CPYI2 367 115  # memory[start_addr] = READY
66  JIF   80  33   # unconditional jump to loop

#
# ADIM 2: Round-robin ile READY thread bul
# MAX_THREADS kere dön
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
67  # TODO: İkinci döngü


End Instruction Section
