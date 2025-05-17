# comment
Begin Data Section
0 0 # comment




1 16383
50 100
51 200
52 -5 # comment

# here before ending data section we cannot begin instruction section (DFA)
Begin Instruction Section
End Data Section

0 CPY 50 600 # comment
1 ADDI 600 51
2 SYSCALL PRN 600
3 HLT
End Instruction Section
