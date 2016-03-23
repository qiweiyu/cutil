#include <stdio.h>
#include <string.h>
#include "qMalloc.h"
#include "qLinkList.h"
#include "qStack.h"
#include "qString.h"
#include "qDict.h"

int main() {
	qDict *dict = qCreateDict(NULL);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qString *s = qCreateString("DEF");
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDictByStrKey(dict, "ABC", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qRmValueFromDictByStrKey(dict, "ABC");
	printf("Memory Used %llu \n", qGetUsedMemory());
	qFreeDict(dict);
	qFree(s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	return 0;
}
