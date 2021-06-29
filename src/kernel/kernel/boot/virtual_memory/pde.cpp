#include "pde.h"

namespace Memory
{
    void PDE::AddAttr(uint32_t attr)
    {
        m_entry |= attr;
    }

    void PDE::DeleteAttr(uint32_t attr)
    {
        m_entry &= ~attr;
    }

    void PDE::SetFrame(uint32_t physicalAddr)
    {
        m_entry |= ((m_entry & ~FRAME) | physicalAddr);
    }

    bool PDE::IsPresent() const
    {
        return m_entry & PRESENT;
    }

    bool PDE::IsUser() const
    {
        return m_entry & USER;
    }

    bool PDE::IsUsing4Mb() const
    {
        return m_entry & USE_4MB;
    }

    bool PDE::IsWritable() const
    {
        return m_entry & WRITABLE;
    }

    uint32_t PDE::GetFrameAddr() const
    {
        return m_entry & FRAME;
    }
}