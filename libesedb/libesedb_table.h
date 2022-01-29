/*
 * Table functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_TABLE_H )
#define _LIBESEDB_TABLE_H

#include <common.h>
#include <types.h>

#include "libesedb_extern.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_table libesedb_internal_table_t;

struct libesedb_internal_table
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libesedb_io_handle_t *io_handle;

	/* The table definition
	 */
	libesedb_table_definition_t *table_definition;

	/* The template table definition
	 */
	libesedb_table_definition_t *template_table_definition;

	/* The pages vector
	 */
	libfdata_vector_t *pages_vector;

	/* The pages cache
	 */
	libfcache_cache_t *pages_cache;

	/* The long values pages vector
	 */
	libfdata_vector_t *long_values_pages_vector;

	/* The long values pages cache
	 */
	libfcache_cache_t *long_values_pages_cache;

	/* The table page tree
	 */
	libesedb_page_tree_t *table_page_tree;

	/* The long values page tree
	 */
	libesedb_page_tree_t *long_values_page_tree;
};

int libesedb_table_initialize(
     libesedb_table_t **table,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_free(
     libesedb_table_t **table,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_identifier(
     libesedb_table_t *table,
     uint32_t *identifier,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf8_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf8_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf16_name_size(
     libesedb_table_t *table,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf16_name(
     libesedb_table_t *table,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf8_template_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf8_template_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf16_template_name_size(
     libesedb_table_t *table,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_utf16_template_name(
     libesedb_table_t *table,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_number_of_columns(
     libesedb_table_t *table,
     int *number_of_columns,
     uint8_t flags,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_column(
     libesedb_table_t *table,
     int column_entry,
     libesedb_column_t **column,
     uint8_t flags,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_number_of_indexes(
     libesedb_table_t *table,
     int *number_of_indexes,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_index(
     libesedb_table_t *table,
     int index_entry,
     libesedb_index_t **index,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_number_of_records(
     libesedb_table_t *table,
     int *number_of_records,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_record(
     libesedb_table_t *table,
     int record_entry,
     libesedb_record_t **record,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_TABLE_H ) */

