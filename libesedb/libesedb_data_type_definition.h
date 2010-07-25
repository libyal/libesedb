/*
 * Data type definition functions
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

#if !defined( _LIBESEDB_DATA_TYPE_DEFINITION_H )
#define _LIBESEDB_DATA_TYPE_DEFINITION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_data_type_definition libesedb_data_type_definition_t;

struct libesedb_data_type_definition
{
	/* The data type definition data
	 */
	uint8_t *data;

	/* The data type definition data size
	 */
	size_t data_size;

	/* The file offset of the data type definition data
	 */
	off64_t data_offset;

	/* The flags
	 */
	uint8_t flags;
};

int libesedb_data_type_definition_initialize(
     libesedb_data_type_definition_t **data_type_definition,
     liberror_error_t **error );

int libesedb_data_type_definition_free(
     intptr_t *data_type_definition,
     liberror_error_t **error );

int libesedb_data_type_definition_set_data(
     libesedb_data_type_definition_t *data_type_definition,
     uint8_t *data,
     size_t data_size,
     off64_t data_offset,
     liberror_error_t **error );

int libesedb_data_type_definition_set_flags(
     libesedb_data_type_definition_t *data_type_definition,
     uint8_t flags,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

