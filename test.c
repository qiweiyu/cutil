#include <stdio.h>
#include <string.h>
#include "qMalloc.h"
#include "qLinkList.h"
#include "qStack.h"
#include "qString.h"
#include "qDict.h"
#include "qEvent.h"

int main() {
	printf("sizeof %lu\n", sizeof(qDictKey));
	printf("Memory Used %llu \n", qGetUsedMemory());
	return 0;
}
