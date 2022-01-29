/*
 * Library column_type type test program
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

#include "../libesedb/libesedb_column_type.h"

#if defined( HAVE_DEBUG_OUTPUT )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_column_type_get_identifier function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_type_get_identifier(
     void )
{
	const char *result = NULL;

	/* Test regular cases
	 */
	result = libesedb_column_type_get_identifier(
	          0x0000 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "result",
	 result );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libesedb_column_type_get_description function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_column_type_get_description(
     void )
{
	const char *result = NULL;

	/* Test regular cases
	 */
	result = libesedb_column_type_get_description(
	          0x0000 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "result",
	 result );

	return( 1 );

on_error:
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

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

#if defined( HAVE_DEBUG_OUTPUT )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	ESEDB_TEST_RUN(
	 "libesedb_column_type_get_identifier",
	 esedb_test_column_type_get_identifier );

	ESEDB_TEST_RUN(
	 "libesedb_column_type_get_description",
	 esedb_test_column_type_get_description );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

