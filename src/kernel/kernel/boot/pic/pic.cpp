#include "pic.h"
#include "../utils/helpers.h"

// PIC 1 register port addresses
static constexpr const uint8_t PIC1_REG_COMMAND = 0x20;
static constexpr const uint8_t PIC1_REG_STATUS = 0x20;
static constexpr const uint8_t PIC1_REG_DATA = 0x21;
static constexpr const uint8_t PIC1_REG_IMR = 0x21;

// PIC 2 register port addresses
static constexpr const uint8_t PIC2_REG_COMMAND = 0xA0;
static constexpr const uint8_t PIC2_REG_STATUS = 0xA0;
static constexpr const uint8_t PIC2_REG_DATA = 0xA1;
static constexpr const uint8_t PIC2_REG_IMR = 0xA1;

//-----------------------------------------------
//	Initialization Command Bit Masks
//-----------------------------------------------

// Initialization Control Word 1 bit masks
static constexpr const uint8_t ICW1_MASK_IC4 = 0x1;			    //00000001
static constexpr const uint8_t ICW1_MASK_SNGL = 0x2;			//00000010
static constexpr const uint8_t ICW1_MASK_ADI = 0x4;			    //00000100
static constexpr const uint8_t ICW1_MASK_LTIM = 0x8;			//00001000
static constexpr const uint8_t ICW1_MASK_INIT = 0x10;		    //00010000

// Initialization Control Words 2 and 3 do not require bit masks

// Initialization Control Word 4 bit masks
static constexpr const uint8_t ICW4_MASK_UPM = 0x1;			    //00000001
static constexpr const uint8_t ICW4_MASK_AEOI = 0x2;			//00000010
static constexpr const uint8_t ICW4_MASK_MS = 0x4;			    //00000100
static constexpr const uint8_t ICW4_MASK_BUF = 0x8;			    //00001000
static constexpr const uint8_t ICW4_MASK_SFNM = 0x10;		    //00010000

//-----------------------------------------------
//	Initialization Command 1 control bits
//-----------------------------------------------

static constexpr const uint8_t ICW1_IC4_EXPECT = 1;			    //1
static constexpr const uint8_t ICW1_IC4_NO = 0;			        //0
static constexpr const uint8_t ICW1_SNGL_YES = 2;			    //10
static constexpr const uint8_t ICW1_SNGL_NO = 0;			    //00
static constexpr const uint8_t ICW1_ADI_CALLINTERVAL4 = 4;		//100
static constexpr const uint8_t ICW1_ADI_CALLINTERVAL8 = 0;		//000
static constexpr const uint8_t ICW1_LTIM_LEVELTRIGGERED = 8;	//1000
static constexpr const uint8_t ICW1_LTIM_EDGETRIGGERED = 0;		//0000
static constexpr const uint8_t ICW1_INIT_YES = 0x10;		    //10000
static constexpr const uint8_t ICW1_INIT_NO = 0;			    //00000

//-----------------------------------------------
//	Initialization Command 4 control bits
//-----------------------------------------------

static constexpr const uint8_t ICW4_UPM_86MODE = 1;			    //1
static constexpr const uint8_t ICW4_UPM_MCSMODE = 0;			//0
static constexpr const uint8_t ICW4_AEOI_AUTOEOI = 2;			//10
static constexpr const uint8_t ICW4_AEOI_NOAUTOEOI = 0;			//0
static constexpr const uint8_t ICW4_MS_BUFFERMASTER = 4;		//100
static constexpr const uint8_t ICW4_MS_BUFFERSLAVE = 0;			//0
static constexpr const uint8_t ICW4_BUF_MODEYES = 8;			//1000
static constexpr const uint8_t ICW4_BUF_MODENO = 0;             //0
static constexpr const uint8_t ICW4_SFNM_NESTEDMODE = 0x10;		//10000
static constexpr const uint8_t ICW4_SFNM_NOTNESTED = 0;         //0

namespace PIC
{
    void PIC::Setup(uint8_t base0, uint8_t base1) const
    {
        uint8_t	icw	= 0;

        // Begin initialization of PIC

        icw = (icw & ~ICW1_MASK_INIT) | ICW1_INIT_YES;
        icw = (icw & ~ICW1_MASK_IC4) | ICW1_IC4_EXPECT;

        SendCommand(icw, 0);
        SendCommand(icw, 1);

        // Send initialization control word 2. This is the base addresses of the irq's

        SendData(base0, 0);
        SendData(base1, 1);

        // Send initialization control word 3. This is the connection between master and slave.
        // ICW3 for master PIC is the IR that connects to secondary pic in binary format
        // ICW3 for secondary PIC is the IR that connects to master pic in decimal format

        SendData(0x04, 0);
        SendData(0x02, 1);

        // Send Initialization control word 4. Enables i86 mode

        icw = (icw & ~ICW4_MASK_UPM) | ICW4_UPM_86MODE;

        SendData(icw, 0);
        SendData(icw, 1);
    }

    uint8_t PIC::ReadData(uint8_t picNumber) const
    {
        if (picNumber > 1)
        {
            return 0;
        }

        const uint8_t reg = (picNumber == 1) ? PIC2_REG_DATA : PIC1_REG_DATA;
        return BootHelpers::InByte(reg);
    }

    void PIC::SendData(uint8_t data, uint8_t picNumber) const
    {
        if (picNumber > 1)
        {
            return;
        }

        const uint8_t reg = (picNumber == 1) ? PIC2_REG_DATA : PIC1_REG_DATA;
        BootHelpers::OutByte(data, reg);
    }

    void PIC::SendCommand(uint8_t command, uint8_t picNumber) const
    {
        if (picNumber > 1)
        {
            return;
        }

        const uint8_t reg = (picNumber == 1) ? PIC2_REG_COMMAND : PIC1_REG_COMMAND;
        BootHelpers::OutByte(command, reg);
    }
}