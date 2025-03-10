CC = gcc #-m32
HOME ?= /cs/faculty/rich/src/cs170.labs
#HOME ?= ..
CFLAGS =-I$(HOME)/fdr -g -DLINUX
PREFIX ?= /cs/faculty/rich/cs170
#PREFIX ?= ./build
KHOME=$(PREFIX)/lib
KINC=$(PREFIX)/include

OBJECT-FILES = kdebug.o kt.o 
HEADER-FILES = kos_world.h kdebug.h jpt.h kt.h
LIBRARIES = $(KHOME)/libfdr.a

EXECUTABLES = test_kt

all:  test_kt libkosutil.a $(KHOME)libkt.a

test_kt: test_kt.o ${OBJECT-FILES} ${HEADER-FILES}
	${CC} -o $@ ${CFLAGS} test_kt.o  ${OBJECT-FILES} ${LIBRARIES}

test_kt.o: ${HEADER-FILES}

libkosutil.a: ${OBJECT-FILES}
	rm -f libkosutil.a
	ar -rc libkosutil.a ${OBJECT-FILES} 

$(KHOME)libkt.a: ${OBJECT-FILES}
	rm -f $(KHOME)/libkt.a
	ar -rc $(KHOME)/libkt.a ${OBJECT-FILES} 
	cp *.h $(KINC)


clean:
	rm -f ${EXECUTABLES} ${OBJECT-FILES} test_kt.o

depend:
	makedepend *.c

# DO NOT DELETE

#dllist.o: /usr/include/stdio.h /usr/include/features.h
#dllist.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
#dllist.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
#dllist.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
#dllist.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
#dllist.o: /usr/include/bits/wchar.h /usr/include/gconv.h
#dllist.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
#dllist.o: dllist.h jval.h
#jrb.o: /usr/include/string.h /usr/include/features.h /usr/include/sys/cdefs.h
#jrb.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
#jrb.o: /usr/include/gnu/stubs-32.h /usr/include/stdio.h
#jrb.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
#jrb.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
#jrb.o: /usr/include/bits/wchar.h /usr/include/gconv.h
#jrb.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
#jrb.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
#jrb.o: /usr/include/endian.h /usr/include/bits/endian.h
#jrb.o: /usr/include/sys/select.h /usr/include/bits/select.h
#jrb.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
#jrb.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
#jrb.o: /usr/include/alloca.h /usr/include/ctype.h jrb.h jval.h
#jval.o: /usr/include/stdio.h /usr/include/features.h /usr/include/sys/cdefs.h
#jval.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
#jval.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
#jval.o: /usr/include/bits/typesizes.h /usr/include/libio.h
#jval.o: /usr/include/_G_config.h /usr/include/wchar.h
#jval.o: /usr/include/bits/wchar.h /usr/include/gconv.h
#jval.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h jval.h
#kdebug.o: kos_world.h /usr/include/unistd.h /usr/include/features.h
#kdebug.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
#kdebug.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
#kdebug.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
#kdebug.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
#kdebug.o: /usr/include/getopt.h /usr/include/stdlib.h
#kdebug.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
#kdebug.o: /usr/include/bits/endian.h /usr/include/sys/select.h
#kdebug.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
#kdebug.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
#kdebug.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
#kdebug.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
#kdebug.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
#kdebug.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
#kdebug.o: /usr/include/math.h /usr/include/bits/huge_val.h
#kdebug.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
#kdebug.o: /usr/include/string.h /usr/include/sys/time.h jval.h dllist.h jrb.h
#kt.o: kos_world.h /usr/include/unistd.h /usr/include/features.h
#kt.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
#kt.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
#kt.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
#kt.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
#kt.o: /usr/include/getopt.h /usr/include/stdlib.h /usr/include/sys/types.h
#kt.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
#kt.o: /usr/include/sys/select.h /usr/include/bits/select.h
#kt.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
#kt.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
#kt.o: /usr/include/alloca.h /usr/include/stdio.h /usr/include/libio.h
#kt.o: /usr/include/_G_config.h /usr/include/wchar.h /usr/include/bits/wchar.h
#kt.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
#kt.o: /usr/include/bits/sys_errlist.h /usr/include/math.h
#kt.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
#kt.o: /usr/include/bits/mathcalls.h /usr/include/string.h
#kt.o: /usr/include/sys/time.h jval.h dllist.h jrb.h /usr/include/setjmp.h
#kt.o: /usr/include/bits/setjmp.h kdebug.h kt.h
#rb.o: /usr/include/string.h /usr/include/features.h /usr/include/sys/cdefs.h
#rb.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
#rb.o: /usr/include/gnu/stubs-32.h /usr/include/stdio.h
#rb.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
#rb.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
#rb.o: /usr/include/bits/wchar.h /usr/include/gconv.h
#rb.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
#rb.o: /usr/include/stdlib.h /usr/include/sys/types.h /usr/include/time.h
#rb.o: /usr/include/endian.h /usr/include/bits/endian.h
#rb.o: /usr/include/sys/select.h /usr/include/bits/select.h
#rb.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
#rb.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
#rb.o: /usr/include/alloca.h /usr/include/ctype.h rb.h
#test_kt.o: kos_world.h /usr/include/unistd.h /usr/include/features.h
#test_kt.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
#test_kt.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
#test_kt.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
#test_kt.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
#test_kt.o: /usr/include/getopt.h /usr/include/stdlib.h
#test_kt.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
#test_kt.o: /usr/include/bits/endian.h /usr/include/sys/select.h
#test_kt.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
#test_kt.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
#test_kt.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
#test_kt.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
#test_kt.o: /usr/include/wchar.h /usr/include/bits/wchar.h
#test_kt.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
#test_kt.o: /usr/include/bits/sys_errlist.h /usr/include/math.h
#test_kt.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
#test_kt.o: /usr/include/bits/mathcalls.h /usr/include/string.h
#test_kt.o: /usr/include/sys/time.h jval.h dllist.h jrb.h kt.h
