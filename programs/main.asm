##### sorting N numbers #####

Begin Data Section
# address 0-9: datas
0   12      # w[0]
1   3       # w[1]
2   89      # w[2]
3   45      # w[3]
4   -3      # w[4]
5   0       # w[5]
6   100     # w[6]
7   1999999 # w[7]
8   -999    # w[8]
9   5       # w[9]
10  10      # N
11  0       # temp value for calculations
12  0       # i (outer loop counter)
13  0       # j (inner loop counter)
14  0       # temp for N copy
15  1       # constant 1
16  0       # temp for w[j]
17  0       # temp for w[j+1]
18  0       # temp for difference
19  0       # temp for swap
20  -1    # constant -1 for unconditional jump
End Data Section

Begin Instruction Section
0   SET 0 12      # i = 0
1   SET 0 13      # j = 0
2   CPY 10 14     # copy N to temp (14)
3   SUBI 14 15    # N-1 (result in 14)
4   CPY 14 18     # copy N-1 to temp (18) for comparison
5   SUBI 18 12    # N-1 - i (result in 18)
6   JIF 18 50     # if N-1-i <= 0 jump to 50 (exit outer loop)

7   SET 0 13      # j = 0 (inner loop start)
8   CPY 14 18     # copy N-1 to temp (18)
9   SUBI 18 12    # N-1-i (result in 18)
10  SUBI 18 15    # N-1-i-1 (result in 18)
11  CPY 10 19     # copy N to temp (19) for inner loop bound
12  SUBI 19 15    # N-1
13  SUBI 19 12    # N-1-i
14  SUBI 19 15    # N-1-i-1 (inner loop bound)
15  CPY 19 18     # copy bound to 18
16  SUBI 18 13    # (N-1-i-1) - j (result in 18)
17  JIF 18 40     # if (N-1-i-1)-j <= 0 jump to 40 (inner loop end)

18  CPY 0 16      # copy w[0] to temp (16) for j=0
19  CPY 1 17      # copy w[1] to temp (17) for j=0
20  CPY 0 16      # copy w[j] to temp (16)
21  CPY 1 18      # copy w[j+1] to temp (18)
22  SUBI 16 18    # w[j] - w[j+1] (result in 18)
23  JIF 18 30     # if w[j] - w[j+1] <= 0 jump to 30 (no swap)

24  CPY 0 19      # swap: copy w[j] to temp (19)
25  CPY 1 0       # copy w[j+1] to w[j]
26  CPY 19 1      # copy temp to w[j+1]

30  ADD 13 1      # j = j + 1
31  JIF 20 8      # unconditional jump to 8 (inner loop start) using constant -1

40  ADD 12 1      # i = i + 1
41  JIF 20 4      # unconditional jump to 4 (outer loop start)

50  SET 0 13      # j = 0 for printing loop
51  CPY 10 18     # copy N to temp (18)
52  SUBI 18 13    # N-j (result in 18)
53  JIF 18 70     # if N-j <= 0 jump to 70 (end print loop)

54  SYSCALL PRN 0 # print w[0] for j=0
55  ADD 13 1      # j = j + 1
56  JIF 20 51     # unconditional jump to 51 (print loop start)

70  SYSCALL HLT   # halt thread
End Instruction Section-
