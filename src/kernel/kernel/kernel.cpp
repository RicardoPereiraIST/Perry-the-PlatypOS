#include "boot/utils/globals.h"
#include "boot/utils/helpers.h"
#include "boot/utils/multiboot.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <kernel/tty.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int _kernel_start;
extern int _kernel_end;
extern int _kernel_file_size;

Devices::Keyboard::Keycode getch ()
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

void cmd()
{
	printf("Command> ");
}

//! gets next command
void get_cmd(char* buf, size_t size)
{
	cmd();

	Devices::Keyboard::Keycode key = Devices::Keyboard::Keycode::KEY_UNKNOWN;
	bool BufChar;

	//! get command string
	size_t i = 0;
	while (i < size - 2)
	{
		//! buffer the next char
		BufChar = true;

		//! grab next char
		key = getch ();

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
				terminal_putchar (c);
				buf [i++] = c;
			}
		}

		//! wait for next key. You may need to adjust this to suite your needs
		BootHelpers::sleep (10);
	}

	//! null terminate the string
	buf [i] = '\0';
}

bool run_cmd (char* cmd_buf)
{
	if (strcmp(cmd_buf, "exit") == 0)
	{
		return true;
	}
	else if (strcmp(cmd_buf, "cls") == 0)
	{
		terminal_initialize();
	}
	else if (strcmp(cmd_buf, "help") == 0)
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
		printf("Unkown command\n");
	}

	return false;
}

void run()
{
	char buf[100];
	
	//! command buffer
	while (true)
	{
		get_cmd(buf, 100);

		if (run_cmd (buf))
		{
			break;
		}
	}
}

void kernel_main(multiboot_info_t* pInfo, unsigned int magic)
{
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		panic("Wrong multiboot magic number.\n");
	}

	const uintptr_t start = (uintptr_t)&_kernel_start;
	const uintptr_t kernel_size = (uintptr_t)&_kernel_file_size;

	s_globals.StartTimer();

	if (!s_globals.PhysicalMemoryManager().Init(pInfo, start, kernel_size))
	{
		panic("Failed to initialize physical memory manager.\n");
	}

	if (!s_globals.VirtualMemoryManager().Setup())
	{
		panic("Failed to initialize virtual memory manager.\n");
	}

	s_globals.Keyboard().Setup();

	run();
}

#ifdef __cplusplus
}
#endif