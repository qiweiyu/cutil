#include <string.h>
#include "qString.h"
#include "qLinkList.h"

char *strCpyByLimit(char *dst, char *src, size_t limit) {
	size_t i = 0;
	for(i = 0; i < limit; i++) {
		*(dst+i) = *(src+i);
	}
	*(dst+i) = '\0';
	return dst;
}

qString *qCreateString(char *src) {
	size_t len = strlen(src);
	qString *ptr = qMalloc(len);
	strcpy(ptr, src);
	return ptr;
}

void qFreeString(qString *str) {
	qFree(str);
}

qString *qCatString(qString *dst, char *src) {
	size_t srcLen = strlen(src);
	size_t dstLen = strlen(dst);
	dst = qRealloc(dst, dstLen+srcLen);
	dst = strcat(dst, src);
	return dst;
}

qLinkList *qExplodeString(qString *string, char *delimiter) {
	size_t maxLen = strlen(string), delimiterLen = strlen(delimiter), len;
	char *buff = qMalloc(maxLen);
	char *pos = string, *newPos;
	qLinkList *list = qCreateLinkList((void (*)(void *))qFreeString, (int (*)(void *, void *))strcmp);
	do {
		newPos = strstr(pos, delimiter);
		if(newPos == NULL) {
			len = strlen(pos);
		}
		else {
			len = newPos-pos;
		}
		buff = strCpyByLimit(buff, pos, len);
		qString *str = qCreateString(buff);
		list = qInsertNodeToLinkList(list, str);
		pos = newPos;
	}while(pos && (pos= pos + delimiterLen));
	
	qFree(buff);
	return list;
}
