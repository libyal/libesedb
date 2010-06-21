/*
 * Data definition functions
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include "libesedb_libfdata.h"
#include "libesedb_list_type.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBESEDB_DATA_DEFINITION_TYPES
{
	LIBESEDB_DATA_DEFINITION_TYPE_LONG_VALUE	= (uint8_t) 'l',
	LIBESEDB_DATA_DEFINITION_TYPE_RECORD		= (uint8_t) 'r',
};

typedef struct libesedb_data_definition libesedb_data_definition_t;

struct libesedb_data_definition
{
	/* The data definition type
	 */
	uint8_t type;

	/* The key data
	 */
	uint8_t *key;

	/* The key data size
	 */
	size_t key_size;

	union
	{
		/* The data block for the long value data definition
		 */
		libfdata_block_t *data_block;

		/* The values array for the record data definition
		 */
		libesedb_array_t *values_array;
	};
};

int libesedb_data_definition_initialize(
     libesedb_data_definition_t **data_definition,
     liberror_error_t **error );

int libesedb_data_definition_free(
     intptr_t *data_definition,
     liberror_error_t **error );

int libesedb_data_definition_set_key_common(
     libesedb_data_definition_t *data_definition,
     uint8_t *common_key,
     size_t common_key_size,
     liberror_error_t **error );

int libesedb_data_definition_set_key_local(
     libesedb_data_definition_t *data_definition,
     uint8_t *local_key,
     size_t local_key_size,
     liberror_error_t **error );

int libesedb_data_definition_read_record(
     libesedb_data_definition_t *data_definition,
     libesedb_list_t *table_column_catalog_definition_list,
     libesedb_list_t *template_table_column_catalog_definition_list,
     libesedb_io_handle_t *io_handle,
     uint8_t *definition_data,
     size_t definition_data_size,
     off64_t definition_data_offset,
     liberror_error_t **error );

int libesedb_data_definition_read_long_value(
     libesedb_data_definition_t *data_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error );

int libesedb_data_definition_read_long_value_segment(
     libesedb_data_definition_t *data_definition,
     uint32_t data_segment_number,
     uint8_t *definition_data,
     size_t definition_data_size,
     off64_t definition_data_offset,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

