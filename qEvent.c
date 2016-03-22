#include <sys/select.h>
#include <stdio.h>
#include "qEvent.h"
#include "qMalloc.h"
#include "qDict.h"

qEvent *qCreateEvent() {
	qEvent *event = qMalloc(sizeof(qEvent));
	FD_ZERO(&(event->readSet));
	FD_ZERO(&(event->writeSet));
	event->fileEventDict = qCreateDict(NULL);
	return event;
}

qFileEvent *qCreateFileEvent(qEvent *event, int fd, char readable, char writeable, void (*readProc)(int fd, void *client), void(*writeProc)(int fd, void *client)) {
	qFileEvent *fileEvent = qMalloc(sizeof(qFileEvent));
	fileEvent->fd = fd;
	fileEvent->readable = readable;
	fileEvent->writeable = writeable;
	fileEvent->readProc = readProc;
	fileEvent->writeProc = writeProc;
	char key[10];
	sprintf(key, "%d", fd);
	qAddValueToDict(event->fileEventDict, key, fileEvent);
	event->maxFd = event->maxFd > fd ? event->maxFd : fd;
	return fileEvent;
}

void qRmFileEvent(qEvent *event, int fd) {
	char key[10];
	sprintf(key, "%d", fd);
	qFileEvent *fileEvent = qFindValueFromDict(event->fileEventDict, key);
	if(fileEvent) {
		qRmValueFromDict(event->fileEventDict, key);
		qFreeFileEvent(fileEvent);
	}
}

void qFreeEvent(qEvent *event) {
	qFreeDict(event->fileEventDict);
	qFree(event);
}

void qFreeFileEvent(qFileEvent *fileEvent) {
	qFree(fileEvent);
}
