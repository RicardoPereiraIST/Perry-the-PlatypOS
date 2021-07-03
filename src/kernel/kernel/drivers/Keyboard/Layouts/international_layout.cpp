#include "international_layout.h"
#include "../keyboard.h"

namespace Devices
{
    int s_internationLayout[] =
    {
        //! key			                        scancode
        Keyboard::Keycode::KEY_UNKNOWN,	        //0
        Keyboard::Keycode::KEY_ESCAPE,		    //1
        Keyboard::Keycode::KEY_1,			    //2
        Keyboard::Keycode::KEY_2,			    //3
        Keyboard::Keycode::KEY_3,			    //4
        Keyboard::Keycode::KEY_4,			    //5
        Keyboard::Keycode::KEY_5,			    //6
        Keyboard::Keycode::KEY_6,			    //7
        Keyboard::Keycode::KEY_7,			    //8
        Keyboard::Keycode::KEY_8,			    //9
        Keyboard::Keycode::KEY_9,			    //0xa
        Keyboard::Keycode::KEY_0,			    //0xb
        Keyboard::Keycode::KEY_MINUS,		    //0xc
        Keyboard::Keycode::KEY_EQUAL,		    //0xd
        Keyboard::Keycode::KEY_BACKSPACE,  	    //0xe
        Keyboard::Keycode::KEY_TAB,		        //0xf
        Keyboard::Keycode::KEY_Q,			    //0x10
        Keyboard::Keycode::KEY_W,			    //0x11
        Keyboard::Keycode::KEY_E,			    //0x12
        Keyboard::Keycode::KEY_R,			    //0x13
        Keyboard::Keycode::KEY_T,			    //0x14
        Keyboard::Keycode::KEY_Y,			    //0x15
        Keyboard::Keycode::KEY_U,			    //0x16
        Keyboard::Keycode::KEY_I,			    //0x17
        Keyboard::Keycode::KEY_O,			    //0x18
        Keyboard::Keycode::KEY_P,			    //0x19
        Keyboard::Keycode::KEY_LEFTBRACKET,     //0x1a
        Keyboard::Keycode::KEY_RIGHTBRACKET,    //0x1b
        Keyboard::Keycode::KEY_RETURN,		    //0x1c  //keypad enter
        Keyboard::Keycode::KEY_LCTRL,		    //0x1d  //right ctrl and pause
        Keyboard::Keycode::KEY_A,			    //0x1e
        Keyboard::Keycode::KEY_S,			    //0x1f
        Keyboard::Keycode::KEY_D,			    //0x20
        Keyboard::Keycode::KEY_F,			    //0x21
        Keyboard::Keycode::KEY_G,			    //0x22
        Keyboard::Keycode::KEY_H,			    //0x23
        Keyboard::Keycode::KEY_J,			    //0x24
        Keyboard::Keycode::KEY_K,			    //0x25
        Keyboard::Keycode::KEY_L,			    //0x26
        Keyboard::Keycode::KEY_SEMICOLON,   	//0x27
        Keyboard::Keycode::KEY_QUOTE,		    //0x28
        Keyboard::Keycode::KEY_GRAVE,		    //0x29
        Keyboard::Keycode::KEY_LSHIFT,		    //0x2a
        Keyboard::Keycode::KEY_BACKSLASH,  	    //0x2b
        Keyboard::Keycode::KEY_Z,			    //0x2c
        Keyboard::Keycode::KEY_X,			    //0x2d
        Keyboard::Keycode::KEY_C,			    //0x2e
        Keyboard::Keycode::KEY_V,			    //0x2f
        Keyboard::Keycode::KEY_B,			    //0x30
        Keyboard::Keycode::KEY_N,			    //0x31
        Keyboard::Keycode::KEY_M,			    //0x32
        Keyboard::Keycode::KEY_COMMA,		    //0x33
        Keyboard::Keycode::KEY_DOT,		        //0x34
        Keyboard::Keycode::KEY_SLASH,		    //0x35  //div
        Keyboard::Keycode::KEY_RSHIFT,		    //0x36
        Keyboard::Keycode::KEY_KP_ASTERISK,     //0x37
        Keyboard::Keycode::KEY_LALT,		    //0x38  //right alt
        Keyboard::Keycode::KEY_SPACE,		    //0x39
        Keyboard::Keycode::KEY_CAPSLOCK,	    //0x3a
        Keyboard::Keycode::KEY_F1,			    //0x3b
        Keyboard::Keycode::KEY_F2,			    //0x3c
        Keyboard::Keycode::KEY_F3,			    //0x3d
        Keyboard::Keycode::KEY_F4,			    //0x3e
        Keyboard::Keycode::KEY_F5,			    //0x3f
        Keyboard::Keycode::KEY_F6,			    //0x40
        Keyboard::Keycode::KEY_F7,			    //0x41
        Keyboard::Keycode::KEY_F8,			    //0x42
        Keyboard::Keycode::KEY_F9,			    //0x43
        Keyboard::Keycode::KEY_F10,		        //0x44
        Keyboard::Keycode::KEY_KP_NUMLOCK,	    //0x45
        Keyboard::Keycode::KEY_SCROLLLOCK,	    //0x46
        Keyboard::Keycode::KEY_KP_7,		    //0x47  //home
        Keyboard::Keycode::KEY_KP_8,		    //0x48	//keypad up arrow
        Keyboard::Keycode::KEY_KP_9,		    //0x49  //page up
        Keyboard::Keycode::KEY_KP_MINUS,	    //0x4a
        Keyboard::Keycode::KEY_KP_4,	        //0x4b  //keypad left arrow
        Keyboard::Keycode::KEY_KP_5,	        //0x4c
        Keyboard::Keycode::KEY_KP_6,	        //0x4d  //keypad right arrow
        Keyboard::Keycode::KEY_KP_PLUS,	        //0x4e
        Keyboard::Keycode::KEY_KP_1,	        //0x4f  //end
        Keyboard::Keycode::KEY_KP_2,		    //0x50	//keypad down arrow
        Keyboard::Keycode::KEY_KP_3,		    //0x51	//keypad page down
        Keyboard::Keycode::KEY_KP_0,		    //0x52	//keypad insert key
        Keyboard::Keycode::KEY_KP_DECIMAL,	    //0x53	//keypad delete key
        Keyboard::Keycode::KEY_UNKNOWN,	        //0x54  //print screen
        Keyboard::Keycode::KEY_UNKNOWN,	        //0x55
        Keyboard::Keycode::KEY_BACKSLASH,       //0x56
        Keyboard::Keycode::KEY_F11,		        //0x57
        Keyboard::Keycode::KEY_F12,			    //0x58
        Keyboard::Keycode::KEY_UNKNOWN,	        //0x59
        Keyboard::Keycode::KEY_UNKNOWN,	        //0x5a
        Keyboard::Keycode::KEY_UNKNOWN,	        //0x5b  //left window
        Keyboard::Keycode::KEY_UNKNOWN,	        //0x5c  //right window
        Keyboard::Keycode::KEY_UNKNOWN,	        //0x5d  //application
    };

    LookUpElement<uint8_t, uint8_t> s_internationalTranslation[] = 
    {
        LookUpElement<uint8_t, uint8_t>('0', Keyboard::Keycode::KEY_RIGHTPARENTHESIS),
        LookUpElement<uint8_t, uint8_t>('1', Keyboard::Keycode::KEY_EXCLAMATION),
        LookUpElement<uint8_t, uint8_t>('2', Keyboard::Keycode::KEY_AT),
        LookUpElement<uint8_t, uint8_t>('3', Keyboard::Keycode::KEY_HASH),
        LookUpElement<uint8_t, uint8_t>('4', Keyboard::Keycode::KEY_DOLLAR),
        LookUpElement<uint8_t, uint8_t>('5', Keyboard::Keycode::KEY_PERCENT),
        LookUpElement<uint8_t, uint8_t>('6', Keyboard::Keycode::KEY_CARRET),
        LookUpElement<uint8_t, uint8_t>('7', Keyboard::Keycode::KEY_AMPERSAND),
        LookUpElement<uint8_t, uint8_t>('8', Keyboard::Keycode::KEY_ASTERISK),
        LookUpElement<uint8_t, uint8_t>('9', Keyboard::Keycode::KEY_LEFTPARENTHESIS),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_COMMA, Keyboard::Keycode::KEY_LESS),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_DOT, Keyboard::Keycode::KEY_GREATER),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_SLASH, Keyboard::Keycode::KEY_QUESTION),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_SEMICOLON, Keyboard::Keycode::KEY_COLON),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_QUOTE, Keyboard::Keycode::KEY_QUOTEDOUBLE),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_LEFTBRACKET, Keyboard::Keycode::KEY_LEFTCURL),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_RIGHTBRACKET, Keyboard::Keycode::KEY_RIGHTCURL),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_GRAVE, Keyboard::Keycode::KEY_TILDE),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_MINUS, Keyboard::Keycode::KEY_UNDERSCORE),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_EQUAL, Keyboard::Keycode::KEY_PLUS),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_BACKSLASH, Keyboard::Keycode::KEY_BAR),
        LookUpElement<uint8_t, uint8_t>(Keyboard::Keycode::KEY_LESS, Keyboard::Keycode::KEY_BAR),
    };

    KeyLayout s_internationalLayout = KeyLayout(s_internationLayout, ARRAY_COUNT(s_internationLayout), s_internationalTranslation, ARRAY_COUNT(s_internationalTranslation));
}