
# Opcodes to asm dict. In the dict, first is the actual opcode (int) and then the in the inner
# dict, the first value is the mnemonic (str) and then there can be 0 to 2 operands.
# operand Rx and Ry means that operand is Register 0-7, # means that operand is immediate value,
# $ means that operand is address
opcodes_to_asm ={
    0:{"nop"},
    1:{"add","Rx","Ry"},
    2:{"add","Rx","#"},
    3:{"sub","Rx","Ry"},
    4:{"sub","Rx","#"},
    5:{"mul","Rx","Ry"},
    6:{"mul","Rx","#"},
    7:{"div","Rx","Ry"},
    8:{"div","Rx","#"},
    9:{"jmp","$"},
    10:{"jmp","Rx"},
    11:{"jsr","$"},
    12:{"jsr","Rx"},
    13:{"ret"},
    14:{"cmp","Rx","#"},
    15:{"cmp","Rx","Ry"},
    16:{"jz","$"},
    17:{"je","$"},
    18:{"syscall","$"},
    19:{"push","Rx"},
    20:{"mov","Rx","Ry"},
    21:{"mov","Rx","#"},
    22:{"mov","Rx","$"},
    23:{"pop","Rx"},
    24:{"mov","Rx","$addr+Ry"},
    26:{"jg","$"},
    27:{"jl","$"},
    28:{"mov","$","Rx"}

}