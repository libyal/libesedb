/*
 * Table functions
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

#include "libesedb_column.h"
#include "libesedb_definitions.h"
#include "libesedb_index.h"
#include "libesedb_file.h"
#include "libesedb_list_type.h"
#include "libesedb_page_tree.h"
#include "libesedb_record.h"
#include "libesedb_table.h"
#include "libesedb_table_definition.h"
#include "libesedb_types.h"

/* Creates a table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_initialize(
     libesedb_table_t **table,
     libbfio_handle_t *file_io_handle,
     libesedb_internal_file_t *internal_file,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_initialize";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
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
	if( *table == NULL )
	{
		internal_table = (libesedb_internal_table_t *) memory_allocate(
		                                                sizeof( libesedb_internal_table_t ) );

		if( internal_table == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create table.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_table,
		     0,
		     sizeof( libesedb_internal_table_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear table.",
			 function );

			memory_free(
			 internal_table );

			return( -1 );
		}
		if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) == 0 )
		{
			internal_table->file_io_handle = file_io_handle;
		}
		else
		{
			if( libbfio_handle_clone(
			     &( internal_table->file_io_handle ),
			     file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy file io handle.",
				 function );

				memory_free(
				 internal_table );

				return( -1 );
			}
			if( libbfio_handle_set_open_on_demand(
			     internal_table->file_io_handle,
			     1,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to set open on demand in file io handle.",
				 function );

				libbfio_handle_free(
				 &( internal_table->file_io_handle ),
				 NULL );
				memory_free(
				 internal_table );

				return( -1 );
			}
		}
		internal_table->internal_file             = internal_file;
		internal_table->table_definition          = table_definition;
		internal_table->template_table_definition = template_table_definition;
		internal_table->flags                     = flags;

		*table = (libesedb_table_t *) internal_table;
	}
	return( 1 );
}

/* Frees table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_free(
     libesedb_table_t **table,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_free";
	int result                                = 1;

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		internal_table = (libesedb_internal_table_t *) *table;
		*table         = NULL;

		/* The internal_file and table_definition references
		 * are freed elsewhere
		 */
		if( ( internal_table->flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
		{
			if( internal_table->file_io_handle != NULL )
			{
				if( libbfio_handle_close(
				     internal_table->file_io_handle,
				     error ) != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_IO,
					 LIBERROR_IO_ERROR_CLOSE_FAILED,
					 "%s: unable to close file io handle.",
					 function );

					return( -1 );
				}
				if( libbfio_handle_free(
				     &( internal_table->file_io_handle ),
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file io handle.",
					 function );

					return( -1 );
				}
			}
		}
		if( internal_table->table_page_tree != NULL )
		{
			if( libesedb_page_tree_free(
			     &( internal_table->table_page_tree ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free table page tree.",
				 function );

				result = -1;
			}
		}
		if( internal_table->long_value_page_tree != NULL )
		{
			if( libesedb_page_tree_free(
			     &( internal_table->long_value_page_tree ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free long value page tree.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_table );
	}
	return( result );
}

/* Reads the page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_read_page_tree(
     libesedb_internal_table_t *internal_table,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_read_page_tree";

	if( internal_table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal table.",
		 function );

		return( -1 );
	}
	if( internal_table->internal_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing internal file.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->table_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_page_tree != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal table - table page tree already set.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->long_value_catalog_definition != NULL )
	{
		if( internal_table->long_value_page_tree != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: invalid internal table - long value page table already set.",
			 function );

			return( -1 );
		}
		if( libesedb_page_tree_initialize(
		     &( internal_table->long_value_page_tree ),
		     internal_table->internal_file->io_handle,
		     internal_table->table_definition,
		     internal_table->template_table_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create long value page tree.",
			 function );

			return( -1 );
		}
		if( libesedb_page_tree_read(
		     internal_table->long_value_page_tree,
		     internal_table->file_io_handle,
		     internal_table->table_definition->long_value_catalog_definition->father_data_page_number,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read long value page tree.",
			 function );

			libesedb_page_tree_free(
			 &( internal_table->long_value_page_tree ),
			 NULL );

			return( -1 );
		}
	}
	if( libesedb_page_tree_initialize(
	     &( internal_table->table_page_tree ),
	     internal_table->internal_file->io_handle,
	     internal_table->table_definition,
	     internal_table->template_table_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table page tree.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_read(
	     internal_table->table_page_tree,
	     internal_table->file_io_handle,
	     internal_table->table_definition->table_catalog_definition->father_data_page_number,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read table page tree.",
		 function );

		libesedb_page_tree_free(
		 &( internal_table->table_page_tree ),
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the table identifier or Father Data Page (FDP) object identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_identifier(
     libesedb_table_t *table,
     uint32_t *identifier,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_identifier";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->table_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = internal_table->table_definition->table_catalog_definition->identifier;

	return( 1 );
}

/* Retrieves the UTF-8 string size of the table name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_name_size";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->table_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = internal_table->table_definition->table_catalog_definition->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 string of the table name
 * The string is formatted in UTF-8
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_name";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->table_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < internal_table->table_definition->table_catalog_definition->name_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string is too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     utf8_string,
	     internal_table->table_definition->table_catalog_definition->name,
	     internal_table->table_definition->table_catalog_definition->name_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the template name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_template_name_size(
     libesedb_table_t *table,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_template_name_size";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->table_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	*utf8_string_size = internal_table->table_definition->table_catalog_definition->template_name_size;

	return( 1 );
}

/* Retrieves the UTF-8 string of the template name
 * The string is formatted in UTF-8
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_utf8_template_name(
     libesedb_table_t *table,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_utf8_template_name";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->table_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < internal_table->table_definition->table_catalog_definition->name_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string is too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     utf8_string,
	     internal_table->table_definition->table_catalog_definition->template_name,
	     internal_table->table_definition->table_catalog_definition->template_name_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of columns in the table
 *
 * Use the flag LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE to retrieve the number of columns
 * ignoring the template table
 *
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_number_of_columns(
     libesedb_table_t *table,
     int *number_of_columns,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_number_of_columns";
	int template_table_number_of_columns      = 0;

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->column_catalog_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing column catalog definition list.",
		 function );

		return( -1 );
	}
	if( number_of_columns == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of columns.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) == 0 )
	 && ( internal_table->template_table_definition != NULL ) )
	{
		if( libesedb_list_get_number_of_elements(
		     internal_table->template_table_definition->column_catalog_definition_list,
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
	if( libesedb_list_get_number_of_elements(
	     internal_table->table_definition->column_catalog_definition_list,
	     number_of_columns,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of columns from table.",
		 function );

		return( -1 );
	}
	*number_of_columns += template_table_number_of_columns;

	return( 1 );
}

/* Retrieves a specific column
 *
 * Use the flag LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE to retrieve the column
 * ignoring the template table
 *
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_column(
     libesedb_table_t *table,
     int column_entry,
     libesedb_column_t **column,
     uint8_t flags,
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_table_t *internal_table                = NULL;
	static char *function                                    = "libesedb_table_get_column";
	int template_table_number_of_columns                     = 0;

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->column_catalog_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing column catalog definition list.",
		 function );

		return( -1 );
	}
	if( column == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column.",
		 function );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE ) == 0 )
	 && ( internal_table->template_table_definition != NULL ) )
	{
		if( libesedb_list_get_number_of_elements(
		     internal_table->template_table_definition->column_catalog_definition_list,
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
	if( column_entry < template_table_number_of_columns )
	{
		if( libesedb_list_get_value(
		     internal_table->template_table_definition->column_catalog_definition_list,
		     column_entry,
		     (intptr_t **) &column_catalog_definition,
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
		if( libesedb_list_get_value(
		     internal_table->table_definition->column_catalog_definition_list,
		     column_entry - template_table_number_of_columns,
		     (intptr_t **) &column_catalog_definition,
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
	if( libesedb_column_initialize(
	     column,
	     internal_table,
	     column_catalog_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of indexes
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_number_of_indexes(
     libesedb_table_t *table,
     int *number_of_indexes,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_number_of_indexes";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_number_of_elements(
	     internal_table->table_definition->index_catalog_definition_list,
	     number_of_indexes,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of indexes.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific index
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_index(
     libesedb_table_t *table,
     int index_entry,
     libesedb_index_t **index,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table               = NULL;
	libesedb_catalog_definition_t *index_catalog_definition = NULL;
	static char *function                                   = "libesedb_table_get_index";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing table definition.",
		 function );

		return( -1 );
	}
	if( internal_table->table_definition->index_catalog_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing index catalog definition list.",
		 function );

		return( -1 );
	}
	if( index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_value(
	     internal_table->table_definition->index_catalog_definition_list,
	     index_entry,
	     (intptr_t **) &index_catalog_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_index_initialize(
	     index,
	     internal_table,
	     internal_table->internal_file,
	     index_catalog_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of records in the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_number_of_records(
     libesedb_table_t *table,
     int *number_of_records,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_number_of_records";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_page_tree == NULL )
	{
		if( libesedb_table_read_page_tree(
		     internal_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read table page tree.",
			 function );

			return( -1 );
		}
		if( internal_table->table_page_tree == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid internal table - missing table page tree.",
			 function );

			return( -1 );
		}
	}
	if( libesedb_page_tree_get_number_of_value_definitions(
	     internal_table->table_page_tree,
	     number_of_records,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of records.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific record
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_record(
     libesedb_table_t *table,
     int record_entry,
     libesedb_record_t **record,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table          = NULL;
	libesedb_data_definition_t *record_data_definition = NULL;
	static char *function                              = "libesedb_table_get_record";

	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	internal_table = (libesedb_internal_table_t *) table;

	if( internal_table->table_page_tree == NULL )
	{
		if( libesedb_table_read_page_tree(
		     internal_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read table page tree.",
			 function );

			return( -1 );
		}
		if( internal_table->table_page_tree == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid internal table - missing table page tree.",
			 function );

			return( -1 );
		}
	}
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
	if( libesedb_page_tree_get_value_definition(
	     internal_table->table_page_tree,
	     record_entry,
	     &record_data_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record data definition: %d.",
		 function,
		 record_entry );

		return( -1 );
	}
	if( libesedb_record_initialize(
	     record,
	     internal_table->file_io_handle,
	     internal_table,
	     record_data_definition,
             LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create record.",
		 function );

		return( -1 );
	}
	return( 1 );
}

