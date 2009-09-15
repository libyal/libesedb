AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libnotify \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@

lib_LTLIBRARIES = libesedb.la

libesedb_la_SOURCES = \
	esedb_file_header.h \
	esedb_page.h \
	libesedb.c \
	libesedb_array_type.c libesedb_array_type.h \
	libesedb_checksum.c libesedb_checksum.h \
	libesedb_debug.c libesedb_debug.h \
	libesedb_definitions.h \
	libesedb_error.c libesedb_error.h \
	libesedb_extern.h \
	libesedb_file.c libesedb_file.h \
	libesedb_guid.c libesedb_guid.h \
	libesedb_io_handle.c libesedb_io_handle.h \
	libesedb_libbfio.h \
	libesedb_libfdatetime.h \
	libesedb_libuna.h \
	libesedb_list_type.c libesedb_list_type.h \
	libesedb_notify.c libesedb_notify.h \
	libesedb_page.c libesedb_page.h \
	libesedb_string.h \
	libesedb_support.c libesedb_support.h \
	libesedb_tree_type.c libesedb_tree_type.h

libesedb_la_LIBADD = \
	../liberror/liberror.la \
	../libnotify/libnotify.la \
	@LIBUNA_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFDATETIME_LIBADD@

libesedb_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libesedb_definitions.h.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libesedb ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libesedb_la_SOURCES) 

