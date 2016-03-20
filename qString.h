#include "qMalloc.h"

typedef qString char;

qString *qCreateString(char *src);
qString *qStringCat(qString *dst, char *src);
void qFreeString(qString *str);
