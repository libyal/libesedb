/*
 * Library compression functions test program
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

#include "../libesedb/libesedb_compression.h"

uint8_t esedb_test_compression_7bit_compressed_data[ 50 ] = {
	0x10, 0xd2, 0xa2, 0x0e, 0x04, 0x42, 0xbd, 0x82, 0xf2, 0x31, 0x3a, 0x5d, 0x36, 0xb7, 0xc3, 0x70,
	0x78, 0xd9, 0xfd, 0xb2, 0x96, 0xe5, 0xf7, 0xb4, 0x9a, 0x5c, 0x96, 0x93, 0xcb, 0xa0, 0x34, 0xbd,
	0xdc, 0x9e, 0xbf, 0xac, 0x65, 0xb9, 0xfe, 0xed, 0x26, 0x97, 0xdd, 0xa0, 0x34, 0xbd, 0xdc, 0x9e,
	0xa7, 0x00
};

uint8_t esedb_test_compression_7bit_uncompressed_data[ 56 ] = {
	0x52, 0x45, 0x3a, 0x20, 0x20, 0x28, 0x2f, 0x41, 0x72, 0x63, 0x68, 0x69, 0x65, 0x66, 0x6d, 0x61,
	0x70, 0x70, 0x65, 0x6e, 0x2f, 0x56, 0x65, 0x72, 0x77, 0x69, 0x6a, 0x64, 0x65, 0x72, 0x64, 0x65,
	0x20, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x2f, 0x56, 0x65, 0x72, 0x7a, 0x6f, 0x6e, 0x64, 0x65, 0x6e,
	0x20, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x29, 0x00
};

uint8_t esedb_test_compression_7bit_uncompressed_utf8_string[ 56 ] = {
	0x52, 0x45, 0x3a, 0x20, 0x20, 0x28, 0x2f, 0x41, 0x72, 0x63, 0x68, 0x69, 0x65, 0x66, 0x6d, 0x61,
	0x70, 0x70, 0x65, 0x6e, 0x2f, 0x56, 0x65, 0x72, 0x77, 0x69, 0x6a, 0x64, 0x65, 0x72, 0x64, 0x65,
	0x20, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x2f, 0x56, 0x65, 0x72, 0x7a, 0x6f, 0x6e, 0x64, 0x65, 0x6e,
	0x20, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x29, 0x00
};

uint16_t esedb_test_compression_7bit_uncompressed_utf16_string[ 56 ] = {
	0x52, 0x45, 0x3a, 0x20, 0x20, 0x28, 0x2f, 0x41, 0x72, 0x63, 0x68, 0x69, 0x65, 0x66, 0x6d, 0x61,
	0x70, 0x70, 0x65, 0x6e, 0x2f, 0x56, 0x65, 0x72, 0x77, 0x69, 0x6a, 0x64, 0x65, 0x72, 0x64, 0x65,
	0x20, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x2f, 0x56, 0x65, 0x72, 0x7a, 0x6f, 0x6e, 0x64, 0x65, 0x6e,
	0x20, 0x69, 0x74, 0x65, 0x6d, 0x73, 0x29, 0x00
};

/* The sequence: abcdefghijklmnopqrstuvwxyz compressed in LZXpress
 * including leading byte 0x18 and uncompressed data size 0x001a
 */
uint8_t esedb_test_compression_lzxpress_compressed_data[ 33 ] = {
	0x18, 0x1a, 0x00, 0x3f, 0x00, 0x00, 0x00, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69,
	0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
	0x7a };

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

/* Tests the libesedb_compression_7bit_decompress_get_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_7bit_decompress_get_size(
     void )
{
	libcerror_error_t *error      = NULL;
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_7bit_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          &uncompressed_data_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 56 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_compression_7bit_decompress_get_size(
	          NULL,
	          50,
	          &uncompressed_data_size,
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

	result = libesedb_compression_7bit_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
	          &uncompressed_data_size,
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

	result = libesedb_compression_7bit_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          0,
	          &uncompressed_data_size,
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

	result = libesedb_compression_7bit_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
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

	result = libesedb_compression_7bit_decompress_get_size(
	          esedb_test_compression_lzxpress_compressed_data,
	          33,
	          &uncompressed_data_size,
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

/* Tests the libesedb_compression_7bit_decompress function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_7bit_decompress(
     void )
{
	uint8_t uncompressed_data[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_7bit_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          uncompressed_data,
	          64,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          esedb_test_compression_7bit_uncompressed_data,
	          56 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libesedb_compression_7bit_decompress(
	          NULL,
	          50,
	          uncompressed_data,
	          64,
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

	result = libesedb_compression_7bit_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
	          uncompressed_data,
	          64,
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

	result = libesedb_compression_7bit_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          0,
	          uncompressed_data,
	          64,
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

	result = libesedb_compression_7bit_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          NULL,
	          64,
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

	result = libesedb_compression_7bit_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          uncompressed_data,
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

	result = libesedb_compression_7bit_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          uncompressed_data,
	          4,
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

	result = libesedb_compression_7bit_decompress(
	          esedb_test_compression_lzxpress_compressed_data,
	          33,
	          uncompressed_data,
	          64,
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

/* Tests the libesedb_compression_lzxpress_decompress_get_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_lzxpress_decompress_get_size(
     void )
{
	libcerror_error_t *error      = NULL;
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_lzxpress_decompress_get_size(
	          esedb_test_compression_lzxpress_compressed_data,
	          33,
	          &uncompressed_data_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 26 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_compression_lzxpress_decompress_get_size(
	          NULL,
	          33,
	          &uncompressed_data_size,
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

	result = libesedb_compression_lzxpress_decompress_get_size(
	          esedb_test_compression_lzxpress_compressed_data,
	          (size_t) SSIZE_MAX + 1,
	          &uncompressed_data_size,
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

	result = libesedb_compression_lzxpress_decompress_get_size(
	          esedb_test_compression_lzxpress_compressed_data,
	          0,
	          &uncompressed_data_size,
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

	result = libesedb_compression_lzxpress_decompress_get_size(
	          esedb_test_compression_lzxpress_compressed_data,
	          33,
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

	result = libesedb_compression_lzxpress_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          &uncompressed_data_size,
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

/* Tests the libesedb_compression_lzxpress_decompress function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_lzxpress_decompress(
     void )
{
	uint8_t uncompressed_data[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_lzxpress_decompress(
	          esedb_test_compression_lzxpress_compressed_data,
	          33,
	          uncompressed_data,
	          64,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          (uint8_t *) "abcdefghijklmnopqrstuvwxyz",
	          26 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libesedb_compression_lzxpress_decompress(
	          NULL,
	          33,
	          uncompressed_data,
	          64,
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

/* Tests the libesedb_compression_decompress_get_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_decompress_get_size(
     void )
{
	libcerror_error_t *error      = NULL;
	size_t uncompressed_data_size = 0;
	int result                    = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          &uncompressed_data_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 56 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libesedb_compression_decompress_get_size(
	          esedb_test_compression_lzxpress_compressed_data,
	          33,
	          &uncompressed_data_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "uncompressed_data_size",
	 uncompressed_data_size,
	 (size_t) 26 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libesedb_compression_decompress_get_size(
	          NULL,
	          50,
	          &uncompressed_data_size,
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

	result = libesedb_compression_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
	          &uncompressed_data_size,
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

	result = libesedb_compression_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          0,
	          &uncompressed_data_size,
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

	result = libesedb_compression_decompress_get_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
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

/* Tests the libesedb_compression_decompress function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_decompress(
     void )
{
	uint8_t uncompressed_data[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          uncompressed_data,
	          64,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          esedb_test_compression_7bit_uncompressed_data,
	          56 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	result = libesedb_compression_decompress(
	          esedb_test_compression_lzxpress_compressed_data,
	          33,
	          uncompressed_data,
	          64,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = memory_compare(
	          uncompressed_data,
	          (uint8_t *) "abcdefghijklmnopqrstuvwxyz",
	          26 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	/* Test error cases
	 */
	result = libesedb_compression_decompress(
	          NULL,
	          50,
	          uncompressed_data,
	          64,
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

	result = libesedb_compression_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
	          uncompressed_data,
	          64,
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

	result = libesedb_compression_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          0,
	          uncompressed_data,
	          64,
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

	result = libesedb_compression_decompress(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          NULL,
	          64,
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

/* Tests the libesedb_compression_get_utf8_string_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_get_utf8_string_size(
     void )
{
	libcerror_error_t *error = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_get_utf8_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          &utf8_string_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf8_string_size",
	 utf8_string_size,
	 (size_t) 56 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO add test with LZXPRESS compressed string */

	/* Test error cases
	 */
	result = libesedb_compression_get_utf8_string_size(
	          NULL,
	          50,
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

	result = libesedb_compression_get_utf8_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
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

	result = libesedb_compression_get_utf8_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          0,
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

	result = libesedb_compression_get_utf8_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
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

/* Tests the libesedb_compression_copy_to_utf8_string function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_copy_to_utf8_string(
     void )
{
	uint8_t utf8_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_copy_to_utf8_string(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          utf8_string,
	          64,
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
	          esedb_test_compression_7bit_uncompressed_utf8_string,
	          sizeof( uint8_t ) * 56 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

/* TODO add test with LZXPRESS compressed string */

	/* Test error cases
	 */
	result = libesedb_compression_copy_to_utf8_string(
	          NULL,
	          50,
	          utf8_string,
	          64,
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

	result = libesedb_compression_copy_to_utf8_string(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
	          utf8_string,
	          64,
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

	result = libesedb_compression_copy_to_utf8_string(
	          esedb_test_compression_7bit_compressed_data,
	          0,
	          utf8_string,
	          64,
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

	result = libesedb_compression_copy_to_utf8_string(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          NULL,
	          64,
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

/* Tests the libesedb_compression_get_utf16_string_size function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_get_utf16_string_size(
     void )
{
	libcerror_error_t *error = NULL;
	size_t utf16_string_size = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_get_utf16_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          &utf16_string_size,
	          &error );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	ESEDB_TEST_ASSERT_EQUAL_SIZE(
	 "utf16_string_size",
	 utf16_string_size,
	 (size_t) 56 );

	ESEDB_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

/* TODO add test with LZXPRESS compressed string */

	/* Test error cases
	 */
	result = libesedb_compression_get_utf16_string_size(
	          NULL,
	          50,
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

	result = libesedb_compression_get_utf16_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
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

	result = libesedb_compression_get_utf16_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          0,
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

	result = libesedb_compression_get_utf16_string_size(
	          esedb_test_compression_7bit_compressed_data,
	          50,
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

/* Tests the libesedb_compression_copy_to_utf16_string function
 * Returns 1 if successful or 0 if not
 */
int esedb_test_compression_copy_to_utf16_string(
     void )
{
	uint16_t utf16_string[ 64 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libesedb_compression_copy_to_utf16_string(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          utf16_string,
	          64,
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
	          esedb_test_compression_7bit_uncompressed_utf16_string,
	          sizeof( uint16_t ) * 56 );

	ESEDB_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

/* TODO add test with LZXPRESS compressed string */

	/* Test error cases
	 */
	result = libesedb_compression_copy_to_utf16_string(
	          NULL,
	          50,
	          utf16_string,
	          64,
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

	result = libesedb_compression_copy_to_utf16_string(
	          esedb_test_compression_7bit_compressed_data,
	          (size_t) SSIZE_MAX + 1,
	          utf16_string,
	          64,
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

	result = libesedb_compression_copy_to_utf16_string(
	          esedb_test_compression_7bit_compressed_data,
	          0,
	          utf16_string,
	          64,
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

	result = libesedb_compression_copy_to_utf16_string(
	          esedb_test_compression_7bit_compressed_data,
	          50,
	          NULL,
	          64,
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
	ESEDB_TEST_UNREFERENCED_PARAMETER( argc )
	ESEDB_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT )

	ESEDB_TEST_RUN(
	 "libesedb_compression_7bit_decompress_get_size",
	 esedb_test_compression_7bit_decompress_get_size );

	ESEDB_TEST_RUN(
	 "libesedb_compression_7bit_decompress",
	 esedb_test_compression_7bit_decompress );

	ESEDB_TEST_RUN(
	 "libesedb_compression_lzxpress_decompress_get_size",
	 esedb_test_compression_lzxpress_decompress_get_size );

	ESEDB_TEST_RUN(
	 "libesedb_compression_lzxpress_decompress",
	 esedb_test_compression_lzxpress_decompress );

	ESEDB_TEST_RUN(
	 "libesedb_compression_decompress_get_size",
	 esedb_test_compression_decompress_get_size );

	ESEDB_TEST_RUN(
	 "libesedb_compression_decompress",
	 esedb_test_compression_decompress );

	ESEDB_TEST_RUN(
	 "libesedb_compression_get_utf8_string_size",
	 esedb_test_compression_get_utf8_string_size );

	ESEDB_TEST_RUN(
	 "libesedb_compression_copy_to_utf8_string",
	 esedb_test_compression_copy_to_utf8_string );

	ESEDB_TEST_RUN(
	 "libesedb_compression_get_utf16_string_size",
	 esedb_test_compression_get_utf16_string_size );

	ESEDB_TEST_RUN(
	 "libesedb_compression_copy_to_utf16_string",
	 esedb_test_compression_copy_to_utf16_string );

#endif /* defined( __GNUC__ ) && !defined( LIBESEDB_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

