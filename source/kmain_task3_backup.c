#include "io.h"
#include "../drivers/framebuffer.h"

/* Test functions from Task 2 */
int sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}

int multiply(int a, int b)
{
    return a * b;
}

int factorial(int n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

void kmain(void)
{
    /* Clear the screen */
    fb_clear();
    
    /* Set color to light cyan on blue */
    fb_set_color(FB_LIGHT_CYAN, FB_BLUE);
    
    /* Test writing strings */
    fb_write_string("========================================\n");
    fb_write_string("  Welcome to Tiny OS - Worksheet 2!\n");
    fb_write_string("========================================\n\n");
    
    /* Test color change */
    fb_set_color(FB_LIGHT_GREEN, FB_BLACK);
    fb_write_string("Testing C Functions:\n");
    
    /* Test sum_of_three */
    fb_set_color(FB_WHITE, FB_BLACK);
    fb_write_string("sum_of_three(1, 2, 3) = ");
    fb_write_num(sum_of_three(1, 2, 3));
    fb_write_string("\n");
    
    /* Test multiply */
    fb_write_string("multiply(4, 5) = ");
    fb_write_num(multiply(4, 5));
    fb_write_string("\n");
    
    /* Test factorial */
    fb_write_string("factorial(5) = ");
    fb_write_num(factorial(5));
    fb_write_string("\n\n");
    
    /* Test cursor positioning */
    fb_set_color(FB_YELLOW, FB_BLACK);
    fb_move(0, 10);
    fb_write_string("Cursor moved to row 10!\n");
    
    /* Test different colors */
    fb_set_color(FB_RED, FB_BLACK);
    fb_write_string("Red text\n");
    
    fb_set_color(FB_MAGENTA, FB_BLACK);
    fb_write_string("Magenta text\n");
    
    fb_set_color(FB_LIGHT_BLUE, FB_BLACK);
    fb_write_string("Light blue text\n");
    
    /* Final message */
    fb_move(0, 20);
    fb_set_color(FB_WHITE, FB_GREEN);
    fb_write_string(" Task 3 Complete! Framebuffer Driver Working! ");
    
    /* Infinite loop */
    while(1);
}
