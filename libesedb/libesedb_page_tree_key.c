/*
 * Page tree key functions
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

#include "libesedb_definitions.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree_key.h"

/* Creates a page tree key
 * Make sure the value page_tree_key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_key_initialize(
     libesedb_page_tree_key_t **page_tree_key,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_key_initialize";

	if( page_tree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree key.",
		 function );

		return( -1 );
	}
	if( *page_tree_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page tree key value already set.",
		 function );

		return( -1 );
	}
	*page_tree_key = memory_allocate_structure(
	                  libesedb_page_tree_key_t );

	if( *page_tree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page tree key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *page_tree_key ),
	     0,
	     sizeof( libesedb_page_tree_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear page tree key.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *page_tree_key != NULL )
	{
		memory_free(
		 *page_tree_key );

		*page_tree_key = NULL;
	}
	return( -1 );
}

/* Frees a page tree key
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_key_free(
     libesedb_page_tree_key_t **page_tree_key,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_key_free";

	if( page_tree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree key.",
		 function );

		return( -1 );
	}
	if( *page_tree_key != NULL )
	{
		if( ( *page_tree_key )->data != NULL )
		{
			memory_free(
			 ( *page_tree_key )->data );
		}
		memory_free(
		 *page_tree_key );

		*page_tree_key = NULL;
	}
	return( 1 );
}

/* Sets data
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_key_set_data(
     libesedb_page_tree_key_t *page_tree_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_key_set_data";

	if( page_tree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree key.",
		 function );

		return( -1 );
	}
	if( page_tree_key->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page tree key - data already set.",
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
	if( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum allocation size.",
		 function );

		return( -1 );
	}
	if( data_size > 0 )
	{
		page_tree_key->data = (uint8_t *) memory_allocate(
		                                   sizeof( uint8_t ) * data_size );

		if( page_tree_key->data == NULL )
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
		     page_tree_key->data,
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
		page_tree_key->data_size = data_size;
	}
	return( 1 );

on_error:
	if( page_tree_key->data != NULL )
	{
		memory_free(
		 page_tree_key->data );

		page_tree_key->data = NULL;
	}
	page_tree_key->data_size = 0;

	return( -1 );
}

/* Append data
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_key_append_data(
     libesedb_page_tree_key_t *page_tree_key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	void *reallocation    = NULL;
	static char *function = "libesedb_page_tree_key_append_data";

	if( page_tree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree key.",
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
		                page_tree_key->data,
		                sizeof( uint8_t ) * ( page_tree_key->data_size + data_size ) );

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
		page_tree_key->data = (uint8_t *) reallocation;

		if( memory_copy(
		     &( page_tree_key->data[ page_tree_key->data_size ] ),
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
		page_tree_key->data_size += data_size;
	}
	return( 1 );
}

/* Compares two page tree keys
 * Returns LIBFDATA_COMPARE_LESS, LIBFDATA_COMPARE_EQUAL, LIBFDATA_COMPARE_GREATER,
 * LIBFDATA_COMPARE_GREATER_EQUAL if successful or -1 on error
 */
int libesedb_page_tree_key_compare(
     libesedb_page_tree_key_t *first_page_tree_key,
     libesedb_page_tree_key_t *second_page_tree_key,
     libcerror_error_t **error )
{
	static char *function                  = "libesedb_page_tree_key_compare";
	size_t compare_data_size               = 0;
	size_t first_page_tree_key_data_index  = 0;
	size_t second_page_tree_key_data_index = 0;
	int16_t compare_result                 = -1;
	uint8_t first_page_tree_key_data       = 0;
	int result                             = -1;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_tree_key_data            = NULL;
	size_t page_tree_key_data_size         = 0;
	uint8_t is_flexible_match              = 0;
#endif

	if( first_page_tree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first page tree key.",
		 function );

		return( -1 );
	}
	if( first_page_tree_key->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid first page tree key - missing data.",
		 function );

		return( -1 );
	}
	if( ( first_page_tree_key->type != LIBESEDB_KEY_TYPE_INDEX_VALUE )
	 && ( first_page_tree_key->type != LIBESEDB_KEY_TYPE_LONG_VALUE )
	 && ( first_page_tree_key->type != LIBESEDB_KEY_TYPE_LONG_VALUE_SEGMENT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported first page tree key type.",
		 function );

		return( -1 );
	}
	if( second_page_tree_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second page tree key.",
		 function );

		return( -1 );
	}
	if( ( second_page_tree_key->type != LIBESEDB_KEY_TYPE_BRANCH )
	 && ( second_page_tree_key->type != LIBESEDB_KEY_TYPE_LEAF ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported second page tree key type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: first page tree key\t\t\t: ",
		 function );

		page_tree_key_data      = first_page_tree_key->data;
		page_tree_key_data_size = first_page_tree_key->data_size;

		while( page_tree_key_data_size > 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%02" PRIx8 " ",
				 *page_tree_key_data );
			}
			page_tree_key_data++;
			page_tree_key_data_size--;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: first page tree key type\t\t: ",
		 function );

		switch( first_page_tree_key->type )
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
				 "unknown" );
				break;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: second page tree key\t\t\t: ",
		 function );

		page_tree_key_data      = second_page_tree_key->data;
		page_tree_key_data_size = second_page_tree_key->data_size;

		while( page_tree_key_data_size > 0 )
		{
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%02" PRIx8 " ",
				 *page_tree_key_data );
			}
			page_tree_key_data++;
			page_tree_key_data_size--;
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: second page tree key type\t\t: ",
		 function );

		switch( second_page_tree_key->type )
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
				 "unknown" );
				break;
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	/* Check if the page tree key is empty, therefore has no upper bound
	 * and thus the first page tree key will be greater than the second page tree key
	 */
	if( second_page_tree_key->data_size > 0 )
	{
		if( second_page_tree_key->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid second page tree key - missing data.",
			 function );

			return( -1 );
		}
		if( first_page_tree_key->data_size <= second_page_tree_key->data_size )
		{
			compare_data_size = first_page_tree_key->data_size;
		}
		else
		{
			compare_data_size = second_page_tree_key->data_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		is_flexible_match = 0;
#endif

		/* The long value page_tree_key is stored reversed
		 */
		if( first_page_tree_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE )
		{
			first_page_tree_key_data_index = first_page_tree_key->data_size - 1;
		}
		else
		{
			first_page_tree_key_data_index = 0;
		}
		for( second_page_tree_key_data_index = 0;
		     second_page_tree_key_data_index < compare_data_size;
		     second_page_tree_key_data_index++ )
		{
			first_page_tree_key_data = first_page_tree_key->data[ first_page_tree_key_data_index ];

			if( ( first_page_tree_key->type == LIBESEDB_KEY_TYPE_INDEX_VALUE )
			 && ( second_page_tree_key->type == LIBESEDB_KEY_TYPE_LEAF ) )
			{
/* TODO does not hold for branch page_tree_keys in Win XP search database */
				if( ( first_page_tree_key_data_index == 1 )
				 && ( ( first_page_tree_key_data & 0x80 ) != 0 )
				 && ( ( first_page_tree_key_data & 0x7f ) == second_page_tree_key->data[ second_page_tree_key_data_index ] ) )
				{
					first_page_tree_key_data &= 0x7f;

#if defined( HAVE_DEBUG_OUTPUT )
					is_flexible_match = 1;
#endif
				}
			}
			compare_result = (int16_t) first_page_tree_key_data - (int16_t) second_page_tree_key->data[ second_page_tree_key_data_index ];

			if( compare_result != 0 )
			{
				break;
			}
			if( first_page_tree_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE )
			{
				first_page_tree_key_data_index--;
			}
			else
			{
				first_page_tree_key_data_index++;
			}
		}
	}
	if( compare_result > 0 )
	{
		result = LIBFDATA_COMPARE_GREATER;
	}
	else if( second_page_tree_key->type == LIBESEDB_KEY_TYPE_BRANCH )
	{
		if( first_page_tree_key->type == LIBESEDB_KEY_TYPE_INDEX_VALUE )
		{
			/* If the page_tree_key exactly matches the branch page_tree_key,
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
		else if( ( first_page_tree_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE )
		      || ( first_page_tree_key->type == LIBESEDB_KEY_TYPE_LONG_VALUE_SEGMENT ) )
		{
			/* If the page_tree_key matches the branch page_tree_key but is longer,
			 * the leaf value is in the next branch node
			 */
			if( ( compare_result == 0 )
			 && ( first_page_tree_key->data_size > second_page_tree_key->data_size ) )
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
	else if( second_page_tree_key->type == LIBESEDB_KEY_TYPE_LEAF )
	{
		if( compare_result < 0 )
		{
			result = LIBFDATA_COMPARE_LESS;
		}
		else if( first_page_tree_key->data_size < second_page_tree_key->data_size )
		{
			result = LIBFDATA_COMPARE_LESS;
		}
		else if( first_page_tree_key->data_size > second_page_tree_key->data_size )
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
		 "%s: result\t\t\t\t\t: ",
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
				 "unknown" );
				break;
		}
		if( is_flexible_match != 0 )
		{
			libcnotify_printf(
			 " (is flexible match)" );
		}
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( result );
}

