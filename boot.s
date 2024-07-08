; 16-bit real mode 512B bootsector.
; It has to load kernel into memory 0x10000 and relocate it to RAM 0x0000.
; Finally it enters protected mode (with temporary GDT) and jumps to kernel.
; Author: gjm

BOOTSEG = 0x07C0
SYSSEG = 0x1000
SYSLEN_SECTORS = 17

entry start
start:  ; we're loaded at 0x7C00 by BIOS
    jmpi go, #BOOTSEG
go:
    mov ax, cs
    mov ds, ax
    mov ss, ax
    mov sp, #0x400

load_kernel:    ; load kernel at 0x10000
; BIOS 0x13 interrupt function 2
; AH - function no, AL - number of sectors to read
; DH - head no, DL - drive no
; CH - low 8 bits of 10 bits track no, CL - high 2 bits (bit 7th, 6th) of 10 bits track no, bits 0-5 represent start sector
; ES:BX - read-in buffer location (0x1000:0x0000)
    mov ax, #SYSSEG         ; for es
    mov es, ax
    mov ah, #0x2
    mov al, #SYSLEN_SECTORS     ; if error then try mov ax, #0x200+SYSLEN
    mov bx, #0
    mov cx, #0x0002
    mov dh, #0
    mov dl, #0
    int 0x13
    jnc ok_load             ; if no carry flag set (BIOS indicates error by setting CF) then it's ok
die:    jmp die

ok_load:
; Relocate kernel code from 0x10000 to 0x0000 in RAM.
; Move from DS:SI (0x1000:0x0000) to ES:DI (0x0000:0x0000) by using rep movw (one word at a time)
; CX - number of blocks (movw - block is equal to a word) to move
    cli
    mov ax, #SYSSEG
    mov ds, ax
    mov si, #0
    mov ax, #0
    mov es, ax
    mov di, #0
    mov cx, #0x1000         ; 0x1000 * 2 bytes (word) = 8 KB (kernel code < 8 KB)
    rep
    movw

; Load temp IDT and GDT
    mov ax, #BOOTSEG
    mov ds, ax
    lgdt gdt_48
    lidt idt_48

; Enter protected mode and jump to kernel
    mov ax, #0x0001
    lmsw ax
    jmpi 0, 8       ; far jump to 0x0000 with CS=0x8 (second GDT descriptor - code segment)

; Temp GDT with 3 segment descriptors (not used, code, data)
gdt:
    .word 0, 0, 0, 0
    ; Code segment descriptor: base address = 0, size (limit) = 8 MB, flags = readable, no-writable, executable, 80386 (32-bit), granularity (units in limit are 4 KB blocks)
    .word 0x07FF, 0x0000, 0x9A00, 0x00C0
    ; Data segment descriptor: base address = 0, size (limit) = 8 MB, flags = readable, writable, no-executable, 80386 (32-bit), granularity
    .word 0x07FF, 0x0000, 0x9200, 0x00C0
gdt_48:
    .word 0x07FF, 0x7C00 + gdt, 0x0000

; Temp IDT: nulled
idt_48:
    .word 0, 0, 0

.org 510
.word 0xAA55
