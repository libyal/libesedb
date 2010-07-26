/*
 * Column functions
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

#include "libesedb_catalog_definition.h"
#include "libesedb_column.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_types.h"

/* Creates a column
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_initialize(
     libesedb_column_t **column,
     libesedb_io_handle_t *io_handle,
     libesedb_catalog_definition_t *catalog_definition,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_initialize";

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
		internal_column->io_handle          = io_handle;
		internal_column->catalog_definition = catalog_definition;

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

		/* The io_handle and catalog_definition references
		 * are freed elsewhere
		 */
		memory_free(
		 internal_column );
	}
	return( result );
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

	if( libesedb_catalog_definition_get_identifier(
	     internal_column->catalog_definition,
	     identifier,
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

	if( libesedb_catalog_definition_get_column_type(
	     internal_column->catalog_definition,
	     type,
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

/* Retrieves the size of the UTF-8 encoded string of the column name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_get_utf8_name_size(
     libesedb_column_t *column,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_get_utf8_name_size";

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

	if( internal_column->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name_size(
	     internal_column->catalog_definition,
	     utf8_string_size,
	     internal_column->io_handle->ascii_codepage,
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

/* Retrieves the UTF-8 encoded string of the column name
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
	static char *function                       = "libesedb_column_get_utf8_name";

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

	if( internal_column->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf8_name(
	     internal_column->catalog_definition,
	     utf8_string,
	     utf8_string_size,
	     internal_column->io_handle->ascii_codepage,
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

/* Retrieves the size of the UTF-16 encoded string of the column name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_get_utf16_name_size(
     libesedb_column_t *column,
     size_t *utf16_string_size,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_get_utf16_name_size";

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

	if( internal_column->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name_size(
	     internal_column->catalog_definition,
	     utf16_string_size,
	     internal_column->io_handle->ascii_codepage,
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

/* Retrieves the UTF-16 encoded string of the column name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_get_utf16_name(
     libesedb_column_t *column,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	libesedb_internal_column_t *internal_column = NULL;
	static char *function                       = "libesedb_column_get_utf16_name";

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

	if( internal_column->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal column - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_utf16_name(
	     internal_column->catalog_definition,
	     utf16_string,
	     utf16_string_size,
	     internal_column->io_handle->ascii_codepage,
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

