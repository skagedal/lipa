VERSION=0.0.13

# uhm.. i really gotta fix this..

OBJS=lib/basic.o lib/strings.o lib/numbers.o lib/list.o lib/read.o  \
	lib/eval.o lib/print.o lib/load.o lib/readfuncs.o \
	lib/typepredicates.o lib/convert.o app/lipa.o
SOURCES=$(OBJS:.o=.c)
HEADERS=$(OBJS:.o=.h)
EXTRAS=Makefile README ChangeLog TODO NEWS AUTHORS start.lipa \
	doc/userobject.txt COPYING
DISTDIR=lipa-$(VERSION)
DISTFILE=lipa-$(VERSION).tar.gz
DISTFILES=$(SOURCES) $(HEADERS) $(EXTRAS)

all:
	(cd lib/lipa && make VERSION=$(VERSION))
	(cd app && make VERSION=$(VERSION))

dist: all
	-rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	for i in $(DISTFILES); do ln ../$$i $(DISTDIR)/$$i -s; done
	tar czhf $(DISTFILE) $(DISTDIR)
	-rm -rf $(DISTDIR)

install:
	(cd lib/lipa && make install)
	(cd app && make install)

clean:
	(cd lib/lipa && make clean)
	(cd app && make clean)
