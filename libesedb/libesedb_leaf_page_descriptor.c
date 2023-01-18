/*
 * Leaf page descriptor functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libesedb_leaf_page_descriptor.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"

/* Creates a page value
 * Make sure the value leaf_page_descriptor is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_leaf_page_descriptor_initialize(
     libesedb_leaf_page_descriptor_t **leaf_page_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libesedb_leaf_page_descriptor_initialize";

	if( leaf_page_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page value.",
		 function );

		return( -1 );
	}
	if( *leaf_page_descriptor != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page value value already set.",
		 function );

		return( -1 );
	}
	*leaf_page_descriptor = memory_allocate_structure(
	                         libesedb_leaf_page_descriptor_t );

	if( *leaf_page_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *leaf_page_descriptor,
	     0,
	     sizeof( libesedb_leaf_page_descriptor_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear page value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *leaf_page_descriptor != NULL )
	{
		memory_free(
		 *leaf_page_descriptor );

		*leaf_page_descriptor = NULL;
	}
	return( -1 );
}

/* Frees a page value
 * Returns 1 if successful or -1 on error
 */
int libesedb_leaf_page_descriptor_free(
     libesedb_leaf_page_descriptor_t **leaf_page_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libesedb_leaf_page_descriptor_free";

	if( leaf_page_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page value.",
		 function );

		return( -1 );
	}
	if( *leaf_page_descriptor != NULL )
	{
		/* The data is referenced and freed elsewhere
		 */
		memory_free(
		 *leaf_page_descriptor );

		*leaf_page_descriptor = NULL;
	}
	return( 1 );
}

/* Compares 2 leaf page descriptors by file reference
 * Returns LIBCDATA_COMPARE_LESS, LIBCDATA_COMPARE_EQUAL, LIBCDATA_COMPARE_GREATER if successful or -1 on error
 */
int libesedb_leaf_page_descriptor_compare(
     libesedb_leaf_page_descriptor_t *first_leaf_page_descriptor,
     libesedb_leaf_page_descriptor_t *second_leaf_page_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libesedb_leaf_page_descriptor_compare";

	if( first_leaf_page_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first leaf page descriptor.",
		 function );

		return( -1 );
	}
	if( second_leaf_page_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid second leaf page descriptor.",
		 function );

		return( -1 );
	}
	if( first_leaf_page_descriptor->last_leaf_value_index < second_leaf_page_descriptor->first_leaf_value_index )
	{
		return( LIBCDATA_COMPARE_LESS );
	}
	else if( first_leaf_page_descriptor->first_leaf_value_index > second_leaf_page_descriptor->last_leaf_value_index )
	{
		return( LIBCDATA_COMPARE_GREATER );
	}
	return( LIBCDATA_COMPARE_EQUAL );
}

