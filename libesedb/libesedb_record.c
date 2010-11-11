/*
 * Record (row) functions
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libfdata.h"
#include "libesedb_libfvalue.h"
#include "libesedb_long_value.h"
#include "libesedb_multi_value.h"
#include "libesedb_record.h"
#include "libesedb_table_definition.h"
#include "libesedb_types.h"
#include "libesedb_values_tree_value.h"

/* Creates a record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_initialize(
     libesedb_record_t **record,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     libfdata_tree_node_t *values_tree_node,
     libfdata_cache_t *values_cache,
     libfdata_tree_t *long_values_tree,
     libfdata_cache_t *long_values_cache,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record     = NULL;
	libesedb_values_tree_value_t *values_tree_value = NULL;
	static char *function                           = "libesedb_record_initialize";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( table_definition->table_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	if( *record == NULL )
	{
		internal_record = (libesedb_internal_record_t *) memory_allocate(
		                                                  sizeof( libesedb_internal_record_t ) );

		if( internal_record == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create record.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_record,
		     0,
		     sizeof( libesedb_internal_record_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear record.",
			 function );

			memory_free(
			 internal_record );

			return( -1 );
		}
		if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) == 0 )
		{
			internal_record->file_io_handle = file_io_handle;
		}
		else
		{
			if( libbfio_handle_clone(
			     &( internal_record->file_io_handle ),
			     file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy file IO handle.",
				 function );

				memory_free(
				 internal_record );

				return( -1 );
			}
			if( libbfio_handle_set_open_on_demand(
			     internal_record->file_io_handle,
			     1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to set open on demand in file IO handle.",
				 function );

				libbfio_handle_free(
				 &( internal_record->file_io_handle ),
				 NULL );
				memory_free(
				 internal_record );

				return( -1 );
			}
		}
		if( libesedb_array_initialize(
		     &( internal_record->values_array ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create values array.",
			 function );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_record->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_record );

			return( -1 );
		}
		if( libfdata_tree_node_get_node_value(
		     values_tree_node,
		     internal_record->file_io_handle,
		     values_cache,
		     (intptr_t **) &values_tree_value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node value from values tree node.",
			 function );

			libesedb_array_free(
			 &( internal_record->values_array ),
			 NULL,
			 NULL );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_record->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_record );

			return( -1 );
		}
		if( libesedb_values_tree_value_read_record(
		     values_tree_value,
		     internal_record->file_io_handle,
		     io_handle,
		     pages_vector,
		     pages_cache,
		     table_definition,
		     template_table_definition,
		     internal_record->values_array,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values tree value record.",
			 function );

			libesedb_array_free(
			 &( internal_record->values_array ),
			 NULL,
			 NULL );

			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_record->file_io_handle ),
				 NULL );
			}
			memory_free(
			 internal_record );

			return( -1 );
		}
		internal_record->io_handle                 = io_handle;
		internal_record->table_definition          = table_definition;
		internal_record->template_table_definition = template_table_definition;
		internal_record->pages_vector              = pages_vector;
		internal_record->pages_cache               = pages_cache;
		internal_record->long_values_tree          = long_values_tree;
		internal_record->long_values_cache         = long_values_cache;
		internal_record->flags                     = flags;

		*record = (libesedb_record_t *) internal_record;
	}
	return( 1 );
}

/* Frees record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_free(
     libesedb_record_t **record,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_free";
	int result                                  = 1;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		internal_record = (libesedb_internal_record_t *) *record;
		*record         = NULL;

		/* The io_handle, table_definition, template_table_definition, pages_vector, pages_cache,
		 * long_values_tree and long_values_cache references are freed elsewhere
		 */
		if( ( internal_record->flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
		{
			if( internal_record->file_io_handle != NULL )
			{
				if( libbfio_handle_close(
				     internal_record->file_io_handle,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_CLOSE_FAILED,
					 "%s: unable to close file IO handle.",
					 function );

					result = -1;
				}
				if( libbfio_handle_free(
				     &( internal_record->file_io_handle ),
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file IO handle.",
					 function );

					result = -1;
				}
			}
		}
		if( libesedb_array_free(
		     &( internal_record->values_array ),
		     &libfvalue_value_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
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
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_get_number_of_values";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_array_get_number_of_entries(
	     internal_record->values_array,
	     number_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the column catalog definition of the specific entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_column_catalog_definition(
     libesedb_internal_record_t *internal_record,
     int value_entry,
     libesedb_catalog_definition_t **column_catalog_definition,
     liberror_error_t **error )
{
	static char *function                = "libesedb_record_get_column_identifier";
	int template_table_number_of_columns = 0;

	if( internal_record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal record.",
		 function );

		return( -1 );
	}
	if( internal_record->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_record->template_table_definition != NULL )
	{
		if( libesedb_list_get_number_of_elements(
		     internal_record->template_table_definition->column_catalog_definition_list,
		     &template_table_number_of_columns,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of columns from template table.",
			 function );

			return( -1 );
		}
	}
	if( value_entry < template_table_number_of_columns )
	{
		if( libesedb_list_get_value_by_index(
		     internal_record->template_table_definition->column_catalog_definition_list,
		     value_entry,
		     (intptr_t **) column_catalog_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve column catalog definition from template table.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libesedb_list_get_value_by_index(
		     internal_record->table_definition->column_catalog_definition_list,
		     value_entry - template_table_number_of_columns,
		     (intptr_t **) column_catalog_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_column_identifier";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_identifier(
	     column_catalog_definition,
	     column_identifier,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_column_type";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf8_column_name_size";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf8_column_name";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf16_column_name_size";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	static char *function                                    = "libesedb_record_get_utf16_column_name";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
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
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	libfvalue_value_t *record_value             = NULL;
	uint8_t *value_metadata                     = NULL;
	static char *function                       = "libesedb_record_get_value";
	size_t value_metadata_size                  = 0;
	uint8_t value_byte_order                    = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( value_flags == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value flags.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_data(
	     record_value,
	     value_data,
	     value_data_size,
	     &value_byte_order,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	/* The metadata contains the value flags
	 */
	if( libfvalue_value_get_metadata(
	     record_value,
	     &value_metadata,
	     &value_metadata_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value metadata: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( value_metadata != NULL )
	{
		*value_flags = *value_metadata;
	}
	else
	{
		*value_flags = 0;
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_boolean";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_BOOLEAN )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_8bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_16bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_32bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_64bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_CURRENCY )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_filetime";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_DATE_TIME )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_floating_point_32bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_FLOAT_32BIT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_floating_point_64bit";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( column_type != LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf8_string_size";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf8_string_size(
		     record_value,
		     0,
		     utf8_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve UTF-8 string size.",
			 function );

			return( -1 );
		}
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf8_string";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf8_string(
		     record_value,
		     0,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-8 string.",
			 function );

			return( -1 );
		}
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf16_string_size";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_utf16_string_size(
		     record_value,
		     0,
		     utf16_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_utf16_string";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_utf16_string(
		     record_value,
		     0,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-16 string.",
			 function );

			return( -1 );
		}
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t *value_data                                      = NULL;
	static char *function                                    = "libesedb_record_get_value_binary_data_size";
	uint32_t column_type                                     = 0;
	uint8_t value_byte_order                                 = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		     binary_data_size,
		     &value_byte_order,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_binary_data";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_data(
		     record_value,
		     binary_data,
		     binary_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable copy value data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the long value of a specific entry
 * Creates a new long value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libesedb_record_get_long_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_long_value_t **long_value,
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t* value_data                                      = NULL;
	uint8_t* value_metadata                                  = NULL;
	static char *function                                    = "libesedb_record_get_long_value";
	size_t value_data_size                                   = 0;
	size_t value_metadata_size                               = 0;
	uint8_t value_byte_order                                 = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( long_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	if( *long_value != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_metadata(
	     record_value,
	     &value_metadata,
	     &value_metadata_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value metadata: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( value_metadata == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value metadata.",
		 function );

		return( -1 );
	}
	/* The metadata contains the value flags
	 */
	if( ( *value_metadata & LIBESEDB_VALUE_FLAG_LONG_VALUE ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value flags: 0x%02" PRIx8 ".",
		 function,
		 *value_metadata );

		return( -1 );
	}
	if( ( *value_metadata & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value flags: 0x%02" PRIx8 ".",
		 function,
		 *value_metadata );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		     &value_byte_order,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve value data.",
			 function );

			return( -1 );
		}
		if( libesedb_long_value_initialize(
		     long_value,
		     internal_record->file_io_handle,
		     internal_record->io_handle,
		     column_catalog_definition,
		     internal_record->pages_vector,
		     internal_record->pages_cache,
		     internal_record->long_values_tree,
		     internal_record->long_values_cache,
		     value_data,
		     value_data_size,
		     LIBESEDB_ITEM_FLAGS_DEFAULT,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create long value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the multi value of a specific entry
 * Creates a new multi value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libesedb_record_get_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_multi_value_t **multi_value,
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t *value_data                                      = NULL;
	uint8_t* value_metadata                                  = NULL;
	static char *function                                    = "libesedb_record_get_multi_value";
	size_t value_data_size                                   = 0;
	size_t value_metadata_size                               = 0;
	uint16_t number_of_value_entries                         = 0;
	uint16_t value_16bit                                     = 0;
	uint16_t value_entry_offset                              = 0;
	uint16_t value_entry_offset_index                        = 0;
	uint16_t value_entry_size                                = 0;
	uint8_t value_byte_order                                 = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( multi_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	if( *multi_value != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( libfvalue_value_get_metadata(
	     record_value,
	     &value_metadata,
	     &value_metadata_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value metadata: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( value_metadata == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing value metadata.",
		 function );

		return( -1 );
	}
	/* The metadata contains the value flags
	 */
	if( ( *value_metadata & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value flags: 0x%02" PRIx8 ".",
		 function,
		 *value_metadata );

		return( -1 );
	}
	if( ( ( *value_metadata & LIBESEDB_VALUE_FLAG_LONG_VALUE ) != 0 )
	 || ( ( *value_metadata & 0x10 ) != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported value flags: 0x%02" PRIx8 ".",
		 function,
		 *value_metadata );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
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
		     &value_byte_order,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve value data.",
			 function );

			return( -1 );
		}
		if( value_data == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value data.",
			 function );

			return( -1 );
		}
		if( value_data_size > (size_t) UINT16_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: value data size exceeds maximum.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: multi value data:\n",
			 function );
			libnotify_print_data(
			 value_data,
			 value_data_size );
		}
#endif
		/* The first 2 bytes contain the offset to the first value
		 * there is an offset for every value
		 * therefore first offset / 2 = the number of value entries
		 */
		byte_stream_copy_to_uint16_little_endian(
		 value_data,
		 value_16bit );

		value_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: multi value offset: %03" PRIu16 "\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value entries.",
			 function );

			return( -1 );
		}
		if( libfvalue_value_resize_value_entries(
		     record_value,
		     (int) number_of_value_entries,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
			 "%s: unable resize value entries.",
			 function );

			return( -1 );
		}
		for( value_entry_offset_index = 1;
		     value_entry_offset_index < number_of_value_entries;
		     value_entry_offset_index++ )
		{
			byte_stream_copy_to_uint16_little_endian(
			 value_data,
			 value_16bit );

			value_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: multi value offset: %03" PRIu16 "\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
				 function,
				 value_entry_offset_index,
				 value_16bit,
				 value_16bit & 0x7fff );
			}
#endif
			value_16bit &= 0x7fff;

			if( value_16bit < value_entry_offset )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid value offset: %" PRIu16 " value is smaller than previous.",
				 function,
				 value_entry_offset_index );

				return( -1 );
			}
			value_entry_size = value_16bit - value_entry_offset;

			if( libfvalue_value_set_value_entry(
			     record_value,
			     (int) ( value_entry_offset_index - 1 ),
			     (size_t) value_entry_offset,
			     (size_t) value_entry_size,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable set value entry: %" PRIu16 ".",
				 function,
				 value_entry_offset_index - 1 );

				return( -1 );
			}
			value_entry_offset = value_16bit;
		}
		value_entry_size = (uint16_t) value_data_size - value_entry_offset;

		if( libfvalue_value_set_value_entry(
		     record_value,
		     (int) ( value_entry_offset_index - 1 ),
		     (size_t) value_entry_offset,
		     (size_t) value_entry_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable set value entry: %" PRIu16 ".",
			 function,
			 value_entry_offset_index - 1 );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "\n" );
		}
#endif
		if( libesedb_multi_value_initialize(
		     multi_value,
		     column_catalog_definition,
		     record_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create multi value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

