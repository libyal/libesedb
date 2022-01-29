/*
 * Library to access the Extensible Storage Engine (ESE) Database File (EDB) format
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_H )
#define _LIBESEDB_H

#include <libesedb/codepage.h>
#include <libesedb/definitions.h>
#include <libesedb/error.h>
#include <libesedb/extern.h>
#include <libesedb/features.h>
#include <libesedb/types.h>

#include <stdio.h>

#if defined( LIBESEDB_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBESEDB_EXTERN \
const char *libesedb_get_version(
             void );

/* Returns the access flags for reading
 */
LIBESEDB_EXTERN \
int libesedb_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_get_codepage(
     int *codepage,
     libesedb_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_set_codepage(
     int codepage,
     libesedb_error_t **error );

/* Determines if a file contains an ESEDB file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_check_file_signature(
     const char *filename,
     libesedb_error_t **error );

#if defined( LIBESEDB_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains an ESEDB file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_check_file_signature_wide(
     const wchar_t *filename,
     libesedb_error_t **error );

#endif /* defined( LIBESEDB_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBESEDB_HAVE_BFIO )

/* Determines if a file contains an ESEDB file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_check_file_signature_file_io_handle(
     libbfio_handle_t *bfio_handle,
     libesedb_error_t **error );

#endif /* defined( LIBESEDB_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBESEDB_EXTERN \
void libesedb_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_notify_set_stream(
     FILE *stream,
     libesedb_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_notify_stream_open(
     const char *filename,
     libesedb_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_notify_stream_close(
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBESEDB_EXTERN \
void libesedb_error_free(
      libesedb_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_error_fprint(
     libesedb_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_error_sprint(
     libesedb_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_error_backtrace_fprint(
     libesedb_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * The end-of-string character is not included in the return value
 * Returns the number of printed characters if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_error_backtrace_sprint(
     libesedb_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * File functions
 * ------------------------------------------------------------------------- */

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_initialize(
     libesedb_file_t **file,
     libesedb_error_t **error );

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_free(
     libesedb_file_t **file,
     libesedb_error_t **error );

/* Signals a file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_signal_abort(
     libesedb_file_t *file,
     libesedb_error_t **error );

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_open(
     libesedb_file_t *file,
     const char *filename,
     int access_flags,
     libesedb_error_t **error );

#if defined( LIBESEDB_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_open_wide(
     libesedb_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libesedb_error_t **error );

#endif /* defined( LIBESEDB_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBESEDB_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_open_file_io_handle(
     libesedb_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libesedb_error_t **error );

#endif /* defined( LIBESEDB_HAVE_BFIO ) */

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_close(
     libesedb_file_t *file,
     libesedb_error_t **error );

/* Retrieves the file type
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_type(
     libesedb_file_t *file,
     uint32_t *type,
     libesedb_error_t **error );

/* Retrieves the file (current) format version
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_format_version(
     libesedb_file_t *file,
     uint32_t *format_version,
     uint32_t *format_revision,
     libesedb_error_t **error );

/* Retrieves the file creation version
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_creation_format_version(
     libesedb_file_t *file,
     uint32_t *format_version,
     uint32_t *format_revision,
     libesedb_error_t **error );

/* Retrieves the file page size
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_page_size(
     libesedb_file_t *file,
     uint32_t *page_size,
     libesedb_error_t **error );

/* Retrieves the number of tables
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_number_of_tables(
     libesedb_file_t *file,
     int *number_of_tables,
     libesedb_error_t **error );

/* Retrieves the table for the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_table(
     libesedb_file_t *file,
     int table_entry,
     libesedb_table_t **table,
     libesedb_error_t **error );

/* Retrieves the table for the UTF-8 encoded name
 * Returns 1 if successful, 0 if no table could be found or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_table_by_utf8_name(
     libesedb_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libesedb_table_t **table,
     libesedb_error_t **error );

/* Retrieves the table for the UTF-16 encoded name
 * Returns 1 if successful, 0 if no table could be found or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_get_table_by_utf16_name(
     libesedb_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libesedb_table_t **table,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Table functions
 * ------------------------------------------------------------------------- */

/* Frees a table
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_free(
     libesedb_table_t **table,
     libesedb_error_t **error );

/* Retrieves the table identifier or Father Data Page (FDP) object identifier
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_identifier(
     libesedb_table_t *table,
     uint32_t *identifier,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-8 encoded string of the table name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf8_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-8 encoded string of the table name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf8_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-16 encoded string of the table name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf16_name_size(
     libesedb_table_t *table,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-16 encoded string of the table name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf16_name(
     libesedb_table_t *table,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-8 encoded string of the template name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf8_template_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-8 encoded string of the template name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf8_template_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-16 encoded string of the template name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf16_template_name_size(
     libesedb_table_t *table,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-16 encoded string of the template name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_utf16_template_name(
     libesedb_table_t *table,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the number of columns in the table
 *
 * Use the flag LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE to retrieve the number of columns
 * ignoring the template table
 *
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_number_of_columns(
     libesedb_table_t *table,
     int *number_of_columns,
     uint8_t flags,
     libesedb_error_t **error );

/* Retrieves the column for the specific entry
 *
 * Use the flag LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE to retrieve the column
 * ignoring the template table
 *
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_column(
     libesedb_table_t *table,
     int column_entry,
     libesedb_column_t **column,
     uint8_t flags,
     libesedb_error_t **error );

/* Retrieves the number of indexes
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_number_of_indexes(
     libesedb_table_t *table,
     int *number_of_indexes,
     libesedb_error_t **error );

/* Retrieves the index for the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_index(
     libesedb_table_t *table,
     int index_entry,
     libesedb_index_t **index,
     libesedb_error_t **error );

/* Retrieves the number of records in the table
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_number_of_records(
     libesedb_table_t *table,
     int *number_of_records,
     libesedb_error_t **error );

/* Retrieves the record for the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_table_get_record(
     libesedb_table_t *table,
     int record_entry,
     libesedb_record_t **record,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Column functions
 * ------------------------------------------------------------------------- */

/* Frees a column
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_column_free(
     libesedb_column_t **column,
     libesedb_error_t **error );

/* Retrieves the column identifier
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_column_get_identifier(
     libesedb_column_t *column,
     uint32_t *identifier,
     libesedb_error_t **error );

/* Retrieves the column type
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_column_get_type(
     libesedb_column_t *column,
     uint32_t *type,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-8 encoded string of the column name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_column_get_utf8_name_size(
     libesedb_column_t *column,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-8 encoded string of the column name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_column_get_utf8_name(
     libesedb_column_t *column,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-16 encoded string of the column name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_column_get_utf16_name_size(
     libesedb_column_t *column,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-16 encoded string of the column name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_column_get_utf16_name(
     libesedb_column_t *column,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Index functions
 * ------------------------------------------------------------------------- */

/* Frees a index
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_free(
     libesedb_index_t **index,
     libesedb_error_t **error );

/* Retrieves the index identifier or Father Data Page (FDP) object identifier
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_get_identifier(
     libesedb_index_t *index,
     uint32_t *identifier,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-8 encoded string of the index name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_get_utf8_name_size(
     libesedb_index_t *index,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-8 encoded string of the index name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_get_utf8_name(
     libesedb_index_t *index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-16 encoded string of the index name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_get_utf16_name_size(
     libesedb_index_t *index,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-16 encoded string of the index name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_get_utf16_name(
     libesedb_index_t *index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the number of records in the index
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_get_number_of_records(
     libesedb_index_t *index,
     int *number_of_records,
     libesedb_error_t **error );

/* Retrieves the record for the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_index_get_record(
     libesedb_index_t *index,
     int record_entry,
     libesedb_record_t **record,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Record (row) functions
 * ------------------------------------------------------------------------- */

/* Frees a record
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_free(
     libesedb_record_t **record,
     libesedb_error_t **error );

/* Retrieves the number of values in the record
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_number_of_values(
     libesedb_record_t *record,
     int *number_of_values,
     libesedb_error_t **error );

/* Retrieves the column identifier of the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_column_identifier(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_identifier,
     libesedb_error_t **error );

/* Retrieves the column type of the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_column_type(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_type,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-8 encoded string of the column name of the specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_utf8_column_name_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-8 encoded string of the column name of the specific entry
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_utf8_column_name(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-16 encoded string of the column name of the specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_utf16_column_name_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-16 encoded string of the column name of the specific entry
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_utf16_column_name(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the value data size of the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_data_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *value_data_size,
     libesedb_error_t **error );

/* Retrieves the value data of the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_data(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_data,
     size_t value_data_size,
     libesedb_error_t **error );

/* Retrieves the value data flags of the specific entry
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_data_flags(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_data_flags,
     libesedb_error_t **error );

/* Retrieves the boolean value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_boolean(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_boolean,
     libesedb_error_t **error );

/* Retrieves the 8-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_8bit(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_8bit,
     libesedb_error_t **error );

/* Retrieves the 16-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_16bit(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *value_16bit,
     libesedb_error_t **error );

/* Retrieves the 32-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_32bit(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *value_32bit,
     libesedb_error_t **error );

/* Retrieves the 64-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_64bit(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value_64bit,
     libesedb_error_t **error );

/* Retrieves the 64-bit filetime value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_filetime(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value_filetime,
     libesedb_error_t **error );

/* Retrieves the single precision floating point value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_floating_point_32bit(
     libesedb_record_t *record,
     int value_entry,
     float *value_floating_point_32bit,
     libesedb_error_t **error );

/* Retrieves the double precision floating point value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_floating_point_64bit(
     libesedb_record_t *record,
     int value_entry,
     double *value_floating_point_64bit,
     libesedb_error_t **error );

/* Retrieves the size of an UTF-8 encoded string a specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_utf8_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-8 encoded string of a specific entry
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_utf8_string(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of an UTF-16 encoded string a specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_utf16_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-16 encoded string of a specific entry
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_utf16_string(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the binary data size of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_binary_data_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *binary_data_size,
     libesedb_error_t **error );

/* Retrieves the binary data value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_value_binary_data(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *binary_data,
     size_t binary_data_size,
     libesedb_error_t **error );

/* Determines if a specific entry is a long value
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_is_long_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_error_t **error );

/* Determines if a specific entry is a multi value
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_is_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_error_t **error );

/* Retrieves the long value of a specific entry
 * Creates a new long value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_long_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_long_value_t **long_value,
     libesedb_error_t **error );

/* Retrieves the multi value of a specific entry
 * Creates a new multi value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_record_get_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_multi_value_t **multi_value,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Record (row) functions - deprecated
 * ------------------------------------------------------------------------- */

/* Retrieves the value of the specific entry
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use the libesedb_record_get_value_data functions instead.
 */
LIBESEDB_DEPRECATED \
LIBESEDB_EXTERN \
int libesedb_record_get_value(
     libesedb_record_t *record,
     int value_entry,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t *value_flags,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Long value functions
 * ------------------------------------------------------------------------- */

/* Frees a long value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_free(
     libesedb_long_value_t **long_value,
     libesedb_error_t **error );

/* Retrieve the data size
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_data_size(
     libesedb_long_value_t *long_value,
     size64_t *data_size,
     libesedb_error_t **error );

/* Retrieve the data
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_data(
     libesedb_long_value_t *long_value,
     uint8_t *data,
     size_t data_size,
     libesedb_error_t **error );

/* Retrieve the number of data segments
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_number_of_data_segments(
     libesedb_long_value_t *long_value,
     int *number_of_data_segments,
     libesedb_error_t **error );

/* Retrieve the data segment size
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_data_segment_size(
     libesedb_long_value_t *long_value,
     int data_segment_index,
     size_t *data_size,
     libesedb_error_t **error );

/* Retrieve the data segment
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_data_segment(
     libesedb_long_value_t *long_value,
     int data_segment_index,
     uint8_t *data_segment,
     size_t data_size,
     libesedb_error_t **error );

/* Retrieves the size of the data as an UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_utf8_string_size(
     libesedb_long_value_t *long_value,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the data as an UTF-8 encoded string
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_utf8_string(
     libesedb_long_value_t *long_value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the data as an UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_utf16_string_size(
     libesedb_long_value_t *long_value,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the data as an UTF-16 encoded string
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_long_value_get_utf16_string(
     libesedb_long_value_t *long_value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Multi value functions
 * ------------------------------------------------------------------------- */

/* Frees a multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_free(
     libesedb_multi_value_t **multi_value,
     libesedb_error_t **error );

/* Retrieves the column type
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_column_type(
     libesedb_multi_value_t *multi_value,
     uint32_t *column_type,
     libesedb_error_t **error );

/* Retrieves the number of values in a multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_number_of_values(
     libesedb_multi_value_t *multi_value,
     int *number_of_values,
     libesedb_error_t **error );

/* Retrieves the value data size of the specific value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_data_size(
     libesedb_multi_value_t *multi_value,
     int value_entry,
     size_t *value_data_size,
     libesedb_error_t **error );

/* Retrieves the value data of the specific value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_data(
     libesedb_multi_value_t *multi_value,
     int value_entry,
     uint8_t *value_data,
     size_t value_data_size,
     libesedb_error_t **error );

/* Retrieves the 8-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_8bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint8_t *value_8bit,
     libesedb_error_t **error );

/* Retrieves the 16-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_16bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint16_t *value_16bit,
     libesedb_error_t **error );

/* Retrieves the 32-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_32bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint32_t *value_32bit,
     libesedb_error_t **error );

/* Retrieves the 64-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_64bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint64_t *value_64bit,
     libesedb_error_t **error );

/* Retrieves the 32-bit floating point value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_floating_point_32bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     float *value_32bit,
     libesedb_error_t **error );

/* Retrieves the 64-bit floating point value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_floating_point_64bit(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     double *value_64bit,
     libesedb_error_t **error );

/* Retrieves the 64-bit filetime value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_filetime(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint64_t *value_filetime,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-8 encoded string of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf8_string_size(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     size_t *utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-8 encoded string value of a specific value of the multi value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf8_string(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libesedb_error_t **error );

/* Retrieves the size of the UTF-16 encoded string of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf16_string_size(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     size_t *utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the UTF-16 encoded string value of a specific value of the multi value
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_utf16_string(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libesedb_error_t **error );

/* Retrieves the size of a binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_binary_data_size(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     size_t *binary_data_size,
     libesedb_error_t **error );

/* Retrieves the binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value_binary_data(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint8_t *binary_data,
     size_t binary_data_size,
     libesedb_error_t **error );

/* -------------------------------------------------------------------------
 * Multi value functions - deprecated
 * ------------------------------------------------------------------------- */

/* Retrieves a specific value of the multi value
 * Returns 1 if successful or -1 on error
 *
 * This function is deprecated use the libesedb_multi_value_get_value_data functions instead.
 */
LIBESEDB_DEPRECATED \
LIBESEDB_EXTERN \
int libesedb_multi_value_get_value(
     libesedb_multi_value_t *multi_value,
     int multi_value_index,
     uint32_t *value_type,
     uint8_t **value_data,
     size_t *value_data_size,
     libesedb_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_H ) */

