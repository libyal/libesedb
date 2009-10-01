/*
 * Column functions
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

#if !defined( _LIBESEDB_COLUMN_H )
#define _LIBESEDB_COLUMN_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_extern.h"
#include "libesedb_table.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_column libesedb_internal_column_t;

struct libesedb_internal_column
{
	/* The internal table
	 */
	libesedb_internal_table_t *internal_table;

	/* The catalog definition
	 */
	libesedb_catalog_definition_t *catalog_definition;

	/* The column reference list element
	 */
	libesedb_list_element_t *list_element;
};

int libesedb_column_initialize(
     libesedb_column_t **column,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_column_free(
                     libesedb_column_t **column,
                     liberror_error_t **error );

int libesedb_column_free_no_detach(
     intptr_t *internal_column,
     liberror_error_t **error );

int libesedb_column_attach(
     libesedb_internal_column_t *internal_column,
     libesedb_internal_table_t *internal_table,
     libesedb_catalog_definition_t *catalog_definition,
     liberror_error_t **error );

int libesedb_column_detach(
     libesedb_internal_column_t *internal_column,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_column_get_identifier(
                     libesedb_column_t *column,
                     uint32_t *identifier,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_column_get_utf8_name_size(
                     libesedb_column_t *column,
                     size_t *utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_column_get_utf8_name(
                     libesedb_column_t *column,
                     uint8_t *utf8_string,
                     size_t utf8_string_size,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

