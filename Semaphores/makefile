CC=gcc
CFLAGS=-O2

all: market-semaphore market3

gprof: market-semaphore-g market3-g

market-semaphore: market-semaphore.c c-timer.h c-timer.o sema.h sema.o
	${CC} ${CFLAGS} -o market-semaphore market-semaphore.c c-timer.o sema.o -lpthread

market-semaphore-g: market-semaphore.c c-timer.h c-timer.o sema.h sema.o
	${CC} ${CFLAGS} -pg -o market-semaphore-g market-semaphore.c c-timer.o sema.o -lpthread

market3: market3.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market3 market3.c c-timer.o -lpthread

market3-g: market3.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -pg -o market3-g market3.c c-timer.o -lpthread

c-timer.o: c-timer.c c-timer.h
	${CC} ${CFLAGS} -c c-timer.c

sema.o: sema.c sema.h
	${CC} ${CFLAGS} -c sema.c

clean:
	rm -f market-semaphore *.o

