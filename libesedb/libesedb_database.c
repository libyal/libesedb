/*
 * Database functions
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

#include "libesedb_database.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page.h"
#include "libesedb_page_value.h"
#include "libesedb_page_tree.h"
#include "libesedb_page_tree_value.h"

/* Creates a database
 * Make sure the value database is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_database_initialize(
     libesedb_database_t **database,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error )
{
	static char *function = "libesedb_database_initialize";

	if( database == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid database.",
		 function );

		return( -1 );
	}
	if( *database != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid database value already set.",
		 function );

		return( -1 );
	}
	*database = memory_allocate_structure(
	             libesedb_database_t );

	if( *database == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create database.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     ( *database ),
	     0,
	     sizeof( libesedb_database_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear database.",
		 function );

		memory_free(
		 *database );

		*database = NULL;

		return( -1 );
	}
	if( libesedb_page_tree_initialize(
	     &( ( *database )->page_tree ),
	     io_handle,
	     pages_vector,
	     pages_cache,
	     LIBESEDB_FDP_OBJECT_IDENTIFIER_DATABASE,
	     LIBESEDB_PAGE_NUMBER_DATABASE,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create page tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *database != NULL )
	{
		memory_free(
		 *database );

		*database = NULL;
	}
	return( -1 );
}

/* Frees a database
 * Returns 1 if successful or -1 on error
 */
int libesedb_database_free(
     libesedb_database_t **database,
     libcerror_error_t **error )
{
	static char *function = "libesedb_database_free";
	int result            = 1;

	if( database == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid database.",
		 function );

		return( -1 );
	}
	if( *database != NULL )
	{
		if( libesedb_page_tree_free(
		     &( ( *database )->page_tree ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page tree.",
			 function );

			result = -1;
		}
		memory_free(
		 *database );

		*database = NULL;
	}
	return( result );
}

/* Reads the database values from a leaf page
 * Returns 1 if successful or -1 on error
 */
int libesedb_database_read_values_from_leaf_page(
     libesedb_database_t *database,
     libesedb_page_t *page,
     int *current_leaf_value_index,
     libcerror_error_t **error )
{
	libesedb_page_tree_value_t *page_tree_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	static char *function                       = "libesedb_database_read_values_from_leaf_page";
	uint32_t page_flags                         = 0;
	uint16_t number_of_page_values              = 0;
	uint16_t page_value_index                   = 0;

	if( database == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid database.",
		 function );

		return( -1 );
	}
	if( database->page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid database - missing page tree.",
		 function );

		return( -1 );
	}
	if( current_leaf_value_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid current leaf value index.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_flags(
	     page,
	     &page_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page flags.",
		 function );

		goto on_error;
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page - not a leaf page.",
		 function );

		goto on_error;
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		goto on_error;
	}
	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		if( libesedb_page_get_value_by_index(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
		if( page_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) != 0 )
		{
			continue;
		}
		if( libesedb_page_tree_value_initialize(
		     &page_tree_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create page tree value.",
			 function );

			goto on_error;
		}
		if( libesedb_page_tree_value_read_data(
		     page_tree_value,
		     page_value->data,
		     (size_t) page_value->size,
		     page_value->flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page tree value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
/* TODO implement */

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: database value: %d data:\n",
			 function,
			 *current_leaf_value_index );
			libcnotify_print_data(
			 page_tree_value->data,
			 page_tree_value->data_size,
			 0 );
		}
#endif
		if( *current_leaf_value_index == INT_MAX )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid current leaf value index value out of bounds.",
			 function );

			goto on_error;
		}
		*current_leaf_value_index += 1;

		if( libesedb_page_tree_value_free(
		     &page_tree_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page tree value.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( page_tree_value != NULL )
	{
		libesedb_page_tree_value_free(
		 &page_tree_value,
		 NULL );
	}
	return( -1 );
}

/* Reads the database
 * Returns 1 if successful or -1 on error
 */
int libesedb_database_read_file_io_handle(
     libesedb_database_t *database,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libesedb_page_t *page        = NULL;
	static char *function        = "libesedb_database_read_file_io_handle";
	uint32_t leaf_page_number    = 0;
	int current_leaf_value_index = 0;
	int recursion_depth          = 0;

	if( database == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid database.",
		 function );

		return( -1 );
	}
	if( database->page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid database - missing page tree.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_get_get_first_leaf_page_number(
	     database->page_tree,
	     file_io_handle,
	     &leaf_page_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve first leaf page number from page tree.",
		 function );

		return( -1 );
	}
	while( leaf_page_number != 0 )
	{
		if( recursion_depth > LIBESEDB_MAXIMUM_LEAF_PAGE_RECURSION_DEPTH )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid recursion depth value out of bounds.",
			 function );

			return( -1 );
		}
#if ( SIZEOF_INT <= 4 )
		if( leaf_page_number > (uint32_t) INT_MAX )
#else
		if( leaf_page_number > (unsigned int) INT_MAX )
#endif
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid leaf page number value out of bounds.",
			 function );

			return( -1 );
		}
		if( libfdata_vector_get_element_value_by_index(
		     database->page_tree->pages_vector,
		     (intptr_t *) file_io_handle,
		     (libfdata_cache_t *) database->page_tree->pages_cache,
		     (int) leaf_page_number - 1,
		     (intptr_t **) &page,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page: %" PRIu32 ".",
			 function,
			 leaf_page_number );

			return( -1 );
		}
		if( libesedb_database_read_values_from_leaf_page(
		     database,
		     page,
		     &current_leaf_value_index,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to read values from page: %" PRIu32 ".",
			 function,
			 leaf_page_number );

			return( -1 );
		}
		if( libesedb_page_get_next_page_number(
		     page,
		     &leaf_page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next page number from page: %" PRIu32 ".",
			 function,
			 leaf_page_number );

			return( -1 );
		}
		recursion_depth++;
	}
	return( 1 );
}

