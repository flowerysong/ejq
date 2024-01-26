AM_CFLAGS = -Wall

bin_PROGRAMS = ejq

ejq_SOURCES = json.c json.h json-selector.c json-selector.h ejq.c

EXTRA_DIST = VERSION packaging/rpm/ejq.spec

rpm: dist-xz
	rpmbuild -ta $(distdir).tar.xz
