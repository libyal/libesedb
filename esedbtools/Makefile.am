AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
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
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBESEDB_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	esedbexport \
	esedbinfo

esedbexport_SOURCES = \
	esedbexport.c \
	esedboutput.c esedboutput.h \
	esedbtools_libbfio.h \
	esedbtools_libcerror.h \
	esedbtools_libclocale.h \
	esedbtools_libcfile.h \
	esedbtools_libcnotify.h \
	esedbtools_libcpath.h \
	esedbtools_libcstring.h \
	esedbtools_libcsystem.h \
	esedbtools_libesedb.h \
	esedbtools_libfdatetime.h \
	esedbtools_libfguid.h \
	esedbtools_libfmapi.h \
	esedbtools_libfvalue.h \
	esedbtools_libfwnt.h \
	esedbtools_libuna.h \
	exchange.c exchange.h \
	export.c export.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	webcache.c webcache.h \
	windows_search.c windows_search.h \
	windows_security.c windows_security.h

esedbexport_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	@LIBFMAPI_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
	@LIBINTL@

esedbinfo_SOURCES = \
	esedbinfo.c \
	esedboutput.c esedboutput.h \
	esedbtools_libbfio.h \
	esedbtools_libcerror.h \
	esedbtools_libclocale.h \
	esedbtools_libcnotify.h \
	esedbtools_libcstring.h \
	esedbtools_libcsystem.h \
	esedbtools_libesedb.h \
	esedbtools_libfdatetime.h \
	esedbtools_libfguid.h \
	esedbtools_libfmapi.h \
	esedbtools_libfvalue.h \
	esedbtools_libfwnt.h \
	esedbtools_libuna.h \
	info_handle.c info_handle.h

esedbinfo_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@ \
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

