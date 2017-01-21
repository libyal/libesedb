/*
 * Table definition functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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
	if( libcdata_list_initialize(
	     &( ( *table_definition )->column_catalog_definition_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create column catalog definition list.",
		 function );

		goto on_error;
	}
	if( libcdata_list_initialize(
	     &( ( *table_definition )->index_catalog_definition_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create index catalog definition list.",
		 function );

		goto on_error;
	}
	( *table_definition )->table_catalog_definition = table_catalog_definition;

	return( 1 );

on_error:
	if( *table_definition != NULL )
	{
		if( ( *table_definition )->column_catalog_definition_list != NULL )
		{
			libcdata_list_free(
			 &( ( *table_definition )->column_catalog_definition_list ),
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
		if( libcdata_list_free(
		     &( ( *table_definition )->column_catalog_definition_list ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_catalog_definition_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free column catalog definition list.",
			 function );

			result = -1;
		}
		if( libcdata_list_free(
		     &( ( *table_definition )->index_catalog_definition_list ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_catalog_definition_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free index catalog definition list.",
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

/* Appends a column catalog definition to the table definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_definition_append_column_catalog_definition(
     libesedb_table_definition_t *table_definition,
     libesedb_catalog_definition_t *column_catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_table_definition_append_column_catalog_definition";

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
	if( libcdata_list_append_value(
	     table_definition->column_catalog_definition_list,
	     (intptr_t *) column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append column catalog definition to list.",
		 function );

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
	if( libcdata_list_append_value(
	     table_definition->index_catalog_definition_list,
	     (intptr_t *) index_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append index catalog definition to list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

