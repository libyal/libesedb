AM_CPPFLAGS = -I$(top_srcdir)/include

EXTRA_DIST = \
	common.h \
	config.h \
	config_borlandc.h \
	config_msc.h \
	endian.h \
	file_stream.h \
	memory.h \
	narrow_string.h \
	types.h \
	types.h.in \
	wide_string.h

if HAVE_MEMWATCH
noinst_LTLIBRARIES = libmemwatch.la

libmemwatch_la_SOURCES = \
        memwatch.c memwatch.h
endif

distclean: clean
	/bin/rm -f config.h
	/bin/rm -f types.h
	/bin/rm -f Makefile

