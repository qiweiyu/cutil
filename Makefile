CC = gcc

test : qStack.o qLinkList.o qMalloc.o

qStack.o : qLinkList.o

qLinkList.o : qMalloc.o

.PHONY : clean

clean :
	rm ./*.o
