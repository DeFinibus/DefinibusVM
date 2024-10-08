# VM

DefinibusVM is a custom virtual machine designed to execute bytecode.

## Overview

DefinibusVM is a software implementation of a virtual machine that provides a platform for executing bytecode. It includes a kernel, runtime environment, and other supporting components.
Made just for fun and maybe also for learning purposes, hopefully.

VM "CPU" has 8 general purpose registers, R0-R7, Program Counter(PC), stack pointer(SP) and status
register. 
See kernel/instr_set.h for opcodes. No other documentation yet about instruction set.

## Features

* Custom bytecode execution
* Kernel and runtime environment
  * continuous or single step running mode
  * memory dump with disassembly in single step mode
* Compiler (written in Python) that compiles custom assembler code to VM bytecode.

## Example assembler code

```
# Hello asm

const print 1
const exit 2

mov R1,#0
mov R0,hello_txt
loop:
syscall print
add R1,#1
cmp R1,#5  //repeat 5 times
jl loop
jsr sub_routine
syscall exit //end of program

sub_routine:
mov R0,hello2_txt
syscall print
ret

hello_txt:
0x6c6c6548,0x5a202c6f,0x77204d56,0x646c726f,0x0
hello2_txt:
1819043176,1919295599,1931505007,1869767285,1852404853,101,0

```
## TODO

* enhance compiler functionality (e.g. text data support)
* Better debugger
* Disassembler
* VM: support for more instructions
* VM: multitasking/threading(!)
- ---

## Building and Running

Currently x86 Linux environment supported only.

### Required dependencies:

* build-essential package
* GNU make
* Python 3


### Usage

1. Build the project: `make`
2. Compile vm assembler program `python3 compiler/vm_compiler.py examples/hello.asm`
3. Run the VM : `build/bin/dfbvm examples/hello.dvm` 
4. Run in debug mode: `build/bin/dfbvm examples/hello.dvm --debug`

Debug mode enables single stepping of instructions and memory dump with disassembly.
memory dump can be invoked with following command: `mem [addr_start] [addr_end]`

## License

DefinibusVM is licensed under MIT license.
