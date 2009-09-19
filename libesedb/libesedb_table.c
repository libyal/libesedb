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

#include "libesedb_definitions.h"
#include "libesedb_table.h"
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
		if( libesedb_array_initialize(
		     &( internal_table->columns_array ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table columns array.",
			 function );

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

		if( libesedb_array_free(
		     &( internal_table->columns_array ),
		     &libesedb_table_column_value_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the table columns array.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_table );

		*table = NULL;
	}
	return( result );
}

