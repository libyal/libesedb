/*
 * Record value functions
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

#if !defined( _LIBESEDB_RECORD_VALUE_H )
#define _LIBESEDB_RECORD_VALUE_H

#include <common.h>
#include <types.h>

#include "libesedb_libcerror.h"
#include "libesedb_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libesedb_record_value_get_utf8_string_size(
     libfvalue_value_t *record_value,
     size_t *utf8_string_size,
     libcerror_error_t **error );

int libesedb_record_value_get_utf8_string(
     libfvalue_value_t *record_value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

int libesedb_record_value_get_utf16_string_size(
     libfvalue_value_t *record_value,
     size_t *utf16_string_size,
     libcerror_error_t **error );

int libesedb_record_value_get_utf16_string(
     libfvalue_value_t *record_value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_RECORD_VALUE_H ) */

