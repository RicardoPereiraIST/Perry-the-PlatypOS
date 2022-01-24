#include "fsys.h"

FILE FileSystem::OpenFile(const char* szFileName)
{
    if (szFileName && *szFileName)
    {
        unsigned char device = DEFAULT_DEVICE;

        if (szFileName[1] == ':')
        {
            device = szFileName[0];
            szFileName += 3; // strip it from pathname
        }

        if (FILESYSTEM* pFileSystem = m_fileSystems[device - DEFAULT_DEVICE])
        {
            FILE file = pFileSystem->Open(szFileName);
            file.deviceID = device;
            return file;
        }
    }

    FILE file;
    file.flags = FileFlags::Invalid;
    return file;
}

void FileSystem::ReadFile(FILE* pFile, unsigned char* szBuffer, unsigned int length)
{
    if (pFile)
    {
        if (FILESYSTEM* pFileSystem = m_fileSystems[pFile->deviceID - DEFAULT_DEVICE])
        {
            pFileSystem->Read(pFile, szBuffer, length);
        }
    }
}

void FileSystem::CloseFile(FILE* pFile)
{
    if (pFile)
    {
        if (FILESYSTEM* pFileSystem = m_fileSystems[pFile->deviceID - DEFAULT_DEVICE])
        {
            pFileSystem->Close(pFile);
        }
    }
}

void FileSystem::RegisterFileSystem(FILESYSTEM* pFileSystem, unsigned int deviceID)
{
    if (m_registeredDevices < MAX_DEVICES)
    {
        if (pFileSystem && !m_fileSystems[deviceID])
        {
            m_fileSystems[deviceID] = pFileSystem;
            ++m_registeredDevices;
        }
    }
}

void FileSystem::UnregisterFileSystem(FILESYSTEM* pFileSystem)
{
    for (int i = 0; i < MAX_DEVICES; ++i)
    {
        if (m_fileSystems[i] == pFileSystem)
        {
            m_fileSystems[i] = nullptr;
            --m_registeredDevices;
        }
    }
}

void FileSystem::UnregisterFileSystemByID(unsigned int deviceID)
{
    if (deviceID < MAX_DEVICES)
    {
        if (m_fileSystems[deviceID])
        {
            m_fileSystems[deviceID] = nullptr;
            --m_registeredDevices;
        }
    }
}
