AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCSTRING_CPPFLAGS@ \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCNOTIFY_CPPFLAGS@ \
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
	@MEMWATCH_CPPFLAGS@ \
	@LIBESEDB_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	esedbexport \
	esedbinfo

esedbexport_SOURCES = \
	esedbexport.c \
	esedboutput.c esedboutput.h \
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
	esedbtools_libfwnt.h \
	esedbtools_libuna.h \
	export.c export.h \
	export_exchange.c export_exchange.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	windows_search.c windows_search.h \
	windows_security.c windows_security.h

esedbexport_LDADD  = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBUNA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@LIBFMAPI_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCSYSTEM_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

esedbinfo_SOURCES = \
	esedbinfo.c \
	esedboutput.c esedboutput.h \
	esedbtools_libcerror.h \
	esedbtools_libclocale.h \
	esedbtools_libcnotify.h \
	esedbtools_libcstring.h \
	esedbtools_libcsystem.h \
	esedbtools_libesedb.h \
	esedbtools_libfdatetime.h \
	esedbtools_libfguid.h \
	esedbtools_libuna.h \
	info_handle.c info_handle.h

esedbinfo_LDADD  = \
	@LIBCSTRING_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCSYSTEM_LIBADD@ \
	@LIBINTL@ \
	@MEMWATCH_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

splint:
	@echo "Running splint on esedbexport ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(esedbexport_SOURCES) 
	@echo "Running splint on esedbinfo ..." 
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(esedbinfo_SOURCES) 

