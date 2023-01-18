/*
 * Record (row) functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libesedb_compression.h"
#include "libesedb_data_definition.h"
#include "libesedb_data_segment.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_libfvalue.h"
#include "libesedb_long_value.h"
#include "libesedb_multi_value.h"
#include "libesedb_page_tree.h"
#include "libesedb_page_tree_key.h"
#include "libesedb_record.h"
#include "libesedb_record_value.h"
#include "libesedb_table_definition.h"
#include "libesedb_types.h"

/* Creates a record
 * Make sure the value record is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_initialize(
     libesedb_record_t **record,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libfdata_vector_t *long_values_pages_vector,
     libfcache_cache_t *long_values_pages_cache,
     libesedb_data_definition_t *data_definition,
     libesedb_page_tree_t *long_values_page_tree,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_initialize";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record value already set.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( table_definition->table_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	internal_record = memory_allocate_structure(
	                   libesedb_internal_record_t );

	if( internal_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create record.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_record,
	     0,
	     sizeof( libesedb_internal_record_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear record.",
		 function );

		memory_free(
		 internal_record );

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( internal_record->values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
	}
	if( libesedb_data_definition_read_record(
	     data_definition,
	     file_io_handle,
	     io_handle,
	     pages_vector,
	     pages_cache,
	     table_definition,
	     template_table_definition,
	     internal_record->values_array,
	     &( internal_record->flags ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data definition record.",
		 function );

		goto on_error;
	}
	internal_record->file_io_handle            = file_io_handle;
	internal_record->io_handle                 = io_handle;
	internal_record->table_definition          = table_definition;
	internal_record->template_table_definition = template_table_definition;
	internal_record->pages_vector              = pages_vector;
	internal_record->pages_cache               = pages_cache;
	internal_record->long_values_pages_vector  = long_values_pages_vector;
	internal_record->long_values_pages_cache   = long_values_pages_cache;
	internal_record->data_definition           = data_definition;
	internal_record->long_values_page_tree     = long_values_page_tree;

	*record = (libesedb_record_t *) internal_record;

	return( 1 );

on_error:
	if( internal_record != NULL )
	{
		if( internal_record->values_array != NULL )
		{
			/* Note that if libesedb_data_definition_read_record fails values_array
			 * can contain values that need to be freed with libfvalue_value_free.
			 */
			libcdata_array_free(
			 &( internal_record->values_array ),
			 (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
			 NULL );
		}
		memory_free(
		 internal_record );
	}
	return( -1 );
}

/* Frees a record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_free(
     libesedb_record_t **record,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_free";
	int result                                  = 1;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		internal_record = (libesedb_internal_record_t *) *record;
		*record         = NULL;

		/* The io_handle, file_io_handle, table_definition, template_table_definition, pages_vector,
		 * pages_cache, * long_values_pages_vector, long_values_pages_cache and long_values_page_tree
		 * references are freed elsewhere
		 */
		if( internal_record->data_definition != NULL )
		{
			if( libesedb_data_definition_free(
			     &( internal_record->data_definition ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data definition.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( internal_record->values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_record );
	}
	return( result );
}

/* Retrieves the number of values in the record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_number_of_values(
     libesedb_record_t *record,
     int *number_of_values,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_get_number_of_values";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libcdata_array_get_number_of_entries(
	     internal_record->values_array,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific column catalog definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_column_catalog_definition(
     libesedb_internal_record_t *internal_record,
     int value_entry,
     libesedb_catalog_definition_t **column_catalog_definition,
     libcerror_error_t **error )
{
	static char *function                = "libesedb_record_get_column_identifier";
	int template_table_number_of_columns = 0;

	if( internal_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( internal_record->table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_record->template_table_definition != NULL )
	{
		if( libesedb_table_definition_get_number_of_column_catalog_definitions(
		     internal_record->template_table_definition,
		     &template_table_number_of_columns,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of columns from template table.",
			 function );

			return( -1 );
		}
	}
	if( value_entry < template_table_number_of_columns )
	{
		if( libesedb_table_definition_get_column_catalog_definition_by_index(
		     internal_record->template_table_definition,
		     value_entry,
		     column_catalog_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column catalog definition from template table.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libesedb_table_definition_get_column_catalog_definition_by_index(
		     internal_record->table_definition,
		     value_entry - template_table_number_of_columns,
		     column_catalog_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column catalog definition from table.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the column identifier of the specific entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_column_identifier(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_identifier,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_column_identifier";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_identifier(
	     column_catalog_definition,
	     column_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition identifier.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the column type of the specific entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_column_type(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_type,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_column_type";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded string of the column name of the specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_utf8_column_name_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf8_column_name_size";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name_size(
	     column_catalog_definition,
	     utf8_string_size,
	     internal_record->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded string of the column name of the specific entry
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_utf8_column_name(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf8_column_name";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name(
	     column_catalog_definition,
	     utf8_string,
	     utf8_string_size,
	     internal_record->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded string of the column name of the specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_utf16_column_name_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf16_column_name_size";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name_size(
	     column_catalog_definition,
	     utf16_string_size,
	     internal_record->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded string of the column name of the specific entry
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_utf16_column_name(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf16_column_name";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name(
	     column_catalog_definition,
	     utf16_string,
	     utf16_string_size,
	     internal_record->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value of the specific entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_value(
     libesedb_record_t *record,
     int value_entry,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t *value_flags,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	libfvalue_value_t *record_value             = NULL;
	static char *function                       = "libesedb_record_get_value";
	uint32_t data_flags                         = 0;
	int encoding                                = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( value_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value flags.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data(
	     record_value,
	     value_data,
	     value_data_size,
	     &encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data_flags(
	     record_value,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 value_entry );

		return( -1 );
	}
	*value_flags = (uint8_t) data_flags;

	return( 1 );
}

/* Retrieves the value data flags of the specific entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_value_data_flags(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_data_flags,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	libfvalue_value_t *record_value             = NULL;
	static char *function                       = "libesedb_record_get_value_data_flags";
	uint32_t data_flags                         = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( value_data_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data flags.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data_flags(
	     record_value,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 value_entry );

		return( -1 );
	}
	*value_data_flags = (uint8_t) data_flags;

	return( 1 );
}

/* Retrieves the value data size of the specific entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_value_data_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	libfvalue_value_t *record_value             = NULL;
	static char *function                       = "libesedb_record_get_value_data_size";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data_size(
	     record_value,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data size.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value data of the specific entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_value_data(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	libfvalue_value_t *record_value             = NULL;
	static char *function                       = "libesedb_record_get_value_data";

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_copy_data(
	     record_value,
	     value_data,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
		 "%s: unable to copy value: %d data.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the boolean value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_boolean(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_boolean,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_boolean";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_BOOLEAN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_boolean(
		     record_value,
		     0,
		     value_boolean,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to boolean value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 8-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_8bit(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value_8bit,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_8bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_8bit(
		     record_value,
		     0,
		     value_8bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to 8-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 16-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_16bit(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *value_16bit,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_16bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_16bit(
		     record_value,
		     0,
		     value_16bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to 16-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 32-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_32bit(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_32bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_32bit(
		     record_value,
		     0,
		     value_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to 32-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_64bit(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_64bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_CURRENCY )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_DATE_TIME )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_64bit(
		     record_value,
		     0,
		     value_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to 64-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit filetime value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_filetime(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value_filetime,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_filetime";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_DATE_TIME )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		/* Copy the filetime to a 64-bit value
		 */
		if( libfvalue_value_copy_to_64bit(
		     record_value,
		     0,
		     value_filetime,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to 64-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the single precision floating point value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_floating_point_32bit(
     libesedb_record_t *record,
     int value_entry,
     float *value_floating_point_32bit,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_floating_point_32bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_FLOAT_32BIT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_float(
		     record_value,
		     0,
		     value_floating_point_32bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to single precision floating point value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the double precision floating point value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_floating_point_64bit(
     libesedb_record_t *record,
     int value_entry,
     double *value_floating_point_64bit,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_floating_point_64bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_double(
		     record_value,
		     0,
		     value_floating_point_64bit,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to double precision floating point value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of an UTF-8 string a specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf8_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf8_string_size";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libesedb_record_value_get_utf8_string_size(
	          record_value,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size from value: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-8 encoded string of a specific entry
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf8_string(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf8_string";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libesedb_record_value_get_utf8_string(
	          record_value,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string from value: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of an UTF-16 string a specific entry
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf16_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf16_string_size";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libesedb_record_value_get_utf16_string_size(
	          record_value,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size from value: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( result );
}

/* Retrieves the UTF-16 encoded string value of a specific entry
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf16_string(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf16_string";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libesedb_record_value_get_utf16_string(
	          record_value,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string from value: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( result );
}

/* Retrieves the binary data size of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_binary_data_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *binary_data_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t *entry_data                                      = NULL;
	uint8_t *value_data                                      = NULL;
	static char *function                                    = "libesedb_record_get_value_binary_data_size";
	size_t entry_data_size                                   = 0;
	uint32_t column_type                                     = 0;
	uint32_t data_flags                                      = 0;
	int encoding                                             = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data_flags(
		     record_value,
		     &data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d data flags.",
			 function,
			 value_entry );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_COMPRESSED ) != 0 )
		{
			if( libfvalue_value_get_entry_data(
			     record_value,
			     0,
			     &entry_data,
			     &entry_data_size,
			     &encoding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record value entry data.",
				 function );

				return( -1 );
			}
			result = libesedb_compression_decompress_get_size(
			          entry_data,
			          entry_data_size,
			          binary_data_size,
			          error );
		}
		else
		{
			 result = libfvalue_value_get_data(
			           record_value,
			           &value_data,
			           binary_data_size,
			           &encoding,
			           error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve value data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the binary data value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_binary_data(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *binary_data,
     size_t binary_data_size,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t *entry_data                                      = NULL;
	static char *function                                    = "libesedb_record_get_value_binary_data";
	size_t entry_data_size                                   = 0;
	uint32_t column_type                                     = 0;
	uint32_t data_flags                                      = 0;
	int encoding                                             = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data_flags(
		     record_value,
		     &data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d data flags.",
			 function,
			 value_entry );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_COMPRESSED ) != 0 )
		{
			if( libfvalue_value_get_entry_data(
			     record_value,
			     0,
			     &entry_data,
			     &entry_data_size,
			     &encoding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record value entry data.",
				 function );

				return( -1 );
			}
			result = libesedb_compression_decompress(
			          entry_data,
			          entry_data_size,
			          binary_data,
			          binary_data_size,
			          error );
		}
		else
		{
			result = libfvalue_value_copy_data(
			          record_value,
			          binary_data,
			          binary_data_size,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable copy value data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Determines if a specific entry is a long value
 * Returns 1 if true, 0 if not or -1 on error
 */
int libesedb_record_is_long_value(
     libesedb_record_t *record,
     int value_entry,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_is_long_value";
	libfvalue_value_t *record_value             = NULL;
	uint32_t data_flags                         = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data_flags(
	     record_value,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( ( data_flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) != 0 );
}

/* Determines if a specific entry is a multi value
 * Returns 1 if true, 0 if not or -1 on error
 */
int libesedb_record_is_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libcerror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_is_multi_value";
	libfvalue_value_t *record_value             = NULL;
	uint32_t data_flags                         = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data_flags(
	     record_value,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 value_entry );

		return( -1 );
	}
	return( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 );
}

/* Retrieves the long value data segments list of a specific entry
 * Creates a new data segments list
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libesedb_record_get_long_value_data_segments_list(
     libesedb_internal_record_t *internal_record,
     const uint8_t *long_value_key,
     size_t long_value_key_size,
     libfdata_list_t **data_segments_list,
     libcerror_error_t **error )
{
	uint8_t long_value_segment_key[ 8 ];

	libesedb_data_definition_t *data_definition = NULL;
	libesedb_page_tree_key_t *key               = NULL;
	static char *function                       = "libesedb_record_get_long_value_data_segments_list";
	uint32_t long_value_segment_offset          = 0;
	int result                                  = 0;

	if( internal_record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( long_value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value key.",
		 function );

		return( -1 );
	}
	if( long_value_key_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupport long value key size: %" PRIzd ".",
		 function,
		 long_value_key_size );

		return( -1 );
	}
	if( data_segments_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data segments list.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_key_initialize(
	     &key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key.",
		 function );

		goto on_error;
	}
	if( libesedb_page_tree_key_set_data(
	     key,
	     long_value_key,
	     long_value_key_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set long value key data in key.",
		 function );

		goto on_error;
	}
	key->type = LIBESEDB_KEY_TYPE_LONG_VALUE;

	result = libesedb_page_tree_get_leaf_value_by_key(
	          internal_record->long_values_page_tree,
	          internal_record->file_io_handle,
	          key,
	          &data_definition,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value by key.",
		 function );

		goto on_error;
	}
	if( libesedb_page_tree_key_free(
	     &key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free key.",
		 function );

		goto on_error;
	}
	if( result == 0 )
	{
		return( 0 );
	}
	if( libfdata_list_initialize(
	     data_segments_list,
	     NULL,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libesedb_data_segment_read_element_data,
	     NULL,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data segments list.",
		 function );

		goto on_error;
	}
	if( libesedb_data_definition_read_long_value(
	     data_definition,
	     internal_record->file_io_handle,
	     internal_record->long_values_pages_vector,
	     internal_record->long_values_pages_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data definition long value.",
		 function );

		goto on_error;
	}
	if( libesedb_data_definition_free(
	     &data_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free data definition.",
		 function );

		goto on_error;
	}
	/* Reverse the reversed long value key
	 */
	long_value_segment_key[ 0 ] = long_value_key[ 3 ];
	long_value_segment_key[ 1 ] = long_value_key[ 2 ];
	long_value_segment_key[ 2 ] = long_value_key[ 1 ];
	long_value_segment_key[ 3 ] = long_value_key[ 0 ];

	do
	{
		byte_stream_copy_from_uint32_big_endian(
		 &( long_value_segment_key[ 4 ] ),
		 long_value_segment_offset );

		if( libesedb_page_tree_key_initialize(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key.",
			 function );

			goto on_error;
		}
		if( libesedb_page_tree_key_set_data(
		     key,
		     long_value_segment_key,
		     8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set long value segment key data in key.",
			 function );

			goto on_error;
		}
		key->type = LIBESEDB_KEY_TYPE_LONG_VALUE_SEGMENT;

		result = libesedb_page_tree_get_leaf_value_by_key(
		          internal_record->long_values_page_tree,
		          internal_record->file_io_handle,
		          key,
		          &data_definition,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf value by key.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libesedb_data_definition_read_long_value_segment(
			     data_definition,
			     internal_record->file_io_handle,
			     internal_record->io_handle,
			     internal_record->long_values_pages_vector,
			     internal_record->long_values_pages_cache,
			     long_value_segment_offset,
			     *data_segments_list,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read data definition long value segment.",
				 function );

				goto on_error;
			}
			long_value_segment_offset += data_definition->data_size;

			if( libesedb_data_definition_free(
			     &data_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data definition.",
				 function );

				goto on_error;
			}
		}
		if( libesedb_page_tree_key_free(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			goto on_error;
		}
	}
	while( result == 1 );

	return( 1 );

on_error:
	if( *data_segments_list != NULL )
	{
		libfdata_list_free(
		 data_segments_list,
		 NULL );
	}
	if( data_definition != NULL )
	{
		libesedb_data_definition_free(
		 &data_definition,
		 NULL );
	}
	if( key != NULL )
	{
		libesedb_page_tree_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the long value of a specific entry
 * Creates a new long value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libesedb_record_get_long_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_long_value_t **long_value,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfdata_list_t *data_segments_list                      = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t* value_data                                      = NULL;
	static char *function                                    = "libesedb_record_get_long_value";
	size_t value_data_size                                   = 0;
	uint32_t data_flags                                      = 0;
	int encoding                                             = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	if( *long_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: long value already set.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		goto on_error;
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		goto on_error;
	}
	if( libfvalue_value_get_data_flags(
	     record_value,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 value_entry );

		goto on_error;
	}
	if( ( data_flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data flags: 0x%02" PRIx32 ".",
		 function,
		 data_flags );

		goto on_error;
	}
	if( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data flags: 0x%02" PRIx32 ".",
		 function,
		 data_flags );

		goto on_error;
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libfvalue_value_get_data(
	     record_value,
	     &value_data,
	     &value_data_size,
	     &encoding,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve value data.",
		 function );

		goto on_error;
	}
	result = libesedb_record_get_long_value_data_segments_list(
		  internal_record,
		  value_data,
		  value_data_size,
		  &data_segments_list,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable retrieve value data.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		return( 0 );
	}
	if( libesedb_long_value_initialize(
	     long_value,
	     internal_record->file_io_handle,
	     internal_record->io_handle,
	     column_catalog_definition,
	     data_segments_list,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create long value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_segments_list != NULL )
	{
		libfdata_list_free(
		 &data_segments_list,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the multi value of a specific entry
 * Creates a new multi value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libesedb_record_get_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_multi_value_t **multi_value,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t *value_data                                      = NULL;
	static char *function                                    = "libesedb_record_get_multi_value";
	size_t value_data_offset                                 = 0;
	size_t value_data_size                                   = 0;
	uint32_t data_flags                                      = 0;
	uint16_t number_of_value_entries                         = 0;
	uint16_t value_16bit                                     = 0;
	uint16_t value_entry_offset                              = 0;
	uint16_t value_entry_offset_index                        = 0;
	uint16_t value_entry_size                                = 0;
	int encoding                                             = 0;
	int result                                               = 0;
	int value_entry_index                                    = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( multi_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	if( *multi_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: multi value already set.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( column_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data_flags(
	     record_value,
	     &data_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data flags: 0x%02" PRIx32 ".",
		 function,
		 data_flags );

		return( -1 );
	}
	if( ( ( data_flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) != 0 )
	 || ( ( data_flags & 0x10 ) != 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data flags: 0x%02" PRIx32 ".",
		 function,
		 data_flags );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data(
		     record_value,
		     &value_data,
		     &value_data_size,
		     &encoding,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve value data.",
			 function );

			return( -1 );
		}
		if( value_data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			return( -1 );
		}
		if( value_data_size > (size_t) UINT16_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid value data size value out of bounds.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: multi value data:\n",
			 function );
			libcnotify_print_data(
			 value_data,
			 value_data_size,
			 0 );
		}
#endif
		/* The first 2 bytes contain the offset to the first value
		 * there is an offset for every value
		 * therefore first offset / 2 = the number of value entries
		 */
		byte_stream_copy_to_uint16_little_endian(
		 value_data,
		 value_16bit );

		value_data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: multi value offset: %03" PRIu16 "\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 value_entry_offset_index,
			 value_16bit,
			 value_16bit & 0x7fff );
		}
#endif
		value_entry_offset = value_16bit & 0x7fff;

		number_of_value_entries = value_entry_offset / 2;

		if( number_of_value_entries == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value entries.",
			 function );

			return( -1 );
		}
		for( value_entry_offset_index = 1;
		     value_entry_offset_index < number_of_value_entries;
		     value_entry_offset_index++ )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( value_data[ value_data_offset ] ),
			 value_16bit );

			value_data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: multi value offset: %03" PRIu16 "\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
				 function,
				 value_entry_offset_index,
				 value_16bit,
				 value_16bit & 0x7fff );
			}
#endif
			value_16bit &= 0x7fff;

			if( value_16bit < value_entry_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid value offset: %" PRIu16 " value is smaller than previous.",
				 function,
				 value_entry_offset_index );

				return( -1 );
			}
			value_entry_size = value_16bit - value_entry_offset;

			if( libfvalue_value_append_entry(
			     record_value,
			     &value_entry_index,
			     (size_t) value_entry_offset,
			     (size_t) value_entry_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable set value entry: %" PRIu16 ".",
				 function,
				 value_entry_offset_index - 1 );

				return( -1 );
			}
			value_entry_offset = value_16bit;
		}
		value_entry_size = (uint16_t) value_data_size - value_entry_offset;

		if( libfvalue_value_append_entry(
		     record_value,
		     &value_entry_index,
		     (size_t) value_entry_offset,
		     (size_t) value_entry_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set value entry: %" PRIu16 ".",
			 function,
			 value_entry_offset_index - 1 );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libesedb_multi_value_initialize(
		     multi_value,
		     column_catalog_definition,
		     record_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create multi value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

