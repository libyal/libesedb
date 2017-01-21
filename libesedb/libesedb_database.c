/*
 * Database functions
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

#include "libesedb_database.h"
#include "libesedb_data_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree.h"

/* Creates a database
 * Make sure the value database is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_database_initialize(
     libesedb_database_t **database,
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
		memory_free(
		 *database );

		*database = NULL;
	}
	return( 1 );
}

/* Reads the database
 * Returns 1 if successful or -1 on error
 */
int libesedb_database_read(
     libesedb_database_t *database,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error )
{
	libesedb_data_definition_t *data_definition = NULL;
	libesedb_page_tree_t *database_page_tree    = NULL;
	libfcache_cache_t *database_values_cache    = NULL;
	libfdata_btree_t *database_values_tree      = NULL;
	uint8_t *data                               = NULL;
	static char *function                       = "libesedb_database_read";
	off64_t node_data_offset                    = 0;
	size_t data_size                            = 0;
	int leaf_value_index                        = 0;
	int number_of_leaf_values                   = 0;

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
	if( libesedb_page_tree_initialize(
	     &database_page_tree,
	     io_handle,
	     pages_vector,
	     pages_cache,
	     LIBESEDB_FDP_OBJECT_IDENTIFIER_DATABASE,
	     NULL,
	     NULL,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create database page tree.",
		 function );

		goto on_error;
	}
	/* TODO clone function
	 */
	if( libfdata_btree_initialize(
	     &database_values_tree,
	     (intptr_t *) database_page_tree,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_page_tree_free,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_btree_node_t *, int, off64_t, size64_t, uint32_t, intptr_t *, uint8_t, libcerror_error_t **)) &libesedb_page_tree_read_node,
	     (int (*)(intptr_t *, intptr_t *, libfdata_btree_t *, libfcache_cache_t *, int, int, off64_t, size64_t, uint32_t, intptr_t *, uint8_t, libcerror_error_t **)) &libesedb_page_tree_read_leaf_value,
	     LIBFDATA_DATA_HANDLE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create database values tree.",
		 function );

		goto on_error;
	}
	database_page_tree = NULL;

	if( libfcache_cache_initialize(
	     &database_values_cache,
	     LIBESEDB_MAXIMUM_CACHE_ENTRIES_TREE_VALUES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create database values cache.",
		 function );

		goto on_error;
	}
	node_data_offset  = LIBESEDB_PAGE_NUMBER_DATABASE - 1;
	node_data_offset *= io_handle->page_size;

	if( libfdata_btree_set_root_node(
	     database_values_tree,
	     0,
	     node_data_offset,
	     0,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set root node in database values tree.",
		 function );

		goto on_error;
	}
	if( libfdata_btree_get_number_of_leaf_values(
	     database_values_tree,
	     (intptr_t *) file_io_handle,
	     database_values_cache,
	     &number_of_leaf_values,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of leaf values from database values tree.",
		 function );

		goto on_error;
	}
	for( leaf_value_index = 0;
	     leaf_value_index < number_of_leaf_values;
	     leaf_value_index++ )
	{
		if( libfdata_btree_get_leaf_value_by_index(
		     database_values_tree,
		     (intptr_t *) file_io_handle,
		     database_values_cache,
		     leaf_value_index,
		     (intptr_t **) &data_definition,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf value: %d from database values tree.",
			 function,
			 leaf_value_index );

			goto on_error;
		}
		if( libesedb_data_definition_read_data(
		     data_definition,
		     file_io_handle,
		     io_handle,
		     pages_vector,
		     pages_cache,
		     &data,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data definition data.",
			 function );

			goto on_error;
		}
		/* TODO */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( data_size > 0 )
			{
				libcnotify_printf(
				 "%s: database value: %d data:\n",
				 function,
				 leaf_value_index );
				libcnotify_print_data(
				 data,
				 data_size,
				 0 );
			}
		}
#endif
	}
	if( libfcache_cache_free(
	     &database_values_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free database values cache.",
		 function );

		goto on_error;
	}
	if( libfdata_btree_free(
	     &database_values_tree,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free database values tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( database_values_cache != NULL )
	{
		libfcache_cache_free(
		 &database_values_cache,
		 NULL );
	}
	if( database_values_tree != NULL )
	{
		libfdata_btree_free(
		 &database_values_tree,
		 NULL );
	}
	if( database_page_tree != NULL )
	{
		libesedb_page_tree_free(
		 &database_page_tree,
		 NULL );
	}
	return( -1 );
}

