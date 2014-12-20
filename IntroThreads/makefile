CC=gcc
CFLAGS=-g

all: avg-nothread avg-1thread avg-manythread

avg-nothread: avg-nothread.c
	${CC} ${CFLAGS} -o avg-nothread avg-nothread.c -lpthread

avg-1thread: avg-1thread.c
	${CC} ${CFLAGS} -o avg-1thread avg-1thread.c -lpthread

avg-manythread: avg-manythread.c
	${CC} ${CFLAGS} -o avg-manythread avg-manythread.c -lpthread

clean:
	rm -f avg-nothread avg-1thread avg-manythread

