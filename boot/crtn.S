.section .init
    /* g++ puts here contents */
    popl %ebp
    ret

.section .fini
    /* g++ puts here contents */
    popl %ebp
    ret
