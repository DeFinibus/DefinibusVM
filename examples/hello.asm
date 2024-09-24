# Hello asm

mov R1,#0
mov R0,data
loop:
syscall 1
add R1,#1
cmp R1,5
jl loop
syscall 2 //end of program

hello_txt:
0x6c6c6548,0x5a202c6f,0x77204d56,0x646c726f,0x0
