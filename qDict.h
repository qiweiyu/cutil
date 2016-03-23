#ifndef _Q_DICT_H
#define _Q_DICT_H
#include "qString.h"

typedef struct qDictKey {
	union key {
		qString *strKey;
		size_t intKey;
	} key;
	char useStr;
} qDictKey;

typedef struct qDictNode {
	qDictKey *key;	
	void *value;
	struct qDictNode *next;
} qDictNode;

typedef struct qDict {
	size_t nodeCount;
	size_t bucketCount;
	qDictNode **bucket;
} qDict;

qDict *qCreateDict();
void qFreeDict(qDict *dict);

int qAddValueToDictByStrKey(qDict *dict, char *key, void *value);
int qAddValueToDictByIntKey(qDict *dict, size_t key, void *value);
int qRmValueFromDictByStrKey(qDict *dict, char *key);
int qRmValueFromDictByIntKey(qDict *dict, size_t key);

void *qFindValueFromDictByStrKey(qDict *dict, char *key);
void *qFindValueFromDictByIntKey(qDict *dict, size_t key);

#endif
