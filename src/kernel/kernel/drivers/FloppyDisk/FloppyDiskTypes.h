#pragma once

static constexpr const int SECTORS_PER_TRACK = 18;

// Controller I/O Ports. Please see chapter for additional ports
enum FLPYDSK_IO
{
    DOR		=	0x3f2,
    MSR		=	0x3f4,
    FIFO	=	0x3f5,
    CTRL	=	0x3f7
};

// Bits 0-4 of command byte. Please see chapter for additional commands
enum FLPYDSK_CMD
{
    FDC_READ_TRACK	 =	2,
    FDC_SPECIFY		 =	3,
    FDC_CHECK_STAT	 =	4,
    FDC_WRITE_SECT	 =	5,
    FDC_READ_SECT	 =	6,
    FDC_CALIBRATE	 =	7,
    FDC_CHECK_INT	 =	8,
    FDC_FORMAT_TRACK =	0xd,
    FDC_SEEK		 =	0xf
};

// Additional command masks. Can be masked with above commands
enum FLPYDSK_CMD_EXT
{
    FDC_SKIP		=	0x20,	//00100000
    FDC_DENSITY		=	0x40,	//01000000
    FDC_MULTITRACK	=	0x80	//10000000
};

// Digital Output Register
enum FLPYDSK_DOR_MASK
{
    DOR_DRIVE0			=	0,	//00000000	= here for completeness sake
    DOR_DRIVE1			=	1,	//00000001
    DOR_DRIVE2			=	2,	//00000010
    DOR_DRIVE3			=	3,	//00000011
    RESET			    =	4,	//00000100
    DOR_DMA			    =	8,	//00001000
    DRIVE0_MOTOR	    =	16,	//00010000
    DRIVE1_MOTOR	    =	32,	//00100000
    DRIVE2_MOTOR	    =	64,	//01000000
    DRIVE3_MOTOR	    =	128	//10000000
};

// Main Status Register
enum FLPYDSK_MSR_MASK
{
    DRIVE1_POS_MODE	=	1,	//00000001
    DRIVE2_POS_MODE	=	2,	//00000010
    DRIVE3_POS_MODE	=	4,	//00000100
    DRIVE4_POS_MODE	=	8,	//00001000
    BUSY			=	16,	//00010000
    MSR_DMA			=	32,	//00100000
    DATAIO			=	64, //01000000
    DATAREG			=	128	//10000000
};

// Controller Status Port 0
enum FLPYDSK_ST0_MASK
{
    ST0_DRIVE0		    =	0,		//00000000	=	for completness sake
    ST0_DRIVE1		    =	1,		//00000001
    ST0_DRIVE2		    =	2,		//00000010
    ST0_DRIVE3		    =	3,		//00000011
    HEADACTIVE  	    =	4,		//00000100
    ST0_NOTREADY	    =	8,		//00001000
    UNITCHECK	        =	16,		//00010000
    SEEKEND	            =	32,		//00100000
    INTCODE	            =	64		//11000000
};

// LPYDSK_ST0_MASK_INTCODE types
enum FLPYDSK_ST0_INTCODE_TYP
{
    NORMAL		    =	0,
    ABNORMAL_ERR    =	1,
    INVALID_ERR	    =	2,
    INT_NOTREADY    =	3
};

// GAP 3 sizes
enum FLPYDSK_GAP3_LENGTH
{
    LEN_STD  = 42,
    LEN_5_14 = 32,
    LEN_3_5  = 27
};

// Formula: 2^sector_number * 128
enum FLPYDSK_SECTOR_DTL
{
    DTL_128	=	0,
    DTL_256	=	1,
    DTL_512	=	2,
    DTL_1024	=	4
};