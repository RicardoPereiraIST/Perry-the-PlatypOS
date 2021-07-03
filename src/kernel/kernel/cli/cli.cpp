#include "cli.h"
#include "../boot/utils/globals.h"
#include "../boot/utils/helpers.h"

#include <stdio.h>
#include <string.h>
#include <kernel/tty.h>

void Cli::Run()
{
	while (true)
	{
		GetCommand();

		if (ExecuteCommand())
		{
			break;
		}
	}
}

void Cli::PromptUser() const
{
	printf("Command> ");
}

bool Cli::ExecuteCommand()
{
	if (strcmp(m_buffer, "exit") == 0)
	{
		return true;
	}
	else if (strcmp(m_buffer, "cls") == 0)
	{
		terminal_initialize();
	}
	else if (strcmp(m_buffer, "help") == 0)
	{
		printf("\n");
		printf("Perry Os\n");
		printf("Supported commands:\n");
		printf("exit: quits and halts the system\n");
		printf("cls: clears the display\n");
		printf("help: displays this message\n");
	}
	else
	{
		printf("\n");
		printf("Unknown command\n");
	}

	return false;
}

Devices::Keyboard::Keycode Cli::GetChr() const
{
	Devices::Keyboard::Keycode key = Devices::Keyboard::KEY_UNKNOWN;

	//! wait for a keypress
	while (key == Devices::Keyboard::KEY_UNKNOWN)
	{
		key = s_globals.Keyboard().GetLastKey();
	}

	//! discard last keypress (we handled it) and return
	s_globals.Keyboard().DiscardLastKey();
	return key;
}

void Cli::GetCommand()
{
	constexpr const uint32_t MAX_SIZE = SIZE - 2;

	PromptUser();

	Devices::Keyboard::Keycode key = Devices::Keyboard::Keycode::KEY_UNKNOWN;
	bool BufChar;

	//! get command string
	size_t i = 0;
	while (i < MAX_SIZE)
	{
		//! buffer the next char
		BufChar = true;

		//! grab next char
		key = GetChr();

		// //! end of command if enter is pressed
		if (key == Devices::Keyboard::Keycode::KEY_RETURN)
		{
			break;
		}

		if (key==Devices::Keyboard::Keycode::KEY_BACKSPACE)
		{
			//! dont buffer this char
			BufChar = false;

			if (i > 0)
			{
				// Remove last char
				// TODO

				//! go back one char in cmd buf
				i--;
			}
		}

		// //! only add the char if it is to be buffered
		if (BufChar)
		{
			//! convert key to an ascii char and put it in buffer
			char c = s_globals.Keyboard().KeyToAscii(key);
			if (c != 0)
			{
				terminal_putchar(c);
				m_buffer[i++] = c;
			}
		}

		//! wait for next key. You may need to adjust this to suite your needs
		BootHelpers::sleep(10);
	}

	//! null terminate the string
	m_buffer[i] = '\0';
}