#include "FloppyDisk.h"
#include "../../boot/utils/globals.h"
#include "../../boot/utils/helpers.h"
#include "../../boot/utils/global_helpers.h"

namespace Devices
{
    volatile uint8_t FloppyDisk::m_diskIRQ;

    void FloppyDisk::Setup()
    {
        s_globals.IRQ().Add(FLOPPY_IRQ, Handler);

        Reset();

        // set drive information
        DriveData(13, 1, 0xf, true);
    }

    bool FloppyDisk::InitializeDMA(uint32_t length) const
    {
        union
        {
            uint8_t byte[4]; //Lo[0], Mid[1], Hi[2]
            unsigned long l;
        } a, c;

        a.l = DMA_BUFFER;
        c.l = length-1;

        //Check for buffer issues
        if ((a.l >> 24) || (c.l >> 16) || (((a.l & 0xffff)+c.l) >> 16)){
        #ifdef _DEBUG
            // TODO: add debug break
        #endif
            return false;
        }

        s_globals.DMA().Reset();
        s_globals.DMA().MaskChannel(FDC_DMA_CHANNEL); // Mask channel 2
        s_globals.DMA().ResetFlipflop(1); // Flipflop reset on DMA 1

        s_globals.DMA().SetAddress(FDC_DMA_CHANNEL, a.byte[0], a.byte[1]); // Buffer address
        s_globals.DMA().ResetFlipflop(1); // Flipflop reset on DMA 1

        s_globals.DMA().SetCount(FDC_DMA_CHANNEL, c.byte[0], c.byte[1]); // Set count
        s_globals.DMA().SetRead(FDC_DMA_CHANNEL);

        s_globals.DMA().UnmaskAll(); // Unmask channel 2

        return true;
    }

    void FloppyDisk::SetDMA(uint32_t addr)
    {
        DMA_BUFFER = addr;
    }

    void FloppyDisk::SetWorkingDrive(uint8_t drive)
    {
        if (drive < MAX_DRIVES)
        {
            m_currentDrive = drive;
        }
    }

    uint8_t FloppyDisk::GetWorkingDrive() const
    {
        return m_currentDrive;
    }

    uint8_t* FloppyDisk::ReadSector(int sectorLBA)
    {
        if (m_currentDrive > MAX_DRIVES)
        {
            return nullptr;
        }

        CHS chs = LBAToCHS(sectorLBA);

        ControlMotor(true);
        if (Seek(chs.track, chs.head) != 0)
        {
            return nullptr;
        }

        ReadSectorImpl(chs);
        ControlMotor(false);

        return (uint8_t*) DMA_BUFFER;
    }

    CHS FloppyDisk::LBAToCHS(int lba) const
    {
        const int head = (lba % (SECTORS_PER_TRACK * 2)) / (SECTORS_PER_TRACK);
        const int track = lba / (SECTORS_PER_TRACK * 2);
        const int sector = lba % SECTORS_PER_TRACK + 1;
        return CHS{head, track, sector};
    }

    void FloppyDisk::DMARead() const
    {
        BootHelpers::OutByte(0x06, 0x0a);   // mask dma channel 2
        BootHelpers::OutByte(0x56, 0x0b);   // single transfer, address increment, autoinit, read, channel 2
        BootHelpers::OutByte(0x02, 0x0a);   // unmask dma channel 2
    }

    void FloppyDisk::DMAWrite() const
    {
        BootHelpers::OutByte(0x06, 0x0a);   // mask dma channel 2
        BootHelpers::OutByte(0x5a, 0x0b);   // single transfer, address increment, autoinit, write, channel 2
        BootHelpers::OutByte(0x02, 0x0a);   // unmask dma channel 2
    }

    uint8_t FloppyDisk::ReadStatus() const
    {
        return BootHelpers::InByte(FLPYDSK_IO::MSR);
    }

    void FloppyDisk::WriteDor(uint8_t val) const
    {
        BootHelpers::OutByte(val, FLPYDSK_IO::DOR);
    }

    void FloppyDisk::SendCommand(uint8_t cmd) const
    {
        constexpr const uint32_t TIMEOUT = 500;

        // wait until data register is ready. We send commands to the data register
        for (uint32_t i = 0; i < TIMEOUT; ++i)
        {
            if (ReadStatus() & FLPYDSK_MSR_MASK::DATAREG)
            {
                return BootHelpers::OutByte(cmd, FLPYDSK_IO::FIFO);
            }
        }
    }

    uint8_t FloppyDisk::ReadData() const
    {
        constexpr const uint32_t TIMEOUT = 500;

        // same as above function but returns data register for reading
        for (uint32_t i = 0; i < TIMEOUT; ++i)
        {
            if (ReadStatus() & FLPYDSK_MSR_MASK::DATAREG)
            {
                return BootHelpers::InByte(FLPYDSK_IO::FIFO);
            }
        }

        return 0;
    }

    void FloppyDisk::WriteCcr(uint8_t val) const
    {
        BootHelpers::OutByte(val, FLPYDSK_IO::CTRL);
    }

    void FloppyDisk::WaitIrq()
    {
        while (m_diskIRQ == 0);
        m_diskIRQ = 0;
    }

    void FloppyDisk::CheckInt(uint32_t& st0, uint32_t& cyl) const
    {
        SendCommand(FLPYDSK_CMD::FDC_CHECK_INT);

        st0 = ReadData();
        cyl = ReadData();
    }

    void FloppyDisk::ControlMotor(bool enable) const
    {
        // sanity check: invalid drive
        if (m_currentDrive > MAX_DRIVES)
        {
            return;
        }

        uint32_t motor = 0;

        switch (m_currentDrive)
        {
            case 0:
            {
                motor = FLPYDSK_DOR_MASK::DRIVE0_MOTOR;
                break;
            }
            case 1:
            {
                motor = FLPYDSK_DOR_MASK::DRIVE1_MOTOR;
                break;
            }
            case 2:
            {
                motor = FLPYDSK_DOR_MASK::DRIVE2_MOTOR;
                break;
            }
            case 3:
            {
                motor = FLPYDSK_DOR_MASK::DRIVE3_MOTOR;
                break;
            }
        }

        if (enable)
        {
            WriteDor(m_currentDrive | motor | FLPYDSK_DOR_MASK::RESET | FLPYDSK_DOR_MASK::DOR_DMA);
        }
        else
        {
            WriteDor(FLPYDSK_DOR_MASK::RESET);
        }

        // in all cases; wait a little bit for the motor to spin up/turn off
        sleep(20);
    }

    void FloppyDisk::DriveData(uint32_t stepr, uint32_t loadt, uint32_t unloadt, bool dma) const
    {
        uint32_t data = 0;

        SendCommand(FLPYDSK_CMD::FDC_SPECIFY);

        data = ( (stepr & 0xf) << 4) | (unloadt & 0xf);
        SendCommand(data);
        
        data = (loadt) << 1 | (dma) ? 1 : 0;
        SendCommand(data);
    }

    int FloppyDisk::Calibrate(uint32_t drive)
    {
        uint32_t st0, cyl;

        if (drive > MAX_DRIVES)
        {
            return ErrorCodes::MaxDrives;
        }

        ControlMotor(true);

        for (int i = 0; i < 10; ++i)
        {
            // send command
            SendCommand(FLPYDSK_CMD::FDC_CALIBRATE);
            SendCommand(drive);
            WaitIrq();
            CheckInt(st0, cyl);

            // did we find cylinder 0? if so, we are done
            if (!cyl)
            {
                ControlMotor(false);
                return ErrorCodes::Success;
            }
        }

        ControlMotor(false);
        return ErrorCodes::Failure;
    }

    void FloppyDisk::EnableController(bool enable) const
    {
        if (enable)
        {
            WriteDor(FLPYDSK_DOR_MASK::RESET | FLPYDSK_DOR_MASK::DOR_DMA);
        }
        else
        {
            WriteDor(0);
        }
    }

    void FloppyDisk::Reset()
    {
        uint32_t st0, cyl;

        // reset the controller
        EnableController(true);
        EnableController(false);
        WaitIrq();

        // send CHECK_INT/SENSE INTERRUPT command to all drives
        for (int i = 0; i < MAX_DRIVES; ++i)
        {
            CheckInt(st0, cyl);
        }

        // transfer speed 500kb/s
        WriteCcr(0);

        // pass mechanical drive info. steprate=3ms, unload time=240ms, load time=16ms
        DriveData(3, 16, 240, true);

        // calibrate the disk
        Calibrate(m_currentDrive);
    }

    void FloppyDisk::ReadSectorImpl(const CHS& chs)
    {
        uint32_t st0, cyl;

        InitializeDMA(512);

        // set the DMA for read transfer
        DMARead();

        // read in a sector
        SendCommand(FLPYDSK_CMD::FDC_READ_SECT | FLPYDSK_CMD_EXT::FDC_MULTITRACK | FLPYDSK_CMD_EXT::FDC_SKIP | FLPYDSK_CMD_EXT::FDC_DENSITY);
        SendCommand(chs.head << 2 | m_currentDrive);
        SendCommand(chs.track);
        SendCommand(chs.head);
        SendCommand(chs.sector);
        SendCommand(FLPYDSK_SECTOR_DTL::DTL_512);
        SendCommand(((chs.sector + 1) >= SECTORS_PER_TRACK) ? SECTORS_PER_TRACK : chs.sector + 1);
        SendCommand(FLPYDSK_GAP3_LENGTH::LEN_3_5);
        SendCommand(0xff);

        WaitIrq();

        // read status info
        for (int i = 0; i < 7; ++i)
        {
            ReadData();
        }

        // let FDC know we handled interrupt
        CheckInt(st0, cyl);
    }

    int FloppyDisk::Seek(uint32_t cyl, uint32_t head)
    {
        uint32_t st0, cyl0;

        if (m_currentDrive > MAX_DRIVES)
        {
            return ErrorCodes::MaxDrives;
        }

        for (int i = 0; i < 10; ++i)
        {
            SendCommand(FLPYDSK_CMD::FDC_SEEK);
            SendCommand((head) << 2 | m_currentDrive);
            SendCommand(cyl);

            WaitIrq();
            CheckInt(st0, cyl0);

            // found the cylinder?
            if (cyl0 == cyl)
            {
                return ErrorCodes::Success;
            }
        }

        return ErrorCodes::Failure;
    }

    void FloppyDisk::Handler(Registers* regs)
    {
        (void) regs;

        m_diskIRQ = 1;
    }
}
