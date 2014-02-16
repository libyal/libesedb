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
	@LIBBFIO_CPPFLAGS@

if HAVE_PYTHON
TESTS_PYESEDB  = \
	test_pyesedb_open_close.sh
endif

TESTS = \
	test_open_close.sh \
	test_esedbinfo.sh \
	test_esedbexport.sh \
	$(TESTS_PYESEDB)

check_SCRIPTS = \
	pyesedb_test_open_close.py \
	test_open_close.sh \
	test_pyesedb_open_close.sh \
	test_esedbexport.sh \
	test_esedbinfo.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS =  \
	esedb_test_open_close

esedb_test_open_close_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_libcstring.h \
	esedb_test_open_close.c

esedb_test_open_close_LDADD = \
	../libesedb/libesedb.la \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

