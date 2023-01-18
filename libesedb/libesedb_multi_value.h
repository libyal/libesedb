/*
 * Multi value functions
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

#if !defined( _LIBESEDB_MULTI_VALUE_H )
#define _LIBESEDB_MULTI_VALUE_H

#include <common.h>
#include <types.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_extern.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfvalue.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_multi_value libesedb_internal_multi_value_t;

struct libesedb_internal_multi_value
{
	/* The column catalog definition
	 */
	libesedb_catalog_definition_t *column_catalog_definition;

	/* The record value
	 */
	libfvalue_value_t *record_value;
};

int libesedb_multi_value_initialize(
     libesedb_multi_value_t **multi_value,
     libesedb_catalog_definition_t *column_catalog_definition,
     libfvalue_value_t *record_value,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_free(
     libesedb_multi_value_t **multi_value,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_column_type(
     libesedb_multi_value_t *multi_value,
     uint32_t *column_type,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_number_of_values(
     libesedb_multi_value_t *multi_value,
     int *number_of_values,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_data_size(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_data(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

/* TODO deprecate */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint32_t *column_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_data_size(
     libesedb_record_t *record,
     int multi_value_index,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_data(
     libesedb_record_t *record,
     int multi_value_index,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_8bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint8_t *value_8bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_16bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint16_t *value_16bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_32bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint32_t *value_32bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_64bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint64_t *value_64bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_floating_point_32bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     float *value_32bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_floating_point_64bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     double *value_64bit,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_filetime(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint64_t *value_filetime,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf8_string_size(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf8_string(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf16_string_size(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf16_string(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_binary_data_size(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     size_t *binary_data_size,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_binary_data(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_MULTI_VALUE_H ) */

