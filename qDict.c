#include <string.h>
#include "qDict.h"
#include "qString.h"
#include "qMalloc.h"

#define MIN_BUCKET_COUNT 17

static size_t qStrHash(char *key) {
	size_t hash = 5381;
	while(*key) {
		hash += (hash << 5) + (*key++);
	}
	return hash;
}

static size_t qHash(qDictKey *key, size_t size) {
	size_t hash;
	if(key->useStr) {
		hash = qStrHash(key->key.strKey);
	}
	else {
		hash = key->key.intKey;
	}
	return hash % size;
}

static void qFreeDictKey(qDictKey *key) {
	if(key->useStr) {
		qFreeString(key->key.strKey);
	}
	qFree(key);
}

static int qDictKeyCmp(qDictKey *keyA, qDictKey *keyB) {
	//str > number	
	if(keyA->useStr && keyB->useStr) {
		return strcmp(keyA->key.strKey, keyB->key.strKey);
	}
	else if(keyA->useStr && keyB->useStr == 0) {
		return 1;
	}
	else if(keyA->useStr == 0 && keyB->useStr) {
		return -1;
	}
	else {
		size_t diff = keyA->key.intKey - keyB->key.intKey;
		return diff == 0 ? 0 : (diff > 0 ? 1 : -1);
	}
}

qDict *qCreateDict(void (*freeValue)(void *value)) {
	size_t bucketCount = MIN_BUCKET_COUNT;
	qDict *dict = qMalloc(sizeof(qDict));
	dict->nodeCount = 0;
	dict->bucketCount = bucketCount;
	dict->bucket = qCalloc(bucketCount, sizeof(qDictNode*));
	dict->freeValue = freeValue;
	return dict;
}

void qFreeDict(qDict *dict) {
	qDictNode *node, *bucket;
	for(size_t i = 0; i < dict->bucketCount; i++) {
		bucket = node = *(dict->bucket+i);
		while(node) {
			bucket = node->next;
			qFreeDictKey(node->key);
			if(dict->freeValue) {
				dict->freeValue(node->value);
			}
			qFree(node);
			node = bucket;	
		}
	}
	qFree(dict->bucket);
	qFree(dict);
}

static void qRehash(qDict *dict) {
	size_t bucketCount = 0;
	if(dict->nodeCount) {
		if((dict->nodeCount / dict->bucketCount) > 2) {
			bucketCount = ((dict->bucketCount-1) << 1) + 1;
		}
		else if((dict->bucketCount / dict->nodeCount) > 2 && dict->bucketCount > MIN_BUCKET_COUNT) {
			bucketCount = ((dict->bucketCount-1) >> 1) + 1;
		}
		else {
			return ;
		}
	}
	else {
		return ;
	}
	qDictNode **oldBucket = dict->bucket;
	size_t oldBucketCount = dict->bucketCount;
	dict->bucket = qCalloc(bucketCount, sizeof(qDictNode*));
	dict->bucketCount = bucketCount;
	qDictNode *node, *newNode, *last;
	for(size_t i = 0; i < oldBucketCount; i++) {
		node = *(oldBucket + i);
		while(node) {
			size_t hash = qHash(node->key, dict->bucketCount);
			last = node->next;
			node->next = NULL;
			newNode = *(dict->bucket+hash);
			while(newNode && newNode->next) newNode = newNode->next;
			if(newNode == NULL) {
				*(dict->bucket+hash) = node;
			}
			else {
				newNode->next = node;
			}
			node = last;
		}
	}
	qFree(oldBucket);
}

static int qAddValueToDict(qDict *dict, qDictKey *key, void *value) {
	size_t hash = qHash(key, dict->bucketCount);
	qDictNode *node = *(dict->bucket+hash);
	qDictNode *last = *(dict->bucket+hash);
	while(node) {
		if(qDictKeyCmp(node->key, key) == 0) {
			qFreeDictKey(key);
			if(dict->freeValue) {
				dict->freeValue(value);
			}
			return -1;
		}
		last = node;
		node = node->next;
	}
	node = qMalloc(sizeof(qDictNode));
	node->key = key;
	node->value = value;
	node->next = NULL;
	dict->nodeCount ++;
	if(last) {
		last->next = node;
	}
	else {
		*(dict->bucket+hash) = node;
	}
	qRehash(dict);
	return 1;
}

int qAddValueToDictByStrKey(qDict *dict, char *key, void *value) {
	qDictKey *dictKey = qMalloc(sizeof(qDictKey));
	dictKey->key.strKey = qCreateString(key);
	dictKey->useStr = 1;
	return qAddValueToDict(dict, dictKey, value);
}

int qAddValueToDictByIntKey(qDict *dict, size_t key, void *value) {
	qDictKey *dictKey = qMalloc(sizeof(qDictKey));
	dictKey->key.intKey = key;
	dictKey->useStr = 0;
	return qAddValueToDict(dict, dictKey, value);
}

static int qRmValueFromDict(qDict *dict, qDictKey *key) {
	size_t hash = qHash(key, dict->bucketCount);
	qDictNode *node = *(dict->bucket+hash);
	qDictNode *last = NULL;
	while(node) {
		if(qDictKeyCmp(node->key, key) == 0) {
			if(last == NULL) {
				*(dict->bucket+hash) = node->next;
			}
			else {
				last->next = node->next;
			}
			qFreeDictKey(node->key);
			qFreeDictKey(key);
			if(dict->freeValue) {
				dict->freeValue(node->value);
			}
			qFree(node);
			dict->nodeCount--;
			qRehash(dict);
			return 1;
		}
		last = node;
		node = node->next;
	}
	qFreeDictKey(node->key);
	qFreeDictKey(key);
	return -1;
}

int qRmValueFromDictByStrKey(qDict *dict, char *key) {
	qDictKey *dictKey = qMalloc(sizeof(qDictKey));
	dictKey->key.strKey = qCreateString(key);
	dictKey->useStr = 1;
	return qRmValueFromDict(dict, dictKey);
}

int qRmValueFromDictByIntKey(qDict *dict, size_t key) {
	qDictKey *dictKey = qMalloc(sizeof(qDictKey));
	dictKey->key.intKey = key;
	dictKey->useStr = 0;
	return qRmValueFromDict(dict, dictKey);
}

static void *qFindValueFromDict(qDict *dict, qDictKey *key) {
	size_t hash = qHash(key, dict->bucketCount);
	qDictNode *node = *(dict->bucket+hash);
	while(node) {
		if(qDictKeyCmp(node->key, key) == 0) {
			qFreeDictKey(key);
			return node->value;
		}
		node = node->next;
	}
	qFreeDictKey(key);
	return NULL;
}

void *qFindValueFromDictByStrKey(qDict *dict, char *key) {
	qDictKey *dictKey = qMalloc(sizeof(qDictKey));
	dictKey->key.strKey = qCreateString(key);
	dictKey->useStr = 1;
	return qFindValueFromDict(dict, dictKey);
}

void *qFindValueFromDictByIntKey(qDict *dict, size_t key) {
	qDictKey *dictKey = qMalloc(sizeof(qDictKey));
	dictKey->key.intKey = key;
	dictKey->useStr = 0;
	return qFindValueFromDict(dict, dictKey);
}
