CC=gcc
CFLAGS=-Wall -O2 -g `pkg-config --cflags glib-2.0` -I../lib -DVERSION=\"$(VERSION)\"
LIBS=-L../lib/lipa `pkg-config --libs glib-2.0`  -lreadline -lm -llipa 

OBJS=lipa.o
SOURCES=$(OBJS:.o=.c)
HEADERS=

all: lipa

lipa: $(OBJS) ../lib/lipa/liblipa.a
	$(CC) -o lipa $(OBJS) $(LIBS)

install:
	cp lipa /usr/local/bin/

clean:
	rm -f *.o lipa

