/*
 * Export functions
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EXPORT_H )
#define _EXPORT_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "esedbtools_libesedb.h"

#if defined( __cplusplus )
extern "C" {
#endif

void export_binary_data(
      const uint8_t *data,
      size_t data_size,
      FILE *stream );

void export_narrow_text(
      const char *string,
      size_t string_size,
      FILE *stream );

void export_text(
      const system_character_t *string,
      size_t string_size,
      FILE *stream );

int export_filetime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int export_floatingtime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int export_get_long_value_data(
     libesedb_long_value_t *long_value,
     uint8_t **long_value_data,
     size_t *long_value_data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EXPORT_H ) */

