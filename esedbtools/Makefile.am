AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	-I$(top_srcdir)/libcstring \
	-I$(top_srcdir)/liberror \
	-I$(top_srcdir)/libsystem \
	@LIBUNA_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@MEMWATCH_CPPFLAGS@ \
	@LIBESEDB_DLL_IMPORT@

AM_LDFLAGS = @STATIC_LDFLAGS@

bin_PROGRAMS = \
	esedbexport \
	esedbinfo

esedbexport_SOURCES = \
	ascii7.c ascii7.h \
	guid.c guid.h \
	esedbcommon.h \
	esedbexport.c \
	esedbinput.c esedbinput.h \
	esedboutput.c esedboutput.h \
	export_handle.c export_handle.h \
	log_handle.c log_handle.h \
	windows_search.c windows_search.h

esedbexport_LDADD  = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libesedb/libesedb.la \
	../libsystem/libsystem.la \
	@LIBUNA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@MEMWATCH_LIBADD@

esedbinfo_SOURCES = \
	guid.c guid.h \
	esedbinfo.c \
	esedbinput.c esedbinput.h \
	esedboutput.c esedboutput.h

esedbinfo_LDADD  = \
	../libcstring/libcstring.la \
	../liberror/liberror.la \
	../libesedb/libesedb.la \
	../libsystem/libsystem.la \
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

