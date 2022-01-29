/*
 * Compression functions
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

#if !defined( _LIBESEDB_COMPRESSION_H )
#define _LIBESEDB_COMPRESSION_H

#include <common.h>
#include <types.h>

#include "libesedb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libesedb_compression_7bit_decompress_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

int libesedb_compression_7bit_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error );

int libesedb_compression_lzxpress_decompress_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

int libesedb_compression_lzxpress_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error );

int libesedb_compression_decompress_get_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *uncompressed_data_size,
     libcerror_error_t **error );

int libesedb_compression_decompress(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     libcerror_error_t **error );

int libesedb_compression_get_utf8_string_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libesedb_compression_copy_to_utf8_string(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libesedb_compression_get_utf16_string_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libesedb_compression_copy_to_utf16_string(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_COMPRESSION_H ) */

