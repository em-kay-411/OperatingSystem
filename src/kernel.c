#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/parser.h"
#include "string/string.h"
#include "disk/streamer.h"
#include "fs/file.h"

// uint16_t is a 16-bit integer
uint16_t *video_mem = 0;

// Initialise global coordinates for the terminal
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

// Color is shifted 8 bits to the left because color should be present in the higher byte.
// THe lower byte is taken up by the character ASCII value. SO OR opertaion is preformed.
uint16_t terminal_make_char(char c, char color)
{
    return (color << 8) | c;
}

// Function to put character on a specific location on the terminal
void terminal_putchar(int x, int y, char c, char color)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, color);
}

// Function to initialise the terminal by cleaning it
void terminal_initialise()
{
    // Initialise a pointer of 16-bit integer type at the absolute location 0xB8000 which is the location
    //  we need to write into the video memory
    video_mem = (uint16_t *)(0xB8000);
    terminal_row = 0;
    terminal_col = 0;

    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            // The equation determines the index in a 1D array for any coordinates
            video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(' ', 0);
        }
    }
}

// Function to append character
void terminal_writechar(char c, char color)
{
    if (c == '\n')
    {
        terminal_row++;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, color);
    terminal_col++;

    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }
}

// Function to print on the terminal
void print(const char *str)
{
    int len = strlen(str);

    for (int i = 0; i < len; i++)
    {
        terminal_writechar(str[i], 15);
    }
}

static struct paging_4gb_chunk * kernel_chunk = 0;

void kernel_main()
{
    terminal_initialise();       // Initialise terminal
    print("Hello World\nNew line\n");

    kheap_init();               // Initialise Heap
    fs_init();                  // Initialise filesystems
    disk_search_and_init();     // Search the disk and initialise it
    idt_init();                 // Initialise Interrupt Descriptor Table
    kernel_chunk = paging_new_4gb(IS_WRITABLE | IS_PRESENT | ACCESS_FROM_ALL);  // Setup paging
    paging_switch(get_page_directory(kernel_chunk));                // Switch to kernel paging chunk
    enable_paging();            // Enable Paging
    enable_interrupts();        // Enable interrupts

    char buf[20];
    strcpy(buf, "hello");
    while(1){}
}
