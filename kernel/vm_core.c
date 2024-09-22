#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "instr_set.h"
#include <stdbool.h>
#include "vm_core.h"
#include "logging.h"
#include <unistd.h>
#include "vm_bios.h"
/*
ZVM - kernel/CPU functionality

Specification:
- It shall be possible to have several programs in the memory
- It shall be possible to swap active program either via external interface or another
  ZVM program running in privileged mode
TODO:
BootLoader which is run when ZVM starts
BootLoader then checks if there is a program to execute (boot vector in ZVM memory)

*/
const int empty_pattern = 0xfeedabba;
ZVM *theVM=0;

static inline void update_status_reg(int32_t result)
{
    if(result<0)
        theVM->REGS[EZVM_Reg_S] = -1;
    else if(result == 0)
       theVM->REGS[EZVM_Reg_S] = 0;
    else
       theVM->REGS[EZVM_Reg_S] = 1;
}
int zvm_init(void)
{
    logging_init();
    theVM = (ZVM*)malloc(sizeof(ZVM));
    if(theVM == NULL) {
        
        exit(1);
    }
 
    memset(theVM->prog_memory,0,sizeof(theVM->prog_memory));
    memset(theVM->REGS,0,sizeof(theVM->REGS));
    theVM->video_memory = 0; // not in use by default. Initialized by (possible) video driver
    theVM->debug = true;
    theVM->panic = false;
    theVM->terminate_program = false;
    return 1;
}

uint8_t*zvm_load_program_from_file(const char*fname, uint32_t *size)
{
    //TODO load program from file
    FILE *fp = fopen(fname,"rb");
    if(fp != 0) {
        *size = fseek(fp,0,SEEK_END);
        rewind(fp);
        uint8_t*data = malloc(*size);
        if(data == 0){
            fclose(fp);
            return 0;
        }
        fread(data,1,*size,fp);
        fclose(fp);
        return data;  
        }
    return 0;
}
bool zvm_load_program_to_memory(uint8_t*data, uint32_t size_in_bytes,uint32_t address)
{
    uint32_t *prog_start = theVM->prog_memory + address;
    uint32_t idx = 0;
    for(int b=0;b<size_in_bytes;b+=4)
    {
        *(prog_start+idx) = *(data+b) | *(data+b+1) << 8 | *(data+b+2) << 16 | *(data+b+3) << 24;
        idx++;
    }
    return true;
}


// a program must be loaded to memory before calling this function
bool zvm_run_vm()
{
    bool running = true;
    while (running) {


        // Get the current program counter (PC)
        int32_t pc = theVM->REGS[EZVM_Reg_PC];

        // Check if the PC is within the valid program memory range
        if (pc < 0 || pc >= MAX_PROG_MEM_SIZE) {
            zvm_set_panic("PC out of range");
            return false;
        }

        // Read the next instruction from memory
        int32_t instruction = theVM->prog_memory[pc];

        // Increment the PC to point to the next instruction
        theVM->REGS[EZVM_Reg_PC]++;

        // Decode and execute the instruction
        switch (instruction) {
            case EZVM_NOP:
                // No operation, do nothing
                break;
            case EZVM_ADD:
                // Add instruction, read operands and execute
                {
                    int32_t reg1 = theVM->prog_memory[pc];
                    int32_t reg2 = theVM->prog_memory[pc + 1];
                    int32_t result = theVM->REGS[reg1] + theVM->REGS[reg2];
                    theVM->REGS[reg1] = result;
                    theVM->REGS[EZVM_Reg_PC] += 2; // Increment PC to skip operands
                }
                break;
            case EZVM_ADDI:
                // Add immediate instruction, read operands and execute
                {
                    int32_t reg1 = theVM->prog_memory[pc];
                    int32_t imm = theVM->prog_memory[pc + 1];
                    int32_t result = theVM->REGS[reg1] + imm;
                    theVM->REGS[reg1] = result;
                    theVM->REGS[EZVM_Reg_PC] += 2; // Increment PC to skip operands
                }
                break;
            case EZVM_SUB:
            {
                int32_t reg1 = theVM->prog_memory[pc];
                int32_t reg2 = theVM->prog_memory[pc + 1];
                int32_t result = theVM->REGS[reg1] - theVM->REGS[reg2];
                theVM->REGS[reg1] = result;
                theVM->REGS[EZVM_Reg_PC] += 2; // Increment PC to skip operands
            }
            break;
            case EZVM_SUBI:
            {
                int32_t reg1 = theVM->prog_memory[pc];
                int32_t imm = theVM->prog_memory[pc + 1];
                int32_t result = theVM->REGS[reg1] - imm;
                theVM->REGS[reg1] = result;
                update_status_reg(result);
                theVM->REGS[EZVM_Reg_PC] += 2; // Increment PC to skip operands
            }
            break;
            case EZVM_MUL:
            {
                int32_t reg1 = theVM->prog_memory[pc];
                int32_t reg2 = theVM->prog_memory[pc + 1];
                int32_t result = theVM->REGS[reg1] * theVM->REGS[reg2];
                theVM->REGS[reg1] = result;
                theVM->REGS[EZVM_Reg_PC] += 2; // Increment PC to skip operands
            }
            break;
            case EZVM_SYSCALL:
            {
                int32_t syscall = theVM->prog_memory[pc+1];
                call_bios_func(syscall);
                
            }    
            break;
            default:
                // Handle unknown instruction
                return false;
        }
        usleep(CPU_TICK_DELAY);
        // check if the program should be terminated
        if(theVM->panic == true) 
            running = false;
        if(theVM->terminate_program == true)
            running = false;
    }
    if(theVM->panic == true)
    {
        logging_log("Program terminated abnormally.\n");
    }
    else
    {
        logging_log("Program finished gracefully.\n");

    }
    return true;
}

void zvm_set_panic(const char*msg){
    if(theVM->panic == true)
        return;
    theVM->panic = true;
    
    logging_log("ZVM Kernel PANIC:");
    logging_log(msg);    
    logging_log("System Halted\n"); 
}
void zvm_set_mode(CPUMode m)
{
    theVM->mode = m;
}

void zvm_single_step()
{
    if(theVM->mode != CPUMode_SingleStep) // stepping only in continuous mode
    {
        return;
    }
}
