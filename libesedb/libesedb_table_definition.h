/*
 * Table definition functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_TABLE_DEFINITION_H )
#define _LIBESEDB_TABLE_DEFINITION_H

#include <common.h>
#include <types.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"

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

	/* The column catalog definition array
	 */
	libcdata_array_t *column_catalog_definition_array;

	/* The index catalog definition array
	 */
	libcdata_array_t *index_catalog_definition_array;
};

int libesedb_table_definition_initialize(
     libesedb_table_definition_t **table_definition,
     libesedb_catalog_definition_t *table_catalog_definition,
     libcerror_error_t **error );

int libesedb_table_definition_free(
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error );

int libesedb_table_definition_set_long_value_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *long_value_catalog_definition,
     libcerror_error_t **error );

int libesedb_table_definition_set_callback_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *callback_catalog_definition,
     libcerror_error_t **error );

int libesedb_table_definition_get_number_of_column_catalog_definitions(
     libesedb_table_definition_t *table_definition,
     int *number_of_definitions,
     libcerror_error_t **error );

int libesedb_table_definition_get_column_catalog_definition_by_index(
     libesedb_table_definition_t *table_definition,
     int definition_index,
     libesedb_catalog_definition_t **column_catalog_definition,
     libcerror_error_t **error );

int libesedb_table_definition_append_column_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *column_catalog_definition,
     libcerror_error_t **error );

int libesedb_table_definition_get_number_of_index_catalog_definitions(
     libesedb_table_definition_t *table_definition,
     int *number_of_definitions,
     libcerror_error_t **error );

int libesedb_table_definition_get_index_catalog_definition_by_index(
     libesedb_table_definition_t *table_definition,
     int definition_index,
     libesedb_catalog_definition_t **index_catalog_definition,
     libcerror_error_t **error );

int libesedb_table_definition_append_index_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *index_catalog_definition,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_TABLE_DEFINITION_H ) */

