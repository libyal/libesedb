/*
 * Export handle
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

#if !defined( _EXPORT_HANDLE_H )
#define _EXPORT_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "esedbtools_libcdata.h"
#include "esedbtools_libcerror.h"
#include "esedbtools_libesedb.h"
#include "log_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

enum EXPORT_MODES
{
	EXPORT_MODE_ALL			= (int) 'a',
	EXPORT_MODE_TABLES		= (int) 't'
};

typedef struct export_handle export_handle_t;

struct export_handle
{
	/* The export mode
	 */
	uint8_t export_mode;

	/* The libesedb input file
	 */
	libesedb_file_t *input_file;

	/* The target path
	 */
	system_character_t *target_path;

	/* The target path size
	 */
	size_t target_path_size;

	/* The items export path
	 */
	system_character_t *items_export_path;

	/* The items export path size
	 */
	size_t items_export_path_size;

	/* The notification output stream
	 */
	FILE *notify_stream;

	/* The ascii codepage
	 */
	int ascii_codepage;

	/* Value to indicate if abort was signalled
	 */
	int abort;
};

int export_handle_initialize(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_free(
     export_handle_t **export_handle,
     libcerror_error_t **error );

int export_handle_signal_abort(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_set_export_mode(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_ascii_codepage(
     export_handle_t *export_handle,
     const system_character_t *string,
     libcerror_error_t **error );

int export_handle_set_target_path(
     export_handle_t *export_handle,
     const system_character_t *target_path,
     libcerror_error_t **error );

int export_handle_set_export_path(
     export_handle_t *export_handle,
     const system_character_t *base_path,
     size_t base_path_length,
     const system_character_t *suffix,
     size_t suffix_length,
     system_character_t **export_path,
     size_t *export_path_size,
     libcerror_error_t **error );

int export_handle_create_items_export_path(
     export_handle_t *export_handle,
     libcerror_error_t **error );

int export_handle_open(
     export_handle_t *export_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int export_handle_close(
     export_handle_t *export_handle,
     libcerror_error_t **error );

/* Database functions
 */
int export_handle_free_table_name(
     char **table_name,
     libcerror_error_t **error );

/* Item generic export functions
 */
int export_handle_create_item_filename(
     export_handle_t *export_handle,
     int item_index,
     const system_character_t *item_name,
     size_t item_name_length,
     system_character_t **item_filename,
     size_t *item_filename_size,
     libcerror_error_t **error );

int export_handle_create_text_item_file(
     export_handle_t *export_handle,
     const system_character_t *item_filename,
     size_t item_filename_length,
     const system_character_t *export_path,
     size_t export_path_length,
     FILE **item_file_stream,
     libcerror_error_t **error );

int export_handle_export_table(
     export_handle_t *export_handle,
     int database_type,
     libesedb_table_t *table,
     int table_index,
     const system_character_t *table_name,
     size_t table_name_length,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_indexes(
     export_handle_t *export_handle,
     libesedb_table_t *table,
     const system_character_t *table_name,
     size_t table_name_length,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_index(
     export_handle_t *export_handle,
     libesedb_index_t *index,
     int index_iterator,
     const system_character_t *index_name,
     size_t index_name_length,
     const system_character_t *export_path,
     size_t export_path_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_record(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_record_value(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_basic_record_value(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_long_record_value(
     libesedb_record_t *record,
     int record_value_entry,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

int export_handle_export_file(
     export_handle_t *export_handle,
     const system_character_t *export_table_name,
     size_t export_table_name_length,
     log_handle_t *log_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _EXPORT_HANDLE_H ) */

