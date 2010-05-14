/* 
 * Export handle
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (C) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

/* If libtool DLL support is enabled set LIBESEDB_DLL_IMPORT
 * before including libesedb_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBESEDB_DLL_EXPORT
#endif

#include <libesedb.h>

#include "log_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* The libesedb input handle
	 */
	libesedb_file_t *input_handle;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     liberror_error_t **error );

int export_handle_open(
     export_handle_t *export_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error );

int export_handle_make_directory(
     libcstring_system_character_t *directory_name,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_sanitize_filename(
     libcstring_system_character_t *filename,
     size_t filename_size,
     liberror_error_t **error );

int export_handle_create_target_path(
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     uint8_t *utf8_filename,
     size_t utf8_filename_size,
     libcstring_system_character_t **target_path,
     size_t *target_path_size,
     liberror_error_t **error );

int export_handle_export_table(
     export_handle_t *export_handle,
     libesedb_table_t *table,
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_export_record(
     libesedb_record_t *record,
     FILE *table_file_stream,
     liberror_error_t **error );

int export_handle_export_record_value(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *table_file_stream,
     liberror_error_t **error );

int export_handle_export_file(
     export_handle_t *export_handle,
     libcstring_system_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error );

int decompress_7bit_ascii(
     uint8_t *uncompressed_data,
     size_t uncompressed_data_size,
     uint8_t *compressed_data,
     size_t compressed_data_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

