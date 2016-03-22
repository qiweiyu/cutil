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
	qAddValueToDict(dict, "ABC", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "1", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "2", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "3", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "4", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "5", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "6", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "7", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "8", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "9", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "a", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qAddValueToDict(dict, "b", s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qRmValueFromDict(dict, "ABC");
	printf("Memory Used %llu \n", qGetUsedMemory());
	printf("Data is %s\n", qFindValueFromDict(dict, "9"));
	qFreeDict(dict);
	qFree(s);
	printf("Memory Used %llu \n", qGetUsedMemory());
	return 0;
}
