#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "disassembler.h"

// Define the opcodes_to_asm array
typedef struct {
    int opcode;
    char *name;
    char *operands[3];
} opcode_info;

opcode_info opcodes_to_asm[] = {
    {0, "nop", {NULL}},
    {1, "add", {"R", "R", NULL}},
    {2, "add", {"R", "#", NULL}},
    {3, "sub", {"R", "R", NULL}},
    {4, "sub", {"R", "#", NULL}},
    {5, "mul", {"R", "R", NULL}},
    {6, "mul", {"R", "#", NULL}},
    {7, "div", {"R", "R", NULL}},
    {8, "div", {"R", "#", NULL}},
    {9, "jmp", {"$", NULL, NULL}},
    {10, "jmp", {"R", NULL, NULL}},
    {11, "jsr", {"$", NULL, NULL}},
    {12, "jsr", {"R", NULL, NULL}},
    {13, "ret", {NULL}},
    {14, "cmp", {"R", "#", NULL}},
    {15, "cmp", {"R", "R", NULL}},
    {16, "jz", {"$", NULL, NULL}},
    {17, "je", {"$", NULL, NULL}},
    {18, "syscall", {"$", NULL, NULL}},
    {19, "push", {"R", NULL, NULL}},
    {20, "mov", {"R", "R", NULL}},
    {21, "mov", {"R", "#", NULL}},
    {22, "mov", {"R", "$", NULL}},
    {23, "pop", {"R", NULL, NULL}},
    {24, "mov", {"R", "$", "R"}},
    {26, "jg", {"$", NULL, NULL}},
    {27, "jl", {"$", NULL, NULL}},
    {28, "mov", {"$", "R", NULL}},
    {-1,"",{NULL}}
};

uint32_t disassemble_line(uint32_t *address, char *line) {

        int32_t opcode = *address;
        int instr_len=0;
        address++;
        opcode_info *info = NULL;
        int32_t i = 0;
        while(opcodes_to_asm[i].opcode != -1) {
            if (opcodes_to_asm[i].opcode == opcode) {
                info = &opcodes_to_asm[i];
                break;
            }
            i++;
        }
        if (info == NULL) {
            return;
        }
        sprintf(line,"%s ",info->name);
        instr_len++;
        line += strlen(info->name);
        for (int i = 0; i < 3; i++) {
            char sep[1] ="";
            char tmp[20];
            if (info->operands[i] != NULL) {
                instr_len++;
                if(i==1){
                    if(info->operands[i]!=NULL)
                    sep[0] = ',';
                }
                switch (info->operands[i][0]) {
                    case 'R':
                        sprintf(tmp," R%d", *(address+i));
                        snprintf(tmp+strlen(tmp),1,"%s",sep);
                        sprintf(line,"%s",tmp);
                        line += strlen(tmp);
                    break;
                    case '#':
                        sprintf(tmp," #%d", *(address+i));
                        snprintf(tmp+strlen(tmp),1,"%s",sep);
                        sprintf(line,"%s",tmp);
                        line += strlen(tmp);
                    break;
                    case '$':
                        sprintf(tmp," %d", *(address+i));
                        snprintf(tmp+strlen(tmp),1,"%s",sep);
                        sprintf(line,"%s",tmp);
                        line += strlen(tmp);
                    break;
                }
            }
        }
        printf("\n");

    return instr_len;
}