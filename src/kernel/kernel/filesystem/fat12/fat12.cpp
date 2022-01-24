#include "fat12.h"
#include "../../boot/utils/globals.h"
#include "../vfs/bpb.h"
#include <string.h>
#include <cctype>

FILESYSTEM FAT12::s_fileSystem;
MOUNT_INFO FAT12::s_mountInfo;
uint8_t FAT12::FAT[SECTOR_SIZE * 2];

void FAT12::Initialize()
{
    strcpy(s_fileSystem.Name, "FAT12");
	s_fileSystem.Directory = Directory;
	s_fileSystem.Mount = Mount;
	s_fileSystem.Open = Open;
	s_fileSystem.Read = Read;
	s_fileSystem.Close = Close;

	s_globals.VFS().RegisterFileSystem(&s_fileSystem, 0);

	Mount();   
}

void FAT12::Mount()
{
    // Boot sector info
	BOOTSECTOR* bootsector = (BOOTSECTOR*) s_globals.FloppyDisk().ReadSector(0);

	s_mountInfo.numSectors = bootsector->Bpb.NumSectors;
	s_mountInfo.fatOffset = 1;
	s_mountInfo.fatSize = bootsector->Bpb.SectorsPerFat;
	s_mountInfo.fatEntrySize = 8;
	s_mountInfo.numRootEntries = bootsector->Bpb.NumDirEntries;
	s_mountInfo.rootOffset = (bootsector->Bpb.NumberOfFats * bootsector->Bpb.SectorsPerFat) + 1;
	s_mountInfo.rootSize = ( bootsector->Bpb.NumDirEntries * 32 ) / bootsector->Bpb.BytesPerSector;
}

FILE FAT12::Open(const char* szFileName)
{
    FILE curDirectory;
	bool rootDir = true;

    char* szPath = (char*) szFileName;
	char* szAnyFolders = strchr(szPath, '\\');

	if (!szAnyFolders)
    {
		// Must be in root directory, search it
		curDirectory = Directory(szPath);

		if (curDirectory.flags == FileFlags::File)
        {
			return curDirectory;
        }

		FILE ret;
		ret.flags = FileFlags::Invalid;
		return ret;
	}

	// Go to next character after first '\'
	++szAnyFolders;

	while (szAnyFolders)
    {
		char szPathname[16];
		int i = 0;
		for (i = 0; i < 16; ++i)
        {
			// If another '\' or end of line is reached, we are done
			if (szAnyFolders[i] == '\\' || szAnyFolders[i] == '\0')
            {
				break;
            }

			szPathname[i] = szAnyFolders[i];
		}

		szPathname[i] = 0;

		// Open subdirectory or file
		if (rootDir)
        {
			// Search root directory - open pathname
			curDirectory = Directory(szPathname);
			rootDir = false;
		}
		else
        {
			// Search a subdirectory instead for pathname
			curDirectory = OpenSubDir(curDirectory, szPathname);
		}

		if (curDirectory.flags == FileFlags::Invalid)
        {
			break;
        }

		if (curDirectory.flags == FileFlags::File)
        {
			return curDirectory;
        }

		// find next '\'
		szAnyFolders = strchr(szAnyFolders + 1, '\\');
		if (szAnyFolders)
        {
			++szAnyFolders;
        }
	}

	FILE ret;
	ret.flags = FileFlags::Invalid;
	return ret;
}

void FAT12::Close(FILE* pFile)
{
    if (pFile)
    {
		pFile->flags = FileFlags::Invalid;
    }
}

FILE FAT12::Directory(const char* szDirectoryName)
{
    FILE file;
    unsigned char* szBuf;
    DIRECTORY* directory;

    char szDosFileName[MAX_FILENAME_SIZE];
    ToDosFileName(szDirectoryName, szDosFileName, MAX_FILENAME_SIZE);
    szDosFileName[MAX_FILENAME_SIZE - 1] = 0;

    // 14 sectors per directory
	for (int sector = 0; sector < 14; ++sector)
    {
		// Read in sector of root directory
		szBuf = (unsigned char*) s_globals.FloppyDisk().ReadSector(s_mountInfo.rootOffset + sector);

		// Get directory info
		directory = (DIRECTORY*) szBuf;

		// 16 entries per sector
		for (int i = 0; i < 16; ++i)
        {
			// get current filename
			char szName[MAX_FILENAME_SIZE];
			memcpy(szName, directory->Filename, MAX_FILENAME_SIZE);
			szName[MAX_FILENAME_SIZE - 1] = 0;

			if (strncmp(szDosFileName, szName, MAX_FILENAME_SIZE) == 0)
            {
				strcpy(file.name, szDirectoryName);
				file.id = 0;
				file.currentCluster = directory->FirstCluster;
				file.fileLength = directory->FileSize;
				file.eof = 0;
				file.fileLength = directory->FileSize;

				if (directory->Attrib == 0x10)
                {
					file.flags = FileFlags::Directory;
                }
				else
                {
					file.flags = FileFlags::File;
                }

				return file;
			}

			directory++;
		}
	}

	file.flags = FileFlags::Invalid;
	return file;
}

void FAT12::Read(FILE* pFile, unsigned char* szBuffer, unsigned int length)
{
	(void) length;

    if (pFile)
    {
		// Starting physical sector
		unsigned int physSector = 32 + (pFile->currentCluster - 1);

		//! read in sector
		unsigned char* sector = (unsigned char*) s_globals.FloppyDisk().ReadSector(physSector);

		memcpy(szBuffer, sector, SECTOR_SIZE);

		// locate FAT sector
		unsigned int FATOffset = pFile->currentCluster + (pFile->currentCluster / 2); // multiply by 1.5
		unsigned int FATSector = 1 + (FATOffset / SECTOR_SIZE);
		unsigned int entryOffset = FATOffset % SECTOR_SIZE;

		// Read 1st FAT sector
		sector = (unsigned char*) s_globals.FloppyDisk().ReadSector(FATSector);
		memcpy(FAT, sector, SECTOR_SIZE);

		//! read 2nd FAT sector
		sector = (unsigned char*) s_globals.FloppyDisk().ReadSector(FATSector + 1);
		memcpy(FAT + SECTOR_SIZE, sector, SECTOR_SIZE);

		// Read entry for next cluster
		uint16_t nextCluster = *(uint16_t*) &FAT[entryOffset];

		// Test if entry is odd or even
		if(pFile->currentCluster & 0x0001)
        {
			nextCluster >>= 4;      //grab high 12 bits
        }
		else
        {
			nextCluster &= 0x0FFF;   //grab low 12 bits
        }

		// Test for end of file
		if (nextCluster >= 0xff8)
        {
			pFile->eof = 1;
			return;
		}

		// Test for file corruption
		if (nextCluster == 0)
        {
			pFile->eof = 1;
			return;
		}

		pFile->currentCluster = nextCluster;
	}
}

void FAT12::ToDosFileName(const char* szFileName, char* szOutName, unsigned int outNameLength)
{
    unsigned int i = 0;
    if (outNameLength > MAX_FILENAME_SIZE)
    {
        return;
    }

    if (!szOutName || !szFileName)
    {
        return;
    }

    memset(szOutName, ' ', outNameLength);

    const size_t fileNameLength = strlen(szFileName) - 1;
    for (i = 0; i <  fileNameLength && i < outNameLength; ++i)
    {
        if (szFileName[i] == '.' || i == 8)
        {
            break;
        }

        szOutName[i] = toupper(szFileName[i]);
    }

    // extension
    if (szFileName[i] == '.')
    {
        for (int j = 0; j < 3; ++j)
        {
            ++i;
            if (szFileName[i])
            {
                szOutName[8 + j] = szFileName[i];
            }
        }
    }

    for (i = 0; i < 3; ++i)
    {
		szOutName[8 + i] = toupper(szOutName[8 + i]);
    }
}

FILE FAT12::OpenSubDir(FILE originalFile, const char* szFilename)
{
    FILE file;

	char szDosFileName[MAX_FILENAME_SIZE];
	ToDosFileName(szFilename, szDosFileName, MAX_FILENAME_SIZE);
	szDosFileName[MAX_FILENAME_SIZE - 1] = 0;

	// Read directory
	while (!originalFile.eof)
    {
		// Read directory
		unsigned char buf[SECTOR_SIZE];
		Read(&file, buf, SECTOR_SIZE);

		// Set directory
		DIRECTORY* pDir = (DIRECTORY*) buf;

		//! 16 entries in buffer
		for (unsigned int i = 0; i < 16; ++i)
        {
			// Get current filename
			char szName[MAX_FILENAME_SIZE];
			memcpy(szName, pDir->Filename, MAX_FILENAME_SIZE);
			szName[MAX_FILENAME_SIZE - 1] = 0;

			if (strncmp(szName, szDosFileName, MAX_FILENAME_SIZE) == 0)
            {
				strcpy(file.name, szFilename);
				file.id = 0;
				file.currentCluster = pDir->FirstCluster;
				file.fileLength = pDir->FileSize;
				file.eof = 0;
				file.fileLength = pDir->FileSize;

				if (pDir->Attrib == 0x10)
                {
					file.flags = FileFlags::Directory;
                }
				else
                {
					file.flags = FileFlags::File;
                }

				return file;
			}

			pDir++;
		}
	}

	file.flags = FileFlags::Invalid;
	return file;
}
