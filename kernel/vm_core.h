
#ifndef _VM_CORE_H_
#define _VM_CORE_H_

#include <stdint.h>
#include <stdbool.h>
#define  CPU_TICK_DELAY 10 // 10 us
#define VM_VERSION "0.0.0.1"

typedef enum EZVM_Regs {
    EZVM_Reg_R0=0,
    EZVM_Reg_R1,
    EZVM_Reg_R2,
    EZVM_Reg_R3,
    EZVM_Reg_R4,
    EZVM_Reg_R5,
    EZVM_Reg_R6,
    EZVM_Reg_R7,
    EZVM_Reg_SP, //stack pointer
    EZVM_Reg_PC, // Program Counter
    EZVM_Reg_S, //status register
    EZVM_Reg_Last
}EZVM_Regs;

#define PROG_MEM_OFFET 0
#define MAX_PROG_MEM_SIZE  1000000 // 1M 32bit words => 4MB  memory (includes also data)
#define MAX_STACK_SIZE 1000
#define MAX_SYSCALL_SIZE 1000
#define VIDEO_MEM_OFFSET 1000000

typedef enum {
    CPUMode_Continuous,
    CPUMode_SingleStep
}CPUMode;

typedef struct  {
    uint32_t prog_memory[MAX_PROG_MEM_SIZE];
    uint32_t stack[MAX_STACK_SIZE];
    uint32_t bios[MAX_SYSCALL_SIZE];
    int32_t REGS[EZVM_Reg_Last];
    uint32_t *video_memory; // allocated (possibly) by video driver
    uint32_t cpu_cycles_per_second;
    bool terminate_program;
    CPUMode mode;
    bool panic;
    bool debug;
    bool running;
}VM;

// -- ZVM API functions --
uint8_t *vm_load_program_from_file(const char*fname, uint32_t *size);
bool vm_load_program_to_memory(uint8_t*data, uint32_t size_in_bytes,uint32_t address);
bool vm_save_memory_to_file(const char *fname,uint32_t address,uint32_t length);
bool vm_run_vm(); // program must be loaded before calling this function
void vm_set_mode(CPUMode);
void vm_single_step();
void vm_set_panic(const char *msg);
int32_t vm_init(CPUMode);
void vm_warm_reset(CPUMode);
bool vm_handle_next_instruction();
void dump_regs();
void dump_memory(int32_t addr_start, int32_t addr_end);




#endif
