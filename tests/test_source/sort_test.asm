Begin Data Section
0 -1
1 -2
2 -3
End Data Section
Begin Instruction Section
0 USER 50    # Switch to user mode and jump to address in mem[50]
1 HLT
End Instruction Section

# Working Bubble Sort for 5 numbers
# Performs 4 passes to ensure complete sorting
# Array: [5, 2, 8, 1, 3] -> [1, 2, 3, 5, 8]

Begin Data Section
# Array elements
0   5       # arr[0]
1   2       # arr[1]
2   8       # arr[2]
3   1       # arr[3]
4   3       # arr[4]

# Working variables
5   0       # temp for comparisons
6   0       # swap temp
7   4       # pass counter (4 passes needed)
8   -1      # constant -1 for unconditional jumps
50  2000    # Jump address for USER command (Thread 1 instruction start)
End Data Section

Begin Instruction Section
# Main sorting loop - do 4 passes
0   CPY 7 5         # temp = pass_counter
1   JIF 5 29        # if pass_counter <= 0, jump to print
2   JIF 8 3         # else continue (unconditional)

# Pass: Compare all adjacent pairs
# Compare arr[0] and arr[1]
3   CPY 0 5         # temp = arr[0]
4   SUBI 5 1        # temp = arr[0] - arr[1]
5   JIF 5 9         # if arr[0] <= arr[1], skip swap
6   CPY 0 6         # swap_temp = arr[0]
7   CPY 1 0         # arr[0] = arr[1]
8   CPY 6 1         # arr[1] = swap_temp

# Compare arr[1] and arr[2]
9   CPY 1 5         # temp = arr[1]
10  SUBI 5 2        # temp = arr[1] - arr[2]
11  JIF 5 15        # if arr[1] <= arr[2], skip swap
12  CPY 1 6         # swap_temp = arr[1]
13  CPY 2 1         # arr[1] = arr[2]
14  CPY 6 2         # arr[2] = swap_temp

# Compare arr[2] and arr[3]
15  CPY 2 5         # temp = arr[2]
16  SUBI 5 3        # temp = arr[2] - arr[3]
17  JIF 5 21        # if arr[2] <= arr[3], skip swap
18  CPY 2 6         # swap_temp = arr[2]
19  CPY 3 2         # arr[2] = arr[3]
20  CPY 6 3         # arr[3] = swap_temp

# Compare arr[3] and arr[4]
21  CPY 3 5         # temp = arr[3]
22  SUBI 5 4        # temp = arr[3] - arr[4]
23  JIF 5 27        # if arr[3] <= arr[4], skip swap
24  CPY 3 6         # swap_temp = arr[3]
25  CPY 4 3         # arr[3] = arr[4]
26  CPY 6 4         # arr[4] = swap_temp

# End of pass, decrement counter
27  ADD 7 -1        # pass_counter--
28  JIF 8 0         # jump back to start of main loop

# Sorting complete, just halt
29  HLT             # halt

End Instruction Section
