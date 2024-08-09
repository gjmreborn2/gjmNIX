AR=i686-elf-ar
AS=i686-elf-as
LD=i686-elf-ld
CC=i686-elf-gcc
CPP=i686-elf-cpp

LDFLAGS=-s -x -M
CCFLAGS=-Wall -Wextra -O2 -ffreestanding -fstrength-reduce -fomit-frame-pointer
CPPFLAGS=-I../include -ffreestanding

.S.s:
	$(CPP) $(CPPFLAGS) -o $*.s $<

.c.s:
	$(CC) $(CCFLAGS) -I../include -S -o $*.s $<

.s.o:
	$(AS) -c -o $*.o $<

.c.o:
	$(CC) $(CCFLAGS) -I../include -c -o $*.o $<

OBJS= kprintf.o kstring.o kutils.o multiboot.o

all: klib.a

klib.a: $(OBJS)
	$(AR) rcs klib.a $(OBJS)

clean:
	rm -f *.o *.a

kprintf.o: kprintf.c
kstring.o: kstring.c
kutils.o: kutils.c
multiboot.o: multiboot.c
