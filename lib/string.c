#include <string.h>

#include <klib/kutils.h>

void *memmove(void *pdest, const void *psrc, size_t size) {
	byte *dest = (byte *) pdest;
	const byte *src = (const byte *) psrc;

	if(dest < src) {
		for(size_t i = 0; i < size; i++) {
			dest[i] = src[i];
		}
	} else {
		for(size_t i = size; i != 0; i--) {
			dest[i - 1] = src[i - 1];
		}
	}
	return pdest;
}

size_t strlen(const char *str) {
	size_t len = 0;
	
	while(str[len]) {
		len++;
	}
	return len;
}

int memcmp(const void *aptr, const void *bptr, size_t size) {
	const byte *a = (const byte *) aptr;
	const byte *b = (const byte *) bptr;

	for(size_t i = 0; i < size; i++) {
		if(a[i] < b[i]) {
			return -1;
		} else if(b[i] < a[i]) {
			return 1;
		}
	}
	return 0;
}

void *memset(void *bufptr, int value, size_t size) {
	byte *buf = (byte *) bufptr;

	for(size_t i = 0; i < size; i++) {
		buf[i] = (byte) value;
	}
	return bufptr;
}

void *memcpy(void *restrict pdest, const void *restrict psrc, size_t size) {
	byte *dest = (byte *) pdest;
	const byte *src = (const byte *) psrc;

	for(size_t i = 0; i < size; i++) {
		dest[i] = src[i];
	}
	return pdest;
}

