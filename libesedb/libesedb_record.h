/*
 * Record (row) functions
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

#if !defined( _LIBESEDB_INTERNAL_RECORD_H )
#define _LIBESEDB_INTERNAL_RECORD_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_array_type.h"
#include "libesedb_data_type_definition.h"
#include "libesedb_extern.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libfdata.h"
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

	/* The pages vector
	 */
	libfdata_vector_t *pages_vector;

	/* The pages cache
	 */
	libfdata_cache_t *pages_cache;

	/* The long values tree
	 */
	libfdata_tree_t *long_values_tree;

	/* The long values cache
	 */
	libfdata_cache_t *long_values_cache;

	/* The item flags
	 */
	uint8_t flags;

	/* The values array
	*/
	libesedb_array_t *values_array;
};

int libesedb_record_initialize(
     libesedb_record_t **record,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     libfdata_tree_node_t *values_tree_node,
     libfdata_cache_t *values_cache,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libfdata_tree_t *long_values_tree,
     libfdata_cache_t *long_values_cache,
     uint8_t flags,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_free(
                     libesedb_record_t **record,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_number_of_values(
                     libesedb_record_t *record,
                     int *number_of_values,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_column_identifier(
                     libesedb_table_t *table,
                     int value_entry,
                     uint32_t *column_identifier,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_column_type(
                     libesedb_table_t *table,
                     int value_entry,
                     uint32_t *column_type,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_utf8_column_name_size(
                     libesedb_record_t *record,
                     int value_entry,
                     size_t *utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_utf8_column_name(
                     libesedb_record_t *record,
                     int value_entry,
                     uint8_t *utf8_string,
                     size_t utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_utf16_column_name_size(
                     libesedb_record_t *record,
                     int value_entry,
                     size_t *utf16_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_utf16_column_name(
                     libesedb_record_t *record,
                     int value_entry,
                     uint16_t *utf16_string,
                     size_t utf16_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value(
                     libesedb_table_t *table,
                     int value_entry,
                     uint8_t **value_data,
                     size_t *value_data_size,
                     uint8_t *value_flags,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_boolean(
                     libesedb_record_t *record,
                     int value_entry,
                     uint8_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_8bit(
                     libesedb_record_t *record,
                     int value_entry,
                     uint8_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_16bit(
                     libesedb_record_t *record,
                     int value_entry,
                     uint16_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_32bit(
                     libesedb_record_t *record,
                     int value_entry,
                     uint32_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_64bit(
                     libesedb_record_t *record,
                     int value_entry,
                     uint64_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_filetime(
                     libesedb_record_t *record,
                     int value_entry,
                     uint64_t *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_floating_point(
                     libesedb_record_t *record,
                     int value_entry,
                     double *value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_utf8_string_size(
                     libesedb_record_t *record,
                     int value_entry,
                     size_t *utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_utf8_string(
                     libesedb_record_t *record,
                     int value_entry,
                     uint8_t *utf8_string,
                     size_t utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_utf16_string_size(
                     libesedb_record_t *record,
                     int value_entry,
                     size_t *utf16_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_utf16_string(
                     libesedb_record_t *record,
                     int value_entry,
                     uint16_t *utf16_string,
                     size_t utf16_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_binary_data_size(
                     libesedb_record_t *record,
                     int value_entry,
                     size_t *binary_data_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_binary_data(
                     libesedb_record_t *record,
                     int value_entry,
                     uint8_t *binary_data,
                     size_t binary_data_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_long_value(
                     libesedb_record_t *record,
                     int value_entry,
                     libesedb_long_value_t **long_value,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_multi_value(
                     libesedb_record_t *record,
                     int value_entry,
                     libesedb_multi_value_t **multi_value,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

