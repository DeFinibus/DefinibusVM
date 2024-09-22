#ifndef __VM_BIOS_H
#define __VM_BIOS_H

bool bios_call_print();
bool bios_call_end_program();

bool call_bios_func(int32_t func_addr);

#endif
