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
syscall exit //end of program

hello_txt:
0x6c6c6548,0x5a202c6f,0x77204d56,0x646c726f,0x0
