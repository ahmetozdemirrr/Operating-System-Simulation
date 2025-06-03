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
100 0   # current_running_thread_id (başlangıçta Thread 1)
101 1   # next_thread_id (başlangıçta Thread 1)
102 0   # print_block_counter (PRN syscall için)
103 100 # PRINT_BLOCK_DURATION (PRN syscall 100 instruction block eder)

# Round-robin scheduler variables
121 10  # max_thread_count

# Thread Table (500'den başlıyor)
# Her entry için 8 bilgi: thread_id, thread_state, PC, SP, data_base_addr,
# instr_base_addr, register_instruction_count, wake_up_instruction_count

# OS Thread Table Entry (500-507)
500 0    # thread_id = 0 (OS)
501 1    # thread_state = 1 (RUNNING)
502 1000 # PC = 1000 (OS instruction start)
503 1999 # SP = 1999 (OS stack top)
504 20   # data_base_addr = 20 (OS data start)
505 1000 # instr_base_addr = 1000 (OS instruction start)
506 0    # register_instruction_count = 0
507 0    # wake_up_instruction_count = 0

# OS Thread Table Entry (508-515) (idle thread)
508 1    # thread_id = 1
509 0    # thread_state = 0 (READY)
510 3000 # PC = 3000 (Thread 1 instruction start)
511 3999 # SP = 3999 (Thread 1 stack top)
512 2000 # data_base_addr = 2000
513 3000 # instr_base_addr = 3000
514 0    # register_instruction_count = 0
515 0    # wake_up_instruction_count = 0

# User Thread 2 Table Entry (516-523)
516 2    # thread_id = 2
517 0    # thread_state = 0 (READY)
518 5000 # PC = 5000 (Thread 2 instruction start)
519 5999 # SP = 5999 (Thread 2 stack top)
520 4000 # data_base_addr = 4000
521 5000 # instr_base_addr = 5000
522 0    # register_instruction_count = 0
523 0    # wake_up_instruction_count = 0

# User Thread 3 Table Entry (524-531)
524 3    # thread_id = 3
525 0    # thread_state = 0 (READY)
526 7000 # PC = 7000 (Thread 3 instruction start)
527 7999 # SP = 7999 (Thread 3 stack top)
528 6000 # data_base_addr = 6000
529 7000 # instr_base_addr = 7000
530 0    # register_instruction_count = 0
531 0    # wake_up_instruction_count = 0

# User Thread 4 Table Entry (532-539)
532 4    # thread_id = 4
533 0    # thread_state = 0 (READY)
534 9000 # PC = 9000 (Thread 4 instruction start)
535 9999 # SP = 9999 (Thread 4 stack top)
536 8000 # data_base_addr = 8000
537 9000 # instr_base_addr = 9000
538 0    # register_instruction_count = 0
539 0    # wake_up_instruction_count = 0

# User Thread 5 Table Entry (540-547)
540 5     # thread_id = 5
541 0     # thread_state = 0 (READY)
542 11000 # PC = 11000 (Thread 5 instruction start)
543 11999 # SP = 11999 (Thread 5 stack top)
544 10000 # data_base_addr = 10000
545 11000 # instr_base_addr = 11000
546 0     # register_instruction_count = 0
547 0     # wake_up_instruction_count = 0

# User Thread 6 Table Entry (548-555)
548 6     # thread_id = 6
549 0     # thread_state = 0 (READY)
550 13000 # PC = 13000 (Thread 6 instruction start)
551 13999 # SP = 13999 (Thread 6 stack top)
552 12000 # data_base_addr = 12000
553 13000 # instr_base_addr = 13000
554 0     # register_instruction_count = 0
555 0     # wake_up_instruction_count = 0

# User Thread 7 Table Entry (556-563)
556 7     # thread_id = 7
557 0     # thread_state = 0 (READY)
558 15000 # PC = 15000 (Thread 7 instruction start)
559 15999 # SP = 15999 (Thread 7 stack top)
560 14000 # data_base_addr = 14000
561 15000 # instr_base_addr = 15000
562 0     # register_instruction_count = 0
563 0     # wake_up_instruction_count = 0

# User Thread 8 Table Entry (564-571)
564 8     # thread_id = 8
565 0     # thread_state = 0 (READY)
566 17000 # PC = 17000 (Thread 8 instruction start)
567 17999 # SP = 17999 (Thread 8 stack top)
568 16000 # data_base_addr = 16000
569 17000 # instr_base_addr = 17000
570 0     # register_instruction_count = 0
571 0     # wake_up_instruction_count = 0

# User Thread 9 Table Entry (572-579)
572 9     # thread_id = 9
573 0     # thread_state = 0 (READY)
574 19000 # PC = 19000 (Thread 9 instruction start)
575 19999 # SP = 19999 (Thread 9 stack top)
576 18000 # data_base_addr = 18000
577 19000 # instr_base_addr = 19000
578 0     # register_instruction_count = 0
579 0     # wake_up_instruction_count = 0

# User Thread 10 Table Entry (580-587)
580 10    # thread_id = 10
581 0     # thread_state = 0 (READY)
582 21000 # PC = 21000 (Thread 10 instruction start)
583 21999 # SP = 21999 (Thread 10 stack top)
584 20000 # data_base_addr = 20000
585 21000 # instr_base_addr = 21000
586 0     # register_instruction_count = 0
587 0     # wake_up_instruction_count = 0


# Context Switch Target: The scheduler writes the entries of the thread it decides will work here.
# then a signal is written to a special register. The CPU simulator constantly checks this signal, if the
# signal indicates that the scheduler has finished its operation, the CPU should update the cpu struct by
# reading 600 - 607 (in KERNEL mode, offset: 0)
End Data Section

Begin Instruction Section
#subroutine# booting OS: entry point
0   SET   0 100      # OS başarıyla başlatıldı
1   CALL  30         # Scheduler’ı çağır
2   USER  112        # Scheduler’ın belirlediği thread PC’sine atla
#subroutine# booting OS end

#subroutine# scheduler start
30  CPY   100 800  # şuan çalışan threadin id'sini al: M[100], her zaman bu bilgiyi tutar
31  ADDI  800 97   # next thread's id, 97 sabit '1' sayısını tutar
32  CPY   800 801  # temp next_thread_id
33  SUBI  121 801  # max_thread - next_id -> 801
34  JIF   801 45   # eğer max thread aşılıyorsa, başa dön
35  SET   501 802  # M[802] table sorguları için offset 500 (+1 table'ın ikinci sütunu anlamına gelir) değerini tutar

# çarpma işlemi: 500 + id * 8 + 1: next_thread'in state'ine id'si üzerinden ulaşmak için bu döngüyü kullanacağız
36  CPY   98  803  # operand_1 -> 803, 98 her zaman sabit '8' sayısını tutar
37  SET   0   804  # i -> 804
38  CPY   804 805  # i -> 805 (temp), subi'de dirty olacağı için kopyalıyoruz
39  CPY   801 806  # operand_1 -> 806 (temp), burada id kadar toplama işlemi yapacağız
40  SUBI  806 805  # N - i, o yüzden id - i <= 0 kontrolü yapıyoruz
41  JIF   805 46   # 46 devam edilecek yer (scheduler contextte)
42  ADDI  802 803  # 501 + 8 her seferinde (M[802]: 501 offsetini tutar, M[803]: 8 sabitini tutar)
43  ADDI  804 97   # i++
44  JIF   99  36   # çarpma döngüsün başına dön koşulsz
45  SET   1   800  # burayı 1 yap schedulerburadan aramaya devam etmeli, çünkü table'da sona gelindi başa dön

# context:
# - M[800]: Geçici thread ID, önce current_running_thread_id, sonra next_thread_id, sıfırlamada 1
# - M[802]: Hedef thread’in state adresi, tarama başlangıç noktası

46  SET   581 807  # N -> 807: döngü için son index
47  CPY   802 808  # i -> 808: değer yukarıda bulunan, start thread_state adresi olacak
48  SET   0   809  # found_ready
49  CPY   807 810  # temp N
50  CPY   808 811  # temp i
51  SUBI  810 811  # N - i
52  JIF   811 70   # idle threadine git
53  CPY   802 812  # wakeup instruction count -> 812
54  SET   6   813  # wakeup sütununa atlamak için sabit 6
55  ADDI  812 813  # sütun ayarlaması

# wakeup mechanism:
56  CPYI  812 814  # M[814] = wakeup_instruction_count (M[812] adresinden oku)
57  SET   1   815  # çıkarma işlemi için, dirty temp değişken
58  SUBI  814 815  # M[814] -= 1 -> elde edilen değer 815'e yazılsın (wakeup değerini azaltıyoruz)
59  SET   815 819  # 815 -> 819
60  CPYI2 819 812  # dolaylı atamayı bu şekilde yapmış olduk
61  JIF   815 64   # eğer wakeup değeri <= 0 ise Y'ye atla, Y'de bu threadin seçimi gerçekleştirilecek
# Wakeup > 0, bir sonraki thread’e geç, şuan bu threadin seçilmeyeceğini anladık
62  ADDI  808 98   # M[808] += 8 (bir sonraki thread’in state adresine geç, M[98] sabit 8)
63  JIF   99  50   # Döngü başına dön (50’den devam et)
64  SET   0   817  # 817'ye 0 yaz
65  SET   817 818  # 817 adresini 818'e yaz
66  CPYI2 818 808  # M[M[818]] : 0, M[M[808]]: M[808] = M[5XX]... (state 0'lanır) çünkü wakeuo değeri <= 0

67  SET   1   809  # found_ready = 1 (M[809])
68  JIF   99  90   # Context switch hazırlığına git

# !!!!! Context Switchden önce mevcut threadin state'i de güncellenmeli:
# eğer syscall hlt ile bitmişse terminated olarak işaretlenmel
# eğer yield ile bitmişse ready olarak ayarlanır
# veya prn ile bitmişse blocked olarak ayarlanır
# yeni çalışana da running değerini atıyoruz (bunu yaptım sanırım)
# bu ayarlamalar syscall numarası kontrol edilerek yapılmalı

# context switch mechanism (instruction 90'da başlar): X idle thread için context switcj yapılacak komut
# başlangıcı olacak. OS'un simülasyonda çalışan cpu ile haberleşebilmesi için bir mailbox kullanacağı (600-607)
#
# - 600: thread_id
# - 601: thread_state
# - 602: thread_pc
# - 603: thread_sp
# - 604: thread_data_base
# - 605: thread_instruction_base
# - 606: thread_instruction_count
# - 607: thread_wakeup_instruction_count
#
# - Register 17: signal receiver
#
# ardından context swithc bildirimi için yine mailbox olarak 17. registerı context switch olduğuna
# dair bilgi veren bir sinyal alıcısı olarak kullanacağız. Buradan okunan değer -999 ise context switch
# yapılmıştır ve 600-607 mailbox'ından yeni core image bilgileri alınmalıdır. 999 olduğu zman da aynı
# contextte devam edilir.
#

# idle thread için switch yapılacak yer
70  SET   1    509 # thread_state'i running yap
# switching the core image
71  CPY   508  600  # copying thread_id
72  CPY   509  601  # copying thread_state
73  CPY   510  602  # copying thread_pc
74  CPY   511  603  # copying thread_sp
75  CPY   512  604  # copying thread_data_base
76  CPY   513  605  # copying thread_instruction_base
77  CPY   514  606  # copying thread_instruction_count
78  CPY   515  607  # copying thread_wakeup_instruction_count
79  SET   1    100  # current_thread_id = 1
80  CPY   602  112  # setting block 112: new threads PC OS will jump there using (USER 112)
81  SET   -999 17   # REG_CONTEXT_SWITCH_SIGNAL registerıyla sinyal ver
82  JIF   99   2    # USER komutuna koşulsuz atla

# normal user threadler için context switch bloğu
90  CPY   100  820  # 820 adresine current çalışan threadin id'sini alalım
# current thread için table'da güncellenecek değerler: state, pc, sp, reg_inst_cnt, wakeup_inst_cnt
91  SET   500  821  # 821'e thread table'ın offsetini koy
92  CPY   98   822  # 822'ye TABLE_ENTRY_SIZE kopyala
# current thread'in entry'sine gelmek için çarpma döngüsü: id * TABLE_ENTRY_SIZE + 500
93  SET   0    823  # i = 0
94  CPY   823  824  # temp i
95  SUBI  822  824  # TABLE_ENTRY_SIZE - i -> 824
96  JIF   824  100  # TABLE_ENTRY_SIZE - i <= 0 ise döngü bitmiştir çık
97  ADDI  821  820  # 500'e (offset) 8 (TABLE_ENTRY_SIZE) ekle
98  ADD   823  1    # i++ sonraki iterasyon
99  JIF   99   93   # döngü başına koşulsuz dön
#
# context: 821, ilgili entrynin id sütun adresini tutar (tablonun satır başı)
# şimdi ilgili bilgileri çekip thread table'a kaydedeceğiz
# aşağıdaki sayıları satır offsetine ekleyerek değerlere ulaşacağız:
# - +1: thread_state
# - +2: thread_pc
# - +3: thread_sp
# - +6: thread_instruction_count
# - +7: wakeup_instruction_count
#
100 SET   1    824
101 SET   2    825
102 SET   3    826
103 SET   6    827
104 SET   7    828
# 821 zaten table'ın satır başını tutuyordu, 824 işin sonunda state'i
# değiştirebilecek adrese sahip olacak. Diğerleri de aynı şekilde devam edecek
105 ADDI  824  821
106 ADDI  825  821
107 ADDI  826  821
108 ADDI  827  821
109 ADDI  828  821
# şimdi yazacağımız adresleri elde ettik, copy ile register değerlerini bu adreslere yazacağız
# indirect writing yok elimizde o yüzden register adreslerini bir temp tutucuya kopyalayıp onun
# üzerinden CPYI2 kullanacağız

# şimdi state güncellemesi için önce 2. registerı okuyacağız
110 CPY   2    829 # 829 şuan 2'deki syscall id'yi tutuyor
111 CPY   94   830 # 94 prn_id tutar, yani 1
112 CPY   95   831 # 95 hlt_id tutar, yani 2
113 CPY   96   832 # 96 yield_id tutar, yani 3
114 SUBI  829  830 # syscall_id - prn_id -> 830
115 JIF   830  120
116 SUBI  829  831 # syscall_id - hlt_id -> 831
117 JIF   831  124
118 SUBI  829  832 # syscall_id - yield_id -> 832
119 JIF   832  128

120 CPY   829  833 # temp for syscall id
121 CPY   94   830 # dirty olan id dğerini yeniden al
122 SUBI  830  833 # ters çıkarma
123 JIF   833  133 # ikisi de şartı sağlarsa syscall_id == prn_id'dir

124 CPY   829  834 # temp for syscall id
125 CPY   95   831 # dirty olan id değerini yeniden al
126 SUBI  831  834 # ters çıkarma
127 JIF   834  136 # ikisi de şartı sağlarsa syscall_id == hlt_id'dir

128 CPY   829  835 # temp for syscall id
129 CPY   96   832 # dirty olan id değerini yeniden al
130 SUBI  832  835 # ters çıkarma
131 JIF   835  140 # ikisi de şartı sağlarsa syscall_id == yield_id'dir

# 824'ü 2 yap (syscall_prn_id), 2: blocked:
132 SET   2    836
133 SET   836  837
134 CPYI2 837  824
135 JIF   99   143

# 824'ü 3 yap (syscall_hlt_id), 3: terminated:
136 SET   3    838
137 SET   838  839
138 CPYI2 839  824
139 JIF   99   143

# 824'ü 0 yap (syscall_yield_id), 0: ready:
140 SET   0    840
141 SET   840  841
142 CPYI2 841  824

# şimdi pc güncelleme, 825 table'ın o sütununa işaret eder
143 SET   0    842 # 0 (pc) tutan register adresidir
144 CPYI2 842  825

145 SET   1    843 # 1 (sp) tutan register adresidir
146 CPYI2 843  826

147 SET   3    844 # 3 (register instruction counter) register adresidir
148 CPYI2 844  827

149 SET   4    845 # 4 (wakeup instruction count) register adresidir
150 CPYI2 845  828

# thread table güncellendi
# şimdi context switch yapacağız, 808 adresi scheduler'ın seçtiği threadin state adresini tutar
# thread table'dan seçilen threadin verilerini çekelim ve mailbox'a (600-607) yazalım:

151 CPY   808  846  # state pointer'ı temp olarak alalım
# 808 adresi baz alınarak her bir sütuna erişim için eklenecek değerler
152 SET   -1   847
153 SET   1    848
154 SET   2    849
155 SET   3    850
156 SET   4    851
157 SET   5    852
158 SET   6    853
# her bir sütuna işaret eden pointerlar:
159 ADDI  847  846  # thread id
160 ADDI  848  846  # thread pc
161 ADDI  849  846  # thread sp
162 ADDI  850  846  # thread data base
163 ADDI  851  846  # thread instruction base
164 ADDI  852  846  # thread instruction count
165 ADDI  853  846  # thread wakeup instruction count
# 846'nın kendisi de thread state'i tutar

# state'i running yap
166 SET   1    854
167 SET   854  855
168 CPYI2 855  846

# mailbox'a değerleri kopyala
169 CPYI  846  601
170 CPYI  847  600
171 CPYI  848  602
172 CPYI  849  603
173 CPYI  850  604
174 CPYI  851  605
175 CPYI  852  606
176 CPYI  853  607

177 CPY   602  112  # setting block 112: new threads PC OS will jump there using (USER 112)
178 SET   -999 17   # REG_CONTEXT_SWITCH_SIGNAL registerıyla sinyal ver
179 CPY   600  100  # current çalışan thread değiştir
180 RET             # OS booting kısmındaki CALL çağrısına geri dön, yeni bir thread seçimi için
#subroutine# scheduler end

#subroutine# syscall handler start
#
# * SYSCALL_ID değeri REGISTER 2'de tutuluyor
# * syscall prn için wakeup değeri, 103. adreste tutuluyor
#
# reading syscall id:
200 CPY   2    856  # 856 şuan syscall id tutuyor
201 CPY   94   857  # 94 prn_id tutar, yani 1
202 CPY   95   858  # 95 hlt_id tutar, yani 2
203 CPY   96   859  # 96 yield_id tutar, yani 3

204 SUBI  856  857  # syscall id - prn_id
205 JIF   857  210

206 SUBI  856  858  # syscall id - hlt_id
207 JIF   858  214

208 SUBI  856  859  # syscall id - yield_id
209 JIF   859  218

# yukarıdaki SUBI işlemleri ile id'lerin tutulduğu adresler dirty oluyor o yüzden
# ikinci çıkarmadan önce yine kopyalıyoruz

210 CPY   856  860
211 CPY   94   857
212 SUBI  857  860
213 JIF   860  222  # syscall prn handler'a atla

214 CPY   856  861
215 CPY   95   857
216 SUBI  857  861
217 JIF   861  226  # syscall hlt handler'a atla

218 CPY   856  862
219 CPY   96   858
220 SUBI  858  862
221 JIF   862  228  # syscall yield handler'a atla

# şimdi yukarıdaki bağlamda biz hangi syscall'ın çağrıldığını
# anlamış olup gerekli yerlere zıplamasını sağlıyoruz

222 CPY   103  863  # 863 adresine prn için wakeup_count kopyala
223 CPY   863  4    # register 4'e (REG_WAKEUP_INSTR_COUNT) 103'te tutulan 100 değerini yaz
224 CALL  30
225 JIF   99   230  # diğer case'lerin çalışmaması için atla

226 CALL  30
227 JIF   99   230  # diğer case'lerin çalışmaması için atla

228 CALL  30
229 JIF   99   230  # diğer case'lerin çalışmaması için atla

230 RET

#subroutine# syscall handler end

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
