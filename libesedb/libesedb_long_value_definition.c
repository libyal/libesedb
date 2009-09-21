/*
 * Long Value definition functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_long_value_definition.h"

#include "esedb_page_values.h"

/* Creates a long value definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_definition_initialize(
     libesedb_long_value_definition_t **long_value_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_long_value_definition_initialize";

	if( long_value_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value definition.",
		 function );

		return( -1 );
	}
	if( *long_value_definition == NULL )
	{
		*long_value_definition = (libesedb_long_value_definition_t *) memory_allocate(
		                                                               sizeof( libesedb_long_value_definition_t ) );

		if( *long_value_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create long value definition.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *long_value_definition,
		     0,
		     sizeof( libesedb_long_value_definition_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear long value definition.",
			 function );

			memory_free(
			 *long_value_definition );

			*long_value_definition = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees long value definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_definition_free(
     intptr_t *long_value_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_long_value_definition_free";

	if( long_value_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value definition.",
		 function );

		return( -1 );
	}
	if( ( (libesedb_long_value_definition_t *) long_value_definition )->name != NULL )
	{
		memory_free(
		 ( (libesedb_long_value_definition_t *) long_value_definition )->name );
	}
	memory_free(
	 long_value_definition );

	return( 1 );
}

/* Reads the long value definition from the definition data
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_definition_read(
     libesedb_long_value_definition_t *long_value_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     uint16_t definition_flags,
     liberror_error_t **error )
{
	static char *function = "libesedb_long_value_definition_read";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t test         = 0;
#endif

	if( long_value_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value definition.",
		 function );

		return( -1 );
	}
	if( definition_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid definition data.",
		 function );

		return( -1 );
	}
	if( definition_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid definition data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( definition_data_size != sizeof( esedb_long_value_definition_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported definition data size.",
		 function );

		return( -1 );
	}
	if( definition_flags != 0x8007 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported definition flags: 0x%08" PRIx32 ".",
		 function,
		 definition_flags );

		return( -1 );
	}
	endian_little_convert_32bit(
	 long_value_definition->father_data_page_object_identifier,
	 ( ( esedb_long_value_definition_t *) definition_data )->father_data_page_object_identifier );
	endian_little_convert_32bit(
	 long_value_definition->father_data_page_number,
	 ( ( esedb_long_value_definition_t *) definition_data )->father_data_page_number );

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: father data page (FDP) object identifier\t: %" PRIu32 "\n",
	 function,
	 long_value_definition->father_data_page_object_identifier );
	libnotify_verbose_printf(
	 "%s: father data page (FDP) number\t\t: %" PRIu32 "\n",
	 function,
	 long_value_definition->father_data_page_number );

	endian_little_convert_32bit(
	 test,
	 ( ( esedb_long_value_definition_t *) definition_data )->initial_amount_of_pages );

	libnotify_verbose_printf(
	 "%s: initial amount of pages\t\t\t\t: %" PRIu32 "\n",
	 function,
	 test );

	endian_little_convert_32bit(
	 test,
	 ( ( esedb_long_value_definition_t *) definition_data )->unknown1 );

	libnotify_verbose_printf(
	 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 "\n",
	 function,
	 test );

	endian_little_convert_32bit(
	 test,
	 ( ( esedb_long_value_definition_t *) definition_data )->extent_space );

	libnotify_verbose_printf(
	 "%s: extent space\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 test );

	libnotify_verbose_printf(
	 "%s: unknown2\t\t\t\t\t: 0x%02" PRIx8 "\n",
	 function,
	 ( ( esedb_long_value_definition_t *) definition_data )->unknown2 );

	libnotify_verbose_printf(
	 "\n" );
#endif
	return( 1 );
}

