#include <string.h>
#include "qString.h"

#define PREFIX_SIZE typeof(qString)

static qString *qCalRealStringPtr(qString *ptr) {
	return (qString *)((void *)ptr - PREFIX_SIZE);
}

static size_t calRecStringLen(size_t len) {
	size_t i, size = 1;
	for(i = 0; i < sizeof(size_t) && len > size; i++)
		size = size << 1;
	return size;
}

qString *qCreateString(char *src) {
	size_t len = strlen(src);
	size_t size = calRecStringLen(len);
	size_t emptyLength
}
