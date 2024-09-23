/// ZVM BIOS functions, that can be called frklå¨'789+´m ZVM code
/*
bios_call_print()
expects that R0 contains the address of the string to be printed and R1 contains the length
*/
#include "vm_core.h"
#include "vm_bios.h"
#include <stdio.h>
extern ZVM* theVM;
#include "logging.h"

/* 
Bios functions Memory map
0x0 -> reserved
0x1 -> bios_call_print()
0x2 -> bios_call_end_program()

*/
void (*bios_functions[])(void)={0,bios_call_print,bios_call_end_program};

/**
 * Call a BIOS function given by address
 *
 * @param func_addr BIOS function address
 * @return true if function was called successfully, false otherwise
 */
bool call_bios_func(int32_t func_addr)
{
    if(func_addr > sizeof(bios_functions)){
        printf("Invalid address:%d\n",func_addr);
        zvm_set_panic("Invalid BIOS function address");
        return false;
    }
    void(*func) = bios_functions[func_addr];
    if(func == 0)
    {
        printf("Invalid address:%d\n",func_addr);
        zvm_set_panic("Invalid BIOS function address");
        return false;
    }
    (*bios_functions[func_addr])();
    return true;

}
// Bios functions below

// TODO Handle endianess so that data can be always big endian. now it is host platform dependant
bool bios_call_print()
{
    uint8_t*addr = (uint8_t*)(theVM->prog_memory);
    char*txt=addr+(theVM->REGS[EZVM_Reg_R0]*sizeof(int32_t));
    printf("ZVM program:");
    logging_log( txt );
    printf("\n");
    return true;
}
bool bios_call_end_program()
{
    theVM->terminate_program = true;
    return true;

}