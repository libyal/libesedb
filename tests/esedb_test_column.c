/*
 * Library column type test program
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
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "esedb_test_libcerror.h"
#include "esedb_test_libesedb.h"
#include "esedb_test_macros.h"
#include "esedb_test_memory.h"
#include "esedb_test_unused.h"

#include "../libesedb/libesedb_catalog_definition.h"
#include "../libesedb/libesedb_column.h"
#include "../libesedb/libesedb_io_handle.h"

uint8_t esedb_test_column_catalog_definition_data1[ 45 ] = {
	0x08, 0x80, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x14, 0x00, 0x00, 0x00, 0xff, 0x00,
	0x0b, 0x00, 0x4d, 0x53, 0x79, 0x73, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x73 };

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_column_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_initialize(
     void )
{
	libcerror_error_t *error                          = NULL;
	libesedb_catalog_definition_t *catalog_definition = NULL;
	libesedb_column_t *column                         = NULL;
	libesedb_io_handle_t *io_handle                   = NULL;
	int result                                        = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests                   = 1;
	int number_of_memset_fail_tests                   = 1;
	int test_number                                   = 0;
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

	result = libesedb_catalog_definition_initialize(
	          &catalog_definition,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "catalog_definition",
	 catalog_definition );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_column_initialize(
	          &column,
	          io_handle,
	          catalog_definition,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "column",
	 column );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_column_free(
	          &column,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "column",
	 column );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_column_initialize(
	          NULL,
	          io_handle,
	          catalog_definition,
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

	column = (libesedb_column_t *) 0x12345678UL;

	result = libesedb_column_initialize(
	          &column,
	          io_handle,
	          catalog_definition,
	          &error );

	column = NULL;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libesedb_column_initialize(
	          &column,
	          NULL,
	          catalog_definition,
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

	result = libesedb_column_initialize(
	          &column,
	          io_handle,
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
		/* Test libesedb_column_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_column_initialize(
		          &column,
		          io_handle,
		          catalog_definition,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( column != NULL )
			{
				libesedb_column_free(
				 &column,
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
			 "column",
			 column );

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
		/* Test libesedb_column_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_column_initialize(
		          &column,
		          io_handle,
		          catalog_definition,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( column != NULL )
			{
				libesedb_column_free(
				 &column,
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
			 "column",
			 column );

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
	result = libesedb_catalog_definition_free(
	          &catalog_definition,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "catalog_definition",
	 catalog_definition );

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
	if( column != NULL )
	{
		libesedb_column_free(
		 &column,
		 NULL );
	}
	if( catalog_definition != NULL )
	{
		libesedb_catalog_definition_free(
		 &catalog_definition,
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

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

/* Tests the libesedb_column_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_column_free(
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

/* Tests the libesedb_column_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_get_identifier(
     libesedb_column_t *column )
{
	libcerror_error_t *error = NULL;
	uint32_t identifier      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_column_get_identifier(
	          column,
	          &identifier,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_UINT32(
	 "identifier",
	 identifier,
	 2 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_column_get_identifier(
	          NULL,
	          &identifier,
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

	result = libesedb_column_get_identifier(
	          column,
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

/* Tests the libesedb_column_get_type function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_get_type(
     libesedb_column_t *column )
{
	libcerror_error_t *error = NULL;
	uint32_t column_type     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_column_get_type(
	          column,
	          &column_type,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_UINT32(
	 "column_type",
	 column_type,
	 4 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_column_get_type(
	          NULL,
	          &column_type,
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

	result = libesedb_column_get_type(
	          column,
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

/* Tests the libesedb_column_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_get_utf8_name_size(
     libesedb_column_t *column )
{
	libcerror_error_t *error        = NULL;
	libesedb_io_handle_t *io_handle = NULL;
	size_t utf8_string_size         = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libesedb_column_get_utf8_name_size(
	          column,
	          &utf8_string_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 12 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_column_get_utf8_name_size(
	          NULL,
	          &utf8_string_size,
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

	io_handle = ( (libesedb_internal_column_t *) column )->io_handle;

	( (libesedb_internal_column_t *) column )->io_handle = NULL;

	result = libesedb_column_get_utf8_name_size(
	          column,
	          &utf8_string_size,
	          &error );

	( (libesedb_internal_column_t *) column )->io_handle = io_handle;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libesedb_column_get_utf8_name_size(
	          column,
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

/* Tests the libesedb_column_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_get_utf8_name(
     libesedb_column_t *column )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 12 ] = {
		'M', 'S', 'y', 's', 'O', 'b', 'j', 'e', 'c', 't', 's', 0 };

	libcerror_error_t *error           = NULL;
	libesedb_io_handle_t *io_handle    = NULL;
	int result                         = 0;

	/* Test regular cases
	 */
	result = libesedb_column_get_utf8_name(
	          column,
	          utf8_string,
	          32,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf8_string,
	          expected_utf8_string,
	          12 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libesedb_column_get_utf8_name(
	          NULL,
	          utf8_string,
	          32,
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

	io_handle = ( (libesedb_internal_column_t *) column )->io_handle;

	( (libesedb_internal_column_t *) column )->io_handle = NULL;

	result = libesedb_column_get_utf8_name(
	          column,
	          utf8_string,
	          32,
	          &error );

	( (libesedb_internal_column_t *) column )->io_handle = io_handle;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libesedb_column_get_utf8_name(
	          column,
	          NULL,
	          32,
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

/* Tests the libesedb_column_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_get_utf16_name_size(
     libesedb_column_t *column )
{
	libcerror_error_t *error        = NULL;
	libesedb_io_handle_t *io_handle = NULL;
	size_t utf16_string_size        = 0;
	int result                      = 0;

	/* Test regular cases
	 */
	result = libesedb_column_get_utf16_name_size(
	          column,
	          &utf16_string_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 12 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_column_get_utf16_name_size(
	          NULL,
	          &utf16_string_size,
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

	io_handle = ( (libesedb_internal_column_t *) column )->io_handle;

	( (libesedb_internal_column_t *) column )->io_handle = NULL;

	result = libesedb_column_get_utf16_name_size(
	          column,
	          &utf16_string_size,
	          &error );

	( (libesedb_internal_column_t *) column )->io_handle = io_handle;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libesedb_column_get_utf16_name_size(
	          column,
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

/* Tests the libesedb_column_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_get_utf16_name(
     libesedb_column_t *column )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 12 ] = {
		'M', 'S', 'y', 's', 'O', 'b', 'j', 'e', 'c', 't', 's', 0 };

	libcerror_error_t *error             = NULL;
	libesedb_io_handle_t *io_handle      = NULL;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libesedb_column_get_utf16_name(
	          column,
	          utf16_string,
	          32,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          utf16_string,
	          expected_utf16_string,
	          12 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libesedb_column_get_utf16_name(
	          NULL,
	          utf16_string,
	          32,
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

	io_handle = ( (libesedb_internal_column_t *) column )->io_handle;

	( (libesedb_internal_column_t *) column )->io_handle = NULL;

	result = libesedb_column_get_utf16_name(
	          column,
	          utf16_string,
	          32,
	          &error );

	( (libesedb_internal_column_t *) column )->io_handle = io_handle;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libesedb_column_get_utf16_name(
	          column,
	          NULL,
	          32,
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

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

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
	libcerror_error_t *error                          = NULL;
	libesedb_catalog_definition_t *catalog_definition = NULL;
	libesedb_column_t *column                         = NULL;
	libesedb_io_handle_t *io_handle                   = NULL;
	int result                                        = 0;
#endif

	ESEDB_TEST_UNREFERENCED_PARAMETER( argc )
	ESEDB_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	ESEDB_TEST_RUN(
	 "libesedb_column_initialize",
	 esedb_test_column_initialize );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	ESEDB_TEST_RUN(
	 "libesedb_column_free",
	 esedb_test_column_free );

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize column for tests
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

	result = libesedb_catalog_definition_initialize(
	          &catalog_definition,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "catalog_definition",
	 catalog_definition );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_column_catalog_definition_data1,
	          45,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_column_initialize(
	          &column,
	          io_handle,
	          catalog_definition,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "column",
	 column );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_column_get_identifier",
	 esedb_test_column_get_identifier,
	 column );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_column_get_type",
	 esedb_test_column_get_type,
	 column );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_column_get_utf8_name_size",
	 esedb_test_column_get_utf8_name_size,
	 column );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_column_get_utf8_name",
	 esedb_test_column_get_utf8_name,
	 column );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_column_get_utf16_name_size",
	 esedb_test_column_get_utf16_name_size,
	 column );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_column_get_utf16_name",
	 esedb_test_column_get_utf16_name,
	 column );

	/* Clean up
	 */
	result = libesedb_column_free(
	          &column,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "column",
	 column );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_catalog_definition_free(
	          &catalog_definition,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "catalog_definition",
	 catalog_definition );

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

on_error:
#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( column != NULL )
	{
		libesedb_column_free(
		 &column,
		 NULL );
	}
	if( catalog_definition != NULL )
	{
		libesedb_catalog_definition_free(
		 &catalog_definition,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libesedb_io_handle_free(
		 &io_handle,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

