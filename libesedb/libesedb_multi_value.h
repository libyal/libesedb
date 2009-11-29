/*
 * Multi value functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _LIBESEDB_INTERNAL_MULTI_VALUE_H )
#define _LIBESEDB_INTERNAL_MULTI_VALUE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_extern.h"
#include "libesedb_list_type.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_multi_value libesedb_internal_multi_value_t;

struct libesedb_internal_multi_value
{
	/* The multi column type
	 */
	uint32_t column_type;

	/* The value data
	 */
	uint8_t *value_data;

	/* The value data size
	 */
	size_t value_data_size;

	/* The amount of values
	 */
	uint16_t amount_of_values;

	/* The value byte offsets
	 */
	uint16_t *value_offset;

	/* The value byte sizes
	 */
	size_t *value_size;

	/* A copy of the codepage
	 */
	int codepage;
};

int libesedb_multi_value_initialize(
     libesedb_multi_value_t **multi_value,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_free(
                     libesedb_multi_value_t **multi_value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_amount_of_values(
                     libesedb_multi_value_t *multi_value,
                     int *amount_of_values,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint32_t *column_type,
                     uint8_t **value_data,
                     size_t *value_data_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_32bit(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint32_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_64bit(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint64_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_filetime(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint64_t *filetime,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_utf8_string_size(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     size_t *utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_utf8_string(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint8_t *utf8_string,
                     size_t utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_utf16_string_size(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     size_t *utf16_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_utf16_string(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint16_t *utf16_string,
                     size_t utf16_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_binary_data_size(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     size_t *size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_binary_data(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint8_t *binary_data,
                     size_t size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_multi_value_get_value_guid(
                     libesedb_multi_value_t *multi_value,
                     int value_index,
                     uint8_t *guid,
                     size_t size,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

