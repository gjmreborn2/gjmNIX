#define COLUMNS     80
#define LINES       24
#define ATTRIBUTE   7
#define VRAM        0xB8000

static int xpos;
static int ypos;
static volatile unsigned char *video;

void console_init() {
    video = (unsigned char *) VRAM;
    xpos = 0;
    ypos = 0;
}

void console_cls() {
    for(int i = 0; i < COLUMNS * LINES * 2; i++) {
        *(video + i) = 0;
    }
}

void putchar(int ch) {
    if(ch == '\n' || ch == '\r') {
    newline:
        xpos = 0;
        ypos++;
        if(ypos >= LINES) {
            ypos = 0;
        }
        return;
    }

    *(video + (xpos + ypos * COLUMNS) * 2) = ch & 0xFF;
    *(video + (xpos + ypos * COLUMNS) * 2 + 1) = ATTRIBUTE;

    xpos++;
    if(xpos >= COLUMNS) {
        goto newline;
    }
}