/*
 * Data type definition functions
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
#include "libesedb_data_type_definition.h"

/* Creates a data type definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_type_definition_initialize(
     libesedb_data_type_definition_t **data_type_definition,
     libesedb_catalog_definition_t *column_catalog_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_data_type_definition_initialize";

	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( *data_type_definition == NULL )
	{
		*data_type_definition = (libesedb_data_type_definition_t *) memory_allocate(
		                                                             sizeof( libesedb_data_type_definition_t ) );

		if( *data_type_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data type definition.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *data_type_definition,
		     0,
		     sizeof( libesedb_data_type_definition_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data type definition.",
			 function );

			memory_free(
			 *data_type_definition );

			*data_type_definition = NULL;

			return( -1 );
		}
		( *data_type_definition )->column_catalog_definition = column_catalog_definition;
	}
	return( 1 );
}

/* Frees the data type definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_type_definition_free(
     intptr_t *data_type_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_data_type_definition_free";

	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	/* The column_catalog_definition is only referenced an is freed elsewhere
	 */
	if( ( ( libesedb_data_type_definition_t *) data_type_definition )->data != NULL )
	{
		memory_free(
		 ( (libesedb_data_type_definition_t *) data_type_definition )->data );
	}
	memory_free(
	 data_type_definition );

	return( 1 );
}

/* Sets the data in the data type definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_type_definition_set_data(
     libesedb_data_type_definition_t *data_type_definition,
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_data_type_definition_set_data";

	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->data != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data type definition - data already set.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	data_type_definition->data = (uint8_t *) memory_allocate(
	                                          sizeof( uint8_t ) * data_size );

	if( data_type_definition->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data type definition data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     data_type_definition->data,
	     data,
	     sizeof( uint8_t ) * data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy data.",
		 function );

		return( -1 );
	}
	data_type_definition->data_size = data_size;

	return( 1 );
}

/* Sets the flags in the data type definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_type_definition_set_flags(
     libesedb_data_type_definition_t *data_type_definition,
     uint8_t flags,
     liberror_error_t **error )
{
	static char *function = "libesedb_data_type_definition_set_flags";

	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	data_type_definition->flags = flags;

	return( 1 );
}

