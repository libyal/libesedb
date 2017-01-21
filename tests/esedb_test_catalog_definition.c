/*
 * Library catalog_definition type test program
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
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

#include "../libesedb/libesedb_catalog_definition.h"

#if defined( __GNUC__ )

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

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        ESEDB_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	catalog_definition = NULL;

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

/* Tests the libesedb_catalog_definition_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_identifier(
     void )
{
	libcerror_error_t *error                          = NULL;
	libesedb_catalog_definition_t *catalog_definition = NULL;
	uint32_t identifier                               = 0;
	int identifier_is_set                             = 0;
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
	result = libesedb_catalog_definition_get_identifier(
	          catalog_definition,
	          &identifier,
	          &error );

	ESEDB_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	identifier_is_set = result;

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

	if( identifier_is_set != 0 )
	{
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
	}
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

/* Tests the libesedb_catalog_definition_get_column_type function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_catalog_definition_get_column_type(
     void )
{
	libcerror_error_t *error                          = NULL;
	libesedb_catalog_definition_t *catalog_definition = NULL;
	uint32_t column_type                              = 0;
	int column_type_is_set                            = 0;
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
	result = libesedb_catalog_definition_get_column_type(
	          catalog_definition,
	          &column_type,
	          &error );

	ESEDB_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	column_type_is_set = result;

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

	if( column_type_is_set != 0 )
	{
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
	}
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

#endif /* defined( __GNUC__ ) */

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

#if defined( __GNUC__ )

	ESEDB_TEST_RUN(
	 "libesedb_catalog_definition_initialize",
	 esedb_test_catalog_definition_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_catalog_definition_free",
	 esedb_test_catalog_definition_free );

	/* TODO: add tests for libesedb_catalog_definition_read */

	ESEDB_TEST_RUN(
	 "libesedb_catalog_definition_get_identifier",
	 esedb_test_catalog_definition_get_identifier );

	ESEDB_TEST_RUN(
	 "libesedb_catalog_definition_get_column_type",
	 esedb_test_catalog_definition_get_column_type );

	/* TODO: add tests for libesedb_catalog_definition_get_utf8_name_size */

	/* TODO: add tests for libesedb_catalog_definition_get_utf8_name */

	/* TODO: add tests for libesedb_catalog_definition_get_utf16_name_size */

	/* TODO: add tests for libesedb_catalog_definition_get_utf16_name */

	/* TODO: add tests for libesedb_catalog_definition_get_utf8_template_name_size */

	/* TODO: add tests for libesedb_catalog_definition_get_utf8_template_name */

	/* TODO: add tests for libesedb_catalog_definition_get_utf16_template_name_size */

	/* TODO: add tests for libesedb_catalog_definition_get_utf16_template_name */

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

