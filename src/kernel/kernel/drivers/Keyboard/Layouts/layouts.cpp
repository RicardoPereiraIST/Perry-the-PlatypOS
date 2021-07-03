#include "layouts.h"
#include "../keyboard.h"

namespace Devices
{
    KeyLayout::KeyLayout(const int* layout, int size, LookUpElement<uint8_t, uint8_t> table[], uint32_t tableSize)
        : m_layout(layout)
        , m_size(size)
        , m_translateTable(table, tableSize)
    {}

    int KeyLayout::GetKey(int scanCode) const
    {
        if (m_layout)
        {
            if (scanCode >= 0 && scanCode < m_size)
            {
                return m_layout[scanCode];
            }
        }
        return Keyboard::Keycode::KEY_UNKNOWN;
    }

    uint8_t KeyLayout::TranslateKey(uint8_t key) const
    {
        const uint8_t* pValue = m_translateTable.Get(key);
        return pValue ? *pValue : key;
    }
}