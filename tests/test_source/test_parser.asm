# tests/test_source/test_parser.asm

# OS
Begin Data Section
0 0    # Program Counter
1 16383 # Stack Pointer
End Data Section
Begin Instruction Section
0 HLT  # Dummy OS instruction
End Instruction Section

# Thread 1
Begin Data Section
0 100    # Thread 1 data at address 1000
50 100   # Thread 1 data at address 1050
51 200   # Thread 1 data at address 1051
52 -5    # Thread 1 data at address 1052
End Data Section
Begin Instruction Section
0 CPY 50 200    # Copy value from address 1050 to 1600
1 ADDI 200 51   # Add value at address 1051 to 1600
2 SYSCALL PRN 200 # Print value at address 1600
3 HLT           # Halt the thread
End Instruction Section

# Thread 2 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT  # Inactive thread
End Instruction Section

# Thread 3 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section

# Thread 4 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section

# Thread 5 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section

# Thread 6 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section

# Thread 7 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section

# Thread 8 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section

# Thread 9 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section

# Thread 10 (inactive)
Begin Data Section
End Data Section
Begin Instruction Section
0 SYSCALL HLT
End Instruction Section
