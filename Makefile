RAMDISK = #-DRAMDISK=512

AS=i686-elf-as
LD=i686-elf-ld
CC=i686-elf-gcc $(RAMDISK)
CPP=i686-elf-cpp

LDFLAGS=-s -x -M
CCFLAGS=-Wall -Wextra -O2 -ffreestanding -fstrength-reduce -fomit-frame-pointer
CPPFLAGS=-Iinclude -ffreestanding

.S.s:
	$(CPP) $(CPPFLAGS) -o $*.s $<

.c.s:
	$(CC) $(CCFLAGS) -Iinclude -S -o $*.s $<

.s.o:
	$(AS) -c -o $*.o $<

.c.o:
	$(CC) $(CCFLAGS) -Iinclude -c -o $*.o $<

all: build/gjmNIX.bin

LINK_LIST=boot/crti.o boot/boot.o boot/crtn.o \
	init/init.o \
	drivers/block_dev/block_dev.a drivers/character_dev/character_dev.a \
	klib/klib.a \
	lib/lib.a \
	mm/mm.o \
	fs/fs.o \
	kernel/kernel.o \
	kernel/math/math.a

build/gjmNIX.bin: $(LINK_LIST)
	$(CC) -T build/linker.ld -o $@ $(CPPFLAGS) -nostdlib -lgcc $(LINK_LIST)

boot/crti.s: boot/crti.S
boot/crti.o: boot/crti.s

boot/boot.s: boot/boot.S
boot/boot.o: boot/boot.s

boot/crtn.s: boot/crtn.S
boot/crtn.o: boot/crtn.s

kernel/math/math.a:
	(cd kernel/math; make)

drivers/block_dev/block_dev.a:
	(cd drivers/block_dev; make)

drivers/character_dev/character_dev.a:
	(cd drivers/character_dev; make)

kernel/kernel.o:
	(cd kernel; make)

mm/mm.o:
	(cd mm; make)

fs/fs.o:
	(cd fs; make)

lib/lib.a:
	(cd lib; make)

klib/klib.a:
	(cd klib; make)

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
	rm -f init/*.o *.iso
	(cd mm; make clean)
	(cd fs; make clean)
	(cd kernel; make clean)
	(cd lib; make clean)
	(cd klib; make clean)
	(cd drivers/character_dev; make clean)
	(cd drivers/block_dev; make clean)
	(cd kernel/math; make clean)

init/init.o: init/init.c