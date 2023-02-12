/*
 * Library page_tree type testing program
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "esedb_test_libcerror.h"
#include "esedb_test_libesedb.h"
#include "esedb_test_macros.h"
#include "esedb_test_memory.h"
#include "esedb_test_unused.h"

#include "../libesedb/libesedb_io_handle.h"
#include "../libesedb/libesedb_page_tree.h"

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_page_tree_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libesedb_io_handle_t *io_handle = NULL;
	libesedb_page_tree_t *page_tree = NULL;
	int result                      = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests = 3;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Initialize test
	 */
	result = libesedb_io_handle_initialize(
	          &io_handle,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	io_handle->format_revision = 0x0000000c;
	io_handle->file_size       = 16777216;
	io_handle->page_size       = 4096;

	/* Test page_tree initialization
	 */
	result = libesedb_page_tree_initialize(
	          &page_tree,
	          io_handle,
	          NULL,
	          NULL,
	          0,
	          1,
	          NULL,
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_tree",
	 page_tree );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_tree_free(
	          &page_tree,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_tree",
	 page_tree );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_tree_initialize(
	          NULL,
	          io_handle,
	          NULL,
	          NULL,
	          0,
	          1,
	          NULL,
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	page_tree = (libesedb_page_tree_t *) 0x12345678UL;

	result = libesedb_page_tree_initialize(
	          &page_tree,
	          io_handle,
	          NULL,
	          NULL,
	          0,
	          1,
	          NULL,
	          NULL,
	          &error );

	page_tree = NULL;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libesedb_page_tree_initialize(
	          &page_tree,
	          NULL,
	          NULL,
	          NULL,
	          0,
	          1,
	          NULL,
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_ESEDB_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libesedb_page_tree_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_page_tree_initialize(
		          &page_tree,
		          io_handle,
		          NULL,
		          NULL,
		          0,
		          1,
		          NULL,
		          NULL,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( page_tree != NULL )
			{
				libesedb_page_tree_free(
				 &page_tree,
				 NULL );
			}
		}
		else
		{
			ESEDB_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			ESEDB_TEST_ASSERT_IS_NULL(
			 "page_tree",
			 page_tree );

			ESEDB_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libesedb_page_tree_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_page_tree_initialize(
		          &page_tree,
		          io_handle,
		          NULL,
		          NULL,
		          0,
		          1,
		          NULL,
		          NULL,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( page_tree != NULL )
			{
				libesedb_page_tree_free(
				 &page_tree,
				 NULL );
			}
		}
		else
		{
			ESEDB_TEST_ASSERT_EQUAL_INT(
			 "result",
			 result,
			 -1 );

			ESEDB_TEST_ASSERT_IS_NULL(
			 "page_tree",
			 page_tree );

			ESEDB_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_ESEDB_TEST_MEMORY ) */

	/* Clean up
	 */
	result = libesedb_io_handle_free(
	          &io_handle,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( page_tree != NULL )
	{
		libesedb_page_tree_free(
		 &page_tree,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libesedb_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_page_tree_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_page_tree_free(
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libesedb_page_tree_read_root_page_header function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_read_root_page_header(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_page_tree_read_root_page_header(
	          NULL,
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc ESEDB_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] ESEDB_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc ESEDB_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] ESEDB_TEST_ATTRIBUTE_UNUSED )
#endif
{
	ESEDB_TEST_UNREFERENCED_PARAMETER( argc )
	ESEDB_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_initialize",
	 esedb_test_page_tree_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_free",
	 esedb_test_page_tree_free );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_read_root_page_header",
	 esedb_test_page_tree_read_root_page_header );

	/* TODO: add tests for libesedb_page_tree_read_space_trees */

	/* TODO: add tests for libesedb_page_tree_get_key */

	/* TODO: add tests for libesedb_page_tree_get_number_of_leaf_values_from_page */

	/* TODO: add tests for libesedb_page_tree_get_number_of_leaf_values */

	/* TODO: add tests for libesedb_page_tree_get_leaf_value_by_index_from_page */

	/* TODO: add tests for libesedb_page_tree_get_leaf_value_by_index */

	/* TODO: add tests for libesedb_page_tree_get_leaf_value_by_key_from_page */

	/* TODO: add tests for libesedb_page_tree_get_leaf_value_by_key */

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

