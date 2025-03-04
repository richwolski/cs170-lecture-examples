CC = gcc

EXECUTABLES = p1 p2 p3 p4 pointer0 pointer1 pointer2 pointer-array pointer-pointer in-out string1 string2 string3 string4 sys-call1 sys-call2 file-create1 file-create2 file-read1 file-read2 file-seek1 file-fd1 file-fd2 file-fd3 mem1 mem2 mem3 mem4 mem5 mem6 mem7 mem8 fork-1 fork-2 fork-3 fork-4 pipe-1 pipe-2 pipe-3 my-cat my-cat2 my-cat-noprint pipe-4 file-read-write stat-print pipe-short pipe-stall pipe-stall-2

CFLAGS = -g

all: $(EXECUTABLES)
	chmod 755 $(EXECUTABLES)

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

pointer0: pointer0.c
	$(CC) $(CFLAGS) -o pointer0 pointer0.c

pointer1: pointer1.c
	$(CC) $(CFLAGS) -o pointer1 pointer1.c

pointer2: pointer2.c
	$(CC) $(CFLAGS) -o pointer2 pointer2.c

pointer-array: pointer-array.c
	$(CC) $(CFLAGS) -o pointer-array pointer-array.c

pointer-pointer: pointer-pointer.c
	$(CC) $(CFLAGS) -o pointer-pointer pointer-pointer.c

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

sys-call2: sys-call2.c
	$(CC) $(CFLAGS) -o sys-call2 sys-call2.c

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

file-fd1: file-fd1.c
	$(CC) $(CFLAGS) -o file-fd1 file-fd1.c

file-fd2: file-fd2.c
	$(CC) $(CFLAGS) -o file-fd2 file-fd2.c

file-fd3: file-fd3.c
	$(CC) $(CFLAGS) -o file-fd3 file-fd3.c

file-read-write: file-read-write.c
	$(CC) $(CFLAGS) -o file-read-write file-read-write.c


mem1: mem1.c
	$(CC) $(CFLAGS) -o mem1 mem1.c

mem2: mem2.c
	$(CC) $(CFLAGS) -o mem2 mem2.c

mem3: mem3.c
	$(CC) $(CFLAGS) -o mem3 mem3.c

mem4: mem4.c
	$(CC) $(CFLAGS) -o mem4 mem4.c

mem5: mem5.c
	$(CC) $(CFLAGS) -o mem5 mem5.c

mem6: mem6.c
	$(CC) $(CFLAGS) -o mem6 mem6.c

mem7: mem7.c
	$(CC) $(CFLAGS) -o mem7 mem7.c

mem8: mem8.c
	$(CC) $(CFLAGS) -o mem8 mem8.c

fork-1: fork-1.c
	$(CC) $(CFLAGS) -o fork-1 fork-1.c

fork-2: fork-2.c
	$(CC) $(CFLAGS) -o fork-2 fork-2.c

fork-3: fork-3.c
	$(CC) $(CFLAGS) -o fork-3 fork-3.c

fork-4: fork-4.c
	$(CC) $(CFLAGS) -o fork-4 fork-4.c

pipe-1: pipe-1.c
	$(CC) $(CFLAGS) -o pipe-1 pipe-1.c

pipe-2: pipe-2.c
	$(CC) $(CFLAGS) -o pipe-2 pipe-2.c

pipe-3: pipe-3.c
	$(CC) $(CFLAGS) -o pipe-3 pipe-3.c

pipe-4: pipe-4.c
	$(CC) $(CFLAGS) -o pipe-4 pipe-4.c

pipe-short: pipe-short.c
	$(CC) $(CFLAGS) -o pipe-short pipe-short.c

pipe-stall: pipe-stall.c
	$(CC) $(CFLAGS) -o pipe-stall pipe-stall.c

pipe-stall-2: pipe-stall-2.c
	$(CC) $(CFLAGS) -o pipe-stall-2 pipe-stall-2.c

my-cat: my-cat.c
	$(CC) $(CFLAGS) -o my-cat my-cat.c

my-cat2: my-cat2.c
	$(CC) $(CFLAGS) -o my-cat2 my-cat2.c

my-cat-noprint: my-cat-noprint.c
	$(CC) $(CFLAGS) -o my-cat-noprint my-cat-noprint.c

stat-print: stat-print.c
	$(CC) $(CFLAGS) -o stat-print stat-print.c

perms:
	chmod 755 $(EXECUTABLES)
