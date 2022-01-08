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
/*					Helper Functions					*/
/********************************************************/

static uint8_t InByte(uint16_t port)
{
	uint8_t value;
	asm volatile("inb %1, %0" : "=a" (value) : "dN" (port));
	return value;
}

static void OutByte(uint8_t value, uint16_t port)
{
	asm volatile("outb %0, %1" : : "a" (value), "dN" (port));
}

/********************************************************/
/*					Cursor Functions					*/
/********************************************************/

void enable_cursor()
{
	OutByte(0x0A, 0x3D4);
	OutByte(InByte(0x3D5 & 0xC0), 0x3D5);

	OutByte(0x0B, 0x3D4);
	OutByte(InByte(0x3D5 & 0xE0) | VGA_HEIGHT, 0x3D5);
}

void disable_cursor()
{
	OutByte(0x0A, 0x3D4);
	OutByte(0x20, 0x3D5);
}

void update_cursor()
{
	const size_t position = terminal_row * VGA_WIDTH + terminal_column;

	OutByte(0x0F, 0x3D4);
	OutByte(position & 0xFF, 0x3D5);
	OutByte(0x0E, 0x3D4);
	OutByte((position >> 8) & 0xFF, 0x3D5);
}

/********************************************************/
/*					Terminal Functions					*/
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
	enable_cursor();
	update_cursor();
}

void terminal_putchar(char c)
{
	handle_putchar(c);
	handle_scroll();
	update_cursor();
}

void terminal_removechar()
{
	if (terminal_row == 0 && terminal_column == 0)
	{
		return;
	}

	if (terminal_column > 0)
	{
		terminal_putentryat(' ', terminal_color, --terminal_column, terminal_row);
	}
	else
	{
		terminal_column = VGA_WIDTH - 1;
		terminal_putentryat(' ', terminal_color, terminal_column, --terminal_row);
	}

	update_cursor();
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