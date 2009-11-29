/*
 * Table definition functions
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

#if !defined( _LIBESEDB_TABLE_DEFINITION_H )
#define _LIBESEDB_TABLE_DEFINITION_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_list_type.h"
#include "libesedb_string.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_table_definition libesedb_table_definition_t;

struct libesedb_table_definition
{
	/* The table catalog definition
	 */
	libesedb_catalog_definition_t *table_catalog_definition;

	/* The long value catalog definition
	 */
	libesedb_catalog_definition_t *long_value_catalog_definition;

	/* The callback catalog definition
	 */
	libesedb_catalog_definition_t *callback_catalog_definition;

	/* The column catalog definition list
	 */
	libesedb_list_t *column_catalog_definition_list;

	/* The index catalog definition list
	 */
	libesedb_list_t *index_catalog_definition_list;
};

int libesedb_table_definition_initialize(
     libesedb_table_definition_t **table_definition,
     libesedb_catalog_definition_t *table_catalog_definition,
     liberror_error_t **error );

int libesedb_table_definition_free(
     intptr_t *table_definition,
     liberror_error_t **error );

int libesedb_table_definition_set_long_value_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *long_value_catalog_definition,
     liberror_error_t **error );

int libesedb_table_definition_set_callback_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *callback_catalog_definition,
     liberror_error_t **error );

int libesedb_table_definition_append_column_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *column_catalog_definition,
     liberror_error_t **error );

int libesedb_table_definition_append_index_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *index_catalog_definition,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

