#include "qStack.h"

qStack *qCreateStack(void(*free)(void *value)) {
	qStack *p = qMalloc(sizeof(qStack)); 
	p->list = qCreateLinkList(free, NULL);
	return p;
}

void qFreeStack(qStack *stack) {
	qFreeLinkList(stack->list);
	qFree(stack);
}

void *qStackPop(qStack *stack) {
	qLinkListNode *node = stack->list->tail;
	if(node == NULL)
		return NULL;
	stack->list->tail = node->prev;
	if(node->prev) {
		node->prev->next = NULL;
	}
	void *val = node->value;
	qFree(node);
	return val;
}

void qStackPush(qStack *stack, void *value) {
	qInsertNodeToLinkList(stack->list, value);
}
