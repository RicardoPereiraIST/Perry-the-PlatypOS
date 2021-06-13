#pragma once

#include <stdint.h>

namespace TSS
{
    struct task_state_segment_t
    {
        uint16_t link;
        uint16_t reserved_0;
        uint32_t esp_0;
        uint16_t ss0;
        uint16_t reserved_1;
        uint32_t esp_1;
        uint16_t ss1;
        uint16_t reserved_2;
        uint32_t esp_2;
        uint16_t ss2;
        uint16_t reserved_3;
        uint32_t cr3;
        uint32_t eip;
        uint32_t e_flags;
        uint32_t eax;
        uint32_t ecx;
        uint32_t edx;
        uint32_t ebx;
        uint32_t esp;
        uint32_t ebp;
        uint32_t esi;
        uint32_t edi;
        uint16_t es;
        uint16_t reserved_4;
        uint16_t cs;
        uint16_t reserved_5;
        uint16_t ss;
        uint16_t reserved_6;
        uint16_t ds;
        uint16_t reserved_7;
        uint16_t fs;
        uint16_t reserved_8;
        uint16_t gs;
        uint16_t reserved_9;
        uint16_t ldtr;
        uint16_t reserved_10;
        uint16_t reserved_11;
        uint16_t iopb_offset;
    };

    static_assert(sizeof(task_state_segment_t) == 104, "Task State Segment is 104 bytes long");
}