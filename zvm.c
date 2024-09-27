/* 
------------------------------------------------------
Implements main user interface for ZVM virtual machine
------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kernel/vm_core.h"
#include "kernel/logging.h"
//#define DEV_DEBUG 1

#ifdef DEV_DEBUG

uint32_t test_program[] ={
    21,1,0, // MOV R1,#0, 
    21,0,18, // MOV R0,#18, which is address of data
    18,1, // SYSCALL print string
    2,1,1, // ADDI R1,1   increment R1
    14,1,5, // CMPI R1,5
    27,6, // jump if R1 smaller than 5 to address 6 (start of loop)
    18,2, // SYSCALL end of program
    //data as follows, now in host (x86) little endian format as endianess not handled yet in ZVM
   // l l e H    Z   , o    W   M V    d l r o
    0x6c6c6548,0x5a202c6f,0x77204d56,0x646c726f,0x0,0x0
};

#endif
extern ZVM* theVM;
int main(int argc, char *argv[])
{

#ifndef DEV_DEBUG
printf("ZVM Virtual Machine (c) MAP 2024.\n");
printf("This program is free software, you can redistribute it and/or modify it under the terms of the MIT license.\n");
printf("https://github.com/Definibus/ZVM\n");
printf("Version: %s\n",ZVM_VERSION);
CPUMode cfg_cpu_mode = CPUMode_Continuous;
if(argc < 2) {
    printf("Usage: zvm <zvm program name>\n");
    return 1;
}
if(argc>2) {
    if(strcmp("--debug",argv[2]) == 0)
    {
        cfg_cpu_mode= CPUMode_SingleStep;
    }
}

zvm_init(cfg_cpu_mode);
uint32_t prog_size;
uint8_t *prog = zvm_load_program_from_file(argv[1], &prog_size);
if(prog!=0)
    printf("Program size: %d\n",prog_size);


#else
printf("Running in Dev debug mode, loading test program...\n");
uint32_t prog_size = sizeof(test_program);
uint8_t *prog = (uint8_t*)test_program;
logging_log("Program size: %d\n",prog_size);
#endif

if(prog == 0)
{
    printf("Cannot load program: %s\n",argv[1]);
    return 1;
}

bool status = zvm_load_program_to_memory(prog, prog_size, 0);
if(status == false)
{
    printf("Cannot load program: %s\n to memory.",argv[1]);
    return 1;
}

printf("Program loaded to memory: %d\n",status);
if(theVM->mode == CPUMode_SingleStep) {
    printf("Running in Debug / single step mode.\n");
    while (theVM->running) {
        zvm_run_vm();
        printf("Press Enter to step, type r' for registers,'mem <addr> <addr>' for mem dump, 'q' to quit: ");
        char input[10];
        fgets(input, 10, stdin);
        if( strncmp(input,"q",1) == 0) {
            theVM->running = false;
        }    
        else if( strncmp(input,"r",1) == 0) {
            dump_regs();
        }
        else if(strcmp(input,"\n") == 0) {
            // do nothing, just step again
        } 
        else if(strncmp(input,"mem ",4) == 0) {
             // Parse memory address range from input
            char* addr_str = input + 4;
            char* end_str = strchr(addr_str, ' ');
            if (end_str == NULL) {
                printf("Invalid memory address range\n");
                continue;
            }
            uint32_t addr_start = strtoul(addr_str, NULL, 16);
            uint32_t addr_end = strtoul(end_str + 1, NULL, 16);

            // Dump memory contents
            dump_memory(addr_start, addr_end);
        }
        else {
            printf("Invalid input\n");
        }
        dump_regs();
    }
    printf("Program Ended.\n");
} else {
    zvm_run_vm();
}

return 0;
}
