#pragma once

#include <stdint.h>

// 2 DMACs, 32 bit master & 16bit slave each having 8 channels
static constexpr const uint8_t DMA_MAX_CHANNELS = 16;
static constexpr const uint8_t DMA_CHANNELS_PER_DMAC = 8;

// DMA0 address/count registers
enum DMA0_CHANNEL_IO
{
	DMA0_CHAN0_ADDR_REG = 0,
	DMA0_CHAN0_COUNT_REG = 1,
	DMA0_CHAN1_ADDR_REG = 2,
	DMA0_CHAN1_COUNT_REG = 3,
	DMA0_CHAN2_ADDR_REG = 4,
	DMA0_CHAN2_COUNT_REG = 5,
	DMA0_CHAN3_ADDR_REG = 6,
	DMA0_CHAN3_COUNT_REG = 7,
};

// Generic DMA0 registers
enum DMA0_IO
{
	DMA0_STATUS_REG = 0x08,
	DMA0_COMMAND_REG = 0x08,
	DMA0_REQUEST_REG = 0x09,
	DMA0_CHANMASK_REG = 0x0a,
	DMA0_MODE_REG = 0x0b,
	DMA0_CLEARBYTE_FLIPFLOP_REG = 0x0c,
	DMA0_TEMP_REG = 0x0d,
	DMA0_MASTER_CLEAR_REG = 0x0d,
	DMA0_CLEAR_MASK_REG = 0x0e,
	DMA0_MASK_REG = 0x0f
};

// DMA1 address/count registers
enum DMA1_CHANNEL_IO
{
	DMA1_CHAN4_ADDR_REG = 0xc0,
	DMA1_CHAN4_COUNT_REG = 0xc2,
	DMA1_CHAN5_ADDR_REG = 0xc4,
	DMA1_CHAN5_COUNT_REG = 0xc6,
	DMA1_CHAN6_ADDR_REG = 0xc8,
	DMA1_CHAN6_COUNT_REG = 0xca,
	DMA1_CHAN7_ADDR_REG = 0xcc,
	DMA1_CHAN7_COUNT_REG = 0xce,
};

// Generic DMA1 registers
enum DMA1_IO
{
	DMA1_STATUS_REG = 0xd0,
	DMA1_COMMAND_REG = 0xd0,
	DMA1_REQUEST_REG = 0xd2,
	DMA1_CHANMASK_REG = 0xd4,
	DMA1_MODE_REG = 0xd6,
	DMA1_CLEARBYTE_FLIPFLOP_REG = 0xd8,
	DMA1_INTER_REG = 0xda,
	DMA1_UNMASK_ALL_REG = 0xdc,
	DMA1_MASK_REG = 0xde
};

// DMA External Page Registers
enum DMA0_PAGE_REG
{
	EXTRA0 = 0x80, // Also diagnostics port
	CHAN2_ADDRBYTE2 = 0x81,
	CHAN3_ADDRBYTE2 = 0x82,
	CHAN1_ADDRBYTE2 = 0x83,
	EXTRA1 = 0x84,
	EXTRA2 = 0x85,
	EXTRA3 = 0x86,
	CHAN6_ADDRBYTE2 = 0x87,
	CHAN7_ADDRBYTE2 = 0x88,
	CHAN5_ADDRBYTE2 = 0x89,
	EXTRA4 = 0x8c,
	EXTRA5 = 0x8d,
	EXTRA6 = 0x8e,
	DRAM_REFRESH = 0x8f //no longer used in new PCs
};

// DMA mode bit mask (Non MASK Values can be ORed together)
enum DMA_MODE_REG_MASK
{
	MASK_SEL = 3,

	MASK_TRA = 0xc,
	SELF_TEST = 0,
	READ_TRANSFER =4,
	WRITE_TRANSFER = 8,

	MASK_AUTO = 0x10,
	MASK_IDEC = 0x20,

	MASK = 0xc0,
	TRANSFER_ON_DEMAND= 0,
	TRANSFER_SINGLE = 0x40,
	TRANSFER_BLOCK = 0x80,
	TRANSFER_CASCADE = 0xC0
};

enum DMA_CMD_REG_MASK
{
	MEMTOMEM = 1,
	CHAN0ADDRHOLD = 2,
	ENABLE = 4,
	TIMING = 8,
	PRIORITY = 0x10,
	WRITESEL = 0x20,
	DREQ = 0x40,
	DACK = 0x80
};
