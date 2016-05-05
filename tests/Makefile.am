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
	@LIBCSYSTEM_CPPFLAGS@ \
	@LIBESEDB_DLL_IMPORT@

if HAVE_PYTHON_TESTS
TESTS_PYESEDB = \
	test_python_functions.sh
endif

TESTS = \
	test_api_functions.sh \
	test_esedbinfo.sh \
	test_esedbexport.sh \
	$(TESTS_PYESEDB)

check_SCRIPTS = \
	pyesedb_test_get_version.py \
	pyesedb_test_open_close.py \
	test_api_functions.sh \
	test_esedbexport.sh \
	test_esedbinfo.sh \
	test_python_functions.sh \
	test_runner.sh

EXTRA_DIST = \
	$(check_SCRIPTS)

check_PROGRAMS = \
	esedb_test_get_version \
	esedb_test_open_close

esedb_test_get_version_SOURCES = \
	esedb_test_get_version.c \
	esedb_test_libcstring.h \
	esedb_test_libesedb.h \
	esedb_test_unused.h

esedb_test_get_version_LDADD = \
	../libesedb/libesedb.la \
	@LIBCSTRING_LIBADD@

esedb_test_open_close_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libcstring.h \
	esedb_test_libcsystem.h \
	esedb_test_libesedb.h \
	esedb_test_open_close.c

esedb_test_open_close_LDADD = \
	@LIBCSYSTEM_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@ \
	@LIBCSTRING_LIBADD@

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f Makefile

