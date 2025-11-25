#include "io.h"
#include "../drivers/framebuffer.h"

void kmain(void)
{
    // Marker 1: Entered kmain
    __asm__ volatile("mov $0x1111, %eax");
    
    fb_clear();
    
    // Marker 2: After fb_clear
    __asm__ volatile("mov $0x2222, %eax");
    
    fb_set_color(FB_LIGHT_CYAN, FB_BLUE);
    
    // Marker 3: After fb_set_color
    __asm__ volatile("mov $0x3333, %eax");
    
    fb_write_string("Framebuffer Test String\n");
    
    // Marker 4: After fb_write_string
    __asm__ volatile("mov $0x4444, %eax");
    
    fb_write_num(42);
    
    // Marker 5: After fb_write_num
    __asm__ volatile("mov $0x5555, %eax");
    
    fb_move(10, 5);
    
    // Marker 6: After fb_move
    __asm__ volatile("mov $0x6666, %eax");
    
    fb_write_string(" <- Positioned at (10,5)");
    
    // Marker 7: After second write
    __asm__ volatile("mov $0x7777, %eax");
    
    // Test all color functions
    fb_set_color(FB_RED, FB_BLACK);
    fb_set_color(FB_GREEN, FB_BLACK);
    fb_set_color(FB_BLUE, FB_BLACK);
    
    // Marker 8: After color changes
    __asm__ volatile("mov $0x8888, %eax");
    
    // Final marker - ALL functions executed successfully!
    __asm__ volatile("mov $0xAAAA, %eax");
    
    while(1);
}
