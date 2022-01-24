#pragma once

#include <stdint.h>
#include "../vfs/fsys_types.h"

struct DIRECTORY
{
	uint8_t   Filename[8];
	uint8_t   Ext[3];
	uint8_t   Attrib;
	uint8_t   Reserved;
	uint8_t   TimeCreatedMs;
	uint16_t  TimeCreated;
	uint16_t  DateCreated;
	uint16_t  DateLastAccessed;
	uint16_t  FirstClusterHiBytes;
	uint16_t  LastModTime;
	uint16_t  LastModDate;
	uint16_t  FirstCluster;
	uint32_t  FileSize;
};

struct MOUNT_INFO
{
	uint32_t numSectors;
	uint32_t fatOffset;
	uint32_t numRootEntries;
	uint32_t rootOffset;
	uint32_t rootSize;
	uint32_t fatSize;
	uint32_t fatEntrySize;
};

class FAT12
{
    static constexpr const uint16_t SECTOR_SIZE = 512;
    static constexpr const uint8_t MAX_FILENAME_SIZE = 11;

public:
    static void Initialize();
    static void Mount();
    static FILE Open(const char* szFileName);
    static void Close(FILE* pFile);
    static FILE Directory(const char* szDirectoryName);
    static void Read(FILE* pFile, unsigned char* szBuffer, unsigned int length);

private:
    static void ToDosFileName(const char* szFileName, char* szOutName, unsigned int outNameLength);
    static FILE OpenSubDir(FILE originalFile, const char* szFilename);

    static FILESYSTEM s_fileSystem;
    static MOUNT_INFO s_mountInfo;
    // File Allocation Table (FAT)
    static uint8_t FAT[SECTOR_SIZE * 2];
};