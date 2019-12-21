AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@LIBFMAPI_CPPFLAGS@ \
	@LIBESEDB_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	esedbexport \
	esedbinfo

esedbexport_SOURCES = \
	database_types.c database_types.h \
	esedbexport.c \
	esedbtools_getopt.c esedbtools_getopt.h \
	esedbtools_i18n.h \
	esedbtools_libbfio.h \
	esedbtools_libcdata.h \
	esedbtools_libcerror.h \
	esedbtools_libclocale.h \
	esedbtools_libcfile.h \
	esedbtools_libcnotify.h \
	esedbtools_libcpath.h \
	esedbtools_libesedb.h \
	esedbtools_libfdatetime.h \
	esedbtools_libfguid.h \
	esedbtools_libfmapi.h \
	esedbtools_libfvalue.h \
	esedbtools_libfwnt.h \
	esedbtools_libuna.h \
	esedbtools_output.c esedbtools_output.h \
	esedbtools_signal.c esedbtools_signal.h \
	esedbtools_system_string.c esedbtools_system_string.h \
	esedbtools_unused.h \
	exchange.c exchange.h \
	export.c export.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	srumdb.c srumdb.h \
	webcache.c webcache.h \
	windows_search.c windows_search.h \
	windows_search_compression.c windows_search_compression.h \
	windows_security.c windows_security.h

esedbexport_LDADD = \
	@LIBFMAPI_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

esedbinfo_SOURCES = \
	esedbinfo.c \
	esedbtools_getopt.c esedbtools_getopt.h \
	esedbtools_i18n.h \
	esedbtools_libbfio.h \
	esedbtools_libcerror.h \
	esedbtools_libclocale.h \
	esedbtools_libcnotify.h \
	esedbtools_libesedb.h \
	esedbtools_libfdatetime.h \
	esedbtools_libfguid.h \
	esedbtools_libfmapi.h \
	esedbtools_libfvalue.h \
	esedbtools_libfwnt.h \
	esedbtools_libuna.h \
	esedbtools_output.c esedbtools_output.h \
	esedbtools_signal.c esedbtools_signal.h \
	esedbtools_unused.h \
	info_handle.c info_handle.h

esedbinfo_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on esedbexport ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(esedbexport_SOURCES)
	@echo "Running splint on esedbinfo ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(esedbinfo_SOURCES)

