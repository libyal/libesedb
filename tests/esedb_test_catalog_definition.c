/*
 * Library catalog_definition type test program
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

/* Catalog definition: MSysObjects with name
 */
uint8_t esedb_test_catalog_definition_data1[ 45 ] = {
	0x08, 0x80, 0x20, 0x00, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04, 0x00,
	0x00, 0x00, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x14, 0x00, 0x00, 0x00, 0xff, 0x00,
	0x0b, 0x00, 0x4d, 0x53, 0x79, 0x73, 0x4f, 0x62, 0x6a, 0x65, 0x63, 0x74, 0x73 };

/* Catalog definition: SSystem_Photo_TagViewAggregate with template_name
 */
uint8_t esedb_test_catalog_definition_data2[ 97 ] = {
	0x07, 0x82, 0x1f, 0x00, 0x0f, 0x00, 0x00, 0x00, 0x02, 0x00, 0x27, 0x02, 0x00, 0x00, 0x0c, 0x00,
	0x00, 0x00, 0xd0, 0x07, 0x00, 0x00, 0x90, 0x10, 0x00, 0x00, 0xb0, 0x04, 0x00, 0x00, 0x80, 0x14,
	0x00, 0x14, 0x80, 0x2f, 0x00, 0x53, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x5f, 0x4d, 0x75, 0x73,
	0x69, 0x63, 0x5f, 0x41, 0x72, 0x74, 0x69, 0x73, 0x74, 0x6d, 0x73, 0x73, 0x72, 0x63, 0x68, 0x2e,
	0x64, 0x6c, 0x6c, 0x21, 0x43, 0x6f, 0x6e, 0x63, 0x61, 0x74, 0x56, 0x65, 0x63, 0x74, 0x56, 0x61,
	0x6c, 0x75, 0x65, 0x73, 0x00, 0x01, 0x04, 0x00, 0x01, 0x26, 0x02, 0x00, 0x00, 0x0c, 0x00, 0x00,
	0x00 };

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_catalog_definition_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_initialize(
     void )
{
	libcerror_error_t *error                          = NULL;
	libesedb_catalog_definition_t *catalog_definition = NULL;
	int result                                        = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests                   = 1;
	int number_of_memset_fail_tests                   = 1;
	int test_number                                   = 0;
#endif

	/* Test regular cases
	 */
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

	/* Test error cases
	 */
	result = libesedb_catalog_definition_initialize(
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

	catalog_definition = (libesedb_catalog_definition_t *) 0x12345678UL;

	result = libesedb_catalog_definition_initialize(
	          &catalog_definition,
	          &error );

	catalog_definition = NULL;

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
		/* Test libesedb_catalog_definition_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_catalog_definition_initialize(
		          &catalog_definition,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( catalog_definition != NULL )
			{
				libesedb_catalog_definition_free(
				 &catalog_definition,
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
			 "catalog_definition",
			 catalog_definition );

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
		/* Test libesedb_catalog_definition_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_catalog_definition_initialize(
		          &catalog_definition,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( catalog_definition != NULL )
			{
				libesedb_catalog_definition_free(
				 &catalog_definition,
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
			 "catalog_definition",
			 catalog_definition );

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
	if( catalog_definition != NULL )
	{
		libesedb_catalog_definition_free(
		 &catalog_definition,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_catalog_definition_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_catalog_definition_free(
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

/* Tests the libesedb_catalog_definition_read_data function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_read_data(
     void )
{
	libcerror_error_t *error                          = NULL;
	libesedb_catalog_definition_t *catalog_definition = NULL;
	int result                                        = 0;

	/* Initialize test
	 */
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
	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
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

	/* Test error cases
	 */
	result = libesedb_catalog_definition_read_data(
	          NULL,
	          esedb_test_catalog_definition_data1,
	          45,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          NULL,
	          45,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
	          (size_t) SSIZE_MAX + 1,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
	          0,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	/* Test with variable_size_data_types_offset > data_size
	 */
	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
	          30,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	/* Test libesedb_catalog_definition_read_data with malloc failing for creation of name
	 */
	esedb_test_malloc_attempts_before_fail = 0;

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
	          45,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	if( esedb_test_malloc_attempts_before_fail != -1 )
	{
		esedb_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libesedb_catalog_definition_read_data with memcpy failing for creation of name
	 */
	esedb_test_memcpy_attempts_before_fail = 0;

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
	          45,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	if( esedb_test_memcpy_attempts_before_fail != -1 )
	{
		esedb_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */

	/* Test libesedb_catalog_definition_read_data with malloc failing for creation of template_name
	 */
	esedb_test_malloc_attempts_before_fail = 0;

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data2,
	          97,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	if( esedb_test_malloc_attempts_before_fail != -1 )
	{
		esedb_test_malloc_attempts_before_fail = -1;
	}
	else
	{
		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#if defined( OPTIMIZATION_DISABLED )

	/* Test libesedb_catalog_definition_read_data with memcpy failing for creation of template_name
	 */
	esedb_test_memcpy_attempts_before_fail = 0;

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data2,
	          97,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	if( esedb_test_memcpy_attempts_before_fail != -1 )
	{
		esedb_test_memcpy_attempts_before_fail = -1;
	}
	else
	{
		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( OPTIMIZATION_DISABLED ) */

/* TODO test catalog definition with default_value */

#endif /* defined( HAVE_ESEDB_TEST_MEMORY ) */

	/* Test with last_fixed_size_data_type < 5
	 */
	esedb_test_catalog_definition_data1[ 0 ] = 0x00;

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
	          45,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	esedb_test_catalog_definition_data1[ 0 ] = 0x08;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	/* Test with last_fixed_size_data_type > 12
	 */
	esedb_test_catalog_definition_data1[ 0 ] = 0xff;

	result = libesedb_catalog_definition_read_data(
	          catalog_definition,
	          esedb_test_catalog_definition_data1,
	          45,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	esedb_test_catalog_definition_data1[ 0 ] = 0x08;

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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( catalog_definition != NULL )
	{
		libesedb_catalog_definition_free(
		 &catalog_definition,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_catalog_definition_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_identifier(
     libesedb_catalog_definition_t *catalog_definition )
{
	libcerror_error_t *error = NULL;
	uint32_t identifier      = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_identifier(
	          catalog_definition,
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
	result = libesedb_catalog_definition_get_identifier(
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

	result = libesedb_catalog_definition_get_identifier(
	          catalog_definition,
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

/* Tests the libesedb_catalog_definition_get_column_type function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_column_type(
     libesedb_catalog_definition_t *catalog_definition )
{
	libcerror_error_t *error = NULL;
	uint32_t column_type     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_column_type(
	          catalog_definition,
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
	result = libesedb_catalog_definition_get_column_type(
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

	result = libesedb_catalog_definition_get_column_type(
	          catalog_definition,
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

/* Tests the libesedb_catalog_definition_get_utf8_name_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf8_name_size(
     libesedb_catalog_definition_t *catalog_definition )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf8_name_size(
	          catalog_definition,
	          &utf8_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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
	result = libesedb_catalog_definition_get_utf8_name_size(
	          NULL,
	          &utf8_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf8_name_size(
	          catalog_definition,
	          NULL,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

/* Tests the libesedb_catalog_definition_get_utf8_name function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf8_name(
     libesedb_catalog_definition_t *catalog_definition )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 12 ] = {
		'M', 'S', 'y', 's', 'O', 'b', 'j', 'e', 'c', 't', 's', 0 };

	libcerror_error_t *error           = NULL;
	int result                         = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf8_name(
	          catalog_definition,
	          utf8_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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
	result = libesedb_catalog_definition_get_utf8_name(
	          NULL,
	          utf8_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf8_name(
	          catalog_definition,
	          NULL,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

/* Tests the libesedb_catalog_definition_get_utf16_name_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf16_name_size(
     libesedb_catalog_definition_t *catalog_definition )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf16_name_size(
	          catalog_definition,
	          &utf16_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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
	result = libesedb_catalog_definition_get_utf16_name_size(
	          NULL,
	          &utf16_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf16_name_size(
	          catalog_definition,
	          NULL,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

/* Tests the libesedb_catalog_definition_get_utf16_name function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf16_name(
     libesedb_catalog_definition_t *catalog_definition )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 12 ] = {
		'M', 'S', 'y', 's', 'O', 'b', 'j', 'e', 'c', 't', 's', 0 };

	libcerror_error_t *error             = NULL;
	int result                           = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf16_name(
	          catalog_definition,
	          utf16_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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
	result = libesedb_catalog_definition_get_utf16_name(
	          NULL,
	          utf16_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf16_name(
	          catalog_definition,
	          NULL,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

/* Tests the libesedb_catalog_definition_get_utf8_template_name_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf8_template_name_size(
     libesedb_catalog_definition_t *catalog_definition )
{
	libcerror_error_t *error = NULL;
	uint8_t *template_name   = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf8_template_name_size(
	          catalog_definition,
	          &utf8_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 28 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	template_name = catalog_definition->template_name;

	catalog_definition->template_name = NULL;

	result = libesedb_catalog_definition_get_utf8_template_name_size(
	          catalog_definition,
	          &utf8_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	catalog_definition->template_name = template_name;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 0 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_catalog_definition_get_utf8_template_name_size(
	          NULL,
	          &utf8_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf8_template_name_size(
	          catalog_definition,
	          NULL,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

/* Tests the libesedb_catalog_definition_get_utf8_template_name function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf8_template_name(
     libesedb_catalog_definition_t *catalog_definition )
{
	uint8_t utf8_string[ 32 ];

	uint8_t expected_utf8_string[ 28 ] = {
		'm', 's', 's', 'r', 'c', 'h', '.', 'd', 'l', 'l', '!', 'C', 'o', 'n', 'c', 'a',
		't', 'V', 'e', 'c', 't', 'V', 'a', 'l', 'u', 'e', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf8_template_name(
	          catalog_definition,
	          utf8_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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
	          28 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libesedb_catalog_definition_get_utf8_template_name(
	          NULL,
	          utf8_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf8_template_name(
	          catalog_definition,
	          NULL,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

/* Tests the libesedb_catalog_definition_get_utf16_template_name_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf16_template_name_size(
     libesedb_catalog_definition_t *catalog_definition )
{
	libcerror_error_t *error = NULL;
	uint8_t *template_name   = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf16_template_name_size(
	          catalog_definition,
	          &utf16_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 28 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	template_name = catalog_definition->template_name;

	catalog_definition->template_name = NULL;

	result = libesedb_catalog_definition_get_utf16_template_name_size(
	          catalog_definition,
	          &utf16_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	catalog_definition->template_name = template_name;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 0 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_catalog_definition_get_utf16_template_name_size(
	          NULL,
	          &utf16_string_size,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf16_template_name_size(
	          catalog_definition,
	          NULL,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

/* Tests the libesedb_catalog_definition_get_utf16_template_name function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_utf16_template_name(
     libesedb_catalog_definition_t *catalog_definition )
{
	uint16_t utf16_string[ 32 ];

	uint16_t expected_utf16_string[ 28 ] = {
		'm', 's', 's', 'r', 'c', 'h', '.', 'd', 'l', 'l', '!', 'C', 'o', 'n', 'c', 'a',
		't', 'V', 'e', 'c', 't', 'V', 'a', 'l', 'u', 'e', 's', 0 };

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_catalog_definition_get_utf16_template_name(
	          catalog_definition,
	          utf16_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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
	          28 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libesedb_catalog_definition_get_utf16_template_name(
	          NULL,
	          utf16_string,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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

	result = libesedb_catalog_definition_get_utf16_template_name(
	          catalog_definition,
	          NULL,
	          32,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
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
#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )
	libcerror_error_t *error                          = NULL;
	libesedb_catalog_definition_t *catalog_definition = NULL;
	int result                                        = 0;
#endif

	ESEDB_TEST_UNREFERENCED_PARAMETER( argc )
	ESEDB_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	ESEDB_TEST_RUN(
	 "libesedb_catalog_definition_initialize",
	 esedb_test_catalog_definition_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_catalog_definition_free",
	 esedb_test_catalog_definition_free );

	ESEDB_TEST_RUN(
	 "libesedb_catalog_definition_read_data",
	 esedb_test_catalog_definition_read_data );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize catalog_definition for tests
	 */
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
	          esedb_test_catalog_definition_data1,
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

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_identifier",
	 esedb_test_catalog_definition_get_identifier,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_column_type",
	 esedb_test_catalog_definition_get_column_type,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf8_name_size",
	 esedb_test_catalog_definition_get_utf8_name_size,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf8_name",
	 esedb_test_catalog_definition_get_utf8_name,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf16_name_size",
	 esedb_test_catalog_definition_get_utf16_name_size,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf16_name",
	 esedb_test_catalog_definition_get_utf16_name,
	 catalog_definition );

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

	/* Initialize catalog_definition for tests
	 */
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
	          esedb_test_catalog_definition_data2,
	          97,
	          LIBESEDB_CODEPAGE_WINDOWS_1252,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf8_template_name_size",
	 esedb_test_catalog_definition_get_utf8_template_name_size,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf8_template_name",
	 esedb_test_catalog_definition_get_utf8_template_name,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf16_template_name_size",
	 esedb_test_catalog_definition_get_utf16_template_name_size,
	 catalog_definition );

	ESEDB_TEST_RUN_WITH_ARGS(
	 "libesedb_catalog_definition_get_utf16_template_name",
	 esedb_test_catalog_definition_get_utf16_template_name,
	 catalog_definition );

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
	if( catalog_definition != NULL )
	{
		libesedb_catalog_definition_free(
		 &catalog_definition,
		 NULL );
	}
#endif
	return( EXIT_FAILURE );
}

