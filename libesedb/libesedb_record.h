/*
 * Record (row) functions
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

#if !defined( _LIBESEDB_RECORD_H )
#define _LIBESEDB_RECORD_H

#include <common.h>
#include <types.h>

#include "libesedb_data_definition.h"
#include "libesedb_extern.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_record libesedb_internal_record_t;

struct libesedb_internal_record
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

	/* The data definition
	 */
	libesedb_data_definition_t *data_definition;

	/* The long values page tree
	 */
	libesedb_page_tree_t *long_values_page_tree;

	/* The values array
	 */
	libcdata_array_t *values_array;

	/* The (record) flags 
	 */
	uint8_t flags;
};

int libesedb_record_initialize(
     libesedb_record_t **record,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libfdata_vector_t *long_values_pages_vector,
     libfcache_cache_t *long_values_pages_cache,
     libesedb_data_definition_t *data_definition,
     libesedb_page_tree_t *long_values_page_tree,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_free(
     libesedb_record_t **record,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_number_of_values(
     libesedb_record_t *record,
     int *number_of_values,
     libcerror_error_t **error );

int libesedb_record_get_column_catalog_definition(
     libesedb_internal_record_t *internal_record,
     int value_entry,
     libesedb_catalog_definition_t **column_catalog_definition,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_column_identifier(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_identifier,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_column_type(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_type,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_utf8_column_name_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_utf8_column_name(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_utf16_column_name_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_utf16_column_name(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

/* TODO deprecate */
LIBESEDB_EXTERN \
int libesedb_record_get_value(
     libesedb_record_t *record,
     int value_entry,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t *value_flags,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_data_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_data(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_data_flags(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_data_flags,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_boolean(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_boolean,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_8bit(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_8bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_16bit(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *value_16bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_32bit(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *value_32bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_64bit(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_filetime(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value_filetime,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_floating_point_32bit(
     libesedb_record_t *record,
     int value_entry,
     float *value_floating_point_32bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_floating_point_64bit(
     libesedb_record_t *record,
     int value_entry,
     double *value_floating_point_64bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_utf8_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_utf8_string(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_utf16_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_utf16_string(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_binary_data_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *binary_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_binary_data(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_is_long_value(
     libesedb_record_t *record,
     int value_entry,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_is_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libcerror_error_t **error );

int libesedb_record_get_long_value_data_segments_list(
     libesedb_internal_record_t *internal_record,
     const uint8_t *long_value_key,
     size_t long_value_key_size,
     libfdata_list_t **data_segments_list,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_long_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_long_value_t **long_value,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_multi_value_t **multi_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_RECORD_H ) */

