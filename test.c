#include <stdio.h>
#include <string.h>
#include "qMalloc.h"
#include "qLinkList.h"
#include "qStack.h"

int main() {
	void *p = qMalloc(10);
	printf("Memory Used %llu \n", qGetUsedMemory());
	p = qRealloc(p, 20);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qFree(p);
	printf("Memory Used %llu \n", qGetUsedMemory());
	return 0;
	qStack *stack = qCreateStack(qFree);
	char *c;
	for(int i = 0; i< 10; i++) {
		c = qCalloc(8, sizeof(char));
		sprintf(c, "this %d", i);
		qStackPush(stack, c);
		printf("Memory Used %llu \n", qGetUsedMemory());
	}
	c = qStackPop(stack);
	printf("Memory Used %llu \n", qGetUsedMemory());
	qFree(c);
	printf("Memory Used %llu \n", qGetUsedMemory());
	
	qLinkListNode *node = stack->list->head;
	while(node != NULL) {
		printf("%s\n", node->value);
		node = node->next;
	}

	qFreeStack(stack);
	printf("Memory Used %llu \n", qGetUsedMemory());
	return 0;
}
