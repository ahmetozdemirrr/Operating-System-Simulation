# GTU-C312 Instruction Set

This document provides a concise overview of the GTU-C312 CPU instruction set. All parameters are treated as signed long integers.

___



Instruction	Explanation

___



`SET B A`: Sets memory location A to the value B.
`CPY A1 A2`: Copies the content of memory location A1 to memory location A2.
`CPYI A1 A2`: Copies the content of the memory address stored in A1 to memory location A2.
`ADD A B`: Adds the value B to the content of memory location A, storing the result in A.
`ADDI A1 A2`: Adds the content of memory location A2 to the content of memory location A1, storing the result in A1.
`SUBI A1 A2`: Subtracts the content of memory location A2 from A1, storing the result in A1.
`JIF A C`: Sets the program counter to C if the content of memory location A is ≤ 0.
`PUSH A`: Pushes the content of memory location A onto the stack (stack grows downwards).
`POP A`: Pops a value from the stack into memory location A.
`CALL C`: Calls a subroutine at instruction C, pushing the return address onto the stack.
`RET`: Returns from a subroutine by popping the return address from the stack.
`HLT`: Halts the CPU execution.
`USER`: Switches the CPU to USER mode (restricts memory access to addresses ≥ 1000).
`SYSCALL PRN A`: Prints the content of memory address A to the console with a newline; blocks the thread for 100 instruction executions.
`SYSCALL HLT`: Shuts down the calling thread.
`SYSCALL YIELD`: Yields the CPU to allow the OS to schedule other threads.