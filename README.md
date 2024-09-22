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
* [Other features, e.g. memory management, threading, etc.]

## Building and Running

Currently x86 Linux environment supported only.

1. Build the project: `make`
2. Run the ZVM interpreter: `./zvm` in build/bin -directory


## License

ZVM is licensed under MIT license.
