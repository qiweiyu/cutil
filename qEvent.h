#ifndef _Q_EVENT_H
#define _Q_EVENT_H

#include <sys/select.h>
#include "qDict.h"

typedef struct qFileEvent {
	int fd;
	char readable;
	char writeable;
	void (*readProc)(int fd, void *client); 
	void (*writeProc)(int fd, void *client); 
} qFileEvent;

typedef struct qEvent {
	fd_set readSet;
	fd_set writeSet;
	qDict *fileEventDict;
	int maxFd;
} qEvent;

qEvent *qCreateEvent();
qFileEvent *qCreateFileEvent(qEvent *event, int fd, char readable, char writeable, void (*readProc)(int fd, void *client), void(*writeProc)(int fd, void *client));

void qRmFileEvent(qEvent *event, int fd);

void qFreeEvent(qEvent *event);
void qFreeFileEvent(qFileEvent *fileEvent);

#endif
