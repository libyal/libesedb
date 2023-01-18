/*
 * Windows Search database export functions
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

#if !defined( _WINDOWS_SEARCH_H )
#define _WINDOWS_SEARCH_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "esedbtools_libcerror.h"
#include "esedbtools_libesedb.h"
#include "log_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

int windows_search_decode(
     uint8_t *data,
     size_t data_size,
     uint8_t *encoded_data,
     size_t encoded_data_size,
     libcerror_error_t **error );

int windows_search_export_compressed_string_value(
     uint8_t *value_data,
     size_t value_data_size,
     int ascii_codepage,
     FILE *record_file_stream,
     libcerror_error_t **error );

int windows_search_export_record_value_32bit(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int windows_search_export_record_value_64bit(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     uint8_t format,
     FILE *record_file_stream,
     libcerror_error_t **error );

int windows_search_export_record_value_filetime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int windows_search_export_record_value_compressed_string(
     libesedb_record_t *record,
     int record_value_entry,
     int ascii_codepage,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int windows_search_export_record_value_utf16_string(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int windows_search_export_record_systemindex_0a(
     libesedb_record_t *record,
     int ascii_codepage,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int windows_search_export_record_systemindex_gthr(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _WINDOWS_SEARCH_H ) */

