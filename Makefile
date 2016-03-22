CC = gcc -g

default : test clean

test : qStack.o qLinkList.o qMalloc.o qString.o qDict.o qEvent.o

.PHONY : clean

clean :
	rm ./*.o
