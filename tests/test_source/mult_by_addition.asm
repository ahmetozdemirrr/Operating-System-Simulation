# tests/test_source/mult_by_addition.asm #

Begin Data Section
0 -1
1 -2
2 -3
50  3000
End Data Section

Begin Instruction Section
0 USER 50 # Switch to user mode and jump to address in mem[50]
1 HLT
End Instruction Section

# Multiplication Implementation
Begin Data Section
0   8   # mult operand 1
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
# loop strat

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
