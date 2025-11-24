OBJECTS = source/loader.o source/kmain.o source/io.o

AS = nasm
ASFLAGS = -f elf

CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
         -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c

LD = ld
LDFLAGS = -T ./source/link.ld -melf_i386

all: kernel.elf

source/loader.o: source/loader.asm
	$(AS) $(ASFLAGS) source/loader.asm -o source/loader.o

source/io.o: source/io.asm
	$(AS) $(ASFLAGS) source/io.asm -o source/io.o

source/kmain.o: source/kmain.c
	$(CC) $(CFLAGS) source/kmain.c -o source/kmain.o

kernel.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o kernel.elf

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

run: os.iso
	qemu-system-i386 -nographic -boot d -cdrom os.iso -m 32 -d cpu -D logQ.txt

clean:
	rm -f source/*.o kernel.elf os.iso logQ.txt

.PHONY: all clean run
