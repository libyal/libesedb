/*
 * Library page_header type test program
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
#include "esedb_test_macros.h"
#include "esedb_test_memory.h"
#include "esedb_test_unused.h"

#include "../libesedb/libesedb_io_handle.h"
#include "../libesedb/libesedb_page_header.h"

uint8_t esedb_test_page_header_data1[ 40 ] = {
	0xc2, 0x26, 0x01, 0x00, 0xbe, 0x7e, 0xbe, 0x7e, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0xc4, 0x0f, 0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x03, 0x28, 0x00, 0x00 };

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_page_header_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_header_initialize(
     void )
{
	libcerror_error_t *error            = NULL;
	libesedb_page_header_t *page_header = NULL;
	int result                          = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests     = 1;
	int number_of_memset_fail_tests     = 1;
	int test_number                     = 0;
#endif

	/* Test regular cases
	 */
	result = libesedb_page_header_initialize(
	          &page_header,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_header",
	 page_header );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_header_free(
	          &page_header,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_header",
	 page_header );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_header_initialize(
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

	page_header = (libesedb_page_header_t *) 0x12345678UL;

	result = libesedb_page_header_initialize(
	          &page_header,
	          &error );

	page_header = NULL;

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
		/* Test libesedb_page_header_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_page_header_initialize(
		          &page_header,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( page_header != NULL )
			{
				libesedb_page_header_free(
				 &page_header,
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
			 "page_header",
			 page_header );

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
		/* Test libesedb_page_header_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_page_header_initialize(
		          &page_header,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( page_header != NULL )
			{
				libesedb_page_header_free(
				 &page_header,
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
			 "page_header",
			 page_header );

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
	if( page_header != NULL )
	{
		libesedb_page_header_free(
		 &page_header,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_page_header_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_header_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_page_header_free(
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

/* Tests the libesedb_page_header_read_data function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_header_read_data(
     void )
{
	libcerror_error_t *error            = NULL;
	libesedb_io_handle_t *io_handle     = NULL;
	libesedb_page_header_t *page_header = NULL;
	int result                          = 0;

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
	io_handle->page_size       = 4096;

	result = libesedb_page_header_initialize(
	          &page_header,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_header",
	 page_header );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_page_header_read_data(
	          page_header,
	          io_handle,
	          esedb_test_page_header_data1,
	          40,
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
	result = libesedb_page_header_read_data(
	          NULL,
	          io_handle,
	          esedb_test_page_header_data1,
	          40,
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

	result = libesedb_page_header_read_data(
	          page_header,
	          io_handle,
	          NULL,
	          40,
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

	result = libesedb_page_header_read_data(
	          page_header,
	          io_handle,
	          esedb_test_page_header_data1,
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

	result = libesedb_page_header_read_data(
	          page_header,
	          io_handle,
	          esedb_test_page_header_data1,
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
	result = libesedb_page_header_free(
	          &page_header,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_header",
	 page_header );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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
	if( page_header != NULL )
	{
		libesedb_page_header_free(
		 &page_header,
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

/* Tests the libesedb_page_header_get_previous_page_number function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_header_get_previous_page_number(
     libesedb_page_header_t *page_header )
{
	libcerror_error_t *error      = NULL;
	uint32_t previous_page_number = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libesedb_page_header_get_previous_page_number(
	          page_header,
	          &previous_page_number,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_UINT32(
	 "previous_page_number",
	 previous_page_number,
	  0 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_header_get_previous_page_number(
	          NULL,
	          &previous_page_number,
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

	result = libesedb_page_header_get_previous_page_number(
	          page_header,
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
	return( 0 );
}

/* Tests the libesedb_page_header_get_next_page_number function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_header_get_next_page_number(
     libesedb_page_header_t *page_header )
{
	libcerror_error_t *error  = NULL;
	uint32_t next_page_number = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libesedb_page_header_get_next_page_number(
	          page_header,
	          &next_page_number,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_UINT32(
	 "next_page_number",
	 next_page_number,
	  0 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_header_get_next_page_number(
	          NULL,
	          &next_page_number,
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

	result = libesedb_page_header_get_next_page_number(
	          page_header,
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
	return( 0 );
}

/* Tests the libesedb_page_header_get_father_data_page_object_identifier function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_header_get_father_data_page_object_identifier(
     libesedb_page_header_t *page_header )
{
	libcerror_error_t *error                    = NULL;
	uint32_t father_data_page_object_identifier = 0;
	int result                                  = 0;

	/* Test regular cases
	 */
	result = libesedb_page_header_get_father_data_page_object_identifier(
	          page_header,
	          &father_data_page_object_identifier,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_UINT32(
	 "father_data_page_object_identifier",
	 father_data_page_object_identifier,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_header_get_father_data_page_object_identifier(
	          NULL,
	          &father_data_page_object_identifier,
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

	result = libesedb_page_header_get_father_data_page_object_identifier(
	          page_header,
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
	return( 0 );
}

/* Tests the libesedb_page_header_get_flags function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_header_get_flags(
     libesedb_page_header_t *page_header )
{
	libcerror_error_t *error = NULL;
	uint32_t flags           = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_page_header_get_flags(
	          page_header,
	          &flags,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_UINT32(
	 "flags",
	 flags,
	 (uint32_t) 0x00002803UL );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_page_header_get_flags(
	          NULL,
	          &flags,
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

	result = libesedb_page_header_get_flags(
	          page_header,
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
#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	libcerror_error_t *error            = NULL;
	libesedb_io_handle_t *io_handle     = NULL;
	libesedb_page_header_t *page_header = NULL;
	int result                          = 0;

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	ESEDB_TEST_UNREFERENCED_PARAMETER( argc )
	ESEDB_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	ESEDB_TEST_RUN(
	 "libesedb_page_header_initialize",
	 esedb_test_page_header_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_page_header_free",
	 esedb_test_page_header_free );

	ESEDB_TEST_RUN(
	 "libesedb_page_header_read_data",
	 esedb_test_page_header_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

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
	io_handle->page_size       = 4096;

	result = libesedb_page_header_initialize(
	          &page_header,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "page_header",
	 page_header );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_page_header_read_data(
	          page_header,
	          io_handle,
	          esedb_test_page_header_data1,
	          40,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Run tests
	 */
	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_page_header_get_previous_page_number",
	 esedb_test_page_header_get_previous_page_number,
	 page_header );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_page_header_get_next_page_number",
	 esedb_test_page_header_get_next_page_number,
	 page_header );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_page_header_get_father_data_page_object_identifier",
	 esedb_test_page_header_get_father_data_page_object_identifier,
	 page_header );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_page_header_get_flags",
	 esedb_test_page_header_get_flags,
	 page_header );

	/* Clean up
	 */
	result = libesedb_page_header_free(
	          &page_header,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "page_header",
	 page_header );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

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

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )
on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( page_header != NULL )
	{
		libesedb_page_header_free(
		 &page_header,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libesedb_io_handle_free(
		 &io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */
}

