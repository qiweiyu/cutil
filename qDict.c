#include <string.h>
#include "qDict.h"
#include "qString.h"
#include "qMalloc.h"

#define MIN_BUCKET_COUNT 2

static size_t qHash(char *key) {
	size_t hash = 5381;
	while(*key) {
		hash += (hash << 5) + (*key++);
	}
	return hash;
}

qDict *qCreateDict(size_t (*hash)(char *key)) {
	size_t bucketCount = MIN_BUCKET_COUNT;
	qDict *dict = qMalloc(sizeof(qDict));
	dict->nodeCount = 0;
	dict->bucketCount = bucketCount;
	if(hash == NULL) {
		dict->hash = qHash;
	}
	else {
		dict->hash = hash;
	}
	dict->bucket = qCalloc(bucketCount, sizeof(qDictNode*));
	return dict;
}

void qFreeDict(qDict *dict) {
	qDictNode *node, *bucket;
	for(size_t i = 0; i < dict->bucketCount; i++) {
		bucket = node = *(dict->bucket+i);
		while(node) {
			bucket->next = node->next;
			qFree(node->key);
			qFree(node);
			node = bucket->next;	
		}
	}
	qFree(dict->bucket);
	qFree(dict);
}

static void qRehash(qDict *dict) {
	size_t bucketCount = 0;
	if(dict->nodeCount) {
		if((dict->nodeCount / dict->bucketCount) > 2) {
			bucketCount = dict->bucketCount << 1;
		}
		else if((dict->bucketCount / dict->nodeCount) > 2 && dict->bucketCount > MIN_BUCKET_COUNT) {
			bucketCount = dict->bucketCount >> 1;
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
			size_t hash = dict->hash(node->key) % dict->bucketCount;
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

int qAddValueToDict(qDict *dict, char *key, void *value) {
	size_t hash = dict->hash(key) % dict->bucketCount;
	qDictNode *node = *(dict->bucket+hash);
	qDictNode *last = *(dict->bucket+hash);
	while(node) {
		if(strcmp(node->key, key) == 0) {
			return -1;
		}
		last = node;
		node = node->next;
	}
	node = qMalloc(sizeof(qDictNode));
	node->key = qCreateString(key);
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

int qRmValueFromDict(qDict *dict, char *key) {
	size_t hash = dict->hash(key) % dict->bucketCount;
	qDictNode *node = *(dict->bucket+hash);
	qDictNode *last = NULL;
	while(node) {
		if(strcmp(node->key, key) == 0) {
			if(last == NULL) {
				*(dict->bucket+hash) = node->next;
			}
			else {
				last->next = node->next;
			}
			qFree(node->key);
			qFree(node);
			dict->nodeCount--;
			qRehash(dict);
			return 1;
		}
		last = node;
		node = node->next;
	}
	return -1;
}

void *qFindValueFromDict(qDict *dict, char *key) {
	size_t hash = dict->hash(key) % dict->bucketCount;
	qDictNode *node = *(dict->bucket+hash);
	while(node) {
		if(strcmp(node->key, key) == 0) {
			return node->value;
		}
		node = node->next;
	}
	return NULL;
}
