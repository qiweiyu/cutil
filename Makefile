CC = gcc -g

default : test clean

test : qStack.o qLinkList.o qMalloc.o qString.o

%.o : qMalloc.o %.c

qMalloc.o : qMalloc.c

.PHONY : clean

clean :
	rm ./*.o
