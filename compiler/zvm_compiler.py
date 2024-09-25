import re
import opcodes
import sys
import re
class ZVMCompiler:
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
                    print("codeline after cmd remove:",codeline)
                instr_len = len(codeline)
                program[program.index(instruction)] = codeline # replace with changes

                address += instr_len  
        # remove lines defined in removed array from program
        for i in removed_lines:
            print("removing:",i)
            program.remove(i)
        # second sub-pass: replace label and const references with their addresses
        print("Pass 2...")
        for instruction in program:
            for op in instruction:
                if op in self.labels:
                    prgidx = program.index(instruction)
                    opidx = instruction.index(op)
                    program[prgidx][opidx]=self.labels[op]    
            
        for i in program:
            print(i)
    def compile_instruction(self, instruction):
        # Compile instruction
        mnemonic = instruction[0]
        operands = instruction[1:]
        instruction_bytes = []

        # Find the opcode value that corresponds to the mnemonic
        for opcode, asm in self.opcodes.items():
            if mnemonic in asm:
                # Check if the operand types match
                operand_match = True
                asm_operands = list(asm - {mnemonic})
                for operand, operand_type in zip(operands, asm_operands):
                    if operand_type == "Rx" and not operand.startswith("R"):
                        operand_match = False
                        break
                    elif operand_type == "#":
                        if not operand.startswith("#"):
                            operand_match = False
                            break
                        try:
                            int(operand[1:], 0)
                        except ValueError:
                            operand_match = False
                            break
                    elif operand_type == "$":
                        if not operand.startswith("$"):
                            operand_match = False
                            break
                        try:
                            int(operand[1:], 0)
                        except ValueError:
                            operand_match = False
                            break

                if operand_match and len(operands) == len(asm_operands):
                    # Add the opcode value to the instruction bytes
                    instruction_bytes.append(opcode)

                    # Compile the operands
                    for operand, operand_type in zip(operands, asm_operands):
                        if operand_type == "Rx":
                            # Register operand
                            register_number = int(operand[1:])
                            instruction_bytes.append(register_number)
                        elif operand_type == "#":
                            # Immediate operand
                            immediate_value = int(operand[1:], 0)
                            instruction_bytes.append(immediate_value)
                        elif operand_type == "$":
                            # Address operand
                            address_value = int(operand[1:], 0)
                            instruction_bytes.append(address_value)

                    break

        if not instruction_bytes:
            raise ValueError(f"Invalid instruction: {instruction}")

        return instruction_bytes


    def compile_program(self, program):
        # First and second pass:
        self.preprocess_source(program)
        print("Labels")
        print(self.labels)
        # third pass: Compile instructions
        self.bytecode = []
        data_section = False
        for instruction in program:
                self.bytecode.extend(self.compile_instruction(instruction))
        return self.bytecode

def main():
    if len(sys.argv) != 2:
        print("Usage: python zvm_compiler.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = input_file.split('.')[0] + '.zvm'

    with open(input_file, 'r') as f:
        program = f.readlines()

    program = [line.strip() for line in program]
    compiler = ZVMCompiler()
    bytecode = compiler.compile_program(program)

    with open(output_file, 'wb') as f:
        for byte in bytecode:
            f.write(byte.to_bytes(4, 'little'))

if __name__ == '__main__':
    main()