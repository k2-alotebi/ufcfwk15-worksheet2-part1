# Makefile for Tiny OS - Task 3

# Compiler and linker settings
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T source/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

# Object files
OBJECTS = source/loader.o source/kmain.o drivers/io.o drivers/fb.o

.PHONY: all clean run stop viewlog

all: os.iso

# Compile assembly files
source/loader.o: source/loader.asm
	$(AS) $(ASFLAGS) source/loader.asm -o source/loader.o

drivers/io.o: drivers/io.s
	$(AS) $(ASFLAGS) drivers/io.s -o drivers/io.o

# Compile C files
source/kmain.o: source/kmain.c
	$(CC) $(CFLAGS) source/kmain.c -o source/kmain.o

drivers/fb.o: drivers/fb.c
	$(CC) $(CFLAGS) drivers/fb.c -o drivers/fb.o

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) source/loader.o source/kmain.o drivers/io.o drivers/fb.o -o kernel.elf

# Build ISO image
os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R \
		-b boot/grub/stage2_eltorito \
		-no-emul-boot \
		-boot-load-size 4 \
		-A os \
		-input-charset utf8 \
		-quiet \
		-boot-info-table \
		-o os.iso \
		iso

# Run the OS in QEMU - runs for 3 seconds then quits and shows output
run: os.iso
	@echo "Starting QEMU for 3 seconds..."
	@timeout 3 qemu-system-i386 -nographic -serial file:serial.log -boot d -cdrom os.iso -m 32 2>/dev/null || true
	@echo ""
	@echo "SERIAL OUTPUT"
	@cat serial.log
	@echo ""
	@echo "KERNEL EXECUTION COMPLETE"

stop:
	@pkill -u $(USER) qemu || echo "No QEMU process found"
	@echo "QEMU stopped"

viewlog:
	@cat serial.log

# Clean build files
clean:
	rm -f source/*.o drivers/*.o kernel.elf os.iso logQ.txt
	rm -f iso/boot/kernel.elf
