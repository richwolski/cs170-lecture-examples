CC=gcc -m32
KLIB=/cs/faculty/rich/cs170/lib/libkt.a
FLIB=/cs/faculty/rich/cs170/lib/libfdr.a
KINC=/cs/faculty/rich/cs170/include
CFLAGS=-g -I${KINC}

all: market-kthreads

market-kthreads: market-kthreads.c c-timer.h c-timer.o 
	${CC} ${CFLAGS} -o market-kthreads market-kthreads.c c-timer.o ${KLIB} ${FLIB}

c-timer.o: c-timer.c c-timer.h
	${CC} ${CFLAGS} -c c-timer.c

clean:
	rm -f market-kthreads *.o

