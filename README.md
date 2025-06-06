# GTU-C312 CPU Simulator Usage Guide

This simulator is designed to run operating systems and threads written for the GTU-C312 CPU architecture.

## Compilation

```bash
make simulator
```

This command will generate the `bin/simulator` executable file.

## Usage

```bash
./bin/simulator <filename> [-D debug_mode] [-max max_instructions]
```

### Parameters

- `<filename>`: The assembly program file to execute (e.g., `programs/os.asm`)
- `-D debug_mode`: Debug mode (0-3, default: 0)
- `-max max_instructions`: Maximum number of instructions (default: 50000)

### Debug Modes

#### Debug Mode 0 (Default)

Prints the entire memory contents to stderr when the CPU halts.

```bash
./bin/simulator programs/os.asm -D 0
```

#### Debug Mode 1

Prints the entire memory contents to stderr after each instruction execution.

```bash
./bin/simulator programs/os.asm -D 1
```

**Warning:** This mode generates a large amount of output. Suitable for small programs.

#### Debug Mode 2

Prints the entire memory contents to stderr after each instruction execution and waits for a keypress to continue.

```bash
./bin/simulator programs/os.asm -D 2
```

**Usage:** Ideal for step-by-step debugging. Press ENTER after each instruction to proceed.

#### Debug Mode 3

Prints the thread table contents to stderr after each context switch and system call.

```bash
./bin/simulator programs/os.asm -D 3
```

This mode is most suitable for debugging thread management and scheduling.

## Example Usages

### 1. Normal Execution

```bash
./bin/simulator programs/os.asm
```

### 2. Thread Table Monitoring

```bash
./bin/simulator programs/os.asm -D 3
```

### 3. Step-by-Step Debugging

```bash
./bin/simulator programs/os.asm -D 2 -max 100
```

### 4. Memory Content Monitoring

```bash
./bin/simulator programs/os.asm -D 1 -max 50 2> memory_trace.txt
```

## Output Formats

### Normal Output (stdout)

- General information and execution summary
- Thread SYSCALL PRN outputs
- CPU status and statistics

### Debug Output (stderr)

- Memory dumps (Debug Mode 0, 1, 2)
- Thread table contents (Debug Mode 3)
- Context switch notifications

### Thread Table Format (Debug Mode 3)

```
Thread Table Base Address: 220
Format: [ID] [State] [PC] [SP] [DataBase] [InstrBase] [IC] [WakeupCount]
States: 0=READY, 1=RUNNING, 2=BLOCKED, 3=TERMINATED

Thread 0: [0] [1:RUNNING] [1000] [1999] [20] [1000] [0] [0]
Thread 1: [1] [0:READY] [3000] [3999] [2000] [3000] [0] [0]
Thread 2: [2] [0:READY] [5000] [5999] [4000] [5000] [0] [0]
Thread 3: [3] [0:READY] [7000] [7999] [6000] [7000] [0] [0]
Thread 4: [4] [0:READY] [9000] [9999] [8000] [9000] [0] [0]
```

## Test Programs

Test programs included with the project:

1. **programs/os.asm**: Main OS and 4 user threads (sorting, searching, multiplication, idle)
2. **tests/test_source/sort_test.asm**: Only bubble sort thread
3. **tests/test_source/linear_search.asm**: Only linear search thread
4. **tests/test_source/mult_by_addition.asm**: Only multiplication thread

## Performance Notes

- **Debug Mode 0**: Fastest, only dumps at the end
- **Debug Mode 1**: Very slow, full memory dump for each instruction
- **Debug Mode 2**: Slowest, requires user interaction
- **Debug Mode 3**: Moderately fast, only thread table at necessary points

## Troubleshooting

### Program Takes Too Long

```bash
./bin/simulator programs/os.asm -max 10000
```

### Memory Error

Check memory status with Debug Mode 1 or 2.

### Thread Issues

Track thread states with Debug Mode 3.

### Suspected Infinite Loop

Test with a low max instruction limit:

```bash
./bin/simulator programs/os.asm -max 1000
```
