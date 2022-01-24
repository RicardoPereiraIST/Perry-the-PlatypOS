#pragma once

#include "fsys_types.h"
#include <stdint.h>

class FileSystem
{
    static constexpr const uint8_t MAX_DEVICES = 26;
    #define DEFAULT_DEVICE 'a'

public:
    FILE OpenFile(const char* szFileName);
    void ReadFile(FILE* pFile, unsigned char* szBuffer, unsigned int length);
    void CloseFile(FILE* pFile);
    void RegisterFileSystem(FILESYSTEM* pFileSystem, unsigned int deviceID);
    void UnregisterFileSystem(FILESYSTEM* pFileSystem);
    void UnregisterFileSystemByID(unsigned int deviceID);

private:
    FILESYSTEM* m_fileSystems[MAX_DEVICES];
    unsigned int m_registeredDevices = 0;
};
