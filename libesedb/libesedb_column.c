/*
 * Column functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_column.h"
#include "libesedb_table.h"
#include "libesedb_types.h"

/* Creates a column
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_initialize(
     libesedb_column_t **column,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                      = "libesedb_column_initialize";

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
	if( *column == NULL )
	{
		internal_column = (libesedb_internal_column_t *) memory_allocate(
		                                                  sizeof( libesedb_internal_column_t ) );

		if( internal_column == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create column.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_column,
		     0,
		     sizeof( libesedb_internal_column_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear column.",
			 function );

			memory_free(
			 internal_column );

			return( -1 );
		}
		*column = (libesedb_column_t *) internal_column;
	}
	return( 1 );
}

/* Frees column
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_free(
     libesedb_column_t **column,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_free";
	int result                                  = 1;

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
	if( *column != NULL )
	{
		internal_column = (libesedb_internal_column_t *) *column;
		*column         = NULL;

		/* The internal_table and catalog_definition references
		 * are freed elsewhere
		 */
		if( libesedb_column_detach(
		     internal_column,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to detach internal column.",
			 function );

			return( -1 );
		}
		memory_free(
		 internal_column );
	}
	return( result );
}

/* Attaches the column to the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_attach(
     libesedb_internal_column_t *internal_column,
     libesedb_internal_table_t *internal_table,
     libesedb_catalog_definition_t *catalog_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_column_attach";

	if( internal_column == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal column.",
		 function );

		return( -1 );
	}
	if( internal_column->internal_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal column - already attached to table.",
		 function );

		return( -1 );
	}
	internal_column->internal_table     = internal_table;
	internal_column->catalog_definition = catalog_definition;

	return( 1 );
}

/* Detaches the column from its table reference
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_detach(
     libesedb_internal_column_t *internal_column,
     liberror_error_t **error )
{
	static char *function = "libesedb_column_detach";

	if( internal_column == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal column.",
		 function );

		return( -1 );
	}
	if( internal_column->internal_table != NULL )
	{
		internal_column->internal_table     = NULL;
		internal_column->catalog_definition = NULL;
	}
	return( 1 );
}

/* Retrieves the column identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_get_identifier(
     libesedb_column_t *column,
     uint32_t *identifier,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_get_identifier";

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
	internal_column = (libesedb_internal_column_t *) column;

	if( internal_column->catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing catalog definition.",
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
	*identifier = internal_column->catalog_definition->identifier;

	return( 1 );
}

/* Retrieves the column type
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_get_type(
     libesedb_column_t *column,
     uint32_t *type,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_get_type";

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
	internal_column = (libesedb_internal_column_t *) column;

	if( internal_column->catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing catalog definition.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = internal_column->catalog_definition->column_type;

	return( 1 );
}

/* Retrieves the UTF-8 string size of the column name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_get_utf8_name_size(
     libesedb_column_t *column,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_get_utf8_string_size";

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
	internal_column = (libesedb_internal_column_t *) column;

	if( internal_column->catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing catalog definition.",
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
	*utf8_string_size = internal_column->catalog_definition->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 string of the column name
 * The string is formatted in UTF-8
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_get_utf8_name(
     libesedb_column_t *column,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_get_utf8_string";

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
	internal_column = (libesedb_internal_column_t *) column;

	if( internal_column->catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing catalog definition.",
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
	if( utf8_string_size < internal_column->catalog_definition->name_size )
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
	     internal_column->catalog_definition->name,
	     internal_column->catalog_definition->name_size ) == NULL )
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

