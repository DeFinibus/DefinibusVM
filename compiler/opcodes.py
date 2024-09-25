# Opcodes to asm dict. In the dict, first is the actual opcode (int) and then the in the inner
# dict, the first value is the mnemonic (str) and then there can be 0 to 2 operands.
# operand Rx and Ry means that operand is Register 0-7, # means that operand is immediate value,
# $ means that operand is address
opcodes_to_asm ={
    0:["nop"],
    1:["add","R","R"],
    2:["add","R","#"],
    3:["sub","R","R"],
    4:["sub","R","#"],
    5:["mul","R","R"],
    6:["mul","R","#"],
    7:["div","R","R"],
    8:["div","R","#"],
    9:["jmp","$"],
    10:["jmp","R"],
    11:["jsr","$"],
    12:["jsr","R"],
    13:["ret"],
    14:["cmp","R","#"],
    15:["cmp","R","R"],
    16:["jz","$"],
    17:["je","$"],
    18:["syscall","$"],
    19:["push","R"],
    20:["mov","R","R"],
    21:["mov","R","#"],
    22:["mov","R","$"],
    23:["pop","R"],
    24:["mov","R","$+R"],
    26:["jg","$"],
    27:["jl","$"],
    28:["mov","$","R"]

}