#ifndef __Q_MALLOC_H
#define __Q_MALLOC_H

#include <stddef.h>

void *qMalloc(size_t size);
void *qCalloc(size_t n, size_t size);
void *qRealloc(void *ptr, size_t size);
void qFree(void *ptr);
long long unsigned int qGetUsedMemory();

#endif
