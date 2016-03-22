#ifndef _Q_LINK_LIST_H
#define _Q_LINK_LIST_H

#include "qMalloc.h"

typedef struct qLinkListNode {
	void *value;
	struct qLinkListNode *prev;
	struct qLinkListNode *next;
} qLinkListNode;

typedef struct qLinkList {
	qLinkListNode *head;
	qLinkListNode *tail;
	void (*free) (void *value);
	int (*cmp) (void *valueA, void *valueB);
} qLinkList;

qLinkList *qCreateLinkList(void (*free)(void *value), int (*cmp)(void *valueA, void *valueB));
void qFreeLinkList(qLinkList *list);
qLinkList *qInsertNodeToLinkList(qLinkList *list, void *value);
qLinkList *qRemoveNodeFromLinkList(qLinkList *list, void *value);

#endif
