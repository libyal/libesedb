/*
 * Library page_tree_value type test program
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

#include "../libesedb/libesedb_page_tree_value.h"

uint8_t esedb_test_page_tree_value_data1[ 14 ] = {
	0x08, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x12, 0x7f, 0x80, 0x02, 0x13, 0x00, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_page_tree_value_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_value_initialize(
     void )
{
	libcerror_error_t *error                    = NULL;
	libesedb_page_tree_value_t *page_tree_value = NULL;
	int result                                  = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests             = 1;
	int number_of_memset_fail_tests             = 1;
	int test_number                             = 0;
#endif

	/* Test regular cases
	 */
	result = libesedb_page_tree_value_initialize(
	          &page_tree_value,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_tree_value",
	 page_tree_value );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_tree_value_free(
	          &page_tree_value,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_tree_value",
	 page_tree_value );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_tree_value_initialize(
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

	page_tree_value = (libesedb_page_tree_value_t *) 0x12345678UL;

	result = libesedb_page_tree_value_initialize(
	          &page_tree_value,
	          &error );

	page_tree_value = NULL;

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
		/* Test libesedb_page_tree_value_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_page_tree_value_initialize(
		          &page_tree_value,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( page_tree_value != NULL )
			{
				libesedb_page_tree_value_free(
				 &page_tree_value,
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
			 "page_tree_value",
			 page_tree_value );

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
		/* Test libesedb_page_tree_value_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_page_tree_value_initialize(
		          &page_tree_value,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( page_tree_value != NULL )
			{
				libesedb_page_tree_value_free(
				 &page_tree_value,
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
			 "page_tree_value",
			 page_tree_value );

			ESEDB_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_ESEDB_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_page_tree_value_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_value_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_page_tree_value_free(
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

/* Tests the libesedb_page_tree_value_read_data function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_value_read_data(
     void )
{
	libcerror_error_t *error                    = NULL;
	libesedb_page_tree_value_t *page_tree_value = NULL;
	int result                                  = 0;

	/* Initialize test
	 */
	result = libesedb_page_tree_value_initialize(
	          &page_tree_value,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_tree_value",
	 page_tree_value );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_page_tree_value_read_data(
	          page_tree_value,
	          esedb_test_page_tree_value_data1,
	          14,
	          0,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_tree_value_read_data(
	          NULL,
	          esedb_test_page_tree_value_data1,
	          14,
	          0,
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

	result = libesedb_page_tree_value_read_data(
	          page_tree_value,
	          NULL,
	          14,
	          0,
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

	result = libesedb_page_tree_value_read_data(
	          page_tree_value,
	          esedb_test_page_tree_value_data1,
	          (size_t) SSIZE_MAX + 1,
	          0,
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

	result = libesedb_page_tree_value_read_data(
	          page_tree_value,
	          esedb_test_page_tree_value_data1,
	          0,
	          0,
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

	/* Clean up
	 */
	result = libesedb_page_tree_value_free(
	          &page_tree_value,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_tree_value",
	 page_tree_value );

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
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
		 NULL );
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
	 "libesedb_page_tree_value_initialize",
	 esedb_test_page_tree_value_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_value_free",
	 esedb_test_page_tree_value_free );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_value_read_data",
	 esedb_test_page_tree_value_read_data );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

