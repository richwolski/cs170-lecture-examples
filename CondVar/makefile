CC=gcc
CFLAGS=-g

all: market1 market2 market3 market4

market1: market1.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market1 market1.c c-timer.o -lpthread

market2: market2.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market2 market2.c c-timer.o -lpthread

market3: market3.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market3 market3.c c-timer.o -lpthread

market4: market4.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market4 market4.c c-timer.o -lpthread

c-timer.o: c-timer.c c-timer.h
	${CC} ${CFLAGS} -c c-timer.c

clean: 
	rm -f market1 market2 market3 market4

