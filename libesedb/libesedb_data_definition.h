/*
 * Data definition functions
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

#if !defined( _LIBESEDB_DATA_DEFINITION_H )
#define _LIBESEDB_DATA_DEFINITION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_array_type.h"
#include "libesedb_io_handle.h"
#include "libesedb_list_type.h"
#include "libesedb_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_data_definition libesedb_data_definition_t;

struct libesedb_data_definition
{
	/* The key data
	 */
	uint8_t *key;

	/* The key data size
	 */
	size_t key_size;

	/* The values array
	 */
	libesedb_array_t *values_array;
};

int libesedb_data_definition_initialize(
     libesedb_data_definition_t **data_definition,
     liberror_error_t **error );

int libesedb_data_definition_free(
     intptr_t *data_definition,
     liberror_error_t **error );

int libesedb_data_definition_set_key(
     libesedb_data_definition_t *data_definition,
     uint8_t *key,
     size_t key_size,
     liberror_error_t **error );

int libesedb_data_definition_read_record(
     libesedb_data_definition_t *data_definition,
     libesedb_list_t *column_catalog_definition_list,
     libesedb_io_handle_t *io_handle,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error );

int libesedb_data_definition_read_long_value(
     libesedb_data_definition_t *data_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

