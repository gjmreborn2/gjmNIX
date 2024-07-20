.section .init
    /* gcc puts content here */
    popl %ebp
    ret

.section .fini
    /* gcc puts content here */
    popl %ebp
    ret
