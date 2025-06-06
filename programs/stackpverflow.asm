# Stack Overflow Demo
Begin Data Section
0   0       # dummy data
End Data Section

Begin Instruction Section
# Multiple nested function calls
0   CALL 1         # call function_a
1   CALL 2         # function_a: call function_b
2   CALL 3         # function_b: call function_c
3   CALL 4         # function_c: call function_d
4   CALL 0         # function_d: call main again (creates loop)
5   HLT            # never reached
End Instruction Section
