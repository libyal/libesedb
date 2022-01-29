/*
 * Page value functions
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

#include "libesedb_libcerror.h"
#include "libesedb_page_value.h"

/* Creates a page value
 * Make sure the value page_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_value_initialize(
     libesedb_page_value_t **page_value,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_value_initialize";

	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page value.",
		 function );

		return( -1 );
	}
	if( *page_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page value value already set.",
		 function );

		return( -1 );
	}
	*page_value = memory_allocate_structure(
	               libesedb_page_value_t );

	if( *page_value == NULL )
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
	     *page_value,
	     0,
	     sizeof( libesedb_page_value_t ) ) == NULL )
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
	if( *page_value != NULL )
	{
		memory_free(
		 *page_value );

		*page_value = NULL;
	}
	return( -1 );
}

/* Frees a page value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_value_free(
     libesedb_page_value_t **page_value,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_value_free";

	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page value.",
		 function );

		return( -1 );
	}
	if( *page_value != NULL )
	{
		/* The data is referenced and freed elsewhere
		 */
		memory_free(
		 *page_value );

		*page_value = NULL;
	}
	return( 1 );
}

