#ifndef __INSTR__SET_H
#define __INSTR__SET_H

typedef enum {
    EZVM_NOP,
    EZVM_ADD, // add Rx,Ry: add Ry to Rx. result in Rx. Sets flags
    EZVM_ADDI, //add Rx, #val, add #val to RX, result in Rx. Sets flags
    EZVM_SUB, //syntax sub Rx,Ry. substract Ry from rx, result in rx. 
    EZVM_SUBI, //syntax sub Rx,#val. substract #val from rx, result in rx. Sets flags
    EZVM_MUL, // syntax mul Rx,Ry. Multiply rx by ry, result in Rx. Sets flags
    EZVM_MULI, // syntax mul Rx,#val. Multiply rx by #val, result in Rx. Sets flags
    EZVM_DIV, // syntax div Rx,Ry. Divide rx by ry, result in Rx. Sets flags
    EZVM_DIVI, // syntax divi Rx,#val. Divide rx by #val, result in Rx. Sets flags
    EZVM_JMPI,  // syntax jmp #addr. Jumps to address in program memory
    EZVM_JMPR, // Syntax jmp Rx. jump to address loaded to Rx
    EZVM_JSR, // Syntax jsr #addr. jump to subroutine at address in program memory
    EZVM_JSRR, // Syntax jsr Rx. jump to subroutine at address loaded to Rx
    EZVM_RET, // syntax ret. return from subroutine
    EZVM_CMPI, // syntax cmp Rx,#val. compare Rx with #val and sets flags
    EZVM_CMPR, // syntax cmp Rx,Ry. compare Rx with Ry and sets flags
    EZVM_JZ, // syntax jz #addr. jump to address in program memory if reult vas zero
    EZVM_JE, // syntax je #addr. jump to address in program memory if result vas equal
    EZVM_SYSCALL, //SYSCALL #addr. addr parameter
    EZVM_PUSH, //syntax push Rx. pushes Rx to stack
    EZVM_MOV, //syntax mov Rx,Ry. Rx = Ry
    EZVM_MOVI, //syntax mov Rx,#val. Rx = #val
    EZM_MOVM, // syntax mov Rx,$addr. Rx = memory[$addr]
    EZVM_POP, //syntax pop Rx. pops Rx from stack
    EZVM_MOVR, // syntax mov Rx,$addr+Ry 
    EZVM_CMP, //syntax CMP Rx,#val. compare Rx to value. sets flags
    EZVM_JG, // syntax jg #addr. jump to address in program memory if result vas greater than 0
    EZVM_OPCODEMAX
}EZVM_InstrSet;

//SYSCALL: bios functions are mapped to addresses in ZVM. 

#endif

