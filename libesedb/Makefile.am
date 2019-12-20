AM_CPPFLAGS = \
	-DLOCALEDIR=\"$(datadir)/locale\" \
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
	@LIBFCACHE_CPPFLAGS@ \
	@LIBFDATA_CPPFLAGS@ \
	@LIBFDATETIME_CPPFLAGS@ \
	@LIBFGUID_CPPFLAGS@ \
	@LIBFVALUE_CPPFLAGS@ \
	@LIBFWNT_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBESEDB_DLL_EXPORT@

lib_LTLIBRARIES = libesedb.la

libesedb_la_SOURCES = \
	esedb_file_header.h \
	esedb_page.h \
	esedb_page_values.h \
	libesedb.c \
	libesedb_catalog.c libesedb_catalog.h \
	libesedb_catalog_definition.c libesedb_catalog_definition.h \
	libesedb_checksum.c libesedb_checksum.h \
	libesedb_codepage.c libesedb_codepage.h \
	libesedb_column.c libesedb_column.h \
	libesedb_column_type.c libesedb_column_type.h \
	libesedb_compression.c libesedb_compression.h \
	libesedb_data_definition.c libesedb_data_definition.h \
	libesedb_data_segment.c libesedb_data_segment.h \
	libesedb_database.c libesedb_database.h \
	libesedb_debug.c libesedb_debug.h \
	libesedb_definitions.h \
	libesedb_error.c libesedb_error.h \
	libesedb_extern.h \
	libesedb_file.c libesedb_file.h \
	libesedb_file_header.c libesedb_file_header.h \
	libesedb_i18n.c libesedb_i18n.h \
	libesedb_index.c libesedb_index.h \
	libesedb_io_handle.c libesedb_io_handle.h \
	libesedb_lcid.c libesedb_lcid.h \
	libesedb_leaf_page_descriptor.c libesedb_leaf_page_descriptor.h \
	libesedb_libbfio.h \
	libesedb_libcdata.h \
	libesedb_libcerror.h \
	libesedb_libclocale.h \
	libesedb_libcnotify.h \
	libesedb_libfcache.h \
	libesedb_libfdata.h \
	libesedb_libfvalue.h \
	libesedb_libfwnt.h \
	libesedb_libuna.h \
	libesedb_long_value.c libesedb_long_value.h \
	libesedb_multi_value.c libesedb_multi_value.h \
	libesedb_notify.c libesedb_notify.h \
	libesedb_page.c libesedb_page.h \
	libesedb_page_header.c libesedb_page_header.h \
	libesedb_page_tree.c libesedb_page_tree.h \
	libesedb_page_tree_key.c libesedb_page_tree_key.h \
	libesedb_page_tree_value.c libesedb_page_tree_value.h \
	libesedb_page_value.c libesedb_page_value.h \
	libesedb_record.c libesedb_record.h \
	libesedb_record_value.c libesedb_record_value.h \
	libesedb_root_page_header.c libesedb_root_page_header.h \
	libesedb_space_tree.c libesedb_space_tree.h \
	libesedb_space_tree_value.c libesedb_space_tree_value.h \
	libesedb_support.c libesedb_support.h \
	libesedb_table.c libesedb_table.h \
	libesedb_table_definition.c libesedb_table_definition.h \
	libesedb_types.h \
	libesedb_unused.h \
	libesedb_value_data_handle.c libesedb_value_data_handle.h

libesedb_la_LIBADD = \
	@LIBCERROR_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBFDATA_LIBADD@ \
	@LIBFDATETIME_LIBADD@ \
	@LIBFGUID_LIBADD@ \
	@LIBFVALUE_LIBADD@ \
	@LIBFWNT_LIBADD@ \
	@PTHREAD_LIBADD@

libesedb_la_LDFLAGS = -no-undefined -version-info 1:0:0

EXTRA_DIST = \
	libesedb_definitions.h.in \
	libesedb.rc \
	libesedb.rc.in

MAINTAINERCLEANFILES = \
	Makefile.in

distclean: clean
	/bin/rm -f libesedb_definitions.h
	/bin/rm -f libesedb.rc
	/bin/rm -f Makefile

splint:
	@echo "Running splint on libesedb ..."
	-splint -preproc -redef $(DEFS) $(DEFAULT_INCLUDES) $(INCLUDES) $(AM_CPPFLAGS) $(libesedb_la_SOURCES)

