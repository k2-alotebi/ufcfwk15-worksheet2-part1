#include "../drivers/fb.h"


/* Simple serial output for testing on remote server */
void serial_putc(char c) {
    __asm__ volatile("mov $0x3F8, %%dx; mov %0, %%al; out %%al, %%dx" : : "r"(c) : "dx", "al");
}

void serial_puts(char *str) {
    int i = 0;
    while (str[i] != '\0') {
        serial_putc(str[i]);
        i++;
    }
}

/* Main kernel function called from loader.asm */
void kmain()
{
    /* Clear the screen */
    fb_clear();
    
    /* Test fb_puts */
    fb_puts("Hello from Tiny OS!\n");
    fb_puts("Framebuffer driver is working!\n\n");
    
    /* Also output to serial for verification */
    serial_puts("Hello from Tiny OS via serial!\n");
    serial_puts("Framebuffer driver is working!\n");
    
    /* Test fb_move */
    fb_move(10, 5);
    fb_puts("This is at position (10, 5)\n");
    serial_puts("Moved cursor to (10, 5)\n");
    
    /* Test different colors by writing directly */
    fb_move(0, 10);
    fb_write_cell(10 * FB_WIDTH, 'C', FB_LIGHT_RED, FB_BLACK);
    fb_write_cell(10 * FB_WIDTH + 1, 'O', FB_LIGHT_GREEN, FB_BLACK);
    fb_write_cell(10 * FB_WIDTH + 2, 'L', FB_LIGHT_BLUE, FB_BLACK);
    fb_write_cell(10 * FB_WIDTH + 3, 'O', FB_LIGHT_CYAN, FB_BLACK);
    fb_write_cell(10 * FB_WIDTH + 4, 'R', FB_LIGHT_MAGENTA, FB_BLACK);
    fb_write_cell(10 * FB_WIDTH + 5, 'S', FB_LIGHT_BROWN, FB_BLACK);
    
    serial_puts("Wrote COLORS in different colors at row 10\n");
    
    /* Test the original C functions */
    fb_move(0, 15);
    fb_puts("Testing C functions:\n");
    
    serial_puts("Framebuffer test complete!\n");
    
    /* Infinite loop */
    while(1);
}
