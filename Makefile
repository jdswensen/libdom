CFLAGS += -O0 -g -Wall -std=gnu99 -Iinclude/ -Isrc/ -Ihtml-to-stream/
AR ?= ar
DEPS +=

includedir ?= /usr/include
libdir ?= /usr/lib

all: libdom.a html2stream

dom.o: src/dom.c src/parse.c
	$(CC) $(CFLAGS) -c src/dom.c src/parse.c src/str_utils.c $(DEPS)

libdom.a: dom.o
	rm -f libdom.a
	$(AR) -cvq libdom.a *.o

readable.o: html-to-stream/readable.c
	$(CC) $(CFLAGS) -c html-to-stream/readable.c $(DEPS)

html2stream.o: html-to-stream/html2stream.c
	$(CC) $(CFLAGS) -c html-to-stream/html2stream.c $(DEPS)

html2stream: html2stream.o readable.o libdom.a
	$(CC) $(CFLAGS) -L./libdom html2stream.o readable.o $(DEPS) libdom.a -o html2stream

clean:
	$(RM) *.o *.gch example-* tests-* *.a

install:
	install -d $(DESTDIR)$(includedir)
	install -d $(DESTDIR)$(libdir)
	install include/dom.h $(DESTDIR)$(includedir)/
	install libdom.a $(DESTDIR)$(libdir)/
