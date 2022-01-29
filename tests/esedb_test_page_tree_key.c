/*
 * Library page_tree_key type test program
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include "esedb_test_libfdata.h"
#include "esedb_test_macros.h"
#include "esedb_test_memory.h"
#include "esedb_test_unused.h"

#include "../libesedb/libesedb_definitions.h"
#include "../libesedb/libesedb_page_tree_key.h"

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_page_tree_key_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_key_initialize(
     void )
{
	libcerror_error_t *error                = NULL;
	libesedb_page_tree_key_t *page_tree_key = NULL;
	int result                              = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests         = 1;
	int number_of_memset_fail_tests         = 1;
	int test_number                         = 0;
#endif

	/* Test regular cases
	 */
	result = libesedb_page_tree_key_initialize(
	          &page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_tree_key",
	 page_tree_key );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_tree_key_free(
	          &page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_tree_key",
	 page_tree_key );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_tree_key_initialize(
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

	page_tree_key = (libesedb_page_tree_key_t *) 0x12345678UL;

	result = libesedb_page_tree_key_initialize(
	          &page_tree_key,
	          &error );

	page_tree_key = NULL;

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
		/* Test libesedb_page_tree_key_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_page_tree_key_initialize(
		          &page_tree_key,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( page_tree_key != NULL )
			{
				libesedb_page_tree_key_free(
				 &page_tree_key,
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
			 "page_tree_key",
			 page_tree_key );

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
		/* Test libesedb_page_tree_key_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_page_tree_key_initialize(
		          &page_tree_key,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( page_tree_key != NULL )
			{
				libesedb_page_tree_key_free(
				 &page_tree_key,
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
			 "page_tree_key",
			 page_tree_key );

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
	if( page_tree_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &page_tree_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_page_tree_key_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_key_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_page_tree_key_free(
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

/* Tests the libesedb_page_tree_key_set_data function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_key_set_data(
     void )
{
	uint8_t key_data[ 6 ]                   = { 0x01, 0x7f, 0x80, 0x00, 0x00, 0x02 };

	libcerror_error_t *error                = NULL;
	libesedb_page_tree_key_t *page_tree_key = NULL;
	int result                              = 0;

	/* Initialize test
	 */
	result = libesedb_page_tree_key_initialize(
	          &page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_tree_key",
	 page_tree_key );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_page_tree_key_set_data(
	          page_tree_key,
	          key_data,
	          6,
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
	result = libesedb_page_tree_key_set_data(
	          NULL,
	          key_data,
	          6,
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

	result = libesedb_page_tree_key_set_data(
	          page_tree_key,
	          NULL,
	          6,
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

	result = libesedb_page_tree_key_set_data(
	          page_tree_key,
	          key_data,
	          (size_t) SSIZE_MAX + 1,
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
	result = libesedb_page_tree_key_free(
	          &page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_tree_key",
	 page_tree_key );

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
	if( page_tree_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &page_tree_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_page_tree_key_append_data function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_key_append_data(
     void )
{
	uint8_t key_data[ 6 ]                   = { 0x01, 0x7f, 0x80, 0x00, 0x00, 0x02 };
	uint8_t additional_key_data[ 3 ]        = { 0x7f, 0x80, 0x00 };

	libcerror_error_t *error                = NULL;
	libesedb_page_tree_key_t *page_tree_key = NULL;
	int result                              = 0;

	/* Initialize test
	 */
	result = libesedb_page_tree_key_initialize(
	          &page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_tree_key",
	 page_tree_key );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_tree_key_set_data(
	          page_tree_key,
	          key_data,
	          6,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_page_tree_key_append_data(
	          page_tree_key,
	          additional_key_data,
	          3,
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
	result = libesedb_page_tree_key_append_data(
	          NULL,
	          additional_key_data,
	          3,
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

	result = libesedb_page_tree_key_append_data(
	          page_tree_key,
	          NULL,
	          3,
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

	result = libesedb_page_tree_key_append_data(
	          page_tree_key,
	          additional_key_data,
	          (size_t) SSIZE_MAX + 1,
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
	result = libesedb_page_tree_key_free(
	          &page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_tree_key",
	 page_tree_key );

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
	if( page_tree_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &page_tree_key,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_page_tree_key_compare function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_tree_key_compare(
     void )
{
	uint8_t first_key_data[ 6 ]                    = { 0x01, 0x7f, 0x80, 0x00, 0x00, 0x02 };
	uint8_t second_key_data[ 6 ]                   = { 0x01, 0x7f, 0x80, 0x00, 0x00, 0x02 };

	libcerror_error_t *error                       = NULL;
	libesedb_page_tree_key_t *first_page_tree_key  = NULL;
	libesedb_page_tree_key_t *second_page_tree_key = NULL;
	int result                                     = 0;

	/* Initialize test
	 */
	result = libesedb_page_tree_key_initialize(
	          &first_page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "first_page_tree_key",
	 first_page_tree_key );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_tree_key_set_data(
	          first_page_tree_key,
	          first_key_data,
	          6,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	first_page_tree_key->type = LIBESEDB_KEY_TYPE_INDEX_VALUE;

	result = libesedb_page_tree_key_initialize(
	          &second_page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "second_page_tree_key",
	 second_page_tree_key );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_tree_key_set_data(
	          second_page_tree_key,
	          second_key_data,
	          6,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	second_page_tree_key->type = LIBESEDB_KEY_TYPE_LEAF;

	/* Test regular cases
	 */
	result = libesedb_page_tree_key_compare(
	          first_page_tree_key,
	          second_page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 LIBFDATA_COMPARE_EQUAL );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_tree_key_compare(
	          NULL,
	          second_page_tree_key,
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

	result = libesedb_page_tree_key_compare(
	          first_page_tree_key,
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

	/* Clean up
	 */
	result = libesedb_page_tree_key_free(
	          &second_page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "second_page_tree_key",
	 second_page_tree_key );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_tree_key_free(
	          &first_page_tree_key,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "first_page_tree_key",
	 first_page_tree_key );

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
	if( second_page_tree_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &second_page_tree_key,
		 NULL );
	}
	if( first_page_tree_key != NULL )
	{
		libesedb_page_tree_key_free(
		 &first_page_tree_key,
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
	 "libesedb_page_tree_key_initialize",
	 esedb_test_page_tree_key_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_key_free",
	 esedb_test_page_tree_key_free );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_key_set_data",
	 esedb_test_page_tree_key_set_data );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_key_append_data",
	 esedb_test_page_tree_key_append_data );

	ESEDB_TEST_RUN(
	 "libesedb_page_tree_key_compare",
	 esedb_test_page_tree_key_compare );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

