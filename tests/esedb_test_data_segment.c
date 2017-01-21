/*
 * Library data_segment type test program
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

#include "../libesedb/libesedb_data_segment.h"

#if defined( __GNUC__ )

/* Tests the libesedb_data_segment_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_data_segment_initialize(
     void )
{
	libcerror_error_t *error              = NULL;
	libesedb_data_segment_t *data_segment = NULL;
	int result                            = 0;

	/* Test data_segment initialization
	 */
	result = libesedb_data_segment_initialize(
	          &data_segment,
	          4096,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        ESEDB_TEST_ASSERT_IS_NOT_NULL(
         "data_segment",
         data_segment );

        ESEDB_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libesedb_data_segment_free(
	          &data_segment,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        ESEDB_TEST_ASSERT_IS_NULL(
         "data_segment",
         data_segment );

        ESEDB_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libesedb_data_segment_initialize(
	          NULL,
	          4096,
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

	data_segment = (libesedb_data_segment_t *) 0x12345678UL;

	result = libesedb_data_segment_initialize(
	          &data_segment,
	          4096,
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

	data_segment = NULL;

	result = libesedb_data_segment_initialize(
	          &data_segment,
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

#if defined( HAVE_ESEDB_TEST_MEMORY )

	/* Test libesedb_data_segment_initialize with malloc failing
	 */
	esedb_test_malloc_attempts_before_fail = 0;

	result = libesedb_data_segment_initialize(
	          &data_segment,
	          4096,
	          &error );

	if( esedb_test_malloc_attempts_before_fail != -1 )
	{
		esedb_test_malloc_attempts_before_fail = -1;

		if( data_segment != NULL )
		{
			libesedb_data_segment_free(
			 &data_segment,
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
		 "data_segment",
		 data_segment );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libesedb_data_segment_initialize with memset failing
	 */
	esedb_test_memset_attempts_before_fail = 0;

	result = libesedb_data_segment_initialize(
	          &data_segment,
	          4096,
	          &error );

	if( esedb_test_memset_attempts_before_fail != -1 )
	{
		esedb_test_memset_attempts_before_fail = -1;

		if( data_segment != NULL )
		{
			libesedb_data_segment_free(
			 &data_segment,
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
		 "data_segment",
		 data_segment );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_ESEDB_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( data_segment != NULL )
	{
		libesedb_data_segment_free(
		 &data_segment,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_data_segment_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_data_segment_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_data_segment_free(
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
	 "libesedb_data_segment_initialize",
	 esedb_test_data_segment_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_data_segment_free",
	 esedb_test_data_segment_free );

#endif /* defined( __GNUC__ ) */

	/* TODO: add tests for libesedb_data_segment_get_data_size */

	/* TODO: add tests for libesedb_data_segment_get_data */

	/* TODO: add tests for libesedb_data_segment_read_element_data */


	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

