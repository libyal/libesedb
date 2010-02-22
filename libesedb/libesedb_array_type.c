/* 
 * Array type functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (C) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#include "libesedb_array_type.h"

/* Creates an array
 * Returns 1 if successful or -1 on error
 */
int libesedb_array_initialize(
     libesedb_array_t **array,
     int amount_of_entries,
     liberror_error_t **error )
{
	static char *function = "libesedb_array_initialize";
	size_t entries_size   = 0;

	if( array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array.",
		 function );

		return( -1 );
	}
	if( *array == NULL )
	{
		*array = (libesedb_array_t *) memory_allocate(
		                               sizeof( libesedb_array_t ) );

		if( *array == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create array.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *array,
		     0,
		     sizeof( libesedb_array_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear array.",
			 function );

			memory_free(
			 *array );

			*array = NULL;

			return( -1 );
		}
		if( amount_of_entries > 0 )
		{
			entries_size = sizeof( intptr_t * ) * amount_of_entries;

			if( entries_size > (size_t) SSIZE_MAX )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid entries size value exceeds maximum.",
				 function );

				return( -1 );
			}
			( *array )->entry = (intptr_t **) memory_allocate(
			                                   entries_size );

			if( ( *array )->entry == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create array entries.",
				 function );

				memory_free(
				 *array );

				*array = NULL;

				return( -1 );
			}
			if( memory_set(
			     ( *array )->entry,
			     0,
			     entries_size ) == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_MEMORY,
				 LIBERROR_MEMORY_ERROR_SET_FAILED,
				 "%s: unable to clear array entries.",
				 function );

				memory_free(
				 ( *array )->entry );
				memory_free(
				 *array );

				*array = NULL;

				return( -1 );
			}
			( *array )->amount_of_entries = amount_of_entries;
		}
	}
	return( 1 );
}

/* Frees an array
 * The entries are freed using the entry_free_function
 * Returns 1 if successful or -1 on error
 */
int libesedb_array_free(
     libesedb_array_t **array,
     int (*entry_free_function)(
            intptr_t *entry,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	static char *function = "libesedb_array_free";
	int entry_iterator    = 0;
	int result            = 1;

	if( array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array.",
		 function );

		return( -1 );
	}
	if( *array != NULL )
	{
		if( ( *array )->entry != NULL )
		{
			for( entry_iterator = 0;
			     entry_iterator < ( *array )->amount_of_entries;
			     entry_iterator++ )
			{
				if( ( *array )->entry[ entry_iterator ] != NULL )
				{
					if( ( entry_free_function != NULL )
					 && ( entry_free_function(
					       ( *array )->entry[ entry_iterator ],
					       error ) != 1 ) )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
						 "%s: unable to free array entry: %" PRIu16 ".",
						 function,
						 entry_iterator );

						result = -1;
					}
				}
			}
			memory_free(
			 ( *array )->entry );
		}
		memory_free(
		 *array );

		*array = NULL;
	}
	return( 1 );
}

/* Resizes an array
 * Returns 1 if successful or -1 on error
 */
int libesedb_array_resize(
     libesedb_array_t *array,
     int amount_of_entries,
     liberror_error_t **error )
{
	void *reallocation    = NULL;
	static char *function = "libesedb_array_resize";
	size_t entries_size   = 0;

	if( array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array.",
		 function );

		return( -1 );
	}
	if( amount_of_entries == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of entries.",
		 function );

		return( -1 );
	}
	if( amount_of_entries > array->amount_of_entries )
	{
		entries_size = sizeof( intptr_t * ) * amount_of_entries;

		if( entries_size > (size_t) SSIZE_MAX )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid entries size value exceeds maximum.",
			 function );

			return( -1 );
		}
		reallocation = memory_reallocate(
		                array->entry,
		                entries_size );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize array entries.",
			 function );

			return( -1 );
		}
		array->entry = (intptr_t **) reallocation;

		if( memory_set(
		     &( array->entry[ array->amount_of_entries ] ),
		     0,
		     sizeof( intptr_t ) * ( amount_of_entries - array->amount_of_entries ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear array entries.",
			 function );

			return( -1 );
		}
		array->amount_of_entries = amount_of_entries;
	}
	return( 1 );
}

/* Retrieves the amount of entries in the array
 * Returns 1 if successful or -1 on error
 */
int libesedb_array_get_amount_of_entries(
     libesedb_array_t *array,
     int *amount_of_entries,
     liberror_error_t **error )
{
	static char *function = "libesedb_array_get_amount_of_entries";

	if( array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array.",
		 function );

		return( -1 );
	}
	if( amount_of_entries == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of entries.",
		 function );

		return( -1 );
	}
	*amount_of_entries = array->amount_of_entries;

	return( 1 );
}

/* Retrieves an entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_array_get_entry(
     libesedb_array_t *array,
     int entry_index,
     intptr_t **entry,
     liberror_error_t **error )
{
	static char *function = "libesedb_array_get_entry";

	if( array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array.",
		 function );

		return( -1 );
	}
	if( ( entry_index < 0 )
	 || ( entry_index >= array->amount_of_entries ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry index value out of range.",
		 function );

		return( -1 );
	}
	if( entry == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry.",
		 function );

		return( -1 );
	}
	*entry = array->entry[ entry_index ];

	return( 1 );
}

/* Sets an entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_array_set_entry(
     libesedb_array_t *array,
     int entry_index,
     intptr_t *entry,
     liberror_error_t **error )
{
	static char *function = "libesedb_array_set_entry";

	if( array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array.",
		 function );

		return( -1 );
	}
	if( ( entry_index < 0 )
	 || ( entry_index >= array->amount_of_entries ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid entry index value out of range.",
		 function );

		return( -1 );
	}
	array->entry[ entry_index ] = entry;

	return( 1 );
}

/* Appends an entry
 * Sets the entry index to the newly appended entry
 * Returns 1 if successful or -1 on error
 */
int libesedb_array_append_entry(
     libesedb_array_t *array,
     int *entry_index,
     intptr_t *entry,
     liberror_error_t **error )
{
	static char *function = "libesedb_array_append_entry";

	if( array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid array.",
		 function );

		return( -1 );
	}
	if( entry_index == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry index.",
		 function );

		return( -1 );
	}
	*entry_index = array->amount_of_entries;

	if( libesedb_array_resize(
	     array,
	     array->amount_of_entries + 1,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize array.",
		 function );

		return( -1 );
	}
	array->entry[ *entry_index ] = entry;

	return( 1 );
}

