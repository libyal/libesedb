/*
 * Key functions
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

#include "libesedb_key.h"

/* Creates a key
 * Returns 1 if successful or -1 on error
 */
int libesedb_key_initialize(
     libesedb_key_t **key,
     liberror_error_t **error )
{
	static char *function = "libesedb_key_initialize";

	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key == NULL )
	{
		*key = (libesedb_key_t *) memory_allocate(
		                                         sizeof( libesedb_key_t ) );

		if( *key == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create key.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *key,
		     0,
		     sizeof( libesedb_key_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear key.",
			 function );

			memory_free(
			 *key );

			*key = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the key
 * Returns 1 if successful or -1 on error
 */
int libesedb_key_free(
     intptr_t *key,
     liberror_error_t **error )
{
	static char *function = "libesedb_key_free";

	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( ( ( libesedb_key_t *) key )->data != NULL )
	{
		memory_free(
		 ( (libesedb_key_t *) key )->data );
	}
	memory_free(
	 key );

	return( 1 );
}

/* Sets the data in the key
 * Returns 1 if successful or -1 on error
 */
int libesedb_key_set_data(
     libesedb_key_t *key,
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_key_set_data";

	if( key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key->data != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key - data already set.",
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
	key->data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * data_size );

	if( key->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key data.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     key->data,
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
	key->data_size = data_size;

	return( 1 );
}

