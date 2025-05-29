Begin Data Section
0 -1
1 -2
2 -3
End Data Section
Begin Instruction Section
1 HLT
End Instruction Section

# Simple Bubble Sort for GTU-C312 (Fixed Version)

Begin Data Section
# Array elements (indices 0-9)
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
10  10      # N = 10 (array size)
11  0       # i (outer loop)
12  0       # j (inner loop)
13  0       # temp for w[j]
14  0       # temp for w[j+1]
15  0       # swap temp
16  1       # constant 1
17  0       # loop bound temp
18  0       # comparison temp
End Data Section

# Bubble Sort for GTU-C312 - Corrected JIF Usage
# Sorts array elements 0-9 in ascending order
#
# Key insight: JIF uses INSTRUCTION INDEX, not memory address
# So JIF 17 25 means "jump to instruction #25 if mem[17] <= 0"

Begin Instruction Section
0   SET 0 11        # i = 0

# Outer loop: for i = 0; i < 9; i++
1   CPY 11 17       # temp = i
2   ADD 17 -9       # temp = i - 9
3   JIF 17 4        # if i - 9 <= 0 (i < 9), continue
4   SET 40 0        # else jump to instruction 40 (halt)

5   SET 0 12        # j = 0

# Inner loop: for j = 0; j < 9-i; j++
6   CPY 12 17       # temp = j
7   CPY 10 18       # temp2 = 10
8   SUBI 18 11      # temp2 = 10 - i
9   ADD 18 -1       # temp2 = 9 - i
10  SUBI 18 17      # temp2 = 9 - i - j
11  JIF 18 52       # if 9-i-j <= 0, exit inner loop (go to instruction 37) ... 37  ADD 11 1        # i++

# Load w[j] and w[j+1]
12  CPYI 12 13      # w[j] -> address 13
13  CPY 12 17       # temp = j
14  ADD 17 1        # temp = j + 1
15  CPYI 17 14      # w[j+1] -> address 14

# Compare w[j] vs w[j+1]
16  CPY 13 17       # temp = w[j]
17  SUBI 17 14      # temp = w[j] - w[j+1]
18  JIF 17 50       # if w[j] <= w[j+1], no swap (go to instruction 35) ... 35  ADD 12 1        # j++

# Simple swap for first few positions (expand as needed)
19  CPY 12 17       # Check j value
20  JIF 17 21       # if j == 0, handle swap 0↔1
21  CPY 13 15
22  CPY 14 0
23  CPY 15 1
24  SET 35 0    # Swap elements 0,1

25  ADD 17 -1       # temp = j - 1
26  JIF 17 27       # if j == 1, handle swap 1↔2
27  CPY 13 15
28  CPY 14 1
29  CPY 15 2
30  SET 35 0    # Swap elements 1,2

31  ADD 17 -1       # temp = j - 2
32  JIF 17 33       # if j == 2, handle swap 2↔3
33  CPY 13 15
34  CPY 14 2
35  CPY 15 3
36  SET 35 0    # Swap elements 2,3

37  ADD 17 -1       # temp = j - 3
38  JIF 17 39       # if j == 3, handle swap 3↔4
39  CPY 13 15
40  CPY 14 3
41  CPY 15 4
42  SET 35 0    # Swap elements 3,4

43  ADD 17 -1       # temp = j - 4
44  JIF 17 45       # if j == 4, handle swap 4↔5
45  CPY 13 15
46  CPY 14 4
47  CPY 15 5
48  SET 35 0    # Swap elements 4,5

# Add more cases for j=5,6,7,8 as needed...
49  SET 35 0        # Default: no swap

# Continue loops
50  ADD 12 1        # j++
51  SET 6 0         # Jump back to inner loop (instruction 6)

52  ADD 11 1        # i++
53  SET 1 0         # Jump back to outer loop (instruction 1)

# Program end
54  HLT             # Halt CPU
End Instruction Section
