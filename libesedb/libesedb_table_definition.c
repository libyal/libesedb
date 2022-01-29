/*
 * Table definition functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_table_definition.h"

/* Creates a table definition
 * Make sure the value table_definition is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_initialize(
     libesedb_table_definition_t **table_definition,
     libesedb_catalog_definition_t *table_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_initialize";

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
	if( *table_definition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table definition value already set.",
		 function );

		return( -1 );
	}
	if( table_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table catalog definition.",
		 function );

		return( -1 );
	}
	if( table_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_TABLE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported catalog definition type: %" PRIu16 ".",
		 function,
		 table_catalog_definition->type );

		return( -1 );
	}
	*table_definition = memory_allocate_structure(
	                     libesedb_table_definition_t );

	if( *table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create table definition.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *table_definition,
	     0,
	     sizeof( libesedb_table_definition_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear table definition.",
		 function );

		memory_free(
		 *table_definition );

		*table_definition = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *table_definition )->column_catalog_definition_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column catalog definition array.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *table_definition )->index_catalog_definition_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index catalog definition array.",
		 function );

		goto on_error;
	}
	( *table_definition )->table_catalog_definition = table_catalog_definition;

	return( 1 );

on_error:
	if( *table_definition != NULL )
	{
		if( ( *table_definition )->column_catalog_definition_array != NULL )
		{
			libcdata_array_free(
			 &( ( *table_definition )->column_catalog_definition_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 *table_definition );

		*table_definition = NULL;
	}
	return( -1 );
}

/* Frees a table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_free(
     libesedb_table_definition_t **table_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_free";
	int result            = 1;

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
	if( *table_definition != NULL )
	{
		if( ( *table_definition )->table_catalog_definition != NULL )
		{
			if( libesedb_catalog_definition_free(
			     &( ( *table_definition )->table_catalog_definition ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free table catalog definition.",
				 function );

				result = -1;
			}
		}
		if( ( *table_definition )->long_value_catalog_definition != NULL )
		{
			if( libesedb_catalog_definition_free(
			     &( ( *table_definition )->long_value_catalog_definition ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free long value catalog definition.",
				 function );

				result = -1;
			}
		}
		if( ( *table_definition )->callback_catalog_definition != NULL )
		{
			if( libesedb_catalog_definition_free(
			     &( ( *table_definition )->callback_catalog_definition ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free callback catalog definition.",
				 function );

				result = -1;
			}
		}
		if( libcdata_array_free(
		     &( ( *table_definition )->column_catalog_definition_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_catalog_definition_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free column catalog definition array.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *table_definition )->index_catalog_definition_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_catalog_definition_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index catalog definition array.",
			 function );

			result = -1;
		}
		memory_free(
		 *table_definition );

		*table_definition = NULL;
	}
	return( result );
}

/* Sets a long value catalog definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_set_long_value_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *long_value_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_set_long_value_catalog_definition";

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
	if( table_definition->long_value_catalog_definition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table definition - long value catalog definition already set.",
		 function );

		return( -1 );
	}
	if( long_value_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value catalog definition.",
		 function );

		return( -1 );
	}
	if( long_value_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_LONG_VALUE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported catalog definition type: %" PRIu16 ".",
		 function,
		 long_value_catalog_definition->type );

		return( -1 );
	}
	table_definition->long_value_catalog_definition = long_value_catalog_definition;

	return( 1 );
}

/* Sets a callback catalog definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_set_callback_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *callback_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_set_callback_catalog_definition";

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
	if( table_definition->callback_catalog_definition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table definition - callback catalog definition already set.",
		 function );

		return( -1 );
	}
	if( callback_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid callback catalog definition.",
		 function );

		return( -1 );
	}
	if( callback_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_CALLBACK )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported catalog definition type: %" PRIu16 ".",
		 function,
		 callback_catalog_definition->type );

		return( -1 );
	}
	table_definition->callback_catalog_definition = callback_catalog_definition;

	return( 1 );
}

/* Retrieves the number of column catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_get_number_of_column_catalog_definitions(
     libesedb_table_definition_t *table_definition,
     int *number_of_definitions,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_get_number_of_column_catalog_definitions";

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
	if( libcdata_array_get_number_of_entries(
	     table_definition->column_catalog_definition_array,
	     number_of_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from column catalog definition array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific column catalog definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_get_column_catalog_definition_by_index(
     libesedb_table_definition_t *table_definition,
     int definition_index,
     libesedb_catalog_definition_t **column_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_get_column_catalog_definition_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     table_definition->column_catalog_definition_array,
	     definition_index,
	     (intptr_t **) column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from column catalog definition array.",
		 function,
		 definition_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends a column catalog definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_append_column_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *column_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_append_column_catalog_definition";
	int entry_index       = 0;

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
	if( column_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column catalog definition.",
		 function );

		return( -1 );
	}
	if( column_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported catalog definition type: %" PRIu16 ".",
		 function,
		 column_catalog_definition->type );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     table_definition->column_catalog_definition_array,
	     &entry_index,
	     (intptr_t *) column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append column catalog definition to array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of index catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_get_number_of_index_catalog_definitions(
     libesedb_table_definition_t *table_definition,
     int *number_of_definitions,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_get_number_of_index_catalog_definitions";

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
	if( libcdata_array_get_number_of_entries(
	     table_definition->index_catalog_definition_array,
	     number_of_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from index catalog definition array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific index catalog definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_get_index_catalog_definition_by_index(
     libesedb_table_definition_t *table_definition,
     int definition_index,
     libesedb_catalog_definition_t **index_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_get_index_catalog_definition_by_index";

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
	if( libcdata_array_get_entry_by_index(
	     table_definition->index_catalog_definition_array,
	     definition_index,
	     (intptr_t **) index_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve entry: %d from index catalog definition array.",
		 function,
		 definition_index );

		return( -1 );
	}
	return( 1 );
}

/* Appends an index catalog definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_append_index_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *index_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_append_index_catalog_definition";
	int entry_index       = 0;

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
	if( index_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid index catalog definition.",
		 function );

		return( -1 );
	}
	if( index_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_INDEX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported catalog definition type: %" PRIu16 ".",
		 function,
		 index_catalog_definition->type );

		return( -1 );
	}
	if( libcdata_array_append_entry(
	     table_definition->index_catalog_definition_array,
	     &entry_index,
	     (intptr_t *) index_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append index catalog definition to array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

