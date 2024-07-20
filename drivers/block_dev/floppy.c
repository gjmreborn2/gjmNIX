#include <klib/kutils.h>
#include <gjmnix/drivers/floppy.h>

/*
 * drive letter A
 *
 * Close controller, disable DMA, interrupt of FDC and also turn off motor
 */
void kill_floppy_motor() {
	outb(FLOPPY_PORT_DOR, 0);
}
