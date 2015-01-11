CC = gcc

EXECUTABLES = p1 p2 p3 p4 pointer1 pointer2 in-out string1 string2 string3 string4 sys-call1 file-create1 file-create2 file-read1 file-read2 file-seek1

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

pointer1: pointer1.c
	$(CC) $(CFLAGS) -o pointer1 pointer1.c

pointer2: pointer2.c
	$(CC) $(CFLAGS) -o pointer2 pointer2.c

in-out: in-out.c
	$(CC) $(CFLAGS) -o in-out in-out.c

string1: string1.c
	$(CC) $(CFLAGS) -o string1 string1.c

string2: string2.c
	$(CC) $(CFLAGS) -o string2 string2.c

string3: string3.c
	$(CC) $(CFLAGS) -o string3 string3.c

string4: string4.c
	$(CC) $(CFLAGS) -o string4 string4.c

sys-call1: sys-call1.c
	$(CC) $(CFLAGS) -o sys-call1 sys-call1.c

file-create1: file-create1.c
	$(CC) $(CFLAGS) -o file-create1 file-create1.c

file-create2: file-create2.c
	$(CC) $(CFLAGS) -o file-create2 file-create2.c

file-read1: file-read1.c
	$(CC) $(CFLAGS) -o file-read1 file-read1.c

file-read2: file-read2.c
	$(CC) $(CFLAGS) -o file-read2 file-read2.c

file-seek1: file-seek1.c
	$(CC) $(CFLAGS) -o file-seek1 file-seek1.c

