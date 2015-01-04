
EXECUTABLES = dphil_2 dphil_3 dphil_4 dphil_5 dphil_6

DLOBJ = /cs/faculty/rich/cs170/lib/libfdr.a
CFLAGS = -m32 -I/cs/faculty/rich/cs170/include

all: $(EXECUTABLES)

clean:
	rm -f core *.o $(EXECUTABLES) a.out

.SUFFIXES: .c .o
.c.o:
	$(CC) $(CFLAGS) -c $*.c

dphil_skeleton.o: dphil.h

dphil_2.o: dphil.h
dphil_3.o: dphil.h
dphil_4.o: dphil.h
dphil_5.o: dphil.h
dphil_6.o: dphil.h

dphil_1: dphil_1.o dphil_skeleton.o 
	$(CC) $(CFLAGS) -o dphil_1 dphil_skeleton.o dphil_1.o -lpthread

dphil_2: dphil_2.o dphil_skeleton.o 
	$(CC) $(CFLAGS) -o dphil_2 dphil_skeleton.o dphil_2.o -lpthread

dphil_3: dphil_3.o dphil_skeleton.o 
	$(CC) $(CFLAGS) -o dphil_3 dphil_skeleton.o dphil_3.o -lpthread

dphil_4: dphil_4.o dphil_skeleton.o 
	$(CC) $(CFLAGS) -o dphil_4 dphil_skeleton.o dphil_4.o -lpthread

dphil_5: dphil_5.o dphil_skeleton.o 
	$(CC) $(CFLAGS) -o dphil_5 dphil_skeleton.o dphil_5.o -lpthread

dphil_6: dphil_6.o dphil_skeleton.o $(DLOBJ)
	$(CC) $(CFLAGS) -o dphil_6 dphil_skeleton.o dphil_6.o $(DLOBJ) -lpthread
