#include <string.h>
#include <stdint.h>

void *memmove(void *pdest, const void *psrc, size_t size) {
	uint8_t *dest = (uint8_t *) pdest;
	const uint8_t *src = (const uint8_t *) psrc;

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

void strncpy(char *dst, const char *src, size_t n) {
	size_t i = 0;
	while(i++ != n && (*dst++ = *src++))
		;
}

void strcpy(char *dst, const char *src) {
	while((*dst++ = *src++) != '\0')
		;
}

int memcmp(const void *aptr, const void *bptr, size_t size) {
	const uint8_t *a = (const uint8_t *) aptr;
	const uint8_t *b = (const uint8_t *) bptr;

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
	uint8_t *buf = (uint8_t *) bufptr;

	for(size_t i = 0; i < size; i++) {
		buf[i] = (uint8_t) value;
	}
	return bufptr;
}

void *memcpy(void *restrict pdest, const void *restrict psrc, size_t size) {
	uint8_t *dest = (uint8_t *) pdest;
	const uint8_t *src = (const uint8_t *) psrc;

	for(size_t i = 0; i < size; i++) {
		dest[i] = src[i];
	}
	return pdest;
}

