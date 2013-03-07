CC=gcc
AR=ar
CFLAGS=-Wall -O2 -g `pkg-config --cflags glib-2.0`
LIBS=`pkg-config --libs glib-2.0` -lm 

OBJS=basic.o strings.o numbers.o list.o read.o eval.o print.o \
	load.o readfuncs.o evalprocs.o typepredicates.o convert.o chars.o
SOURCES=$(OBJS:.o=.c)
HEADERS=$(OBJS:.o=.h)

DISTDIR=lipa-$(VERSION)
DISTFILE=lipa-$(VERSION).tar.gz
DISTFILES=$(SOURCES) $(HEADERS) $(EXTRAS)

all: liblipa.a

liblipa.a: $(OBJS)
	$(AR) rc liblipa.a $(OBJS)

basic.o: basic.h list.h strings.h numbers.h read.h evalprocs.h

string.o: basic.h strings.h list.h

numbers.o: basic.h numbers.h list.h

chars.o: basic.h chars.h list.h

list.o: basic.h list.h

read.o: basic.h list.h read.h

readfuncs.o: list.h basic.h eval.h

eval.o: basic.h list.h eval.h

evalprocs.o: basic.h eval.h list.h evalprocs.h

convert.o:	convert.h basic.h list.h

install:
	-mkdir /usr/local/include/lipa
	cp *.h /usr/local/include/lipa
	cp liblipa.a /usr/local/lib

clean: 
	rm -f *.o liblipa.a
