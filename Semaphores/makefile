CC=gcc
CFLAGS=-g

all: market-semaphore

market-semaphore: market-semaphore.c c-timer.h c-timer.o sema.h sema.o
	${CC} ${CFLAGS} -o market-semaphore market-semaphore.c c-timer.o sema.o -lpthread

c-timer.o: c-timer.c c-timer.h
	${CC} ${CFLAGS} -c c-timer.c

sema.o: sema.c sema.h
	${CC} ${CFLAGS} -c sema.c

clean:
	rm -f market-semaphore *.o

