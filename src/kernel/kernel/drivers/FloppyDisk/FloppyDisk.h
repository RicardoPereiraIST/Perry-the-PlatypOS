#pragma once

#include "FloppyDiskTypes.h"
#include "../../boot/interrupts/registers.h"
#include <stdint.h>

namespace Devices
{
    struct CHS
    {
        int head = 0;
        int track = 0;
        int sector = 0;
    };

    class FloppyDisk
    {
        static constexpr const int FLOPPY_IRQ = 6;
        static constexpr const int MAX_DRIVES = 3;
        static constexpr const int FDC_DMA_CHANNEL = 2;

    public:
        enum ErrorCodes
        {
            MaxDrives = -2,
            Failure = -1,
            Success = 0,
        };

        void Setup();

        // DMA tranfer buffer starts here and ends at 0x1000+64k
        // You can change this as needed. It must be below 16MB and in idenitity mapped memory!
        void SetDMA(uint32_t addr);
        void SetWorkingDrive(uint8_t drive);
        uint8_t GetWorkingDrive() const;
        uint8_t* ReadSector(int sectorLBA);
        
        CHS LBAToCHS(int lba) const;

    private:
        // Initialize DMA to use phys addr 84k-128k
        bool InitializeDMA(uint32_t length) const;

        // Prepare the DMA for read transfer
        void DMARead() const;
        // Prepare the DMA for write transfer
        void DMAWrite() const;

        uint8_t ReadStatus() const;
        void WriteDor(uint8_t val) const;
        void SendCommand(uint8_t cmd) const;
        uint8_t ReadData() const;
        void WriteCcr(uint8_t val) const;

        void CheckInt(uint32_t& st0, uint32_t& cyl) const;
        void ControlMotor(bool enable) const;
        void DriveData(uint32_t stepr, uint32_t loadt, uint32_t unloadt, bool dma) const;
        int Calibrate(uint32_t drive);
        void EnableController(bool enable) const;

        void Reset();
        void ReadSectorImpl(const CHS& chs);
        int Seek(uint32_t cyl, uint32_t head);

        void WaitIrq();
        static void Handler(Registers* regs);

        uint8_t m_currentDrive = 0;
        uint32_t DMA_BUFFER = 0x1000;

        static volatile uint8_t m_diskIRQ;
    };
} // namespace Devices