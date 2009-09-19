/*
 * Index definition functions
 *
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

#if !defined( _LIBPFF_INDEX_DEFINITION_H )
#define _LIBPFF_INDEX_DEFINITION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_index_definition libesedb_index_definition_t;

struct libesedb_index_definition
{
	/* The father data page (FDP) object identifier
	 */
	uint32_t father_data_page_object_identifier;

	/* The father data page (FDP) number 
	 */
	uint32_t father_data_page_number;

	/* The name string
	 */
	uint8_t *name;

	/* The size of the name string
	 */
	size_t name_size;
};

int libesedb_index_definition_initialize(
     libesedb_index_definition_t **index_definition,
     liberror_error_t **error );

int libesedb_index_definition_free(
     intptr_t *index_definition,
     liberror_error_t **error );

int libesedb_index_definition_read(
     libesedb_index_definition_t *index_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     uint16_t definition_flags,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

