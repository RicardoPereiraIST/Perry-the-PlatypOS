#include "pte.h"

namespace Memory
{
    void PTE::AddAttr(uint32_t attr)
    {
        m_entry |= attr;
    }

    void PTE::DeleteAttr(uint32_t attr)
    {
        m_entry &= ~attr;
    }

    void PTE::SetFrame(uint32_t physicalAddr)
    {
        m_entry |= ((m_entry & ~FRAME) | physicalAddr);
    }

    bool PTE::IsPresent() const
    {
        return m_entry & PRESENT;
    }

    bool PTE::IsWritable() const
    {
        return m_entry & WRITABLE;
    }
    
    uint32_t PTE::GetFrameAddr() const
    {
        return m_entry & FRAME;
    }
}