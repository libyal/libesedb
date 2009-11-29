/*
 * Long value functions
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

#include "libesedb_long_value.h"

/* Creates a long value
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_initialize(
     libesedb_long_value_t **long_value,
     liberror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_initialize";

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
	if( *long_value == NULL )
	{
		internal_long_value = (libesedb_internal_long_value_t *) memory_allocate(
		                                                          sizeof( libesedb_internal_long_value_t ) );

		if( *long_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create long value.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *long_value,
		     0,
		     sizeof( libesedb_internal_long_value_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear long value.",
			 function );

			memory_free(
			 *long_value );

			*long_value = NULL;

			return( -1 );
		}
		*long_value = (libesedb_long_value_t *) internal_long_value;
	}
	return( 1 );
}

/* Frees the long value
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_free(
     libesedb_long_value_t **long_value,
     liberror_error_t **error )
{
	static char *function = "libesedb_long_value_free";

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

		if( ( ( libesedb_internal_long_value_t *) *long_value )->data != NULL )
		{
			memory_free(
			 ( (libesedb_internal_long_value_t *) *long_value )->data );
		}
		memory_free(
		 *long_value );

		*long_value = NULL;
	}
	return( 1 );
}

/* Sets the data in the long value
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_set_data(
     libesedb_internal_long_value_t *internal_long_value,
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_long_value_set_data";

	if( internal_long_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal long value.",
		 function );

		return( -1 );
	}
	if( internal_long_value->data != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal long value - data already set.",
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
	internal_long_value->data = (uint8_t *) memory_allocate(
	                                         sizeof( uint8_t ) * data_size );

	if( internal_long_value->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create long value data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     internal_long_value->data,
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
	internal_long_value->data_size = data_size;

	return( 1 );
}

