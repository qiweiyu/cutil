#include "qLinkList.h"

qLinkList *qCreateLinkList(void (*free)(void *value), int (*cmp)(void *valueA, void *valueB)) {
	qLinkList *p = qMalloc(sizeof(qLinkList));
	p->head = NULL;
	p->tail = NULL;
	p->free = free;
	p->cmp = cmp;
	return p;
}

void qFreeLinkList(qLinkList *list) {
	qLinkListNode *next, *node = list->head;
	while(node != NULL) {
		if(list->free) {
			list->free(node->value);
		}
		next = node->next;
		qFree(node);
		node = next;
	}
	qFree(list);
}

qLinkList *qInsertNodeToLinkList(qLinkList *list, void *value) {
	qLinkListNode *node = qMalloc(sizeof(qLinkListNode));
	node->value = value;
	node->prev = NULL;
	node->next = NULL;
	if(list->head == NULL) {
		list->head = node;
		list->tail = node;
	}
	else {
		node->prev = list->tail;
		list->tail->next = node;
		list->tail = node;
	}
	return list;
}

qLinkList *qRemoveNodeFromLinkList(qLinkList *list, void *value) {
	qLinkListNode *node = list->head;
	while(node != NULL) {
		if(list->cmp && list->cmp(node->value, value) == 0) {
			break;
		}
		else {
			if(node->value == value) break;
		}
	}
	if(node != NULL) {
		if(node->prev) {
			node->prev->next = node->next;
		}
		else {
			list->head = node->next;
		}
		if(node->next) {
			node->next->prev = node->prev;
		}
		else {
			list->tail = node->next;
		}
		if(list->free) {
			list->free(node->value);
		}
		qFree(node);
	}
	return list;
}
