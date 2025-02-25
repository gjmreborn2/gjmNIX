#!/bin/bash

if grub-file --is-x86-multiboot gjmnix.bin; then
    echo "multiboot ONE confirmed"
else
    echo "the file is not multiboot (myos.bin)"
fi
