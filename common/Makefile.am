AM_CPPFLAGS = -I$(top_srcdir)/include

EXTRA_DIST = \
	byte_stream.h \
	common.h \
	config.h \
	config_borlandc.h \
	config_msc.h \
	file_stream.h \
	gettext.h \
	memory.h \
	types.h \
	types.h.in

if HAVE_MEMWATCH
noinst_LTLIBRARIES = libmemwatch.la

libmemwatch_la_SOURCES = \
        memwatch.c memwatch.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f config.h
	/bin/rm -f types.h
	/bin/rm -f Makefile

