#include <gjmnix/drivers/console.h>

static void itoa(char *buf, int base, int n) {
    char *p = buf;
    char *p1, *p2;
    unsigned long ud = n;
    int divisor = 10;

    if(base == 'd' && n < 0) {
        *p++ = '-';
        buf++;
        ud = -n;
    } else if(base == 'x') {
        divisor = 16;
    }

    do {
        int remainder = ud % divisor;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
    } while(ud /= divisor);

    /* terminate buf */
    *p = 0;

    /* reverse buf */
    p1 = buf;
    p2 = p - 1;
    while(p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void kprintf(const char *format, ...) {
    char **arg = (char **) &format;
    int ch;
    char buf[20];

    arg++;
    while((ch = *format++) != 0) {
        if(ch != '%') {
            putchar(ch);
        } else {
            char *p, *p2;
            int pad0 = 0, pad = 0;

            ch = *format++;
            if(ch == '0') {
                pad0 = 1;
                ch = *format++;
            }
            if(ch >= '0' && ch <= '9') {
                pad = ch - '0';
                ch = *format++;
            }

            switch(ch) {
                case 'd':
                case 'u':
                case 'x':
                    itoa(buf, ch, *((int *) arg++));
                    p = buf;
                    goto string;
                    break;
                case 's':
                    p = *arg++;
                    if(!p) {
                        p = "(null)";
                    }
                string:
                    for(p2 = p; *p2; p2++)
                        ;
                    for(; p2 < p + pad; p2++)
                        putchar(pad0 ? '0' : ' ');
                    while(*p)
                        putchar(*p++);
                    break;
                default:
                    putchar(*((int *) arg++));
                    break;
            }
        }
    }
}