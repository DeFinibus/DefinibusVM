/// ZVM BIOS functions, that can be called frklå¨'789+´m ZVM code
/*
bios_call_print()
expects that R0 contains the address of the string to be printed and R1 contains the length
*/
#include "vm_core.h"
#include "vm_bios.h"
extern ZVM* theVM;

/* 
Bios functions Memory map
0x0 -> reserved
0x1 -> bios_call_print()
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

bool bios_call_print()
{
    char*txt = (char*)theVM->prog_memory[theVM->REGS[EZVM_Reg_R0]];
    int32_t len = theVM->REGS[EZVM_Reg_R1];
    logging_log( txt );
    return true;
}
bool bios_call_end_program()
{
    theVM->terminate_program = true;
    return true;

}