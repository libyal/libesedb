/* 
 * Export handle
 *
 * Copyright (C) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

/* If libtool DLL support is enabled set LIBESEDB_DLL_IMPORT
 * before including libesedb_extern.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBESEDB_DLL_EXPORT
#endif

#include <libesedb.h>

#include <libsystem.h>

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
     const libsystem_character_t *filename,
     liberror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     liberror_error_t **error );

int export_handle_make_directory(
     libsystem_character_t *directory_name,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_sanitize_filename(
     libsystem_character_t *filename,
     size_t filename_size,
     liberror_error_t **error );

int export_handle_create_target_path(
     libsystem_character_t *export_path,
     size_t export_path_size,
     uint8_t *utf8_filename,
     size_t utf8_filename_size,
     libsystem_character_t **target_path,
     size_t *target_path_size,
     liberror_error_t **error );

int export_handle_export_table(
     export_handle_t *export_handle,
     libesedb_table_t *table,
     libsystem_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error );

int export_handle_export_file(
     export_handle_t *export_handle,
     libsystem_character_t *export_path,
     size_t export_path_size,
     log_handle_t *log_handle,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

