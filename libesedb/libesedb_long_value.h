/*
 * Long value functions
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

#if !defined( _LIBESEDB_LONG_VALUE_H )
#define _LIBESEDB_LONG_VALUE_H

#include <common.h>
#include <types.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_extern.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_libfvalue.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_long_value libesedb_internal_long_value_t;

struct libesedb_internal_long_value
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libesedb_io_handle_t *io_handle;

	/* The column catalog definition
	 */
	libesedb_catalog_definition_t *column_catalog_definition;

	/* The data segments list
	 */
	libfdata_list_t *data_segments_list;

	/* The data segments cache
	 */
	libfcache_cache_t *data_segments_cache;

	/* The record value
	 */
	libfvalue_value_t *record_value;
};

int libesedb_long_value_initialize(
     libesedb_long_value_t **long_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_catalog_definition_t *column_catalog_definition,
     libfdata_list_t *data_segments_list,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_free(
     libesedb_long_value_t **long_value,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_data_size(
     libesedb_long_value_t *long_value,
     size64_t *data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_data(
     libesedb_long_value_t *long_value,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libesedb_long_value_get_record_value(
     libesedb_internal_long_value_t *internal_long_value,
     libfvalue_value_t **record_value,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_number_of_data_segments(
     libesedb_long_value_t *long_value,
     int *number_of_data_segments,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_data_segment_size(
     libesedb_long_value_t *long_value,
     int data_segment_index,
     size_t *data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_data_segment(
     libesedb_long_value_t *long_value,
     int data_segment_index,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_utf8_string_size(
     libesedb_long_value_t *long_value,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_utf8_string(
     libesedb_long_value_t *long_value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_utf16_string_size(
     libesedb_long_value_t *long_value,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_long_value_get_utf16_string(
     libesedb_long_value_t *long_value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_LONG_VALUE_H ) */

