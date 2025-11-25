#include "io.h"

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
    /* Task 1: Write 0xCAFEBABE to EAX */
    __asm__ volatile("mov $0xCAFEBABE, %eax");
    
    /* Task 2: Test C functions */
    int result1 = sum_of_three(1, 2, 3);
    int result2 = multiply(4, 5);
    int result3 = factorial(5);
    
    /* Put results in registers */
    __asm__ volatile("mov %0, %%eax" : : "r"(result1));
    __asm__ volatile("mov %0, %%ebx" : : "r"(result2));
    __asm__ volatile("mov %0, %%ecx" : : "r"(result3));
    
    while(1);
}
