#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>
#include <kernel/tty_helpers.h>

#include "vga.h"

/********************************************************/
/*						Data							*/
/********************************************************/

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static const size_t VGA_BUFFER_SIZE = VGA_WIDTH * VGA_HEIGHT;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* const terminal_buffer = VGA_MEMORY;


/********************************************************/
/*						Functions						*/
/********************************************************/

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putentryat(unsigned char c, uint8_t color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void handle_putchar(char c)
{
	const unsigned char uc = c;
	const bool isNewline = (c == '\n');

	if (!isNewline)
	{
		terminal_putentryat(uc, terminal_color, terminal_column, terminal_row);
	}

	if (isNewline || (++terminal_column == VGA_WIDTH))
	{
		terminal_column = 0;
		++terminal_row;
	}
}

void handle_scroll()
{
	if (terminal_row == VGA_HEIGHT)
	{
		memmove(terminal_buffer, terminal_buffer + VGA_WIDTH, (VGA_BUFFER_SIZE - VGA_WIDTH) * 2);
		terminal_memset(terminal_buffer + (VGA_BUFFER_SIZE - VGA_WIDTH), vga_entry(' ', terminal_color), VGA_WIDTH * 2);

		--terminal_row;
	}	
}


/********************************************************/
/*					.h Functions						*/
/********************************************************/

void terminal_initialize()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_memset(terminal_buffer, vga_entry(' ', terminal_color), VGA_BUFFER_SIZE * 2);
}

void terminal_putchar(char c)
{
	handle_putchar(c);
	handle_scroll();
}

void terminal_write(const char* data, size_t size)
{
	if (!data)
	{
		return;
	}

	for (size_t i = 0; i < size; i++)
	{
		terminal_putchar(data[i]);
	}
}

void terminal_writestring(const char* data)
{
	if (!data)
	{
		return;
	}

	while (*data)
	{
		terminal_putchar(*data);
	}
}