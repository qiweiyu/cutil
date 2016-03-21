#ifndef _Q_STRING_H
#define _Q_STRING_H

#include "qMalloc.h"
#include "qLinkList.h"

typedef char qString;

char *strCpyByLimit(char *dst, char *src, size_t limit);

qString *qCreateString(char *src);
void qFreeString(qString *str);

qString *qCatString(qString *dst, char *src);
qLinkList *qExplodeString(qString *string, char *delimiter);

#endif
