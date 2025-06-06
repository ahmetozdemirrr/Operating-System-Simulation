#!/bin/bash

# simulation/demo.sh
# Demo script for GTU-C312 CPU Simulator

echo "==============================================="
echo "GTU-C312 CPU Simulator Demo"
echo "==============================================="
echo ""

# Check if simulator exists
if [ ! -f "bin/simulator" ]; then
    echo "Error: Simulator not found. Please run 'make simulator' first."
    exit 1
fi

# Check if OS program exists
if [ ! -f "programs/os.asm" ]; then
    echo "Error: OS program not found at programs/os.asm"
    exit 1
fi

echo "Found simulator and OS program. Starting demos..."
echo ""

# Demo 1: Debug Mode 0 (Normal run + final memory dump)
echo "==============================================="
echo "DEMO 1: Debug Mode 0 - Normal execution"
echo "==============================================="
echo "Running: ./bin/simulator programs/os.asm -D 0 -max 5000"
echo "This will run the OS and threads, then dump final memory to stderr."
echo ""
read -p "Press ENTER to continue..."

./bin/simulator programs/os.asm -D 0 -max 5000 2> debug_mode_0_output.txt
echo ""
echo "Final memory dump saved to: debug_mode_0_output.txt"
echo "Check the file to see the complete memory state after execution."
echo ""

# Demo 2: Debug Mode 3 (Thread table monitoring)
echo "==============================================="
echo "DEMO 2: Debug Mode 3 - Thread table monitoring"
echo "==============================================="
echo "Running: ./bin/simulator programs/os.asm -D 3 -max 2000"
echo "This will show thread table after each context switch and syscall."
echo ""
read -p "Press ENTER to continue..."

./bin/simulator programs/os.asm -D 3 -max 2000 2> debug_mode_3_output.txt
echo ""
echo "Thread table traces saved to: debug_mode_3_output.txt"
echo "Check the file to see how threads change state during execution."
echo ""

# Demo 3: Debug Mode 1 (Memory dump after each instruction - LIMITED)
echo "==============================================="
echo "DEMO 3: Debug Mode 1 - Memory after each instruction (LIMITED)"
echo "==============================================="
echo "Running: ./bin/simulator programs/os.asm -D 1 -max 20"
echo "This will dump ALL memory after each instruction (only 20 instructions to avoid spam)."
echo ""
read -p "Press ENTER to continue..."

./bin/simulator programs/os.asm -D 1 -max 20 2> debug_mode_1_output.txt
echo ""
echo "Memory traces saved to: debug_mode_1_output.txt"
echo "Check the file to see memory state after each instruction."
echo ""

# Demo 4: Smaller test program
echo "==============================================="
echo "DEMO 4: Single thread test (multiplication)"
echo "==============================================="
echo "Running: ./bin/simulator tests/test_source/mult_by_addition.asm -D 0"
echo "This runs a simpler program with just multiplication thread."
echo ""
read -p "Press ENTER to continue..."

./bin/simulator tests/test_source/mult_by_addition.asm -D 0 -max 1000
echo ""

# Information about Demo 2 (interactive mode)
echo "==============================================="
echo "INFO: Debug Mode 2 - Interactive mode"
echo "==============================================="
echo "Debug Mode 2 is interactive and requires user input after each instruction."
echo "To try it manually, run:"
echo "  ./bin/simulator programs/os.asm -D 2 -max 10"
echo ""
echo "This will pause after each instruction and wait for ENTER key."
echo "Good for step-by-step debugging of small programs."
echo ""

# Summary
echo "==============================================="
echo "DEMO COMPLETE - Summary of generated files:"
echo "==============================================="
echo "- debug_mode_0_output.txt: Final memory dump"
echo "- debug_mode_1_output.txt: Step-by-step memory dumps (20 instructions)"
echo "- debug_mode_3_output.txt: Thread table changes and context switches"
echo ""
echo "You can examine these files to understand how the simulator works."
echo ""
echo "Try different debug modes and programs:"
echo "  ./bin/simulator programs/os.asm -D 0          # Complete OS execution"
echo "  ./bin/simulator programs/os.asm -D 3 -max 3000 # Thread monitoring"
echo "  ./bin/simulator tests/test_source/sort_test.asm -D 1 -max 50 # Step by step"
echo ""
echo "For interactive mode:"
echo "  ./bin/simulator programs/os.asm -D 2 -max 10  # Interactive debugging"
echo ""
echo "Happy debugging!"