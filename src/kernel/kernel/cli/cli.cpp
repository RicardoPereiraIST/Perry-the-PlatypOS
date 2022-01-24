#include "cli.h"
#include "../boot/utils/globals.h"
#include "../boot/utils/helpers.h"
#include "../boot/utils/global_helpers.h"
#include "Commands/ClearTerminal.hpp"
#include "Commands/HelpCommand.hpp"
#include "Commands/ReadCommand.hpp"
#include "Commands/UserCommand.hpp"

#include <stdio.h>
#include <string.h>
#include <cctype>
#include <kernel/tty.h>

static Commands::HelpCommand s_helpCmd;
static Commands::ClearTerminal s_clsCmd;
static Commands::ReadCommand s_readCmd;
static Commands::UserCommand s_userCmd;

Commands::ICommand* Cli::s_commands[COMMANDS] =
{
	&s_helpCmd,
	&s_clsCmd,
	&s_readCmd,
	&s_userCmd,
};

Cli::Cli()
{}

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
	bool success = false;

	for (uint32_t i = 0; i < COMMANDS; ++i)
	{
		if (Commands::ICommand* pCmd = s_commands[i])
		{
			if ((success = pCmd->ExecuteCommand(m_buffer)))
			{
				break;
			}			
		}
	}

	if (!success)
	{
		if (strcmp(m_buffer, "exit") == 0)
		{
			return true;
		}
		else
		{
			printf("\n");
			printf("Unknown command\n");
		}
	}
	

	return false;
}

Devices::Keyboard::Keycode Cli::GetChr() const
{
	Devices::Keyboard::Keycode key = Devices::Keyboard::KEY_UNKNOWN;

	// wait for a keypress
	while (key == Devices::Keyboard::KEY_UNKNOWN)
	{
		key = s_globals.Keyboard().GetLastKey();
	}

	// discard last keypress (we handled it) and return
	s_globals.Keyboard().DiscardLastKey();
	return key;
}

void Cli::GetCommand()
{
	constexpr const uint32_t MAX_SIZE = SIZE - 2;

	PromptUser();

	Devices::Keyboard::Keycode key = Devices::Keyboard::Keycode::KEY_UNKNOWN;
	bool BufChar;

	// get command string
	size_t i = 0;
	while (i < MAX_SIZE)
	{
		// buffer the next char
		BufChar = true;

		// grab next char
		key = GetChr();

		// // end of command if enter is pressed
		if (key == Devices::Keyboard::Keycode::KEY_RETURN)
		{
			break;
			PromptUser();
		}

		if (key==Devices::Keyboard::Keycode::KEY_BACKSPACE)
		{
			// dont buffer this char
			BufChar = false;

			if (i > 0)
			{
				terminal_removechar();
				i--;
			}
		}

		// // only add the char if it is to be buffered
		if (BufChar)
		{
			// convert key to an ascii char and put it in buffer
			const char c = s_globals.Keyboard().KeyToAscii(key);
			if (isprint(c))
			{
				terminal_putchar(c);
				m_buffer[i++] = c;
			}
		}

		// wait for next key. You may need to adjust this to suite your needs
		sleep(10);
	}

	// null terminate the string
	m_buffer[i] = '\0';
}