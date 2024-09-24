import opcodes
import re

class ZVMCompiler:
    def __init__(self):
        self.opcodes = opcodes.opcodes_to_asm

    def compile_instruction(self, instruction):
        match = re.match(r'(\w+)\s+(.*)', instruction)
        if not match:
            raise ValueError(f"Invalid instruction format: {instruction}")

        opcode_name = match.group(1)
        operands = match.group(2).split(',')

        # Find the opcode number
        for opcode, asm in self.opcodes.items():
            if opcode_name in asm:
                opcode_num = opcode
                break
        else:
            raise ValueError(f"Unknown opcode: {opcode_name}")

        # Handle operands
        bytecode = [opcode_num]
        asm_operands = self.opcodes[opcode_num]
        for i, operand in enumerate(operands):
            operand = operand.strip()
            if asm_operands[i + 1].startswith('#'):
                # Immediate value
                value = int(operand[1:], 0)
                bytecode.extend([0, value])  # Immediate value
            elif asm_operands[i + 1].startswith('$'):
                # Address
                value = int(operand[1:], 0)
                bytecode.extend([1, value])  # Address
            elif asm_operands[i + 1].startswith('R'):
                # Register
                value = int(operand[1:])
                bytecode.extend([2, value])  # Register
            else:
                raise ValueError(f"Invalid operand: {operand}")

        return bytecode

    def compile_program(self, program):
        bytecode = []

        for instruction in program:
            bytecode.extend(self.compile_instruction(instruction))

        return bytecode

# Example usage
compiler = ZVMCompiler()

program = [
    'add R0, #$4',
    'mov #$10, R1',
    'jmp $20'
]

bytecode = compiler.compile_program(program)

print(bytecode)