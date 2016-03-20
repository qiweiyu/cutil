#include <stdlib.h>
#include <stdio.h>
#include "qMalloc.h"

#define PREFIX_SIZE sizeof(size_t)

static long long unsigned int usedMemory = 0;
static void outOfMemory(size_t size) {
	fprintf(stderr, "Try To alloc %zu Bytes Failed!\n", size);
	fflush(stderr);
	abort();
}

void *qMalloc(size_t size) {
	void *ptr = malloc(size+PREFIX_SIZE);
	if(ptr == NULL) {
		outOfMemory(size);
	}
	void *returnPtr = ptr+PREFIX_SIZE;
	*(size_t *)ptr = size;
	usedMemory += size+PREFIX_SIZE;
	return returnPtr;
}

void *qCalloc(size_t n, size_t size) {
	size = n*size;
	void *ptr = calloc(1, size+PREFIX_SIZE);
	if(ptr == NULL) {
		outOfMemory(size);
	}
	void *returnPtr = ptr+PREFIX_SIZE;
	*(size_t *)ptr = size;
	usedMemory += size+PREFIX_SIZE;
	return returnPtr;
}

void *qRealloc(void *ptr, size_t size) {
	void *realPtr = ptr-PREFIX_SIZE;
	int oldSize = *(size_t *)realPtr;
	realPtr = realloc(realPtr, size+PREFIX_SIZE);
	if(realPtr == NULL) {
		outOfMemory(size);
	}
	ptr = realPtr+PREFIX_SIZE;
	*(size_t*)realPtr = size;
	usedMemory += size-oldSize;
	return ptr;
}

void qFree(void *ptr) {
	void *realPtr = ptr-PREFIX_SIZE;
	int size = *(size_t *)realPtr;
	usedMemory -= (size + PREFIX_SIZE);
	free(realPtr);
}

long long unsigned int qGetUsedMemory() {
	return usedMemory;
}
