/*
 * Library block_tree functions testing program
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

#include "../libesedb/libesedb_block_descriptor.h"
#include "../libesedb/libesedb_block_tree.h"
#include "../libesedb/libesedb_block_tree_node.h"
#include "../libesedb/libesedb_definitions.h"

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_block_tree_initialize function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_block_tree_initialize(
     void )
{
	libcerror_error_t *error          = NULL;
	libesedb_block_tree_t *block_tree = NULL;
	int result                        = 0;

#if defined( HAVE_ESEDB_TEST_MEMORY )
	int number_of_malloc_fail_tests   = 2;
	int number_of_memset_fail_tests   = 1;
	int test_number                   = 0;
#endif

	/* Test block_tree initialization
	 */
	result = libesedb_block_tree_initialize(
	          &block_tree,
	          0x800000000UL,
	          0x4000,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "block_tree",
	 block_tree );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_block_tree_free(
	          &block_tree,
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "block_tree",
	 block_tree );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_block_tree_initialize(
	          NULL,
	          0x800000000UL,
	          0x4000,
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

	block_tree = (libesedb_block_tree_t *) 0x12345678UL;

	result = libesedb_block_tree_initialize(
	          &block_tree,
	          0x800000000UL,
	          0x4000,
	          &error );

	block_tree = NULL;

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
		/* Test libesedb_block_tree_initialize with malloc failing
		 */
		esedb_test_malloc_attempts_before_fail = test_number;

		result = libesedb_block_tree_initialize(
		          &block_tree,
		          0x800000000UL,
		          0x4000,
		          &error );

		if( esedb_test_malloc_attempts_before_fail != -1 )
		{
			esedb_test_malloc_attempts_before_fail = -1;

			if( block_tree != NULL )
			{
				libesedb_block_tree_free(
				 &block_tree,
				 NULL,
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
			 "block_tree",
			 block_tree );

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
		/* Test libesedb_block_tree_initialize with memset failing
		 */
		esedb_test_memset_attempts_before_fail = test_number;

		result = libesedb_block_tree_initialize(
		          &block_tree,
		          0x800000000UL,
		          0x4000,
		          &error );

		if( esedb_test_memset_attempts_before_fail != -1 )
		{
			esedb_test_memset_attempts_before_fail = -1;

			if( block_tree != NULL )
			{
				libesedb_block_tree_free(
				 &block_tree,
				 NULL,
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
			 "block_tree",
			 block_tree );

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
	if( block_tree != NULL )
	{
		libesedb_block_tree_free(
		 &block_tree,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_block_tree_free function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_block_tree_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libesedb_block_tree_free(
	          NULL,
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

/* Tests the libesedb_block_tree_get_block_descriptor_by_offset function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_block_tree_get_block_descriptor_by_offset(
     void )
{
	libcerror_error_t *error                      = NULL;
	libesedb_block_descriptor_t *block_descriptor = NULL;
	libesedb_block_tree_t *block_tree             = NULL;
	off64_t block_offset                          = 0;
	int result                                    = 0;

	/* Initialize test
	 */
	result = libesedb_block_tree_initialize(
	          &block_tree,
	          0x800000000UL,
	          0x4000,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "block_tree",
	 block_tree );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_block_tree_get_block_descriptor_by_offset(
	          block_tree,
	          0,
	          &block_descriptor,
	          &block_offset,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "block_descriptor",
	 block_descriptor );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_block_tree_get_block_descriptor_by_offset(
	          NULL,
	          0,
	          &block_descriptor,
	          &block_offset,
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

	result = libesedb_block_tree_get_block_descriptor_by_offset(
	          block_tree,
	          0,
	          NULL,
	          &block_offset,
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

	result = libesedb_block_tree_get_block_descriptor_by_offset(
	          block_tree,
	          0,
	          &block_descriptor,
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
	result = libesedb_block_tree_free(
	          &block_tree,
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "block_tree",
	 block_tree );

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
	if( block_tree != NULL )
	{
		libesedb_block_tree_free(
		 &block_tree,
		 NULL,
		 NULL );
	}
	return( 0 );
}

/* Tests the libesedb_block_tree_insert_block_descriptor_by_offset function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_block_tree_insert_block_descriptor_by_offset(
     void )
{
	libcerror_error_t *error                               = NULL;
	libesedb_block_descriptor_t *block_descriptor          = NULL;
	libesedb_block_descriptor_t *existing_block_descriptor = NULL;
	libesedb_block_tree_t *block_tree                      = NULL;
	libesedb_block_tree_node_t *leaf_block_tree_node       = NULL;
	int leaf_value_index                                   = 0;
	int result                                             = 0;

	/* Initialize test
	 */
	result = libesedb_block_tree_initialize(
	          &block_tree,
	          0x800000000UL,
	          0x4000,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "block_tree",
	 block_tree );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_block_descriptor_initialize(
	          &block_descriptor,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "block_descriptor",
	 block_descriptor );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libesedb_block_tree_insert_block_descriptor_by_offset(
	          block_tree,
	          0,
	          block_descriptor,
	          &leaf_value_index,
	          &leaf_block_tree_node,
	          &existing_block_descriptor,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "leaf_value_index",
	 leaf_value_index,
	 0 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "leaf_block_tree_node",
	 leaf_block_tree_node );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "existing_block_descriptor",
	 existing_block_descriptor );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_block_tree_insert_block_descriptor_by_offset(
	          block_tree,
	          0,
	          NULL,
	          &leaf_value_index,
	          &leaf_block_tree_node,
	          &existing_block_descriptor,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "leaf_value_index",
	 leaf_value_index,
	 0 );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "leaf_block_tree_node",
	 leaf_block_tree_node );

	ESEDB_TEST_ASSERT_IS_NOT_NULL(
	 "existing_block_descriptor",
	 existing_block_descriptor );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_block_tree_insert_block_descriptor_by_offset(
	          NULL,
	          0,
	          block_descriptor,
	          &leaf_value_index,
	          &leaf_block_tree_node,
	          &existing_block_descriptor,
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

	result = libesedb_block_tree_insert_block_descriptor_by_offset(
	          block_tree,
	          0,
	          block_descriptor,
	          NULL,
	          &leaf_block_tree_node,
	          &existing_block_descriptor,
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

	result = libesedb_block_tree_insert_block_descriptor_by_offset(
	          block_tree,
	          0,
	          block_descriptor,
	          &leaf_value_index,
	          NULL,
	          &existing_block_descriptor,
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

	result = libesedb_block_tree_insert_block_descriptor_by_offset(
	          block_tree,
	          0,
	          block_descriptor,
	          &leaf_value_index,
	          &leaf_block_tree_node,
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
	result = libesedb_block_descriptor_free(
	          &block_descriptor,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "block_descriptor",
	 block_descriptor );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_block_tree_free(
	          &block_tree,
	          NULL,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "block_tree",
	 block_tree );

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
	if( block_descriptor != NULL )
	{
		libesedb_block_descriptor_free(
		 &block_descriptor,
		 NULL );
	}
	if( block_tree != NULL )
	{
		libesedb_block_tree_free(
		 &block_tree,
		 NULL,
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
	 "libesedb_block_tree_initialize",
	 esedb_test_block_tree_initialize );

	ESEDB_TEST_RUN(
	 "libesedb_block_tree_free",
	 esedb_test_block_tree_free );

	ESEDB_TEST_RUN(
	 "libesedb_block_tree_get_block_descriptor_by_offset",
	 esedb_test_block_tree_get_block_descriptor_by_offset );

	ESEDB_TEST_RUN(
	 "libesedb_block_tree_insert_block_descriptor_by_offset",
	 esedb_test_block_tree_insert_block_descriptor_by_offset );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

on_error:
	return( EXIT_FAILURE );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */
}

