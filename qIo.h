#ifndef _Q_IO_H
#define _Q_IO_H

#include "qString.h"

#define Q_READ_BUFF_SIZE 4096

typedef struct qReadBuff{
	int fd;
	char buff[Q_READ_BUFF_SIZE];
	char buffEnd;
	size_t len;
	size_t pos;
} qReadBuff;

qReadBuff *qCreateReadBuff(int fd);
void qFreeReadBuff(qReadBuff *buff);

qString *qReadSize(int fd, qReadBuff *buff, size_t size);

qString *qReadUntil(int fd, qReadBuff *buff, char *untilStr);

qString *qReadAll(int fd);

#endif
