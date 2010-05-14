/*
 * ASCII 7-bit (un)compression functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _ASCII_7BIT_COMPRESSION_H )
#define _ASCII_7BIT_COMPRESSION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

int ascii7_decompress_get_utf8_string_size(
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     size_t *utf8_string_size,
     liberror_error_t **error );

int ascii7_decompress_get_utf16_string_size(
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     size_t *utf16_string_size,
     liberror_error_t **error );

int ascii7_decompress_to_utf8_string(
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error );

int ascii7_decompress_to_utf16_string(
     uint8_t *compressed_data, 
     size_t compressed_data_size,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

