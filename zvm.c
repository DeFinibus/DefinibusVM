/* 
------------------------------------------------------
Implements main user interface for ZVM virtual machine
------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char *argv[])
{

zvm_init();
#ifndef DEV_DEBUG
printf("ZVM Virtual Machine (c) DeFinibus 2024.\n");
printf("This program is free software, you can redistribute it and/or modify it under the terms of the MIT license.\n");
printf("https://github.com/Definibus/ZVM\n");
printf("Version: %s\n",ZVM_VERSION);
if(argc < 2) {
    printf("Usage: zvm <zvm program name>\n");
    return 1;
}
uint32_t prog_size;
uint8_t *prog = zvm_load_program_from_file(argv[1], &prog_size);
printf("Program size: %d\n",prog_size);


#else
printf("Running in Dev debug mode, loading test program...\n");
uint32_t prog_size = sizeof(test_program);
uint8_t *prog = (uint8_t*)test_program;
logging_log("Program size: %d\n",prog_size);
#endif

if(prog != 0){
    bool status = zvm_load_program_to_memory(prog, prog_size, 0);
    printf("Program loaded to memory: %d\n",status);
    zvm_run_vm();
}

return 0;
}
