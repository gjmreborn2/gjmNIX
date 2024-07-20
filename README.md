# gjmNIX

# Brief description

UNIX, Linux-like operating system created and developed from scratch for educational purposes as a hobby.

# Steps to build and run on qemu emulator (using kvm and Fedora 20 host OS)
1. Prerequisites:
    - Fedora 20 host OS (used by myself) with kvm activated (test it by `lsmod | grep kvm`, should be `kvm` and `kvm_amd` or intel equivalent)
    - qemu for target hardware architecture (install it by `sudo dnf install qemu -y`), for example following executables:
        - qemu-system-i386 (care: qemu-i386 is user-space emulator!)
        - qemu-system-x86_64
    - GNU toolchain, GNU binutils such as gcc, as, as86 etc. cross-compiled as stated in docs
    - GRUB 2
    - xorriso for cdrom iso
2. Build stage:

# Tasks to be done
* The documentation for GCC says you are required to implement the functions memset, memcpy, memcmp and memmove yourself in freestanding mode.
* Add user-space, package-manager etc.
* Add UEFI integration
* Run kernel on the real hardware instead of qemu all the time
* Add support for other architectures such as x86_64, difference between Intel and ARM chips, ARM, AVR (as way to begin studying domain of embedded systems)
* (!!) Write build tool in python (os module etc) and don't use Make build system (!!)
