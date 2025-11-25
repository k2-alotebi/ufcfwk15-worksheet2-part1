#include "io.h"
#include "../drivers/framebuffer.h"

void kmain(void)
{
    /* Direct write test - bypass framebuffer functions */
    char *fb = (char *) 0x000B8000;
    
    /* Write "DIRECT" at top left */
    fb[0] = 'D'; fb[1] = 0x0F;   // White on black
    fb[2] = 'I'; fb[3] = 0x0F;
    fb[4] = 'R'; fb[5] = 0x0F;
    fb[6] = 'E'; fb[7] = 0x0F;
    fb[8] = 'C'; fb[9] = 0x0F;
    fb[10] = 'T'; fb[11] = 0x0F;
    
    /* Now test framebuffer functions */
    fb_clear();  // Should clear the screen
    
    fb_set_color(FB_LIGHT_CYAN, FB_BLUE);
    fb_write_string("FB TEST: Hello World\n");
    
    fb_set_color(FB_WHITE, FB_BLACK);
    fb_write_string("Testing numbers: ");
    fb_write_num(42);
    fb_write_string("\n");
    
    fb_set_color(FB_RED, FB_BLACK);
    fb_write_string("Red text\n");
    
    fb_set_color(FB_GREEN, FB_BLACK);
    fb_write_string("Green text\n");
    
    /* Test positioning */
    fb_move(0, 10);
    fb_set_color(FB_YELLOW, FB_BLACK);
    fb_write_string("This is at row 10!\n");
    
    /* Test sum_of_three to put value in register for verification */
    int result = 1 + 2 + 3;
    __asm__ volatile("mov %0, %%eax" : : "r"(result));
    
    while(1);
}
