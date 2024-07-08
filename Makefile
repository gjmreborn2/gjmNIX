AS_86_SOURCES = boot.s
AS_86_OBJECTS = boot.o
AS_86_FLAGS = -0 -a

LD_86_FLAGS = -0 -s
LD_86_IMAGE = boot

AS_SOURCES = kernel.s
AS_OBJECTS = kernel.o

#LD_FLAGS = --strip-all --discard-all -M #with > Image.map
SYMBOL_MAP_FILE = Image.map
LD_FLAGS = --strip-all --discard-all -Map=$(SYMBOL_MAP_FILE)
LD_IMAGE = kernel

FINAL_SYSTEM_IMAGE = Image

all: $(AS_86_SOURCES) $(AS_SOURCES)
	as86 $(AS_86_FLAGS) -o $(AS_86_OBJECTS) $(AS_86_SOURCES)
	ld86 $(LD_86_FLAGS) -o $(LD_86_IMAGE) $(AS_86_OBJECTS)
	as -o $(AS_OBJECTS) $(AS_SOURCES)
	ld $(LD_FLAGS) $(AS_OBJECTS) -o $(LD_IMAGE)

install: all
# note from scratch_notes
	dd bs=32 if=$(LD_86_IMAGE) of=$(FINAL_SYSTEM_IMAGE) skip=1
# note from scratch_notes
	dd=512 if=$(LD_IMAGE) of=$(FINAL_SYSTEM_IMAGE) skip=2 seek=1

$(FINAL_SYSTEM_IMAGE): install
$(SYMBOL_MAP_FILE): all

run: $(FINAL_SYSTEM_IMAGE) $(SYMBOL_MAP_FILE)
#	qemu-system-i386 ???

clean:
	rm -rf *.o $(LD_86_IMAGE) $(LD_IMAGE)
	rm $(SYMBOL_MAP_FILE)
	rm $(FINAL_SYSTEM_IMAGE)

#as86 -0 -a -o boot.o boot.s
#ld86 -0 -s -o boot boot.o
#gas -o head.o head.s        # as
#gld -s -x -M head.o -o system > System.map  # ld

#dd bs=32 if=boot of=Image skip=1
#dd bs=512 if=system of=Image skip=2 seek=1