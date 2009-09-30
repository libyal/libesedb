/*
 * Table functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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
		if( libesedb_list_element_initialize(
		     &( internal_table->list_element ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create list element.",
			 function );

			memory_free(
			 internal_table );

			return( -1 );
		}
		if( libesedb_list_initialize(
		     &( internal_table->column_reference_list ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create column reference list.",
			 function );

			libesedb_list_element_free(
			 &( internal_table->list_element ),
			 NULL,
			 NULL );
			memory_free(
			 internal_table );

			return( -1 );
		}
		if( libesedb_list_initialize(
		     &( internal_table->index_reference_list ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create index reference list.",
			 function );

			libesedb_list_free(
			 &( internal_table->column_reference_list ),
			 NULL,
			 NULL );
			libesedb_list_element_free(
			 &( internal_table->list_element ),
			 NULL,
			 NULL );
			memory_free(
			 internal_table );

			return( -1 );
		}
		if( libesedb_list_initialize(
		     &( internal_table->record_reference_list ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record reference list.",
			 function );

			libesedb_list_free(
			 &( internal_table->index_reference_list ),
			 NULL,
			 NULL );
			libesedb_list_free(
			 &( internal_table->column_reference_list ),
			 NULL,
			 NULL );
			libesedb_list_element_free(
			 &( internal_table->list_element ),
			 NULL,
			 NULL );
			memory_free(
			 internal_table );

			return( -1 );
		}
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
		if( libesedb_table_detach(
		     internal_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to detach internal table.",
			 function );

			return( -1 );
		}
		if( ( internal_table->list_element != NULL )
		 && ( libesedb_list_element_free(
		       &( internal_table->list_element ),
		       NULL,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			result = -1;
		}
		if( ( internal_table->page_tree != NULL )
		 && ( libesedb_page_tree_free(
		       &( internal_table->page_tree ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page tree.",
			 function );

			result = -1;
		}
		if( libesedb_list_free(
		     &( internal_table->column_reference_list ),
		     &libesedb_column_free_no_detach,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free column reference list.",
			 function );

			result = -1;
		}
		if( libesedb_list_free(
		     &( internal_table->index_reference_list ),
		     &libesedb_index_free_no_detach,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index reference list.",
			 function );

			result = -1;
		}
		if( libesedb_list_free(
		     &( internal_table->record_reference_list ),
		     &libesedb_record_free_no_detach,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record reference list.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_table );
	}
	return( result );
}

/* Frees an table
 * Return 1 if successful or -1 on error
 */
int libesedb_table_free_no_detach(
     intptr_t *internal_table,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_free_no_detach";
	int result            = 1;

	if( internal_table != NULL )
	{
		if( ( (libesedb_internal_table_t *) internal_table )->internal_file != NULL )
		{
			if( ( ( (libesedb_internal_table_t *) internal_table )->list_element != NULL )
			 && ( libesedb_list_element_free(
			       &( ( (libesedb_internal_table_t *) internal_table )->list_element ),
			       NULL,
			       error ) != 1 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free list element.",
				 function );

				result = -1;
			}
		}
		/* The internal_file and table_definition references
		 * are freed elsewhere
		 */
		if( ( ( (libesedb_internal_table_t *) internal_table )->page_tree != NULL )
		 && ( libesedb_page_tree_free(
		       &( ( (libesedb_internal_table_t *) internal_table )->page_tree ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page tree.",
			 function );

			result = -1;
		}
		if( libesedb_list_free(
		     &( ( (libesedb_internal_table_t *) internal_table )->column_reference_list ),
		     &libesedb_column_free_no_detach,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free column reference list.",
			 function );

			result = -1;
		}
		if( libesedb_list_free(
		     &( ( (libesedb_internal_table_t *) internal_table )->index_reference_list ),
		     &libesedb_index_free_no_detach,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index reference list.",
			 function );

			result = -1;
		}
		if( libesedb_list_free(
		     &( ( (libesedb_internal_table_t *) internal_table )->record_reference_list ),
		     &libesedb_record_free_no_detach,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free record reference list.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_table );
	}
	return( result );
}

/* Attaches the table to the file
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_attach(
     libesedb_internal_table_t *internal_table,
     libesedb_internal_file_t *internal_file,
     libesedb_table_definition_t *table_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_attach";

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
	if( internal_table->list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing list element.",
		 function );

		return( -1 );
	}
	if( internal_table->internal_file != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal table - already attached to file.",
		 function );

		return( -1 );
	}
	/* Add table to file
	 */
	if( libesedb_list_append_element(
	     internal_file->table_reference_list,
	     internal_table->list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append internal table to file.",
		 function );

		return( -1 );
	}
	internal_table->internal_file    = internal_file;
	internal_table->table_definition = table_definition;

	return( 1 );
}

/* Detaches the table from its file reference
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_detach(
     libesedb_internal_table_t *internal_table,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_detach";

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
	if( internal_table->list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - missing list element.",
		 function );

		return( -1 );
	}
	if( internal_table->internal_file != NULL )
	{
		/* Remove table from file
		 */
		if( libesedb_list_remove_element(
		     internal_table->internal_file->table_reference_list,
		     internal_table->list_element,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to remove internal table from file.",
			 function );

			return( -1 );
		}
		internal_table->internal_file    = NULL;
		internal_table->table_definition = NULL;
	}
	return( 1 );
}

/* Reads the page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_read_page_tree(
     libesedb_internal_table_t *internal_table,
     liberror_error_t **error )
{
	static char *function = "libesedb_table_detach";

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
	if( internal_table->table_definition->table_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table data definition.",
		 function );

		return( -1 );
	}
	if( internal_table->page_tree != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal table - page table already set.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_initialize(
	     &( internal_table->page_tree ),
	     internal_table->table_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create page tree.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_read(
	     internal_table->page_tree,
	     internal_table->internal_file->io_handle,
	     internal_table->table_definition->table_data_definition->father_data_page_number,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page tree.",
		 function );

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
	if( internal_table->table_definition->table_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table data definition.",
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
	*identifier = internal_table->table_definition->table_data_definition->identifier;

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
	static char *function                     = "libesedb_table_get_utf8_string_size";

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
	if( internal_table->table_definition->table_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table data definition.",
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
	*utf8_string_size = internal_table->table_definition->table_data_definition->name_size;

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
	static char *function                     = "libesedb_table_get_utf8_string";

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
	if( internal_table->table_definition->table_data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing table data definition.",
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
	if( utf8_string_size < internal_table->table_definition->table_data_definition->name_size )
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
	     internal_table->table_definition->table_data_definition->name,
	     internal_table->table_definition->table_data_definition->name_size ) == NULL )
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

/* Retrieves the amount of columns
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_columns(
     libesedb_table_t *table,
     int *amount_of_columns,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_amount_of_columns";

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
	if( internal_table->table_definition->column_data_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing column data definition list.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_amount_of_elements(
	     internal_table->table_definition->column_data_definition_list,
	     amount_of_columns,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of columns.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific column
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_column(
     libesedb_table_t *table,
     int column_entry,
     libesedb_column_t **column,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	libesedb_list_element_t *list_element     = NULL;
	static char *function                     = "libesedb_table_get_column";

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
	if( internal_table->table_definition->column_data_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing column data definition list.",
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
	if( libesedb_list_get_element(
	     internal_table->table_definition->column_data_definition_list,
	     column_entry,
	     &list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column data definition list element.",
		 function );

		return( -1 );
	}
	if( list_element->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid column data definition list element - missing value.",
		 function );

		return( -1 );
	}
	if( libesedb_column_initialize(
	     column,
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
	if( libesedb_column_attach(
	     (libesedb_internal_column_t *) *column,
	     internal_table,
	     (libesedb_data_definition_t *) list_element->value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to attach column.",
		 function );

		libesedb_column_free(
		 column,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of indexes
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_indexes(
     libesedb_table_t *table,
     int *amount_of_indexes,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_amount_of_indexes";

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
	if( internal_table->table_definition->index_data_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing index data definition list.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_amount_of_elements(
	     internal_table->table_definition->index_data_definition_list,
	     amount_of_indexes,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of indexes.",
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
	libesedb_internal_table_t *internal_table = NULL;
	libesedb_list_element_t *list_element     = NULL;
	static char *function                     = "libesedb_table_get_index";

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
	if( internal_table->table_definition->index_data_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid table definition - missing index data definition list.",
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
	if( libesedb_list_get_element(
	     internal_table->table_definition->index_data_definition_list,
	     index_entry,
	     &list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve index data definition list element.",
		 function );

		return( -1 );
	}
	if( list_element->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid index data definition list element - missing value.",
		 function );

		return( -1 );
	}
	if( libesedb_index_initialize(
	     index,
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
	if( libesedb_index_attach(
	     (libesedb_internal_index_t *) *index,
	     internal_table,
	     (libesedb_data_definition_t *) list_element->value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to attach index.",
		 function );

		libesedb_index_free(
		 index,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the amount of records
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_records(
     libesedb_table_t *table,
     int *amount_of_records,
     liberror_error_t **error )
{
	libesedb_internal_table_t *internal_table = NULL;
	static char *function                     = "libesedb_table_get_amount_of_records";

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
	if( internal_table->page_tree == NULL )
	{
		if( libesedb_table_read_page_tree(
		     internal_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page tree.",
			 function );

			return( -1 );
		}
		if( internal_table->page_tree == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid internal table - missing page tree.",
			 function );

			return( -1 );
		}
	}
	if( internal_table->page_tree->value_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid page tree - missing value definition list.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_amount_of_elements(
	     internal_table->page_tree->value_definition_list,
	     amount_of_records,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of records.",
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
	libesedb_internal_table_t *internal_table = NULL;
	libesedb_list_element_t *list_element     = NULL;
	static char *function                     = "libesedb_table_get_record";

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
	if( internal_table->page_tree == NULL )
	{
		if( libesedb_table_read_page_tree(
		     internal_table,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page tree.",
			 function );

			return( -1 );
		}
		if( internal_table->page_tree == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid internal table - missing page tree.",
			 function );

			return( -1 );
		}
	}
	if( internal_table->page_tree->value_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal table - invalid page tree - missing value definition list.",
		 function );

		return( -1 );
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
	if( libesedb_list_get_element(
	     internal_table->page_tree->value_definition_list,
	     record_entry,
	     &list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value definition list element.",
		 function );

		return( -1 );
	}
	if( list_element->value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value definition list element - missing value.",
		 function );

		return( -1 );
	}
	if( libesedb_record_initialize(
	     record,
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
	if( libesedb_record_attach(
	     (libesedb_internal_record_t *) *record,
	     internal_table,
	     (libesedb_data_definition_t *) list_element->value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to attach record.",
		 function );

		libesedb_record_free(
		 record,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

