#pragma once

#include <stdint.h>

// Bios paramater block
struct BIOSPARAMATERBLOCK
{
	uint8_t			OEMName[8];
	uint16_t		BytesPerSector;
	uint8_t			SectorsPerCluster;
	uint16_t		ReservedSectors;
	uint8_t			NumberOfFats;
	uint16_t		NumDirEntries;
	uint16_t		NumSectors;
	uint8_t			Media;
	uint16_t		SectorsPerFat;
	uint16_t		SectorsPerTrack;
	uint16_t		HeadsPerCyl;
	uint32_t		HiddenSectors;
	uint32_t		LongSectors;
};

// Bios paramater block extended attributes
struct BIOSPARAMATERBLOCKEXT
{
	uint32_t			SectorsPerFat32;
	uint16_t			Flags;
	uint16_t			Version;
	uint32_t			RootCluster;
	uint16_t			InfoCluster;
	uint16_t			BackupBoot;
	uint16_t			Reserved[6];
};

// Boot sector
struct BOOTSECTOR
{
	uint8_t					Ignore[3];			//first 3 bytes are ignored
	BIOSPARAMATERBLOCK		Bpb;
	BIOSPARAMATERBLOCKEXT	BpbExt;
	uint8_t					Filler[448];		//needed to make struct 512 bytes
};
