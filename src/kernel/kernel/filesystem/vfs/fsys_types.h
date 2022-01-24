#pragma once

#include <stdint.h>

struct FILE
{
	char        name[32];
	uint32_t    flags;
	uint32_t    fileLength;
	uint32_t    id;
	uint32_t    eof;
	uint32_t    position;
	uint32_t    currentCluster;
	uint32_t    deviceID;
};

struct FILESYSTEM
{
	char Name [8];
	FILE               (*Directory)  (const char* szDirectoryName);
	void	           (*Mount)      ();
	void               (*Read)       (FILE* pFile, unsigned char* szBuffer, unsigned int length);
	void	           (*Close)      (FILE* pFile);
	FILE               (*Open)       (const char* szFileName);
};

enum FileFlags
{
    File = 0,
    Directory,
    Invalid
};
