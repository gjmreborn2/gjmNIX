#!/usr/bin/python

import os

cmds = [
    # boot
    'i686-elf-cpp -Iinclude boot/boot.S -o boot/boot.s',
    'i686-elf-as boot/boot.s -o boot/boot.o',
    'i686-elf-cpp -Iinclude boot/crti.S -o boot/crti.s',
    'i686-elf-as boot/crti.s -o boot/crti.o',
    'i686-elf-cpp -Iinclude boot/crtn.S -o boot/crtn.s',
    'i686-elf-as boot/crtn.s -o boot/crtn.o',

    # kernel
    'i686-elf-g++ -Iinclude -c kernel/init.cpp -o kernel/init.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti',

    # final link
    'i686-elf-g++ -T linker.ld -o gjmnix.bin -ffreestanding -O2 -nostdlib boot/boot.o boot/crti.o boot/crtn.o kernel/init.o -lgcc'
]

for cmd in cmds:
    os.system(cmd)
