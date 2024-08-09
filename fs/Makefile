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

OBJS= fs_example.o

all: fs.o

fs.o: $(OBJS)
	$(LD) -r -o fs.o $(OBJS)

clean:
	rm -f *.o *.a

fs_example.o: fs_example.c
