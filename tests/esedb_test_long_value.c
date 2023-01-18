/*
 * Library long_value type test program
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

#include "../libesedb/libesedb_long_value.h"

/* Tests the libesedb_long_value_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_long_value_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_long_value_free(
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
	ESEDB_TEST_UNREFERENCED_PARAMETER( argc )
	ESEDB_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	/* TODO: add tests for libesedb_long_value_initialize */

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	ESEDB_TEST_RUN(
	 "libesedb_long_value_free",
	 esedb_test_long_value_free );

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	/* TODO: add tests for libesedb_long_value_get_data_size */

	/* TODO: add tests for libesedb_long_value_get_data */

	/* TODO: add tests for libesedb_long_value_get_record_value */

	/* TODO: add tests for libesedb_long_value_get_number_of_data_segments */

	/* TODO: add tests for libesedb_long_value_get_data_segment_size */

	/* TODO: add tests for libesedb_long_value_get_data_segment */

	/* TODO: add tests for libesedb_long_value_get_utf8_string_size */

	/* TODO: add tests for libesedb_long_value_get_utf8_string */

	/* TODO: add tests for libesedb_long_value_get_utf16_string_size */

	/* TODO: add tests for libesedb_long_value_get_utf16_string */

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

