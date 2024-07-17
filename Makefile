AS=i686-elf-as
LD=i686-elf-ld
CC=i686-elf-gcc
CPP=i686-elf-cpp

LDFLAGS=-s -x -M
CCFLAGS=-Wall -O -fstrength-reduce -fomit-frame-pointer -fcombine-regs
CPPFLAGS=-nostdinc -Iinclude

all:
	i686-elf-cpp boot/boot.S -o boot/boot.s
	i686-elf-as boot/boot.s -o boot/boot.o
	i686-elf-gcc -c init/init.c -o init/init.o -ffreestanding -O2 -Wall -Wextra
	i686-elf-gcc -T build/linker.ld -o build/gjmNIX.bin -ffreestanding -O2 -nostdlib boot/boot.o init/init.o -lgcc

install:
	mkdir -p build/isodir/boot/grub
	cp build/gjmNIX.bin build/isodir/boot/gjmNIX.bin
	cp build/grub.cfg build/isodir/boot/grub/grub.cfg
	grub2-mkrescue -o gjmNIX.iso build/isodir

test:
	qemu-system-i386 -cdrom gjmNIX.iso

clean:
	rm -f boot/boot.s boot/*.o
	rm -rf build/isodir build/*.bin
	rm -f init/*.o
	rm -f *.iso
