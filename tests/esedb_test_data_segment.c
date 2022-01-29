/*
 * Library data_segment type test program
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

#include "esedb_test_functions.h"
#include "esedb_test_libbfio.h"
#include "esedb_test_libcerror.h"
#include "esedb_test_libesedb.h"
#include "esedb_test_libfcache.h"
#include "esedb_test_macros.h"
#include "esedb_test_memory.h"
#include "esedb_test_unused.h"

#include "../libesedb/libesedb_data_segment.h"

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_data_segment_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_data_segment_initialize(
     void )
{
	libcerror_error_t *error              = NULL;
	libesedb_data_segment_t *data_segment = NULL;
	int result                            = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests       = 2;
	int number_of_memset_fail_tests       = 1;
	int test_number                       = 0;
#endif

	/* Test regular cases
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

	data_segment = NULL;

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

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

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libesedb_data_segment_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

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
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libesedb_data_segment_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

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

/* Tests the libesedb_data_segment_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_data_segment_get_data_size(
     void )
{
	libcerror_error_t *error              = NULL;
	libesedb_data_segment_t *data_segment = NULL;
	size_t data_size                      = 0;
	int result                            = 0;

	/* Initialize test
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

	/* Test regular cases
	 */
	result = libesedb_data_segment_get_data_size(
	          data_segment,
	          &data_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "data_size",
	 data_size,
	 (size_t) 4096 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_data_segment_get_data_size(
	          NULL,
	          &data_size,
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

	result = libesedb_data_segment_get_data_size(
	          data_segment,
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

/* Tests the libesedb_data_segment_get_data function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_data_segment_get_data(
     void )
{
	uint8_t data[ 4096 ];

	libcerror_error_t *error              = NULL;
	libesedb_data_segment_t *data_segment = NULL;
	int result                            = 0;

	/* Initialize test
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

	/* Test regular cases
	 */
	result = libesedb_data_segment_get_data(
	          data_segment,
	          data,
	          4096,
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
	result = libesedb_data_segment_get_data(
	          NULL,
	          data,
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

	result = libesedb_data_segment_get_data(
	          data_segment,
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

	result = libesedb_data_segment_get_data(
	          data_segment,
	          data,
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

	result = libesedb_data_segment_get_data(
	          data_segment,
	          data,
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

#if defined( HAVE_ESEDB_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED )

	/* Test libesedb_data_segment_get_data with memcpy failing
	 */
	esedb_test_memcpy_attempts_before_fail = 0;

	result = libesedb_data_segment_get_data(
	          data_segment,
	          data,
	          4096,
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
#endif /* defined( HAVE_ESEDB_TEST_MEMORY ) && defined( OPTIMIZATION_DISABLED ) */

	/* Clean up
	 */
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

/* Tests the libesedb_data_segment_read_element_data function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_data_segment_read_element_data(
     void )
{
	uint8_t data[ 4096 ];

	libbfio_handle_t *file_io_handle      = NULL;
	libcerror_error_t *error              = NULL;
	libfcache_cache_t *cache              = NULL;
	libfdata_list_t *list                 = NULL;
	libfdata_list_element_t *list_element = NULL;
	int result                            = 0;

	/* Initialize test
	 */
	result = libfdata_list_initialize(
	          &list,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          NULL,
	          0,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "list",
	 list );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_list_element_initialize(
	          &list_element,
	          list,
	          0,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "list_element",
	 list_element );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfcache_cache_initialize(
	          &cache,
	          1,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "cache",
	 cache );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Initialize file IO handle
	 */
	result = esedb_test_open_file_io_handle(
	          &file_io_handle,
	          data,
	          4096,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_data_segment_read_element_data(
	          NULL,
	          file_io_handle,
	          list_element,
	          (libfdata_cache_t *) cache,
	          0,
	          0,
	          4096,
	          0,
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
	result = libesedb_data_segment_read_element_data(
	          NULL,
	          file_io_handle,
	          list_element,
	          (libfdata_cache_t *) cache,
	          0,
	          0,
	          (size64_t) SSIZE_MAX + 1,
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

	result = libesedb_data_segment_read_element_data(
	          NULL,
	          NULL,
	          list_element,
	          (libfdata_cache_t *) cache,
	          0,
	          0,
	          4096,
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

	result = libesedb_data_segment_read_element_data(
	          NULL,
	          file_io_handle,
	          NULL,
	          (libfdata_cache_t *) cache,
	          0,
	          0,
	          4096,
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

#if defined( HAVE_ESEDB_TEST_MEMORY )

	/* Test libesedb_data_segment_initialize with malloc failing in libesedb_data_segment_initialize
	 */
	esedb_test_malloc_attempts_before_fail = 0;

	result = libesedb_data_segment_read_element_data(
	          NULL,
	          file_io_handle,
	          list_element,
	          (libfdata_cache_t *) cache,
	          0,
	          0,
	          4096,
	          0,
	          0,
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
#endif /* defined( HAVE_ESEDB_TEST_MEMORY ) */

/* TODO test with libbfio_handle_read_buffer failing */

	/* Clean up file IO handle
	 */
	result = esedb_test_close_file_io_handle(
	          &file_io_handle,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libfcache_cache_free(
	          &cache,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "cache",
	 cache );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_list_element_free(
	          &list_element,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "list_element",
	 list_element );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libfdata_list_free(
	          &list,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "list",
	 list );

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
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	if( cache != NULL )
	{
		libfcache_cache_free(
		 &cache,
		 NULL );
	}
	if( list_element != NULL )
	{
		libfdata_list_element_free(
		 &list_element,
		 NULL );
	}
	if( list != NULL )
	{
		libfdata_list_free(
		 &list,
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
	 "libesedb_data_segment_initialize",
	 esedb_test_data_segment_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_data_segment_free",
	 esedb_test_data_segment_free );

	ESEDB_TEST_RUN(
	 "libesedb_data_segment_get_data_size",
	 esedb_test_data_segment_get_data_size );

	ESEDB_TEST_RUN(
	 "libesedb_data_segment_get_data",
	 esedb_test_data_segment_get_data );

	ESEDB_TEST_RUN(
	 "libesedb_data_segment_read_element_data",
	 esedb_test_data_segment_read_element_data );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

