CC=gcc
# need to set optimization flag to -g or Linux gcc will optimize away the spin
# wait in client threads
CFLAGS=-g

all: market1 market2 market3 market4 bad-test

market1: market1.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market1 market1.c c-timer.o -lpthread

market2: market2.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market2 market2.c c-timer.o -lpthread

market3: market3.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market3 market3.c c-timer.o -lpthread

market4: market4.c c-timer.h c-timer.o
	${CC} ${CFLAGS} -o market4 market4.c c-timer.o -lpthread

bad-test: bad-test.c 
	${CC} ${CFLAGS} -o bad-test bad-test.c -lpthread

c-timer.o: c-timer.c c-timer.h
	${CC} ${CFLAGS} -c c-timer.c

clean: 
	rm -f *.o market1 market2 market3 market4
