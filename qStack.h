#ifndef _Q_STACK_H
#define _Q_STACK_H

#include "qLinkList.h"

typedef struct qStack {
	qLinkList *list;
} qStack;

qStack *qCreateStack(void (*free)(void *value));
void qFreeStack(qStack *stack);

void *qStackPop(qStack *stack);
void qStackPush(qStack *stack, void *value);

#endif
