if HAVE_LOCAL_LIBCPATH
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

noinst_LTLIBRARIES = libcpath.la

libcpath_la_SOURCES = \
	libcpath_definitions.h \
	libcpath_extern.h \
	libcpath_error.c libcpath_error.h \
	libcpath_path.c libcpath_path.h \
	libcpath_libcerror.h \
	libcpath_libclocale.h \
	libcpath_libcsplit.h \
	libcpath_libcstring.h \
	libcpath_libuna.h \
	libcpath_support.c libcpath_support.h \
	libcpath_unused.h
endif

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libcpath ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libcpath_la_SOURCES) 

