#pragma once

#include <stdint.h>

namespace BootHelpers
{
    class EarlyLogStorage
    {
        static constexpr const uint8_t MAX_SIZE = 255;

    public:
        EarlyLogStorage() = default;
        ~EarlyLogStorage() = default;

        void AddError(const char* szError);
        uint8_t GetErrorCount() const;
        bool HasErrors() const;
        const char* const* GetErrors(); 

    private:
        const char* m_szErrors[MAX_SIZE] = {};
        uint8_t m_curSize = 0;
    };
}