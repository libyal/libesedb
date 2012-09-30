AM_CPPFLAGS = -I$(top_srcdir)/include

EXTRA_DIST = \
	byte_stream.h \
	common.h \
	config.h \
	config_borlandc.h \
	config_msc.h \
	file_stream.h \
	memory.h \
	types.h \
	types.h.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f config.h
	/bin/rm -f types.h
	/bin/rm -f Makefile

