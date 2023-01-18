/*
 * Index functions
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

#if !defined( _LIBESEDB_INDEX_H )
#define _LIBESEDB_INDEX_H

#include <common.h>
#include <types.h>

#include "libesedb_catalog_definition.h"
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

typedef struct libesedb_internal_index libesedb_internal_index_t;

struct libesedb_internal_index
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

	/* The index catalog definition
	 */
	libesedb_catalog_definition_t *index_catalog_definition;

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

	/* The index page tree
	 */
	libesedb_page_tree_t *index_page_tree;
};

int libesedb_index_initialize(
     libesedb_index_t **index,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libesedb_catalog_definition_t *index_catalog_definition,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libfdata_vector_t *long_values_pages_vector,
     libfcache_cache_t *long_values_pages_cache,
     libesedb_page_tree_t *table_page_tree,
     libesedb_page_tree_t *long_values_page_tree,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_free(
     libesedb_index_t **index,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_get_identifier(
     libesedb_index_t *index,
     uint32_t *identifier,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_get_utf8_name_size(
     libesedb_index_t *index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_get_utf8_name(
     libesedb_index_t *index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_get_utf16_name_size(
     libesedb_index_t *index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_get_utf16_name(
     libesedb_index_t *index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_get_number_of_records(
     libesedb_index_t *index,
     int *number_of_records,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_index_get_record(
     libesedb_index_t *index,
     int record_entry,
     libesedb_record_t **record,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_INDEX_H ) */

