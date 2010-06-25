/*
 * Catalog functions
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

#if !defined( _LIBESEDB_CATALOG_H )
#define _LIBESEDB_CATALOG_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libfdata.h"
#include "libesedb_list_type.h"
#include "libesedb_table_definition.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_catalog libesedb_catalog_t;

struct libesedb_catalog
{
	/* The table definition list
	 */
	libesedb_list_t *table_definition_list;
};

int libesedb_catalog_initialize(
     libesedb_catalog_t **catalog,
     liberror_error_t **error );

int libesedb_catalog_free(
     libesedb_catalog_t **catalog,
     liberror_error_t **error );

int libesedb_catalog_get_number_of_table_definitions(
     libesedb_catalog_t *catalog,
     int *number_of_table_definitions,
     liberror_error_t **error );

int libesedb_catalog_get_table_definition_by_index(
     libesedb_catalog_t *catalog,
     int table_definition_index,
     libesedb_table_definition_t **table_definition,
     liberror_error_t **error );

int libesedb_catalog_get_table_definition_by_identifier(
     libesedb_catalog_t *catalog,
     uint32_t identifier,
     libesedb_table_definition_t **table_definition,
     liberror_error_t **error );

int libesedb_catalog_get_table_definition_by_name(
     libesedb_catalog_t *catalog,
     uint8_t *table_name,
     size_t table_name_size,
     libesedb_table_definition_t **table_definition,
     liberror_error_t **error );

int libesedb_catalog_read(
     libesedb_catalog_t *catalog,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

