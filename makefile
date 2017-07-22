CC = gcc
CFLAGS = -ansi -pedantic -Wall -g -c


OBJS = floodgate.o barrier.o

#executable
RUNNER : $(OBJS) main.c
	$(CC) -o runner $(OBJS) main.c -lpthread

floodgate.o: floodgate.c barrier.c floodgate.h barrier.h
	$(CC) $(CFLAGS) floodgate.c

barrier.o: barrier.c barrier.h
	$(CC) $(CFLAGS) barrier.c


clean:
	rm -f *.o
	rm -f $(OBJS) RUNNER

rebuild:
	make clean
	make

run:
	make rebuild
	./runner
