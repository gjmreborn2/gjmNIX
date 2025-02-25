#!/bin/bash

i686-elf-cpp -Iinclude boot.S -o boot.s
i686-elf-as boot.s -o boot.o

i686-elf-g++ -c init.cpp -o init.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

i686-elf-g++ -T linker.ld -o gjmnix.bin -ffreestanding -O2 -nostdlib boot.o init.o
