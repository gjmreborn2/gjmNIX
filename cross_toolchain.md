wget from https://ftp.gnu.org/gnu/$TOOL and tar -xf file_from_wget

for target i686-elf - we want to build 32-bit ELF object files by default (of course we can make them flat binary later)

Set shell variables for error-pronless:
    export PREFIX="path_to_repo/cross_toolchain"
    export TARGET=i686-elf
    export PATH="$PREFIX/bin:$PATH"       # tips: 1) create empty bin directory 2) add this to ~/.bashrc

binutils:
    mkdir build-binutils
    cd build-binutils
    ../binutils-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
    make
    make install
i686-elf-as etc binutils tools are ready to use.

gdb:
    mkdir build-gdb
    cd build-gdb
    ../gdb-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-werror
    make all-gdb
    make install-gdb

gcc:
    mkdir build-gcc
    cd build-gcc
    ../gcc-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
    make all-gcc
    make all-target-libgcc
    make install-gcc
    make install-target-libgcc

source: https://wiki.osdev.org/GCC_Cross-Compiler

Providen "libc" files: float.h, iso646.h, limits.h, stdalign.h, stdarg.h, stdbool.h, stddef.h, stdint.h and stdnoreturn.h (as of C11)
