#pragma once

#include <stdint.h>
#include "../../../boot/utils/helpers.h"

namespace Devices
{
    // TODO: Very hacky and terrible design
    template<typename TKey, typename TValue>
    struct LookUpElement
    {
        LookUpElement(const TKey& key, const TValue& value)
            : m_key(key)
            , m_value(value)
        {}

        TKey m_key;
        TValue m_value;
    };

    template<typename TKey, typename TValue>
    class LookupTable
    {
        using Element = LookUpElement<TKey, TValue>;

    public:
        LookupTable(LookUpElement<TKey, TValue> elements[], uint32_t size)
            : m_elements(elements)
            , m_size(size)
        {}

        bool Exists(const TKey& key) const
        {
            for (uint32_t i = 0; i < m_size; ++i)
            {
                const Element& el = m_elements[i];
                if (el.m_key == key)
                {
                    return true;
                }
            }
            return false;
        }

        const TValue* Get(const TKey& key) const
        {
            if (m_elements)
            {
                for (uint32_t i = 0; i < m_size; ++i)
                {
                    const Element& el = m_elements[i];
                    if (el.m_key == key)
                    {
                        return &el.m_value;
                    }
                }
            }
            return nullptr;
        }

    private:
        LookUpElement<TKey, TValue>* m_elements;
        const uint32_t m_size;
    };

    class KeyLayout
    {
        using LUTable = LookupTable<uint8_t, uint8_t>;

    public:
        KeyLayout(const int* layout, int size, LookUpElement<uint8_t, uint8_t> table[], uint32_t tableSize);

        int GetKey(int scanCode) const;
        uint8_t TranslateKey(uint8_t key) const;

    private:
        const int* m_layout;
        const int m_size;
        LUTable m_translateTable;
    };
}