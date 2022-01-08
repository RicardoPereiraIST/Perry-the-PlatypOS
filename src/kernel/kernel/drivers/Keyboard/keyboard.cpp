#include "keyboard.h"
#include "../../boot/utils/helpers.h"
#include "../../boot/utils/globals.h"
#include "Layouts/international_layout.h"
#include <cctype>

#include <stdio.h>


#define TO_INTEGRAL(e) static_cast<uint32_t>(e)

namespace Devices
{
    /************************************************************/
    /*    Keyboard encoder/controller register and commands     */
    /************************************************************/

    // keyboard encoder ------------------------------------------
    enum class EncoderPorts
    {
        ReadInputBuffer = 0x60,
        SendCommand 	= 0x60
    };

    enum class EncoderCommands
    {
        SetLed				    =	0xED,
        Echo					=	0xEE,
        ScanCodeSet			    =	0xF0,
        Id					    =	0xF2,
        AutoDelay				=	0xF3,
        Enable				    =	0xF4,
        ResetWait				=	0xF5,
        ResetScan				=	0xF6,
        AllAuto 				=	0xF7,
        AllMakeBreak			=	0xF8,
        AllMakeOnly		    	=	0xF9,
        AllMakeBreakAuto	    =	0xFA,
        SingleAutoRepeat		=	0xFB,
        SingleMakeBreak	    	=	0xFC,
        SingleBreakOnly	    	=	0xFD,
        Resend				    =	0xFE,
        Reset					=	0xFF
    };

    // keyboard controller ---------------------------------------
    enum class ControllerPorts
    {
        StatusRegister  =	0x64,
        SendCommand     =	0x64
    };

    enum class ControllerStatusMask
    {
        OutBuf	    =	1,		//00000001
        InBuf	    =	2,		//00000010
        System	    =	4,		//00000100
        CommandData	=	8,		//00001000
        Locked	    =	0x10,	//00010000
        AuxBuf	    =	0x20,	//00100000
        Timeout	    =	0x40,	//01000000
        Parity	    =	0x80	//10000000
    };

    enum class ControllerCommands
    {
        Read			    =	0x20,
        Write			    =	0x60,
        SelfTest		    =	0xAA,
        InterfaceTest	    =	0xAB,
        Disable			    =	0xAD,
        Enable			    =	0xAE,
        ReadInPort		    =	0xC0,
        ReadOutPort	        =	0xD0,
        WriteOutPort	    =	0xD1,
        ReadTestInputs  	=	0xE0,
        SystemRest  		=	0xFE,
        MouseDisable	    =	0xA7,
        MouseEnable 		=	0xA8,
        MousePortTest	    =	0xA9,
        MouseWrite		    =	0xD4
    };

    // scan error codes ------------------------------------------
    enum class Error
    {
        BufferOverrun		=	0,
        IdRet				=	0x83AB,
        Bat					=	0xAA,	//note: can also be L. shift key make code
        EchoRet			    =	0xEE,
        Ack					=	0xFA,
        BatFailed			=	0xFC,
        DiagFailed			=	0xFD,
        ResendCommand		=	0xFE,
        Key					=	0xFF
    };


    /************************************************************/
    /*       Keyboard controller/encoder helper functions       */
    /************************************************************/

    static uint8_t ControllerReadStatus()
    {
        return BootHelpers::InByte(TO_INTEGRAL(ControllerPorts::StatusRegister));
    }

    static uint8_t EncoderReadBuffer()
    {
        return BootHelpers::InByte(TO_INTEGRAL(EncoderPorts::ReadInputBuffer));
    }

    static void WaitInputBuffer()
    {
        while(ControllerReadStatus() & TO_INTEGRAL(ControllerStatusMask::InBuf));
    }

    static void WaitOutputBuffer()
    {
        while(ControllerReadStatus() & TO_INTEGRAL(ControllerStatusMask::OutBuf));
    }

    static void ControllerSendCommand(uint8_t cmd)
    {
        WaitInputBuffer();
        BootHelpers::OutByte(cmd, TO_INTEGRAL(ControllerPorts::SendCommand));
    }

    static void EncoderSendCommand(uint8_t cmd)
    {
        WaitInputBuffer();
        BootHelpers::OutByte(cmd, TO_INTEGRAL(EncoderPorts::SendCommand));
    }

    /************************************************************/
    /*                       Keyboard                           */
    /************************************************************/

    int Keyboard::s_curScanCode(INVALID_SCANCODE);
    bool Keyboard::s_isNumLock(false);
    bool Keyboard::s_isCapsLock(false);
    bool Keyboard::s_isScrollLock(false);
    bool Keyboard::s_isAlt(false);
    bool Keyboard::s_isShift(false);
    bool Keyboard::s_isCtrl(false);
    bool Keyboard::s_hasBatSucceeded(true);
    bool Keyboard::s_hasDiagnosticSucceeded(false);
    bool Keyboard::s_shouldResend(false);
    InternationalLayout Keyboard::s_layout;

    Keyboard::Keyboard()
        : m_error(0)
        , m_enabled(true)
    {
    }

    void Keyboard::Setup()
	{
        s_globals.IRQ().Add(1, Handler);
        SetLeds(false, false, false);
	}

    bool Keyboard::IsScrollLock() const
	{
        return s_isScrollLock;
	}

    bool Keyboard::IsNumLock() const
	{
        return s_isNumLock;
	}

    bool Keyboard::IsSCapsLock() const
	{
        return s_isCapsLock;
	}

    bool Keyboard::IsAtl() const
	{
        return s_isAlt;
	}

    bool Keyboard::IsCtrl() const
	{
        return s_isCtrl;
	}

    bool Keyboard::IsShift() const
	{
        return s_isShift;
	}

    void Keyboard::SetLeds(bool numLock, bool capsLock, bool scrollLock)
	{
        uint8_t data = 0;

        data = scrollLock | BIT(0);
        data = numLock | BIT(1);
        data = capsLock | BIT(2);

        EncoderSendCommand(TO_INTEGRAL(EncoderCommands::SetLed));
        EncoderSendCommand(data);
	}

    void Keyboard::UpdateLeds()
    {
        SetLeds(s_isNumLock, s_isCapsLock, s_isScrollLock);
    }

    void Keyboard::Enable(bool enable)
	{
        if (enable)
        {
            ControllerSendCommand(TO_INTEGRAL(ControllerCommands::Enable));
        }
        else
        {
            ControllerSendCommand(TO_INTEGRAL(ControllerCommands::Disable));
        }

        m_enabled = enable;
	}

    bool Keyboard::IsEnabled() const
	{
        return m_enabled;
	}

    void Keyboard::ResetSystem()
	{
        ControllerSendCommand(TO_INTEGRAL(ControllerCommands::WriteOutPort));
        EncoderSendCommand(0xfe);
	}

    void Keyboard::IgnoreResend()
	{
        s_shouldResend = false;
	}

    bool Keyboard::CheckResend()
	{
        return s_shouldResend;
	}

    bool Keyboard::GetDiagnosticResult() const
	{
        return s_hasDiagnosticSucceeded;
	}

    bool Keyboard::GetBatResult() const
	{
        return s_hasBatSucceeded;
	}

    bool Keyboard::SelfTest()
	{
        ControllerSendCommand(TO_INTEGRAL(ControllerCommands::SelfTest));
        WaitOutputBuffer();
        return (EncoderReadBuffer() == 0x55);
	}

    uint8_t Keyboard::GetLastScan() const
	{
        return s_curScanCode;
	}

    Keyboard::Keycode Keyboard::GetLastKey() const
	{
        return (s_curScanCode != INVALID_SCANCODE) ? static_cast<Keycode>(s_layout.GetKeyCode(s_curScanCode)) : KEY_UNKNOWN;
	}

    void Keyboard::DiscardLastKey()
	{
        s_curScanCode = INVALID_SCANCODE;
	}

    char Keyboard::KeyToAscii(Keyboard::Keycode code) const
	{
        uint8_t key = code;

        if (isascii (key))
        {
            if (s_isShift || s_isCapsLock)
            {
                if (key >= 'a' && key <= 'z')
                {
                    key = toupper(key);
                }
            }

            if (s_isShift && !s_isCapsLock)
            {
                key = s_layout.GetTranslation(key);
            }

            return key;
        }

        return 0;
	}

    void Keyboard::Handler(Registers* regs)
    {
        (void) regs;

        auto checkAndSetShiftAltCtrl = [](int key, bool keyPressed) -> bool
        {
            switch (key)
            {
                case KEY_LCTRL:
                case KEY_RCTRL:
                {
                    s_isCtrl = keyPressed;
                    return true;
                }
                case KEY_LSHIFT:
                case KEY_RSHIFT:
                {
                    s_isShift = keyPressed;
                    return true;
                }
                case KEY_LALT:
                case KEY_RALT:
                {
                    s_isAlt = keyPressed;
                    return true;
                }
            }
            return false;
        };

        auto checkAndSetLockKeys = [](int key) -> bool
        {
            switch (key)
            {
                case KEY_CAPSLOCK:
                {
                    s_isCapsLock = !s_isCapsLock;
                    return true;
                }
                case KEY_KP_NUMLOCK:
                {
                    s_isNumLock = !s_isNumLock;
                    return true;
                }
                case KEY_SCROLLLOCK:
                {
                    s_isScrollLock = !s_isScrollLock;
                    return true;
                }
            }
            return false;
        };

        auto checkError = [](int code) -> bool
        {
            switch (code)
            {
                case TO_INTEGRAL(Error::BatFailed):
                {
                    s_hasBatSucceeded = false;
                    return true;
                }
                case TO_INTEGRAL(Error::DiagFailed):
                {
                    s_hasDiagnosticSucceeded = false;
                    return true;
                }
                case TO_INTEGRAL(Error::ResendCommand):
                {
                    s_shouldResend = true;
                    return true;
                }
            }
            return false;
        };

        int code = INVALID_SCANCODE;
        static bool extended = false;

        if (ControllerReadStatus() & TO_INTEGRAL(ControllerStatusMask::OutBuf))
        {
            code = EncoderReadBuffer();

            if (checkError(code))
            {
                return;
            }

            // extended keyboard - unused for now
            extended = (code == 0xE0 || code == 0xE1);
            if (!extended)
            {
                // break code
                const bool isKeyRelease = code & 0x80;

                if (isKeyRelease)
                {
                    code -= 0x80;
                }

                const int key = s_layout.GetKeyCode(code);
                checkAndSetShiftAltCtrl(key, !isKeyRelease);

                if (!isKeyRelease)
                {
                    s_curScanCode = code;
                    if (checkAndSetLockKeys(key))
                    {
                        UpdateLeds();
                    }
                }
            }
        }
    }
}

#undef TO_INTEGRAL