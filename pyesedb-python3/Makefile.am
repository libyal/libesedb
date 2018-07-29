# Note that we cannot use: AUTOMAKE_OPTIONS = subdir-objects
# subdir-objects will compile the source files to a single version of Python.
# Since subdir-objects is being deprecated we copy the source files instead.
am__installdirs = "$(DESTDIR)$(pyexecdir3)"
pyexecdir = $(pyexecdir3)

if HAVE_PYTHON3
AM_CFLAGS = \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/common \
	@LIBCERROR_CPPFLAGS@ \
	@LIBCDATA_CPPFLAGS@ \
	@LIBCLOCALE_CPPFLAGS@ \
	@LIBCSPLIT_CPPFLAGS@ \
	@LIBUNA_CPPFLAGS@ \
	@LIBCFILE_CPPFLAGS@ \
	@LIBCPATH_CPPFLAGS@ \
	@LIBBFIO_CPPFLAGS@ \
	@LIBESEDB_DLL_IMPORT@

am_pyesedb_la_rpath = -rpath $(pyexecdir3)

BUILT_SOURCES = \
	pyesedb.c pyesedb.h \
	pyesedb_column.c pyesedb_column.h \
	pyesedb_column_types.c pyesedb_column_types.h \
	pyesedb_columns.c pyesedb_columns.h \
	pyesedb_datetime.c pyesedb_datetime.h \
	pyesedb_error.c pyesedb_error.h \
	pyesedb_file.c pyesedb_file.h \
	pyesedb_file_object_io_handle.c pyesedb_file_object_io_handle.h \
	pyesedb_file_types.c pyesedb_file_types.h \
	pyesedb_index.c pyesedb_index.h \
	pyesedb_indexes.c pyesedb_indexes.h \
	pyesedb_integer.c pyesedb_integer.h \
	pyesedb_libbfio.h \
	pyesedb_libcerror.h \
	pyesedb_libclocale.h \
	pyesedb_libesedb.h \
	pyesedb_long_value.c pyesedb_long_value.h \
	pyesedb_multi_value.c pyesedb_multi_value.h \
	pyesedb_python.h \
	pyesedb_record.c pyesedb_record.h \
	pyesedb_records.c pyesedb_records.h \
	pyesedb_table.c pyesedb_table.h \
	pyesedb_tables.c pyesedb_tables.h \
	pyesedb_unused.h \
	pyesedb_value_flags.c pyesedb_value_flags.h

pyexec_LTLIBRARIES = pyesedb.la

nodist_pyesedb_la_SOURCES = $(BUILT_SOURCES)

pyesedb_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@

pyesedb_la_CPPFLAGS = $(PYTHON3_CPPFLAGS)
pyesedb_la_LDFLAGS  = -module -avoid-version $(PYTHON3_LDFLAGS)

$(BUILT_SOURCES):
	/bin/cp -f $(top_srcdir)/pyesedb/$@ $@

endif

MAINTAINERCLEANFILES = \
	Makefile.in

clean-local:
	/bin/rm -f pyesedb*.[ch]

distclean: clean
	/bin/rm -f Makefile

