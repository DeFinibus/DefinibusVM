import re
import opcodes
import sys
import re
class VMCompiler:
    def __init__(self):
        self.opcodes = opcodes.opcodes_to_asm
        self.labels = {}

    '''
    preprocess_source function:
    In this function, we want to:
    - find addresses of labels
    - remove labels from source code
    - replace label references with their addresses
    '''
    def preprocess_source(self, program):
        # First pass: Compile labels and constants and store their addresses, remove comments
        
        address = 0
        print("Pass 1...")
        removed_lines=[]
        for instruction in program:
            if instruction=="" or instruction.startswith('#'): 
                removed_lines.append(instruction)
                continue
            if instruction.startswith("const"):
                label = instruction.split()[1] 
                value = int(instruction.split()[2])  # Allow hex, dec, or octal
                self.labels[label] = value
                removed_lines.append(instruction)
                continue
            if instruction.endswith(':'):
                label_name = instruction[:-1]
                self.labels[label_name] = address
                removed_lines.append(instruction)
            else:
                # split codeline, separators any white spaces and commas
                codeline = re.split(r'\s+|,',instruction)
                #remove comment // from codeline
                cmd_found = None
                imm_found = None
                for x in codeline:
                    if x.startswith('//'):  
                        cmd_found = x
                    if x.startswith('#'):
                        imm_found = x

                if cmd_found:
                    codeline=codeline[:codeline.index(cmd_found)]
                instr_len = len(codeline)
                program[program.index(instruction)] = codeline # replace with changes

                address += instr_len  
        # remove lines defined in removed array from program
        for i in removed_lines:
            program.remove(i)
        # second sub-pass: replace label and const references with their addresses
        print("Pass 2...")
        for instruction in program:
            for op in instruction:
                if op in self.labels:
                    prgidx = program.index(instruction)
                    opidx = instruction.index(op)
                    program[prgidx][opidx]="#"+str(self.labels[op])    
            
    
    def match(self, instruction, asm):
        mnemonic = instruction[0]
        operands = instruction[1:]

        if len(asm) == 1:
            if mnemonic == asm[0]:
                if not operands:
                    return True, None, None
        elif len(asm) == 2:
            if mnemonic == asm[0]:
                if len(operands) == 1:
                    if asm[1] == "R":
                        if operands[0].startswith("R"):
                            return True, int(operands[0][1:]), None
                        else:
                            return False, None, None
                    elif asm[1] == "#":
                        if operands[0].startswith("#"):
                            value = operands[0][1:]
                            if value.startswith("0x"):
                                return True, int(value, 16), None
                            else:
                                return True, int(value), None
                        else:
                            return False, None, None
                    elif asm[1] == "$":
                        value = operands[0]
                        if '#' in value:
                            value = value[1:]
                        if value.startswith("0x"):
                            return True, int(value, 16), None
                        else:
                            return True, int(value), None
        elif len(asm) == 3:
            if mnemonic == asm[0]:
                if len(operands) == 2:
                    if asm[1] == "R":
                        if operands[0].startswith("R"):
                            if asm[2] == "R":
                                if operands[1].startswith("R"):
                                    return True, int(operands[0][1:]), int(operands[1][1:])
                                else:
                                    return False, None, None
                            elif asm[2] == "#":
                                if operands[1].startswith("#"):
                                    value2 = operands[1][1:]
                                    if value2.startswith("0x"):
                                        value2 = int(value2, 16)
                                    else:
                                        value2 = int(value2)
                                    return True, int(operands[0][1:]), value2
                                else:
                                    return False, None, None
                            elif asm[2] == "$":
                                value2 = operands[1]
                                if value2.startswith("0x"):
                                    value2 = int(value2, 16)
                                else:
                                    value2 = int(value2)
                                return True, int(operands[0][1:]), value2
                            else:
                                return False, None, None
                        else:
                            return False, None, None
                    elif asm[1] == "#":
                        if operands[0].startswith("#"):
                            value1 = operands[0][1:]
                            if value1.startswith("0x"):
                                value1 = int(value1, 16)
                            else:
                                value1 = int(value1)
                            if asm[2] == "R":
                                if operands[1].startswith("R"):
                                    return True, value1, int(operands[1][1:])
                                else:
                                    return False, None, None
                            elif asm[2] == "#":
                                if operands[1].startswith("#"):
                                    value2 = operands[1][1:]
                                    if value2.startswith("0x"):
                                        value2 = int(value2, 16)
                                    else:
                                        value2 = int(value2)
                                    return True, value1, value2
                                else:
                                    return False, None, None
                            elif asm[2] == "$":
                                value2 = operands[1]
                                if value2.startswith("0x"):
                                    value2 = int(value2, 16)
                                else:
                                    value2 = int(value2)
                                return True, value1, value2
                            else:
                                return False, None, None
                    elif asm[1] == "$":
                        value1 = operands[0]
                        if value1.startswith("0x"):
                            value1 = int(value1, 16)
                        else:
                            value1 = int(value1)
                        if asm[2] == "R":
                            if operands[1].startswith("R"):
                                return True, value1, int(operands[1][1:])
                            else:
                                return False, None, None
                        elif asm[2] == "#":
                            if operands[1].startswith("#"):
                                value2 = operands[1][1:]
                                if value2.startswith("0x"):
                                    value2 = int(value2, 16)
                                else:
                                    value2 = int(value2)
                                return True, value1, value2
                            else:
                                return False, None, None
                        elif asm[2] == "$":
                            value2 = operands[1]
                            if value2.startswith("0x"):
                                value2 = int(value2, 16)
                            else:
                                value2 = int(value2)
                            return True, value1, value2
                        else:
                            return False, None, None
                    else:
                        return False, None, None
                else:
                    return False, None, None
            else:
                return False, None, None
        else:
            return False, None, None
        return False, None, None
    def compile_instruction(self, instruction):
        # Compile instruction
        mnemonic = instruction[0]
        operands = instruction[1:]
        instruction_bytes = []

        # Find the opcode value that corresponds to the mnemonic
        found =False
        handled = False
        num_format = 10
        print(f"Compiling instruction:{instruction}")
        for opcode, asm in self.opcodes.items():
                res, oper1, oper2=self.match(instruction,asm)
                if res:
                    found = True
                    instruction_bytes.append(opcode)
                    if oper1!=None:
                        instruction_bytes.append(oper1)
                    if oper2!=None:
                        instruction_bytes.append(oper2)
                    break #we have found match
                
  
        if not found: #check for data
            try:
                num = int(instruction[0])
                handled =True
            except ValueError:
                try:
                    num = int(instruction[0], 16)
                    num_format = 16
                    handled =True
                except ValueError:
                    pass;
            if handled:
                for b in instruction:
                    instruction_bytes.append(int(b,num_format))
        if not instruction_bytes:
            raise ValueError(f"Invalid instruction: {instruction}")

        return instruction_bytes


    def compile_program(self, program):
        # First and second pass:
        self.preprocess_source(program)
        # third pass: Compile instructions
        self.bytecode = []
        data_section = False
        for instruction in program:
                self.bytecode.extend(self.compile_instruction(instruction))
        return self.bytecode

def main():
    print("DefinibusVM compiler v0.0.0.1 (c) MAP 2024")
    print("https://github.com/DeFinibus/DefinibusVM")
    print("This program is free software, you can redistribute it and/or modify it under the terms of the MIT license.")
    print()
    if len(sys.argv) != 2:
        print("Usage: python3 vm_compiler.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = input_file.split('.')[0] + '.dvm'

    with open(input_file, 'r') as f:
        program = f.readlines()

    program = [line.strip() for line in program]
    compiler = VMCompiler()
    bytecode = compiler.compile_program(program)
    print("COMPILED CODE:")
    print(bytecode)

    with open(output_file, 'wb') as f:
        for num in bytecode:
            f.write(num.to_bytes(4, 'little'))
    print("Program saved to file:", output_file)
if __name__ == '__main__':
    main()

