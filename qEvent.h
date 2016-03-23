#ifndef _Q_EVENT_H
#define _Q_EVENT_H

#include <sys/select.h>
#include "qDict.h"

typedef struct qEvent {
	fd_set readSet;
	fd_set writeSet;
	qDict *fileReadEventDict;
	qDict *fileWriteEventDict;
	int maxFd;
	char stop;
} qEvent;

typedef void eventProc(qEvent *event, int fd, void *client);

typedef struct qFileEvent {
	int fd;
	void *client;
	eventProc *proc;
} qFileEvent;

qEvent *qCreateEvent();
qFileEvent *qCreateFileReadEvent(qEvent *event, int fd, eventProc *proc, void *client);
qFileEvent *qCreateFileWriteEvent(qEvent *event, int fd, eventProc *proc, void *client);

void qRmFileReadEvent(qEvent *event, int fd);
void qRmFileWriteEvent(qEvent *event, int fd);

void qFreeEvent(qEvent *event);
void qFreeFileEvent(qFileEvent *fileEvent);

void qEventLoop(qEvent *event);

#endif
