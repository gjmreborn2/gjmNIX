AS=i686-elf-as
LD=i686-elf-ld
CC=i686-elf-gcc
CPP=i686-elf-cpp

LDFLAGS=-s -x -M
CCFLAGS=-Wall -Wextra -O2 -ffreestanding -fstrength-reduce -fomit-frame-pointer
CPPFLAGS=-nostdinc -Iinclude

.PHONY: all clean install test
#.SUFFIXES: .o .c .S .s

#.c.o:
#	$(CC) -c $< -o $@ $(CCFLAGS) $(CPPFLAGS)
#.S.o:
#	$(CC) -c $< -o $@ $(CCFLAGS) $(CPPFLAGS)

LINK_LIST=boot/crti.o boot/boot.o init/init.o boot/crtn.o

all:
	i686-elf-cpp boot/boot.S -o boot/boot.s $(CPPFLAGS)
	i686-elf-as boot/boot.s -o boot/boot.o
	i686-elf-cpp boot/crti.S -o boot/crti.s $(CPPFLAGS)
	i686-elf-as boot/crti.s -o boot/crti.o
	#i686-elf-cpp boot/crtbegin.S -o boot/crtbegin.s
	#i686-elf-as boot/crtbegin.s -o boot/crtbegin.o
	#i686-elf-cpp boot/crtend.S -o boot/crtend.s
	#i686-elf-as boot/crtend.s -o boot/crtend.o
	i686-elf-cpp boot/crtn.S -o boot/crtn.s $(CPPFLAGS)
	i686-elf-as boot/crtn.s -o boot/crtn.o
	i686-elf-gcc -c init/init.c -o init/init.o $(CCFLAGS) $(CPPFLAGS)
	i686-elf-gcc -T build/linker.ld -o build/gjmNIX.bin $(CPPFLAGS) -nostdlib -lgcc $(LINK_LIST)

install:
	mkdir -p build/isodir/boot/grub
	cp build/gjmNIX.bin build/isodir/boot/gjmNIX.bin
	cp build/grub.cfg build/isodir/boot/grub/grub.cfg
	grub2-mkrescue -o gjmNIX.iso build/isodir

test:
	qemu-system-i386 -cdrom gjmNIX.iso

clean:
	rm -f boot/*.s boot/*.o
	rm -rf build/isodir build/*.bin
	rm -f init/*.o
	rm -f *.iso
