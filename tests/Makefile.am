AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = \
	-I../include -I$(top_srcdir)/include \
	-I../common -I$(top_srcdir)/common \
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
	@LIBFMAPI_CPPFLAGS@ \
	@PTHREAD_CPPFLAGS@ \
	@LIBESEDB_DLL_IMPORT@

check_PYTHONdir = $(abs_top_builddir)

BUILT_SOURCES = package.m4

check_AUTOTESTS = \
	test_library \
	test_manpages \
	test_tools

if HAVE_PYTHON_TESTS
check_AUTOTESTS += test_python_module
endif

check_PYTHON_SCRIPTS = \
	pyesedb_test_file.py \
	pyesedb_test_support.py

check_PROGRAMS = \
	esedb_test_block_descriptor \
	esedb_test_block_tree \
	esedb_test_block_tree_node \
	esedb_test_catalog \
	esedb_test_catalog_definition \
	esedb_test_checksum \
	esedb_test_column \
	esedb_test_column_type \
	esedb_test_compression \
	esedb_test_data_definition \
	esedb_test_data_segment \
	esedb_test_database \
	esedb_test_error \
	esedb_test_file \
	esedb_test_file_header \
	esedb_test_index \
	esedb_test_io_handle \
	esedb_test_leaf_page_descriptor \
	esedb_test_long_value \
	esedb_test_multi_value \
	esedb_test_notify \
	esedb_test_page \
	esedb_test_page_header \
	esedb_test_page_tree \
	esedb_test_page_tree_key \
	esedb_test_page_tree_value \
	esedb_test_page_value \
	esedb_test_record \
	esedb_test_root_page_header \
	esedb_test_space_tree \
	esedb_test_space_tree_value \
	esedb_test_support \
	esedb_test_table \
	esedb_test_table_definition \
	esedb_test_tools_info_handle \
	esedb_test_tools_output \
	esedb_test_tools_signal \
	esedb_test_tools_windows_search_compression

esedb_test_block_descriptor_SOURCES = \
	esedb_test_block_descriptor.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_block_descriptor_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_block_tree_SOURCES = \
	esedb_test_block_tree.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_block_tree_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_block_tree_node_SOURCES = \
	esedb_test_block_tree_node.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_block_tree_node_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_catalog_SOURCES = \
	esedb_test_catalog.c \
	esedb_test_functions.c esedb_test_functions.h \
	esedb_test_libbfio.h \
	esedb_test_libcdata.h \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_catalog_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_catalog_definition_SOURCES = \
	esedb_test_catalog_definition.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_catalog_definition_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_checksum_SOURCES = \
	esedb_test_checksum.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_checksum_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_column_SOURCES = \
	esedb_test_column.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_column_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_column_type_SOURCES = \
	esedb_test_column_type.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_column_type_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_compression_SOURCES = \
	esedb_test_compression.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_compression_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_data_definition_SOURCES = \
	esedb_test_data_definition.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_data_definition_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_data_segment_SOURCES = \
	esedb_test_data_segment.c \
	esedb_test_functions.c esedb_test_functions.h \
	esedb_test_libbfio.h \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_libfcache.h \
	esedb_test_libfdata.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_data_segment_LDADD = \
	@LIBFDATA_LIBADD@ \
	@LIBFCACHE_LIBADD@ \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_database_SOURCES = \
	esedb_test_database.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_database_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_error_SOURCES = \
	esedb_test_error.c \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_unused.h

esedb_test_error_LDADD = \
	../libesedb/libesedb.la

esedb_test_file_SOURCES = \
	esedb_test_file.c \
	esedb_test_functions.c esedb_test_functions.h \
	esedb_test_getopt.c esedb_test_getopt.h \
	esedb_test_libbfio.h \
	esedb_test_libcerror.h \
	esedb_test_libclocale.h \
	esedb_test_libcnotify.h \
	esedb_test_libesedb.h \
	esedb_test_libuna.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_file_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCTHREADS_LIBADD@ \
	@LIBCERROR_LIBADD@ \
	@PTHREAD_LIBADD@

esedb_test_file_header_SOURCES = \
	esedb_test_file_header.c \
	esedb_test_functions.c esedb_test_functions.h \
	esedb_test_libbfio.h \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_file_header_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_index_SOURCES = \
	esedb_test_index.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_index_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_io_handle_SOURCES = \
	esedb_test_io_handle.c \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_io_handle_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_leaf_page_descriptor_SOURCES = \
	esedb_test_leaf_page_descriptor.c \
	esedb_test_libcdata.h \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_leaf_page_descriptor_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_long_value_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_long_value.c \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_unused.h

esedb_test_long_value_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_multi_value_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_multi_value.c \
	esedb_test_unused.h

esedb_test_multi_value_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_notify_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_notify.c \
	esedb_test_unused.h

esedb_test_notify_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_page_SOURCES = \
	esedb_test_functions.c esedb_test_functions.h \
	esedb_test_libbfio.h \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_page.c \
	esedb_test_unused.h

esedb_test_page_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_page_header_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_page_header.c \
	esedb_test_unused.h

esedb_test_page_header_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_page_tree_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_page_tree.c \
	esedb_test_unused.h

esedb_test_page_tree_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_page_tree_key_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_libfdata.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_page_tree_key.c \
	esedb_test_unused.h

esedb_test_page_tree_key_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_page_tree_value_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_page_tree_value.c \
	esedb_test_unused.h

esedb_test_page_tree_value_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_page_value_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_page_value.c \
	esedb_test_unused.h

esedb_test_page_value_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_record_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_record.c \
	esedb_test_unused.h

esedb_test_record_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_root_page_header_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_root_page_header.c \
	esedb_test_unused.h

esedb_test_root_page_header_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_space_tree_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_space_tree.c \
	esedb_test_unused.h

esedb_test_space_tree_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_space_tree_value_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_space_tree_value.c \
	esedb_test_unused.h

esedb_test_space_tree_value_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_support_SOURCES = \
	esedb_test_functions.c esedb_test_functions.h \
	esedb_test_getopt.c esedb_test_getopt.h \
	esedb_test_libbfio.h \
	esedb_test_libcerror.h \
	esedb_test_libclocale.h \
	esedb_test_libcnotify.h \
	esedb_test_libesedb.h \
	esedb_test_libuna.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_support.c \
	esedb_test_unused.h

esedb_test_support_LDADD = \
	@LIBBFIO_LIBADD@ \
	@LIBCPATH_LIBADD@ \
	@LIBCFILE_LIBADD@ \
	@LIBUNA_LIBADD@ \
	@LIBCSPLIT_LIBADD@ \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	@LIBCDATA_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_table_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_table.c \
	esedb_test_unused.h

esedb_test_table_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_table_definition_SOURCES = \
	esedb_test_libcerror.h \
	esedb_test_libesedb.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_table_definition.c \
	esedb_test_unused.h

esedb_test_table_definition_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_tools_info_handle_SOURCES = \
	../esedbtools/info_handle.c ../esedbtools/info_handle.h \
	esedb_test_libcerror.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_tools_info_handle.c \
	esedb_test_unused.h

esedb_test_tools_info_handle_LDADD = \
	@LIBFDATETIME_LIBADD@ \
	@LIBCLOCALE_LIBADD@ \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_tools_output_SOURCES = \
	../esedbtools/esedbtools_output.c ../esedbtools/esedbtools_output.h \
	esedb_test_libcerror.h \
	esedb_test_macros.h \
	esedb_test_tools_output.c \
	esedb_test_unused.h

esedb_test_tools_output_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@ \
	@LIBINTL@

esedb_test_tools_signal_SOURCES = \
	../esedbtools/esedbtools_signal.c ../esedbtools/esedbtools_signal.h \
	esedb_test_libcerror.h \
	esedb_test_macros.h \
	esedb_test_tools_signal.c \
	esedb_test_unused.h

esedb_test_tools_signal_LDADD = \
	../libesedb/libesedb.la \
	@LIBCERROR_LIBADD@

esedb_test_tools_windows_search_compression_SOURCES = \
	../esedbtools/windows_search_compression.c ../esedbtools/windows_search_compression.h \
	esedb_test_libcerror.h \
	esedb_test_macros.h \
	esedb_test_memory.c esedb_test_memory.h \
	esedb_test_tools_windows_search_compression.c \
	esedb_test_unused.h

esedb_test_tools_windows_search_compression_LDADD = \
	@LIBCNOTIFY_LIBADD@ \
	@LIBCERROR_LIBADD@

AUTOM4TE = autom4te
AUTOTEST = $(AUTOM4TE) --language=autotest

SUFFIXES = .at

.at:
	$(AM_V_GEN)rm -f $@ $@.tmp; \
	$(AUTOTEST) -I '$(srcdir)' -o $@.tmp $< && \
	mv $@.tmp $@

atconfig: $(top_builddir)/config.status
	cd $(top_builddir) && $(SHELL) ./config.status tests/$@

check-build: $(check_PROGRAMS)

check-local: $(check_AUTOTESTS)
	@fail=0; \
	for test_suite in $(check_AUTOTESTS); do \
		$(AM_TESTS_ENVIRONMENT) $(SHELL) "./$$test_suite" $(TESTSUITEFLAGS) || fail=1; \
		echo ""; \
	done; \
	exit $$fail

clean-local:
	for test_suite in $(check_AUTOTESTS); do \
		test ! -f "$$test_suite" || $(SHELL) "./$$test_suite" --clean; \
	done

package.m4: $(top_srcdir)/configure.ac
	$(AM_V_GEN):; \
	{ \
	  echo 'm4_define([AT_PACKAGE_NAME], [$(PACKAGE_NAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_TARNAME], [$(PACKAGE_TARNAME)])' ; \
	  echo 'm4_define([AT_PACKAGE_VERSION], [$(PACKAGE_VERSION)])' ; \
	  echo 'm4_define([AT_PACKAGE_STRING], [$(PACKAGE_STRING)])' ; \
	  echo 'm4_define([AT_PACKAGE_BUGREPORT], [$(PACKAGE_BUGREPORT)])' ; \
	} > $@

test_inputs_libesedb.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) libesedb

test_inputs_pyesedb.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) pyesedb

test_inputs_esedbinfo.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) esedbinfo

test_inputs_esedbexport.at: generate_test_inputs.sh
	$(SHELL) $(srcdir)/generate_test_inputs.sh $(PACKAGE_NAME) esedbexport

test_library: package.m4 test_inputs_libesedb.at test_library.at test_macros.at

test_manpages: package.m4 test_manpages.at

test_python_module: package.m4 test_inputs_pyesedb.at test_python_module.at

test_tools: package.m4 test_inputs_esedbexport.at test_inputs_esedbinfo.at test_macros.at test_tools.at

EXTRA_DIST = \
	$(check_AUTOTESTS:=.at) \
	$(check_PYTHON_SCRIPTS) \
	generate_test_inputs.sh \
	package.m4 \
	test_macros.at

CLEANFILES = \
	$(check_AUTOTESTS) \
	*.exe \
	*.tmp \
	notify_stream.log \
	test_inputs_*.at

DISTCLEANFILES = \
	*.log \
	atconfig \
	Makefile \
	Makefile.in


