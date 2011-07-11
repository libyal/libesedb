/*
 * Database functions
 *
 * Copyright (c) 2009-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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
#include <libnotify.h>

#include "libesedb_definitions.h"
#include "libesedb_database.h"
#include "libesedb_page_tree.h"

/* Initialize a database
 * Make sure the value database is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_database_initialize(
     libesedb_database_t **database,
     liberror_error_t **error )
{
	static char *function = "libesedb_database_initialize";

	if( database == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid database.",
		 function );

		return( -1 );
	}
	if( *database == NULL )
	{
		*database = memory_allocate_structure(
		             libesedb_database_t );

		if( *database == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create database.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     ( *database ),
		     0,
		     sizeof( libesedb_database_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear database.",
			 function );

			goto on_error;
		}
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
     liberror_error_t **error )
{
	static char *function = "libesedb_database_free";

	if( database == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
     libfdata_cache_t *pages_cache,
     liberror_error_t **error )
{
	libesedb_page_tree_t *database_page_tree        = NULL;
	libesedb_values_tree_value_t *values_tree_value = NULL;
	libfdata_cache_t *database_values_cache         = NULL;
	libfdata_tree_t *database_values_tree           = NULL;
	libfdata_tree_node_t *database_values_tree_node = NULL;
	uint8_t *data                                   = NULL;
	static char *function                           = "libesedb_database_read";
	off64_t node_data_offset                        = 0;
	size_t data_size                                = 0;
	int number_of_leaf_nodes                        = 0;
	int leaf_node_index                             = 0;

	if( database == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create database page tree.",
		 function );

		goto on_error;
	}
	/* TODO clone function
	 */
	if( libfdata_tree_initialize(
	     &database_values_tree,
	     (intptr_t *) database_page_tree,
	     &libesedb_page_tree_free,
	     NULL,
	     &libesedb_page_tree_read_node_value,
	     &libesedb_page_tree_read_sub_nodes,
	     LIBFDATA_FLAG_IO_HANDLE_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create database values tree.",
		 function );

		goto on_error;
	}
	database_page_tree = NULL;

	if( libfdata_cache_initialize(
	     &database_values_cache,
	     LIBESEDB_MAXIMUM_CACHE_ENTRIES_TREE_VALUES,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create database values cache.",
		 function );

		goto on_error;
	}
	node_data_offset  = LIBESEDB_PAGE_NUMBER_DATABASE - 1;
	node_data_offset *= io_handle->page_size;

	if( libfdata_tree_set_root_node(
	     database_values_tree,
	     node_data_offset,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set root node in database values tree.",
		 function );

		goto on_error;
	}
	if( libfdata_tree_get_number_of_leaf_nodes(
	     database_values_tree,
	     file_io_handle,
	     database_values_cache,
	     &number_of_leaf_nodes,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of leaf nodes from database values tree.",
		 function );

		goto on_error;
	}
	for( leaf_node_index = 0;
	     leaf_node_index < number_of_leaf_nodes;
	     leaf_node_index++ )
	{
		if( libfdata_tree_get_leaf_node_by_index(
		     database_values_tree,
		     file_io_handle,
		     database_values_cache,
		     leaf_node_index,
		     &database_values_tree_node,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf node: %d from database values tree.",
			 function,
			 leaf_node_index );

			goto on_error;
		}
		if( libfdata_tree_node_get_node_value(
		     database_values_tree_node,
		     file_io_handle,
		     database_values_cache,
		     (intptr_t **) &values_tree_value,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve node value from values tree node.",
			 function );

			goto on_error;
		}
		if( libesedb_values_tree_value_read_data(
		     values_tree_value,
		     file_io_handle,
		     io_handle,
		     pages_vector,
		     pages_cache,
		     &data,
		     &data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read values tree value data.",
			 function );

			goto on_error;
		}
		/* TODO */
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( data_size > 0 )
			{
				libnotify_printf(
				 "%s: database value: %d data:\n",
				 function,
				 leaf_node_index );
				libnotify_print_data(
				 data,
				 data_size );
			}
		}
#endif
	}
	if( libfdata_cache_free(
	     &database_values_cache,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free database values cache.",
		 function );

		goto on_error;
	}
	if( libfdata_tree_free(
	     &database_values_tree,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free database values tree.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( database_values_cache != NULL )
	{
		libfdata_cache_free(
		 &database_values_cache,
		 NULL );
	}
	if( database_values_tree != NULL )
	{
		libfdata_tree_free(
		 &database_values_tree,
		 NULL );
	}
	if( database_page_tree != NULL )
	{
		libesedb_page_tree_free(
		 (intptr_t *) database_page_tree,
		 NULL );
	}
	return( -1 );
}

