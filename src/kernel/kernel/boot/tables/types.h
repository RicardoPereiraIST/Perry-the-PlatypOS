// Adapted from https://github.com/wichtounet/thor-os/

#pragma once

#include <stdint.h>

namespace GDT
{
    constexpr const uint8_t CODE_SELECTOR = 1;

    struct gdtr
    {
        uint16_t length;
        uint32_t pointer;
    }__attribute__((packed));

    struct gdt_descriptor_t
    {
        uint16_t limit_low          : 16;
        uint32_t base_low           : 24;
        uint8_t ac                  : 1;
        uint8_t rw                  : 1;
        uint8_t dc                  : 1;
        uint8_t ex                  : 1;
        uint8_t s                   : 1;
        uint8_t privl               : 2;
        uint8_t pr                  : 1;
        uint8_t limit_high          : 4;
        uint8_t flag0               : 1;
        uint8_t flag1               : 1;
        uint8_t sz                  : 1;
        uint8_t gr                  : 1;
        uint32_t base_high          : 8;
    }__attribute__((packed));

    static_assert(sizeof(gdt_descriptor_t) == 8, "GDT selector in long mode is 8 bytes long");
}