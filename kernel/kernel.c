#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

void terminal_scoll() 
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
            
            if (y != VGA_HEIGHT - 1) {
                terminal_buffer[index] = terminal_buffer[index + VGA_WIDTH];
            }
            else {
			    terminal_buffer[index] = vga_entry(' ', terminal_color);
            }
		}
	}
    terminal_row = terminal_row - 1;
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;

    if (c == '\n') {
		terminal_column = 0;
        terminal_row = terminal_row + 1;
    }
    if (terminal_row == VGA_HEIGHT) {
        terminal_scoll();
    }
    if (c != '\n') {
	    terminal_buffer[index] = vga_entry(c, color);
    }
}
 
void terminal_putchar(char c) 
{

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}
 
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}
 
// void kernel_main(void) 
// {
// 	/* Initialize terminal interface */
// 	terminal_initialize();
 
// 	/* Newline support is left as an exercise. */
// 	terminal_writestring("           :::      ::::::::  \n       :+:      :+:    :+:  \n     +:+ +:+         +:+    \n   +#+  +:+       +#+       \n +#+#+#+#+#+   +#+          \n      #+#    #+#            \n     ###   ########         \n");
// 	terminal_writestring("           :::      ::::::::  \n       :+:      :+:    :+:  \n     +:+ +:+         +:+    \n   +#+  +:+       +#+       \n +#+#+#+#+#+   +#+          \n      #+#    #+#            \n     ###   ########         \n");
// 	terminal_writestring("           :::      ::::::::  \n       :+:      :+:    :+:  \n     +:+ +:+         +:+    \n   +#+  +:+       +#+       \n +#+#+#+#+#+   +#+          \n      #+#    #+#            \n     ###   ########         \n");
// 	terminal_writestring("           :::      ::::::::  \n       :+:      :+:    :+:  \n     +:+ +:+         +:+    \n   +#+  +:+       +#+       \n +#+#+#+#+#+   +#+          \n      #+#    #+#            \n     ###   ########         \n");
// 	terminal_writestring("           :::      ::::::::  \n       :+:      :+:    :+:  \n     +:+ +:+         +:+    \n   +#+  +:+       +#+       \n +#+#+#+#+#+   +#+          \n      #+#    #+#            \n     ###   ########         \n");
// 	terminal_writestring("           :::      ::::::::  \n       :+:      :+:    :+:  \n     +:+ +:+         +:+    \n   +#+  +:+       +#+       \n +#+#+#+#+#+   +#+          \n      #+#    #+#            \n     ###   ########         \n");
// }
// 

#define INT_DISABLE 0
#define INT_ENABLE  0x200
#define PIC1 0x20
#define PIC2 0xA0

#define ICW1 0x11
#define ICW4 0x01

void outb( unsigned short port, unsigned char val )
{
   asm volatile("outb %0, %1" : : "a"(val), "Nd"(port) );
}

static __inline unsigned char inb (unsigned short int port)
{
  unsigned char _v;

  __asm__ __volatile__ ("inb %w1,%0":"=a" (_v):"Nd" (port));
  return _v;
}

void init_pics(int pic1, int pic2)
{
   /* send ICW1 */
   outb(PIC1, ICW1);
   outb(PIC2, ICW1);

   /* send ICW2 */
   outb(PIC1 + 1, pic1);   
   outb(PIC2 + 1, pic2);   

   /* send ICW3 */
   outb(PIC1 + 1, 4);   
   outb(PIC2 + 1, 2);

   /* send ICW4 */
   outb(PIC1 + 1, ICW4);
   outb(PIC2 + 1, ICW4);

   /* disable all IRQs */
   outb(PIC1 + 1, 0xFF);
}

/*irrelevant code*/


void kernel_main()
{
    terminal_initialize();
	char c = 0;
	init_pics(0x20, 0x28);
	do
	{
		if(inb(0x60)!=c) //PORT FROM WHICH WE READ
		{
			c = inb(0x60);
			if(c>0)
				{
					terminal_putchar(c); //print on screen
				}
			}
	}
	while(c!=1); // 1= ESCAPE
}