/*
 * Library page type testing program
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

#include "../libesedb/libesedb_page.h"

#if defined( __GNUC__ )

/* Tests the libesedb_page_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_initialize(
     void )
{
	libcerror_error_t *error = NULL;
	libesedb_page_t *page    = NULL;
	int result               = 0;

	/* Test page initialization
	 */
	result = libesedb_page_initialize(
	          &page,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        ESEDB_TEST_ASSERT_IS_NOT_NULL(
         "page",
         page );

        ESEDB_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libesedb_page_free(
	          &page,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        ESEDB_TEST_ASSERT_IS_NULL(
         "page",
         page );

        ESEDB_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libesedb_page_initialize(
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

	page = (libesedb_page_t *) 0x12345678UL;

	result = libesedb_page_initialize(
	          &page,
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

	page = NULL;

#if defined( HAVE_ESEDB_TEST_MEMORY )

	/* Test libesedb_page_initialize with malloc failing
	 */
	esedb_test_malloc_attempts_before_fail = 0;

	result = libesedb_page_initialize(
	          &page,
	          &error );

	if( esedb_test_malloc_attempts_before_fail != -1 )
	{
		esedb_test_malloc_attempts_before_fail = -1;

		if( page != NULL )
		{
			libesedb_page_free(
			 &page,
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
		 "page",
		 page );

		ESEDB_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libesedb_page_initialize with memset failing
	 */
	esedb_test_memset_attempts_before_fail = 0;

	result = libesedb_page_initialize(
	          &page,
	          &error );

	if( esedb_test_memset_attempts_before_fail != -1 )
	{
		esedb_test_memset_attempts_before_fail = -1;

		if( page != NULL )
		{
			libesedb_page_free(
			 &page,
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
		 "page",
		 page );

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
	if( page != NULL )
	{
		libesedb_page_free(
		 &page,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_page_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_page_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_page_free(
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
	 "libesedb_page_initialize",
	 esedb_test_page_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_page_free",
	 esedb_test_page_free );

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

