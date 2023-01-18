/*
 * Library file type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "esedb_test_functions.h"
#include "esedb_test_getopt.h"
#include "esedb_test_libbfio.h"
#include "esedb_test_libcerror.h"
#include "esedb_test_libesedb.h"
#include "esedb_test_macros.h"
#include "esedb_test_memory.h"

#include "../libesedb/libesedb_file.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make esedb_test_file generate verbose output
#define ESEDB_TEST_FILE_VERBOSE
 */

#if !defined( LIBESEDB_HAVE_BFIO )

LIBESEDB_EXTERN \
int libesedb_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_file_open_file_io_handle(
     libesedb_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libesedb_error_t **error );

#endif /* !defined( LIBESEDB_HAVE_BFIO ) */

/* Creates and opens a source file
 * Returns 1 if successful or -1 on error
 */
int esedb_test_file_open_source(
     libesedb_file_t **file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "esedb_test_file_open_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_file_initialize(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
	result = libesedb_file_open_file_io_handle(
	          *file,
	          file_io_handle,
	          LIBESEDB_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file != NULL )
	{
		libesedb_file_free(
		 file,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source file
 * Returns 1 if successful or -1 on error
 */
int esedb_test_file_close_source(
     libesedb_file_t **file,
     libcerror_error_t **error )
{
	static char *function = "esedb_test_file_close_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libesedb_file_close(
	     *file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		result = -1;
	}
	if( libesedb_file_free(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libesedb_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libesedb_file_t *file           = NULL;
	int result                      = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libesedb_file_initialize(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_file_free(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_file_initialize(
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

	file = (libesedb_file_t *) 0x12345678UL;

	result = libesedb_file_initialize(
	          &file,
	          &error );

	file = NULL;

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
		/* Test libesedb_file_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_file_initialize(
		          &file,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( file != NULL )
			{
				libesedb_file_free(
				 &file,
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
			 "file",
			 file );

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
		/* Test libesedb_file_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_file_initialize(
		          &file,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( file != NULL )
			{
				libesedb_file_free(
				 &file,
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
			 "file",
			 file );

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
	if( file != NULL )
	{
		libesedb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_file_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_file_free(
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

/* Tests the libesedb_file_open function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libesedb_file_t *file    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = esedb_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_file_initialize(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libesedb_file_open(
	          file,
	          narrow_source,
	          LIBESEDB_OPEN_READ,
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
	result = libesedb_file_open(
	          NULL,
	          narrow_source,
	          LIBESEDB_OPEN_READ,
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

	result = libesedb_file_open(
	          file,
	          NULL,
	          LIBESEDB_OPEN_READ,
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

	result = libesedb_file_open(
	          file,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libesedb_file_open(
	          file,
	          narrow_source,
	          LIBESEDB_OPEN_READ,
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
	result = libesedb_file_free(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libesedb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libesedb_file_open_wide function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libesedb_file_t *file    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = esedb_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_file_initialize(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libesedb_file_open_wide(
	          file,
	          wide_source,
	          LIBESEDB_OPEN_READ,
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
	result = libesedb_file_open_wide(
	          NULL,
	          wide_source,
	          LIBESEDB_OPEN_READ,
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

	result = libesedb_file_open_wide(
	          file,
	          NULL,
	          LIBESEDB_OPEN_READ,
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

	result = libesedb_file_open_wide(
	          file,
	          wide_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libesedb_file_open_wide(
	          file,
	          wide_source,
	          LIBESEDB_OPEN_READ,
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
	result = libesedb_file_free(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libesedb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libesedb_file_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libesedb_file_t *file            = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
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

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_file_initialize(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libesedb_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBESEDB_OPEN_READ,
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
	result = libesedb_file_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBESEDB_OPEN_READ,
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

	result = libesedb_file_open_file_io_handle(
	          file,
	          NULL,
	          LIBESEDB_OPEN_READ,
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

	result = libesedb_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libesedb_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBESEDB_OPEN_READ,
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
	result = libesedb_file_free(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

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
	if( file != NULL )
	{
		libesedb_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_file_close function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_file_close(
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

/* Tests the libesedb_file_open and libesedb_file_close functions
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libesedb_file_t *file    = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libesedb_file_initialize(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_file_open_wide(
	          file,
	          source,
	          LIBESEDB_OPEN_READ,
	          &error );
#else
	result = libesedb_file_open(
	          file,
	          source,
	          LIBESEDB_OPEN_READ,
	          &error );
#endif

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_file_close(
	          file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libesedb_file_open_wide(
	          file,
	          source,
	          LIBESEDB_OPEN_READ,
	          &error );
#else
	result = libesedb_file_open(
	          file,
	          source,
	          LIBESEDB_OPEN_READ,
	          &error );
#endif

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_file_close(
	          file,
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
	result = libesedb_file_free(
	          &file,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libesedb_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_file_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_signal_abort(
     libesedb_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_file_signal_abort(
	          file,
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
	result = libesedb_file_signal_abort(
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

/* Tests the libesedb_file_get_type function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_get_type(
     libesedb_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_file_get_type(
	          file,
	          &type,
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
	result = libesedb_file_get_type(
	          NULL,
	          &type,
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

	result = libesedb_file_get_type(
	          file,
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

/* Tests the libesedb_file_get_page_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_get_page_size(
     libesedb_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint32_t page_size       = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_file_get_page_size(
	          file,
	          &page_size,
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
	result = libesedb_file_get_page_size(
	          NULL,
	          &page_size,
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

	result = libesedb_file_get_page_size(
	          file,
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

/* Tests the libesedb_file_get_number_of_tables function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_file_get_number_of_tables(
     libesedb_file_t *file )
{
	libcerror_error_t *error = NULL;
	int number_of_tables     = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_file_get_number_of_tables(
	          file,
	          &number_of_tables,
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
	result = libesedb_file_get_number_of_tables(
	          NULL,
	          &number_of_tables,
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

	result = libesedb_file_get_number_of_tables(
	          file,
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

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libesedb_file_t *file            = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int result                       = 0;

	while( ( option = esedb_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( ESEDB_TEST_FILE_VERBOSE )
	libesedb_notify_set_verbose(
	 1 );
	libesedb_notify_set_stream(
	 stderr,
	 NULL );
#endif

	ESEDB_TEST_RUN(
	 "libesedb_file_initialize",
	 esedb_test_file_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_file_free",
	 esedb_test_file_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
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

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        ESEDB_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libesedb_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		ESEDB_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		ESEDB_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( result != 0 )
	{
		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_open",
		 esedb_test_file_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_open_wide",
		 esedb_test_file_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_open_file_io_handle",
		 esedb_test_file_open_file_io_handle,
		 source );

		ESEDB_TEST_RUN(
		 "libesedb_file_close",
		 esedb_test_file_close );

		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_open_close",
		 esedb_test_file_open_close,
		 source );

		/* Initialize file for tests
		 */
		result = esedb_test_file_open_source(
		          &file,
		          file_io_handle,
		          &error );

		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "file",
		 file );

		ESEDB_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_signal_abort",
		 esedb_test_file_signal_abort,
		 file );

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

		/* TODO: add tests for libesedb_file_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_get_type",
		 esedb_test_file_get_type,
		 file );

		/* TODO: add tests for libesedb_file_get_format_version */

		/* TODO: add tests for libesedb_file_get_creation_format_version */

		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_get_page_size",
		 esedb_test_file_get_page_size,
		 file );

		ESEDB_TEST_RUN_WITH_ARGS(
		 "libesedb_file_get_number_of_tables",
		 esedb_test_file_get_number_of_tables,
		 file );

		/* TODO: add tests for libesedb_file_get_table */

		/* TODO: add tests for libesedb_file_get_table_by_utf8_name */

		/* TODO: add tests for libesedb_file_get_table_by_utf16_name */

		/* Clean up
		 */
		result = esedb_test_file_close_source(
		          &file,
		          &error );

		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		ESEDB_TEST_ASSERT_IS_NULL(
		 "file",
		 file );

		ESEDB_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( file_io_handle != NULL )
	{
		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		ESEDB_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		ESEDB_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        ESEDB_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libesedb_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

