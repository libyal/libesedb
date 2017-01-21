/*
 * Key functions
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

#include "libesedb_definitions.h"
#include "libesedb_key.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"

/* Creates a key
 * Make sure the value key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_key_initialize(
     libesedb_key_t **key,
     libcerror_error_t **error )
{
	static char *function = "libesedb_key_initialize";

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key value already set.",
		 function );

		return( -1 );
	}
	*key = memory_allocate_structure(
	        libesedb_key_t );

	if( *key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *key ),
	     0,
	     sizeof( libesedb_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key != NULL )
	{
		memory_free(
		 *key );

		*key = NULL;
	}
	return( -1 );
}

/* Frees a key
 * Returns 1 if successful or -1 on error
 */
int libesedb_key_free(
     libesedb_key_t **key,
     libcerror_error_t **error )
{
	static char *function = "libesedb_key_free";

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		if( ( *key )->data != NULL )
		{
			memory_free(
			 ( *key )->data );
		}
		memory_free(
		 *key );

		*key = NULL;
	}
	return( 1 );
}

/* Sets the data
 * Returns 1 if successful or -1 on error
 */
int libesedb_key_set_data(
     libesedb_key_t *key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_key_set_data";

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( key->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key - data already set.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size > 0 )
	{
		key->data = (uint8_t *) memory_allocate(
		                         sizeof( uint8_t ) * data_size );

		if( key->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     key->data,
		     data,
		     sizeof( uint8_t ) * data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			goto on_error;
		}
		key->data_size = data_size;
	}
	return( 1 );

on_error:
	if( key->data != NULL )
	{
		memory_free(
		 key->data );

		key->data = NULL;
	}
	key->data_size = 0;

	return( -1 );
}

/* Append the data
 * Returns 1 if successful or -1 on error
 */
int libesedb_key_append_data(
     libesedb_key_t *key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	void *reallocation    = NULL;
	static char *function = "libesedb_key_append_data";

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size > 0 )
	{
		reallocation = memory_reallocate(
		                key->data,
		                sizeof( uint8_t ) * ( key->data_size + data_size ) );

		if( reallocation == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize data.",
			 function );

			return( -1 );
		}
		key->data = (uint8_t *) reallocation;

		if( memory_copy(
		     &( key->data[ key->data_size ] ),
		     data,
		     sizeof( uint8_t ) * data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			return( -1 );
		}
		key->data_size += data_size;
	}
	return( 1 );
}

/* Compares two keys
 * Returns LIBFDATA_COMPARE_LESS, LIBFDATA_COMPARE_EQUAL, LIBFDATA_COMPARE_GREATER,
 * LIBFDATA_COMPARE_GREATER_EQUAL if successful or -1 on error
 */
int libesedb_key_compare(
     libesedb_key_t *first_key,
     libesedb_key_t *second_key,
     libcerror_error_t **error )
{
	static char *function        = "libesedb_key_compare";
	size_t compare_data_size     = 0;
	size_t first_key_data_index  = 0;
	size_t second_key_data_index = 0;
	int16_t compare_result       = -1;
	uint8_t first_key_data       = 0;
	uint8_t is_flexible_match    = 0;
	int result                   = -1;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *key_data            = NULL;
	size_t key_data_size         = 0;
#endif

	if( first_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first key.",
		 function );

		return( -1 );
	}
	if( first_key->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid first key - missing data.",
		 function );

		return( -1 );
	}
	if( second_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second key.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: first key\t\t\t\t\t\t: ",
		 function );

		key_data      = first_key->data;
		key_data_size = first_key->data_size;

		while( key_data_size > 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%02" PRIx8 " ",
				 *key_data );
			}
			key_data++;
			key_data_size--;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: first key type\t\t\t\t\t: ",
		 function );

		switch( first_key->type )
		{
			case LIBESEDB_KEY_TYPE_INDEX_VALUE:
				libcnotify_printf(
				 "index value" );
				break;

			case LIBESEDB_KEY_TYPE_LONG_VALUE:
				libcnotify_printf(
				 "long value" );
				break;

			case LIBESEDB_KEY_TYPE_LONG_VALUE_SEGMENT:
				libcnotify_printf(
				 "long value segment" );
				break;

			default:
				libcnotify_printf(
				 "invalid" );
				break;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: second key\t\t\t\t\t: ",
		 function );

		key_data      = second_key->data;
		key_data_size = second_key->data_size;

		while( key_data_size > 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%02" PRIx8 " ",
				 *key_data );
			}
			key_data++;
			key_data_size--;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: second key type\t\t\t\t\t: ",
		 function );

		switch( second_key->type )
		{
			case LIBESEDB_KEY_TYPE_BRANCH:
				libcnotify_printf(
				 "branch" );
				break;

			case LIBESEDB_KEY_TYPE_LEAF:
				libcnotify_printf(
				 "leaf" );
				break;

			default:
				libcnotify_printf(
				 "invalid" );
				break;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif
	/* Check if the key is empty, therefore has no upper bound
	 * and thus the first key will be greater than the second key
	 */
	if( second_key->data_size > 0 )
	{
		if( second_key->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid second key - missing data.",
			 function );

			return( -1 );
		}
		if( first_key->data_size <= second_key->data_size )
		{
			compare_data_size = first_key->data_size;
		}
		else
		{
			compare_data_size = second_key->data_size;
		}
/* TODO remove is_flexible_match test */
		is_flexible_match = 0;

		/* The long value key is stored reversed
		 */
		if( first_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE )
		{
			first_key_data_index = first_key->data_size - 1;
		}
		else
		{
			first_key_data_index = 0;
		}
		for( second_key_data_index = 0;
		     second_key_data_index < compare_data_size;
		     second_key_data_index++ )
		{
			first_key_data = first_key->data[ first_key_data_index ];

			if( ( first_key->type == LIBESEDB_KEY_TYPE_INDEX_VALUE )
			 && ( second_key->type == LIBESEDB_KEY_TYPE_LEAF ) )
			{
/* TODO does not hold for branch keys in Win XP search database */
				if( ( first_key_data_index == 1 )
				 && ( ( first_key_data & 0x80 ) != 0 )
				 && ( ( first_key_data & 0x7f ) == second_key->data[ second_key_data_index ] ) )
				{
					first_key_data &= 0x7f;

					is_flexible_match = 1;
				}
			}
			compare_result = (int16_t) first_key_data - (int16_t) second_key->data[ second_key_data_index ];

			if( compare_result != 0 )
			{
				break;
			}
			if( first_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE )
			{
				first_key_data_index--;
			}
			else
			{
				first_key_data_index++;
			}
		}
	}
	if( compare_result > 0 )
	{
		result = LIBFDATA_COMPARE_GREATER;
	}
	else if( second_key->type == LIBESEDB_KEY_TYPE_BRANCH )
	{
		if( first_key->type == LIBESEDB_KEY_TYPE_INDEX_VALUE )
		{
			/* If the key exactly matches the branch key,
			 * the leaf value is in the next branch node
			 */
			if( compare_result == 0 )
			{
				result = LIBFDATA_COMPARE_GREATER;
			}
			else
			{
				result = LIBFDATA_COMPARE_EQUAL;
			}
		}
		else if( ( first_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE )
		      || ( first_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE_SEGMENT ) )
		{
			/* If the key matches the branch key but is longer,
			 * the leaf value is in the next branch node
			 */
			if( ( compare_result == 0 )
			 && ( first_key->data_size > second_key->data_size ) )
			{
				result = LIBFDATA_COMPARE_GREATER;
			}
			else
			{
				result = LIBFDATA_COMPARE_LESS_EQUAL;
			}
		}
		else
		{
/* TODO make sure this works */
			result = LIBFDATA_COMPARE_EQUAL;
		}
	}
	else if( second_key->type == LIBESEDB_KEY_TYPE_LEAF )
	{
		if( is_flexible_match != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
/* TODO remove this after debugging */
fprintf( stderr, "MARKER\n" );
	}
#endif
		}
		if( compare_result < 0 )
		{
			result = LIBFDATA_COMPARE_LESS;
		}
		else if( first_key->data_size < second_key->data_size )
		{
			result = LIBFDATA_COMPARE_LESS;
		}
		else if( first_key->data_size > second_key->data_size )
		{
			result = LIBFDATA_COMPARE_GREATER;
		}
		else
		{
			result = LIBFDATA_COMPARE_EQUAL;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: result\t\t\t\t\t\t: ",
		 function );

		switch( result )
		{
			case LIBFDATA_COMPARE_EQUAL:
				libcnotify_printf(
				 "equal" );
				break;

			case LIBFDATA_COMPARE_GREATER:
				libcnotify_printf(
				 "greater" );
				break;

			case LIBFDATA_COMPARE_GREATER_EQUAL:
				libcnotify_printf(
				 "greater equal" );
				break;

			case LIBFDATA_COMPARE_LESS:
				libcnotify_printf(
				 "less" );
				break;

			case LIBFDATA_COMPARE_LESS_EQUAL:
				libcnotify_printf(
				 "less equal" );
				break;

			default:
				libcnotify_printf(
				 "invalid" );
				break;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "\n" );
	}
#endif
	return( result );
}

