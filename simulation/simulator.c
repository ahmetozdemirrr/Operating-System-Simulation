/* simulation/simulator.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <cpu.h>
#include <memory.h>
#include <parser.h>

typedef enum
{
    DEBUG_MODE_0 = 0,  /* Memory dump after CPU halts                            */
    DEBUG_MODE_1 = 1,  /* Memory dump after each instruction                     */
    DEBUG_MODE_2 = 2,  /* Memory dump after each instruction + wait for keypress */
    DEBUG_MODE_3 = 3   /* Thread table dump after context switch/syscall         */
}
DebugMode;

typedef struct
{
    char * filename;
    DebugMode debug_mode;
    int max_instructions;
}
SimulationConfig;

#define UNTIL_LAST_THREAD 10000

/* helper function to print thread table (for debug mode 3) */
void print_thread_table(Memory * mem)
{
    fprintf(stderr, "\n=== THREAD TABLE DUMP ===\n");

    /* Thread table starts at OS_DATA_START_ADDR + 200 (absolute address 220) */
    long int thread_table_base = OS_DATA_START_ADDR + 200;

    fprintf(stderr, "Thread Table Base Address: %ld\n", thread_table_base);
    fprintf(stderr, "Format: [ID] [State] [PC] [SP] [DataBase] [InstrBase] [IC] [WakeupCount]\n");
    fprintf(stderr, "States: 0=READY, 1=RUNNING, 2=BLOCKED, 3=TERMINATED\n\n");

    /* Print entries for OS + up to 4 user threads (0-4) */
    for (int thread_id = 0; thread_id <= 4; thread_id++)
    {
        long int entry_base = thread_table_base + (thread_id * 8);

        long int id = mem_read(mem, entry_base + 0, KERNEL);
        long int state = mem_read(mem, entry_base + 1, KERNEL);
        long int pc = mem_read(mem, entry_base + 2, KERNEL);
        long int sp = mem_read(mem, entry_base + 3, KERNEL);
        long int data_base = mem_read(mem, entry_base + 4, KERNEL);
        long int instr_base = mem_read(mem, entry_base + 5, KERNEL);
        long int ic = mem_read(mem, entry_base + 6, KERNEL);
        long int wakeup = mem_read(mem, entry_base + 7, KERNEL);

        const char * state_str;

        switch (state)
        {
            case 0:  state_str = "READY";      break;
            case 1:  state_str = "RUNNING";    break;
            case 2:  state_str = "BLOCKED";    break;
            case 3:  state_str = "TERMINATED"; break;
            default: state_str = "UNKNOWN";    break;
        }
        fprintf(stderr, "Thread %d: [%ld] [%ld:%s] [%ld] [%ld] [%ld] [%ld] [%ld] [%ld]\n",
                thread_id,
                id,
                state,
                state_str,
                pc,
                sp,
                data_base,
                instr_base,
                ic,
                wakeup);
    }
    fprintf(stderr, "=== END THREAD TABLE ===\n\n");
}

/* helper function to detect system calls */
int is_syscall_instruction(Memory *mem, long int pc_address)
{
    if (pc_address < 0 || pc_address >= MEM_SIZE) return 0;

    long int opcode = mem_read(mem, pc_address, KERNEL);
    return (opcode == OPCODE_SYSCALL_PRN ||
            opcode == OPCODE_SYSCALL_HLT ||
            opcode == OPCODE_SYSCALL_YIELD);
}

/* helper function to check if all user threads are terminated (only idle thread left) */
int all_user_threads_terminated(Memory * mem)
{
    /* Thread table starts at OS_DATA_START_ADDR + 200 (absolute address 220) */
    long int thread_table_base = OS_DATA_START_ADDR + 200;

    /* Check threads 2, 3, 4 (user threads, skip OS=0 and idle=1) */
    for (int thread_id = 2; thread_id <= 4; thread_id++) {
        long int entry_base = thread_table_base + (thread_id * 8);
        long int state = mem_read(mem, entry_base + 1, KERNEL);

        if (state != 3) { /* 3 = TERMINATED */
            return 0; /* Found a non-terminated user thread */
        }
    }

    /* Check if idle thread (thread 1) is READY */
    long int idle_entry_base = thread_table_base + (1 * 8);
    long int idle_state = mem_read(mem, idle_entry_base + 1, KERNEL);

    return (idle_state == 0); /* 0 = READY */
}

/* wait for keypress function */
void wait_for_keypress()
{
    fprintf(stderr, "\nPress ENTER to continue to next instruction...\n");
    getchar();
}

/* parse command line arguments */
int parse_arguments(int argc, char * argv[], SimulationConfig * config)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename> [-D debug_mode] [-max max_instructions]\n", argv[0]);
        fprintf(stderr, "Debug modes:\n");
        fprintf(stderr, "  0: Memory dump after CPU halts\n");
        fprintf(stderr, "  1: Memory dump after each instruction\n");
        fprintf(stderr, "  2: Memory dump after each instruction + wait for keypress\n");
        fprintf(stderr, "  3: Thread table dump after context switch/syscall\n");
        return -1;
    }

    /* initialize defaults */
    config->filename = argv[1];
    config->debug_mode = DEBUG_MODE_0;
    config->max_instructions = 200000;  /* increased default limit for thread completion */

    /* parse optional arguments */
    for (int i = 2; i < argc; i++)
    {
        if (strcmp(argv[i], "-D") == 0 && i + 1 < argc)
        {
            int mode = atoi(argv[i + 1]);
            if (mode >= 0 && mode <= 3) {
                config->debug_mode = (DebugMode)mode;
                i++; /* Skip next argument */
            }
            else {
                fprintf(stderr, "ERROR: Invalid debug mode %d. Must be 0-3.\n", mode);
                return -1;
            }
        }

        else if (strcmp(argv[i], "-max") == 0 && i + 1 < argc) {
            config->max_instructions = atoi(argv[i + 1]);
            if (config->max_instructions <= 0) {
                fprintf(stderr, "ERROR: Max instructions must be positive.\n");
                return -1;
            }
            i++; /* Skip next argument */
        }
    }
    return 0;
}

int main(int argc, char * argv[])
{
    SimulationConfig config;

    printf("GTU-C312 CPU Simulator\n");
    printf("======================\n\n");

    /* parse command line arguments */
    if (parse_arguments(argc, argv, &config) != 0) {
        return EXIT_FAILURE;
    }

    printf("Configuration:\n");
    printf("- Program file: %s\n", config.filename);
    printf("- Debug mode: %d\n", config.debug_mode);
    printf("- Max instructions: %d\n\n", config.max_instructions);

    /* initialize memory */
    Memory mem;
    mem_init(&mem);
    printf("Memory initialized (%d bytes)\n", MEM_SIZE);

    /* load program from file */
    printf("Loading program from: %s\n", config.filename);
    if (load_program_from_file(config.filename, &mem) != 0) {
        fprintf(stderr, "ERROR: Failed to load program file: %s\n", config.filename);
        mem_free(&mem);
        return EXIT_FAILURE;
    }
    printf("Program loaded successfully!\n\n");

    /* Initialize CPU */
    CPU cpu;
    cpu_init(&cpu, &mem);
    printf("CPU initialized in KERNEL mode\n");
    printf("Starting execution...\n\n");

    /* execution statistics */
    int instruction_count = 0;
    int context_switches = 0;
    int syscalls = 0;
    int last_thread_id = cpu.curr_thread_id;

    /* main execution loop */
    while (!cpu_is_halted(&cpu) && instruction_count < config.max_instructions)
    {
        /* check if all user threads are done (only idle thread left) */
        if (all_user_threads_terminated(&mem)) {
            printf("\n=== ALL USER THREADS COMPLETED ===\n");
            printf("All user threads have terminated. Only idle thread (Thread 1) is running.\n");
            printf("Idle thread will continue indefinitely. Stopping simulation here.\n");
            printf("Instructions executed: %d\n", instruction_count);
            break;
        }

        /* get current PC for syscall detection */
        long int current_pc = mem_read(&mem, REG_PC, KERNEL);
        int is_syscall = is_syscall_instruction(&mem, current_pc);

        /* detect context switch */
        int context_switched = (cpu.curr_thread_id != last_thread_id);
        if (context_switched) {
            context_switches++;
            if (config.debug_mode == DEBUG_MODE_3) {
                fprintf(stderr, "\n[INSTRUCTION %d] CONTEXT SWITCH DETECTED: Thread %d -> Thread %d\n",
                        instruction_count,
                        last_thread_id,
                        cpu.curr_thread_id);
                print_thread_table(&mem);
            }
            last_thread_id = cpu.curr_thread_id;
        }

        /* debug mode 3: Print thread table before syscall */
        if (config.debug_mode == DEBUG_MODE_3 && is_syscall) {
            fprintf(stderr, "\n[INSTRUCTION %d] SYSCALL DETECTED at PC=%ld, Thread=%d\n",
                    instruction_count,
                    current_pc,
                    cpu.curr_thread_id);
            print_thread_table(&mem);
        }

        /* execute one instruction */
        cpu_execute_instruction(&cpu);
        instruction_count++;

        /* debug mode 1: Memory dump after each instruction */
        if (config.debug_mode == DEBUG_MODE_1) {
            fprintf(stderr, "\n[INSTRUCTION %d] Memory dump after execution:\n", instruction_count);
            fprintf(stderr, "Current Thread: %d, Mode: %s, PC: %ld\n",
                    cpu.curr_thread_id,
                    cpu.mode == KERNEL ? "KERNEL" : "USER",
                    mem_read(&mem, REG_PC, KERNEL));
            mem_dump(&mem, 0, UNTIL_LAST_THREAD);
        }

        /* debug mode 2: Memory dump + wait for keypress */
        if (config.debug_mode == DEBUG_MODE_2) {
            fprintf(stderr, "\n[INSTRUCTION %d] Memory dump after execution:\n", instruction_count);
            fprintf(stderr, "Current Thread: %d, Mode: %s, PC: %ld\n",
                    cpu.curr_thread_id,
                    cpu.mode == KERNEL ? "KERNEL" : "USER",
                    mem_read(&mem, REG_PC, KERNEL));
            mem_dump(&mem, 0, UNTIL_LAST_THREAD);
            wait_for_keypress();
        }

        /* debug mode 3: Print thread table after syscall execution */
        if (config.debug_mode == DEBUG_MODE_3 && is_syscall) {
            fprintf(stderr, "\n[INSTRUCTION %d] THREAD TABLE AFTER SYSCALL:\n", instruction_count);
            print_thread_table(&mem);
        }

        /* Count syscalls */
        if (is_syscall) {
            syscalls++;
        }

        /* periodic progress report (except in mode 1 and 2 which are already verbose) */
        if ((config.debug_mode != DEBUG_MODE_1 && config.debug_mode != DEBUG_MODE_2) &&
            (instruction_count % 100 == 0)) {
            printf("Executed %d instructions... (Thread: %d)\n",
                   instruction_count, cpu.curr_thread_id);
        }
    }

    /* execution summary */
    printf("\n=== EXECUTION SUMMARY ===\n");
    printf("Total instructions executed: %d\n", instruction_count);
    printf("Context switches: %d\n", context_switches);
    printf("System calls: %d\n", syscalls);
    printf("CPU halted: %s\n", cpu_is_halted(&cpu) ? "Yes" : "No");

    if (instruction_count >= config.max_instructions) {
        printf("WARNING: Execution stopped at instruction limit (%d)\n", config.max_instructions);
        printf("Some threads may not have completed their execution.\n");
    }
    else if (all_user_threads_terminated(&mem)) {
        printf("SUCCESS: All user threads completed successfully!\n");
        printf("Only idle thread remains active (as expected).\n");
    }

    /* debug mode 0: Memory dump after CPU halts */
    if (config.debug_mode == DEBUG_MODE_0) {
        fprintf(stderr, "\n=== FINAL MEMORY DUMP (Debug Mode 0) ===\n");
        fprintf(stderr, "Final CPU state - Thread: %d, Mode: %s, PC: %ld\n",
                cpu.curr_thread_id,
                cpu.mode == KERNEL ? "KERNEL" : "USER",
                mem_read(&mem, REG_PC, KERNEL));
        mem_dump(&mem, 0, UNTIL_LAST_THREAD);
    }

    /* final thread table for all modes */
    if (config.debug_mode == DEBUG_MODE_3) {
        fprintf(stderr, "\n=== FINAL THREAD TABLE ===\n");
        print_thread_table(&mem);
    }

    /* CPU register dump */
    printf("\n=== FINAL CPU STATE ===\n");
    cpu_dump_registers(&cpu);

    /* cleanup */
    mem_free(&mem);

    printf("\nSimulation completed successfully!\n");
    return EXIT_SUCCESS;
}
