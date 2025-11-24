# Worksheet 2 Part 1 - Tiny OS

## Task 1: Boot a Machine from Scratch

### What I Did
I created a minimal operating system that boots from scratch using GRUB and writes `0xCAFEBABE` to the EAX register.

### Files Created
- `source/loader.asm` - Assembly bootloader
- `source/link.ld` - Linker script
- `iso/boot/grub/menu.lst` - GRUB configuration
- `Makefile` - Build automation

### How to Build and Run
```bash
make clean
make all
make os.iso
make run
```

### Verification
After running, check `logQ.txt` for `EAX=cafebabe` to confirm successful boot.

### Screenshot
[Add screenshot of logQ.txt showing cafebabe here]

## Task 2: Calling C from Assembly

### What I Did
Extended the bootloader to set up a C stack and call C functions.

### Functions Implemented
1. **sum_of_three(1, 2, 3)** - Returns 6
2. **multiply(4, 5)** - Returns 20
3. **factorial(5)** - Returns 120 (recursive function)

### Files Added/Modified
- `source/kmain.c` - Main C kernel with test functions
- `source/io.asm` - I/O port functions (outb/inb)
- `source/io.h` - C header for I/O functions
- `source/loader.asm` - Updated to set up stack and call kmain

### Verification
Check `logQ.txt`:
- EAX = 0x00000006 (6)
- EBX = 0x00000014 (20)
- ECX = 0x00000078 (120)

All three functions work correctly!
