#include "io.h"

/* Function to test calling C from assembly */
int sum_of_three(int arg1, int arg2, int arg3)
{
    return arg1 + arg2 + arg3;
}

/* Function to multiply two numbers */
int multiply(int a, int b)
{
    return a * b;
}

/* Function to calculate factorial (simple recursion test) */
int factorial(int n)
{
    if (n <= 1)
        return 1;
    return n * factorial(n - 1);
}

void kmain(void)
{
    /* Test sum_of_three function */
    int result1 = sum_of_three(1, 2, 3);  // Should be 6
    
    /* Test multiply function */
    int result2 = multiply(4, 5);         // Should be 20
    
    /* Test factorial function */
    int result3 = factorial(5);           // Should be 120
    
    /* Put results in registers so we can see them in the log */
    __asm__ volatile("mov %0, %%eax" : : "r"(result1));
    __asm__ volatile("mov %0, %%ebx" : : "r"(result2));
    __asm__ volatile("mov %0, %%ecx" : : "r"(result3));
    
    /* Infinite loop */
    while(1);
}
