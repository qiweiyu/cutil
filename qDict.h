#ifndef _Q_DICT_H
#define _Q_DICT_H
#include "qString.h"

typedef struct qDictNode {
	qString *key;	
	void *value;
	struct qDictNode *next;
} qDictNode;

typedef struct qDict {
	size_t nodeCount;
	size_t bucketCount;
	size_t (*hash)(char *key);
	qDictNode **bucket;
} qDict;

qDict *qCreateDict(size_t (*hash)(char *key));
void qFreeDict(qDict *dict);

int qAddValueToDict(qDict *dict, char *key, void *value);
int qRmValueFromDict(qDict *dict, char *key);

void *qFindValueFromDict(qDict *dict, char *key);

#endif
