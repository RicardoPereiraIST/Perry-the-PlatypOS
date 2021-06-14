#include "early_errors_storage.h"
#include <string.h>

namespace BootHelpers
{
    void EarlyLogStorage::AddError(const char* szError)
    {
        constexpr const char* szErrorLimit = "Too many errors were reported.";

        const char* szErrorToCopy = szError;

        if (m_curSize >= MAX_SIZE)
        {
            return;
        }
        
        if (m_curSize == (MAX_SIZE - 1))
        {
            szErrorToCopy = szErrorLimit;
        }
        
        // Todo: uncomment when memory is implemented
        // const uint32_t len = strlen(szErrorToCopy);
        // m_szErrors[m_curSize] = (const char*)malloc(len);
        // memcpy((void*)m_szErrors[m_curSize], szErrorToCopy, strlen(szErrorToCopy));

        ++m_curSize;
    }

    uint8_t EarlyLogStorage::GetErrorCount() const
    {
        return m_curSize;
    }

    bool EarlyLogStorage::HasErrors() const
    {
        return m_curSize > 0;
    }

    const char* const* EarlyLogStorage::GetErrors()
    {
        return m_szErrors;
    }
}