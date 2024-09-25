# ZVM

ZVM is a custom virtual machine designed to execute bytecode.

## Overview

ZVM is a software implementation of a virtual machine that provides a platform for executing bytecode. It includes a kernel, runtime environment, and other supporting components.
Made just for fun and maybe also for learning purposes, hopefully.

ZVM "CPU" has 8 general purpose registers, R0-R7, Program Counter(PC), stack pointer(SP) and status
register. 
See kernel/instr_set.h for opcodes. No other documentation yet about instruction set.

## Features

* Custom bytecode execution
* Kernel and runtime environment
* Compiler (written in Python) that compiles custom assembler code to ZVM bytecode.

## Example assembler code

```
const print 1
const exit 2
mov R1,#0
mov R0,hello_txt
loop:
syscall print
add R1,#1
cmp R1,#5  //repeat 5 times
jl loop
syscall exit //end of program

hello_txt:
0x6c6c6548,0x5a202c6f,0x77204d56,0x646c726f,0x0
```
## TODO

* enhance compiler functionality (e.g. text data support)
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
2. Compile zvm assembler program `python3 compiler/zvm_compiler.py examples/hello.asm`
2. Run the ZVM interpreter: `build/bin/zvm examples/hello.zvm` 

## License

ZVM is licensed under MIT license.
