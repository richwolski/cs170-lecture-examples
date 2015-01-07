CC = gcc

EXECUTABLES = p1 p2 p3 p4

CFLAGS = -g

all: $(EXECUTABLES)

clean:
	rm -f core *.o $(EXECUTABLES) a.out *.a

int_add.o: int_add.c int_add.h
	$(CC) $(CFLAGS) -c int_add.c

int_sub.o: int_sub.c int_sub.h
	$(CC) $(CFLAGS) -c int_sub.c

libmylib.a: int_add.o int_sub.o
	ar cr libmylib.a int_add.o int_sub.o

p1: p1.c int_add.o int_add.h
	$(CC) $(CFLAGS) -o p1 p1.c int_add.o

p2: p2.c int_add.o int_add.h int_sub.o int_sub.h
	$(CC) $(CFLAGS) -o p2 p2.c int_add.o int_sub.o

p3: p2.c libmylib.a
	$(CC) $(CFLAGS) -o p3 p2.c libmylib.a

p4: p2.c libmylib.a
	$(CC) $(CFLAGS) -o p4 p2.c -L. -lmylib


