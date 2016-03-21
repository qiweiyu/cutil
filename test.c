#include <stdio.h>
#include <string.h>
#include "qMalloc.h"
#include "qLinkList.h"
#include "qStack.h"
#include "qString.h"

int main() {
	char str[] = "Thereabcareabcaabclotabcofabcwordsabcinabctheabcstring.abc";
	char del[] = "abc";
	qLinkList *list = qExplodeString(str, del);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qLinkListNode *n = list->head;
	while(n) {
		printf("%s\n", n->value);
		n = n->next;
	}
	qFreeLinkList(list);
	printf("Memory Used %llu \n", qGetUsedMemory());
	return 0;
}
