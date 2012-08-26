/*
 * Value data handle functions
 *
 * Copyright (c) 2009-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_VALUE_DATA_HANDLE_H )
#define _LIBESEDB_VALUE_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libesedb_array_type.h"
#include "libesedb_value_libcerror.h"
#include "libesedb_value_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_value_data_handle libesedb_value_data_handle_t;

struct libesedb_value_data_handle
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;

	/* The encoding
	 */
	int encoding;

	/* The value entries array
	 */
	libesedb_array_t *value_entries;

	/* The flags
	 */
	uint8_t flags;
};

int libesedb_value_data_handle_initialize(
     libesedb_value_data_handle_t **data_handle,
     libcerror_error_t **error );

int libesedb_value_data_handle_free(
     libesedb_value_data_handle_t **data_handle,
     libcerror_error_t **error );

int libesedb_value_data_handle_clone(
     libesedb_value_data_handle_t **destination_data_handle,
     libesedb_value_data_handle_t *source_data_handle,
     libcerror_error_t **error );

int libesedb_value_data_handle_get_data(
     libesedb_value_data_handle_t *data_handle,
     uint8_t **data,
     size_t *data_size,
     int *encoding,
     libcerror_error_t **error );

int libesedb_value_data_handle_set_data(
     libesedb_value_data_handle_t *data_handle,
     const uint8_t *data,
     size_t data_size,
     int encoding,
     uint8_t flags,
     libcerror_error_t **error );

int libesedb_value_data_handle_get_number_of_value_entries(
     libesedb_value_data_handle_t *data_handle,
     int *number_of_value_entries,
     libcerror_error_t **error );

int libesedb_value_data_handle_get_value_entry(
     libesedb_value_data_handle_t *data_handle,
     int value_entry_index,
     uint8_t **value_entry_data,
     size_t *value_entry_data_size,
     int *encoding,
     libcerror_error_t **error );

int libesedb_value_data_handle_set_value_entry(
     libesedb_value_data_handle_t *data_handle,
     int value_entry_index,
     const uint8_t *value_entry_data,
     size_t value_entry_data_size,
     int encoding,
     libcerror_error_t **error );

int libesedb_value_data_handle_append_value_entry(
     libesedb_value_data_handle_t *data_handle,
     int *value_entry_index,
     const uint8_t *value_entry_data,
     size_t value_entry_data_size,
     int encoding,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

