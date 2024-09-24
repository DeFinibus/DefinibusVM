import re
import opcodes
import sys
import re
class ZVMCompiler:
    def __init__(self):
        self.opcodes = opcodes.opcodes_to_asm
        self.labels = {}
        self.data_labels = {}

    def compile_labels(self, program):
        # First pass: Compile labels and store their addresses
        address = 0
        for instruction in program:
            if instruction=="" or instruction.startswith('#'): 
                continue
            if instruction.endswith(':'):
                label_name = instruction[:-1]
                self.labels[label_name] = address
            else:
                codeline = re.split(' |,',instruction)
                #remove comment // from codeline
                found = None
                for x in codeline:
                    if x.startswith('//'):
                        found = x
                if found:
                    codeline=codeline[:codeline.index(found)]
                instr_len = len(codeline)

                address += instr_len  

    def compile_instruction(self, instruction):
        # Compile instruction
        opcode, *operands = instruction.split()

        # Find opcode value
        for opcode_value, asm_instructions in self.opcodes.items():
            if opcode in asm_instructions:
                break
        else:
            raise ValueError(f"Invalid opcode: {opcode}")

        # Compile operands
        compiled_operands = []
        if opcode in ['mov', 'add', 'sub', 'mul', 'div', 'cmp']:
            # Handle instructions with two operands
            if len(operands) != 2:
                raise ValueError(f"Invalid number of operands({len(operands)}) for {opcode} instruction")
            for operand in operands:
                if ',' in operand:
                    # Handle register and immediate value
                    register_name, immediate_value = operand.split(',')
                    if register_name.upper() not in ['R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7']:
                        raise ValueError(f"Invalid register: {register_name}")
                    register_value = ['R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7'].index(register_name.upper())
                    compiled_operands.append(register_value)
                    if immediate_value.startswith('#'):
                        # Immediate value
                        value = int(immediate_value[1:], 0)  # Allow hex, dec, or octal
                        compiled_operands.append(value)
                    else:
                        raise ValueError(f"Invalid immediate value: {immediate_value}")
                elif operand.startswith('#'):
                    # Immediate value
                    value = int(operand[1:], 0)  # Allow hex, dec, or octal
                    compiled_operands.append(value)
                elif operand in self.labels:
                    # Label reference
                    label_address = self.labels[operand]
                    compiled_operands.append(label_address)
                else:
                    # Register reference
                    register_name = operand.upper()
                    if register_name not in ['R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7']:
                        raise ValueError(f"Invalid register: {register_name}")
                    register_value = ['R0', 'R1', 'R2', 'R3', 'R4', 'R5', 'R6', 'R7'].index(register_name)
                    compiled_operands.append(register_value)

            # Assemble instruction
            instruction_bytes = [opcode_value] + compiled_operands

            return instruction_bytes

    def compile_program(self, program):
        # First pass: Compile labels
        self.compile_labels(program)
        print("Labels")
        print(self.labels)
        # Second pass: Compile instructions
        self.bytecode = []
        data_section = False
        for instruction in program:
            if instruction.startswith('data:'):
                data_section = True
            elif data_section:
                # Data section
                value = int(instruction, 0)  # Allow hex, dec, or octal
                self.bytecode.append(value)
            elif not instruction.endswith(':'):
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