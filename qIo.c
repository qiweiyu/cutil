#include <unistd.h>
#include "qMalloc.h"
#include "qString.h"

#define MAX_IO_BUFF 512*1024*1024
#define PER_IO_BUFF 4096

qString *qReadInBuff(int fd) {
	char buff[PER_IO_BUFF];
	qString *readStr = qCreateString("");
	int count;
	long long unsigned total = 0;
	while((0 < (count = read(fd, buff, PER_IO_BUFF))) && total < MAX_IO_BUFF) {
		readStr = qCatString(readStr, buff);
		total += count;
	}
	if(count == 0) {
		return readStr;
	}
	else {
		return NULL;
	}
}
