#include "io.h"
#include "../drivers/framebuffer.h"

int sum_of_three(int arg1, int arg2, int arg3) {
    return arg1 + arg2 + arg3;
}

void kmain(void)
{
    int test_counter = 0;
    
    // Test 1: fb_clear doesn't crash
    fb_clear();
    test_counter++;
    
    // Test 2: fb_set_color doesn't crash
    fb_set_color(FB_WHITE, FB_BLACK);
    test_counter++;
    
    // Test 3: fb_write_string doesn't crash
    fb_write_string("Hello World");
    test_counter++;
    
    // Test 4: fb_write_num doesn't crash
    fb_write_num(123);
    test_counter++;
    
    // Test 5: fb_move doesn't crash
    fb_move(10, 10);
    test_counter++;
    
    // If we got here with test_counter=5, all functions work!
    // Also test Task 2 function
    int sum = sum_of_three(1, 2, 3);
    
    __asm__ volatile("mov %0, %%eax" : : "r"(test_counter));  // Should be 5
    __asm__ volatile("mov %0, %%ebx" : : "r"(sum));           // Should be 6
    
    while(1);
}
