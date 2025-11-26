# Worksheet 2 Part 1 - Tiny OS Development

**Student:** Khaled Alotebi  
**Email:** khaled2.alotebi@live.uwe.ac.uk  
**Module:** UFCFWK-15-2 Operating Systems

## Overview

This project implements a minimal operating system kernel that boots from scratch using GRUB, supports calling C code from assembly, and includes a framebuffer driver for console output.

---

## Task 1: Basic Kernel Boot (20%)

### Implementation

Created a minimal kernel that boots via GRUB and writes `0xCAFEBABE` to the EAX register.

**Files:**
- `source/loader.asm` - Assembly bootloader with multiboot header
- `source/link.ld` - Linker script to load kernel at 1MB
- `iso/boot/grub/menu.lst` - GRUB configuration
- `Makefile` - Build automation

**Key Features:**
- Multiboot-compliant header (magic: 0x1BADB002)
- Loads at physical address 0x00100000 (1MB)
- Infinite loop to maintain control

### Build and Run
```bash
make clean
make
make run
```

### Verification

Check that EAX contains CAFEBABE:
```bash
make viewlog
# or
grep "EAX=cafebabe" logQ.txt
```

**Screenshot:**
<img width="1261" height="281" alt="Screenshot 2025-11-26 022145" src="https://github.com/user-attachments/assets/aa61aad7-f091-4e51-afc3-bd2366823492" />

The log shows `EAX=cafebabe` repeated multiple times, proving the kernel executed successfully.
```
EAX=cafebabe EBX=0002cd80 ECX=00000001 EDX=00000000
```

---

## Task 2: Calling C from Assembly (20%)

### Implementation

Extended the kernel to set up a stack and call C functions from assembly code.

**Files Added:**
- `source/kmain.c` - C kernel main function with test functions

**C Functions Implemented:**

1. **sum_of_three(int, int, int)** - Required function
   - Example: `sum_of_three(1, 2, 3)` returns 6

2. **multiply_two(int, int)** - Additional function
   - Example: `multiply_two(5, 7)` returns 35

3. **subtract(int, int)** - Additional function
   - Example: `subtract(10, 4)` returns 6

### How It Works

1. **Stack Setup** - Reserved 4KB in `.bss` section for kernel stack
2. **ESP Initialization** - Points ESP to top of stack (grows downward)
3. **C Calling Convention** - Uses cdecl (arguments pushed right-to-left)
4. **Function Call** - Assembly calls `kmain()` using `call` instruction

**Assembly Code (loader.asm):**
```nasm
KERNEL_STACK_SIZE equ 4096

section .bss
kernel_stack:
    resb KERNEL_STACK_SIZE

loader:
    mov esp, kernel_stack + KERNEL_STACK_SIZE  ; Setup stack
    call kmain                                  ; Call C code
```

### Verification

Results stored in registers for verification:
```bash
tail -20 logQ.txt | grep "EAX"
```

**Expected Output:**
```
EAX=00000006 EBX=00000023 ECX=00000006
```

- EAX = 6 (1+2+3) 
- EBX = 0x23 = 35 (5×7) 
- ECX = 6 (10-4) 

<img width="1059" height="835" alt="Screenshot 2025-11-26 022547" src="https://github.com/user-attachments/assets/2b807a46-f333-4283-b765-72135528e085" />

---

## Task 3: Framebuffer Driver (40%)

### Implementation

Developed a complete framebuffer driver for VGA text mode console output.

**Files Added:**
- `drivers/io.h` - I/O port interface declarations
- `drivers/io.s` - Assembly implementation of `inb`/`outb`
- `drivers/fb.h` - Framebuffer driver interface
- `drivers/fb.c` - Framebuffer driver implementation

### Framebuffer API

#### Core Functions
```c
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg);
void fb_move_cursor(unsigned short pos);
void fb_move(unsigned short x, unsigned short y);
void fb_clear(void);
void fb_putc(char c);
void fb_puts(char *str);
int fb_write(char *buf, unsigned int len);
```

#### Color Support

16 colors available (0-15):
- Black, Blue, Green, Cyan, Red, Magenta, Brown, Light Grey
- Dark Grey, Light Blue, Light Green, Light Cyan, Light Red, Light Magenta, Light Brown, White

### Technical Details

**Memory-Mapped I/O:**
- Framebuffer address: `0x000B8000`
- Dimensions: 80 columns × 25 rows
- Each cell: 2 bytes (character + color attributes)

**Cursor Control:**
- Command port: `0x3D4`
- Data port: `0x3D5`
- Position sent as 16-bit value (high byte, then low byte)

**I/O Port Operations:**
```nasm
; outb - Send byte to I/O port
outb:
    mov al, [esp + 8]    ; data
    mov dx, [esp + 4]    ; port
    out dx, al
    ret

; inb - Read byte from I/O port
inb:
    mov dx, [esp + 4]    ; port
    in al, dx
    ret
```

### Example Usage
```c
fb_clear();                              // Clear screen
fb_puts("Hello from Tiny OS!\n");       // Print string
fb_move(10, 5);                         // Move cursor to (10,5)
fb_write_cell(0, 'A', FB_WHITE, FB_BLACK); // Write colored char
```

### Verification

Run the kernel and check serial output:
```bash
make run
# Wait 3 seconds, it auto-stops
make viewlog
```

**Output:**
```
Hello from Tiny OS via serial!
Framebuffer driver is working!
Moved cursor to (10, 5)
Wrote COLORS in different colors at row 10
Framebuffer test complete!
```

### Screenshots
<img width="1102" height="986" alt="image" src="https://github.com/user-attachments/assets/38971a23-27ed-45dc-86b8-df9abd258872" />
<img width="609" height="260" alt="image" src="https://github.com/user-attachments/assets/251bcc51-c15c-4607-a1fd-084391dae974" />

**Build Output:**
```
nasm -f elf source/loader.asm -o source/loader.o
gcc -m32 ... source/kmain.c -o source/kmain.o
nasm -f elf drivers/io.s -o drivers/io.o
gcc -m32 ... drivers/fb.c -o drivers/fb.o
ld -T source/link.ld -melf_i386 ... -o kernel.elf
```

**GRUB Boot:**
```
[Multiboot-elf, <0x100000:0x4f4:0x0>, entry=0x10000c]
Hello from Tiny OS via serial!
Framebuffer driver is working!
```

---

## Project Structure
```
worksheet2/
├── Makefile              # Build automation
├── README.md             # This file
├── drivers/
│   ├── fb.c             # Framebuffer driver implementation
│   ├── fb.h             # Framebuffer driver interface
│   ├── io.h             # I/O port interface
│   └── io.s             # I/O port assembly implementation
├── iso/
│   └── boot/
│       ├── grub/
│       │   ├── menu.lst           # GRUB config
│       │   └── stage2_eltorito    # GRUB bootloader
│       └── kernel.elf             # Compiled kernel
├── source/
│   ├── kmain.c          # C kernel code
│   ├── link.ld          # Linker script
│   └── loader.asm       # Assembly bootloader
└── os.iso               # Bootable ISO image
```

---

## Build System

### Makefile Targets
```bash
make          # Build os.iso
make clean    # Remove all build artifacts
make run      # Run in QEMU (3 second timeout)
make stop     # Stop any running QEMU instances
make viewlog  # View serial output log
```

### Compilation Flags

**GCC (C Compiler):**
```
-m32                  # 32-bit target
-nostdlib             # No standard library
-nostdinc             # No standard includes
-fno-builtin          # No builtin functions
-fno-stack-protector  # No stack protection
-Wall -Wextra -Werror # All warnings as errors
```

**NASM (Assembler):**
```
-f elf                # ELF32 object format
```

**LD (Linker):**
```
-T source/link.ld     # Custom linker script
-melf_i386            # 32-bit x86 ELF
```

---

## Key Learnings

1. **Boot Process Understanding**
   - BIOS → GRUB → Kernel handoff
   - Multiboot specification requirements
   - Memory layout and addressing

2. **Low-Level Programming**
   - Assembly and C interoperability
   - Stack management
   - Calling conventions (cdecl)

3. **Hardware Interaction**
   - Memory-mapped I/O (framebuffer)
   - Port I/O (cursor control)
   - VGA text mode operation

4. **Systems Programming**
   - Freestanding C environment
   - No standard library
   - Direct hardware control

---

## References

- *The Little Book About OS Development* by Erik Helin & Adam Renberg
- [OSDev Wiki](https://wiki.osdev.org/)
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/)
- [Intel x86 Manual](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)

---

## GitHub Repository

[https://github.com/k2-alotebi/ufcfwk15-worksheet2-part1](https://github.com/k2-alotebi/ufcfwk15-worksheet2-part1)
