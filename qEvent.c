#include <sys/select.h>
#include <stdio.h>
#include "qEvent.h"
#include "qMalloc.h"
#include "qDict.h"

qEvent *qCreateEvent() {
	qEvent *event = qMalloc(sizeof(qEvent));
	FD_ZERO(&(event->readSet));
	FD_ZERO(&(event->writeSet));
	event->fileReadEventDict = qCreateDict(NULL);
	event->fileWriteEventDict = qCreateDict(NULL);
	event->stop = 0;
	return event;
}

static qFileEvent *qCreateFileEvent(qEvent *event, int fd, eventProc *proc, void *client, int isRead){
	qFileEvent *fileEvent = qMalloc(sizeof(qFileEvent));
	fileEvent->fd = fd;
	fileEvent->proc = proc;
	fileEvent->client = client;
	qDict *eventDict;
	if(isRead) {
		eventDict = event->fileReadEventDict;
		FD_SET(fd, &event->readSet);
	}
	else {
		eventDict = event->fileWriteEventDict;
		FD_SET(fd, &event->writeSet);
	}
	qAddValueToDictByIntKey(eventDict, fd, fileEvent);
	event->maxFd = event->maxFd > fd ? event->maxFd : fd;
	return fileEvent;
}

qFileEvent *qCreateFileReadEvent(qEvent *event, int fd, eventProc *proc, void *client) {
	return qCreateFileEvent(event, fd, proc, client, 1);
}

qFileEvent *qCreateFileWriteEvent(qEvent *event, int fd, eventProc *proc, void *client) {
	return qCreateFileEvent(event, fd, proc, client, 0);
}

static void qRmFileEvent(qEvent *event, int fd, int isRead) {
	qDict *eventDict;
	if(isRead) {
		eventDict = event->fileReadEventDict;
	}
	else {
		eventDict = event->fileWriteEventDict;
	}
	qFileEvent *fileEvent = qFindValueFromDictByIntKey(eventDict, fd);
	if(fileEvent) {
		qRmValueFromDictByIntKey(eventDict, fd);
		qFreeFileEvent(fileEvent);
	}
}

void qRmFileReadEvent(qEvent *event, int fd) {
	qRmFileEvent(event, fd, 1);
}

void qRmFileWriteEvent(qEvent *event, int fd) {
	qRmFileEvent(event, fd, 0);
}

void qFreeEvent(qEvent *event) {
	qFreeDict(event->fileReadEventDict);
	qFreeDict(event->fileWriteEventDict);
	qFree(event);
}

void qFreeFileEvent(qFileEvent *fileEvent) {
	qFree(fileEvent);
}

static void qProcessEvent(qEvent *event, int fd, int isRead) {
	qDict *eventDict;
	if(isRead) {
		eventDict = event->fileReadEventDict;
		FD_CLR(fd, &event->readSet);
	}
	else {
		eventDict = event->fileWriteEventDict;
		FD_CLR(fd, &event->writeSet);
	}
	qFileEvent *fileEvent = qFindValueFromDictByIntKey(eventDict, fd);
	if(fileEvent) {
		fileEvent->proc(event, fd, fileEvent->client);
		qRmFileEvent(event, fd, isRead);
	}
}

void qEventLoop(qEvent *event) {
	while(event->stop == 0) {
		fd_set readSet = event->readSet;
		fd_set writeSet = event->writeSet;
		int retval = select(event->maxFd+1, &readSet, &writeSet, NULL, NULL);	
		if(retval > 0) {
			for(int i = 0; i < event->maxFd+1; i++) {
				if(FD_ISSET(i, &readSet)) {
					qProcessEvent(event, i, 1);
				}
				if(FD_ISSET(i, &writeSet)) {
					qProcessEvent(event, i, 0);
				}
			}
		}
	}
}
