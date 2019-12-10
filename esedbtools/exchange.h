/*
 * Exchange database export functions
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

#if !defined( _EXPORT_EXCHANGE_H )
#define _EXPORT_EXCHANGE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "esedbtools_libcerror.h"
#include "esedbtools_libesedb.h"
#include "log_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

int exchange_export_record_binary_data(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_32bit(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_64bit(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_filetime(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_guid(
     libesedb_record_t *record,
     int record_value_entry,
     uint8_t byte_order,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_mapi_entryid(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_mapi_multi_value(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_sid(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_value_string(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     libcerror_error_t **error );

int exchange_export_record_folders(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int exchange_export_record_global(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int exchange_export_record_mailbox(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int exchange_export_record_msg(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int exchange_export_record_per_user_read(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EXPORT_EXCHANGE_H ) */

