#pragma once

#include <stdint.h>

struct Registers
{
    uint32_t __ignored, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_no;
    uint32_t eip, cs, efl, useresp, ss;
};