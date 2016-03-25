#include <errno.h>
#include <unistd.h>
#include <sys/fcntl.h>
#include "qMalloc.h"
#include "qString.h"

#define MAX_IO_BUFF 512*1024*1024
#define PER_IO_BUFF 4096

static int qSetNoDelayRead(int fd) {
	fcntl(fd, F_SETFL, O_NONBLOCK);
	return fd;
}

static int qReadBuff(int fd, char *buff, size_t len) {
	qSetNoDelayRead(fd);
	int count = read(fd, buff, len);
	if(count == -1 && errno == EAGAIN) {
		count = 0;
	}
	return count;
}

static int qRefillBuff(qReadBuff *buff) {
	char bak[Q_READ_BUFF_SIZE];
	size_t unusedCount = buff->len-buff->pos;
	if(unusedCount > 0) {
		strCpyByLimit(bak, buff->buff+buff->pos, unusedCount);
		strCpyByLimit(buff->buff, bak, unusedCount);
	}
	buff->pos = 0;
	size_t readCount = Q_READ_BUFF_SIZE - unusedCount;
	if(readCount > 0) {
		readCount = qReadBuff(buff->fd, buff->buff+buff->pos, readCount);
	}
	if(readCount >= 0) {
		buff->len = unusedCount+readCount;
	}
	else {
		buff->len = unusedCount;
	}
	buff->buff[buff->len] = '\0';
	return readCount;
}

qReadBuff *qCreateReadBuff(int fd) {
	qReadBuff *buff = qMalloc(sizeof(readbuff));
	memset(buff->buff, 0, Q_READ_BUFF_SIZE);
	buff->len = 0;
	buff->pos = 0;
	buff->fd = fd;
	buff->buffEnd = '\0';
	return buff;
}

void qFreeReadBuff(qReadBuff *buff) {
	qFree(buff);
}

qString *qReadSize(int fd, qReadBuff *buff, size_t size) {
	qString *str = qMalloc(size+1);
	do {
		qRefillBuff(buff);
	}
}

qString *qReadUntil(int fd, qReadBuff *buff, char *untilStr) {}

qString *qReadAll(int fd) {
	char buff[PER_IO_BUFF];
	qString *readStr = qCreateString("");
	int count;
	long long unsigned total = 0;
	fcntl(fd, F_SETFL, O_NONBLOCK);
	while((0 < (count = read(fd, buff, PER_IO_BUFF))) && total < MAX_IO_BUFF) {
		readStr = qCatString(readStr, buff);
		total += count;
	}
	if((count == -1 && errno == EAGAIN) || count >= 0) {
		return readStr;
	}
	else {
		return NULL;
	}
}
