/*
 * Column definition functions
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
#include "libesedb_column_definition.h"

#include "esedb_page_values.h"

/* Creates a column definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_definition_initialize(
     libesedb_column_definition_t **column_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_column_definition_initialize";

	if( column_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definition.",
		 function );

		return( -1 );
	}
	if( *column_definition == NULL )
	{
		*column_definition = (libesedb_column_definition_t *) memory_allocate(
		                                                       sizeof( libesedb_column_definition_t ) );

		if( *column_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create column definition.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *column_definition,
		     0,
		     sizeof( libesedb_column_definition_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear column definition.",
			 function );

			memory_free(
			 *column_definition );

			*column_definition = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees column definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_definition_free(
     intptr_t *column_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_column_definition_free";

	if( column_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definition.",
		 function );

		return( -1 );
	}
	if( ( (libesedb_column_definition_t *) column_definition )->name != NULL )
	{
		memory_free(
		 ( (libesedb_column_definition_t *) column_definition )->name );
	}
	memory_free(
	 column_definition );

	return( 1 );
}

/* Reads the column definition from the definition data
 * Returns 1 if successful or -1 on error
 */
int libesedb_column_definition_read(
     libesedb_column_definition_t *column_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     uint16_t definition_flags,
     liberror_error_t **error )
{
	static char *function = "libesedb_column_definition_read";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t column_type  = 0;
	uint32_t test         = 0;
#endif

	if( column_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column definition.",
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
	/* TODO is a size check possible for this definition type ? */
	if( ( definition_flags != 0x8007 )
	 && ( definition_flags != 0x8009 )
	 && ( definition_flags != 0x8309 ) )
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
#if defined( HAVE_DEBUG_OUTPUT )
	endian_little_convert_32bit(
	 test,
	 definition_data );

	definition_data      += 4;
	definition_data_size -= 4;

	libnotify_verbose_printf(
	 "%s: column identifier\t\t\t\t: %" PRIu32 "\n",
	 function,
	 test );

	endian_little_convert_32bit(
	 column_type,
	 definition_data );

	definition_data      += 4;
	definition_data_size -= 4;

	libnotify_verbose_printf(
	 "%s: column type\t\t\t\t\t: %" PRIu32 " ",
	 function,
	 column_type );
	libesedb_debug_print_column_type(
	 column_type );
	libnotify_verbose_printf(
	 "\n" );

	endian_little_convert_32bit(
	 test,
	 definition_data );

	definition_data      += 4;
	definition_data_size -= 4;

	libnotify_verbose_printf(
	 "%s: maximum size\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 test );

	endian_little_convert_32bit(
	 test,
	 definition_data );

	definition_data      += 4;
	definition_data_size -= 4;

	libnotify_verbose_printf(
	 "%s: column group of bits\t\t\t\t: ",
	 function );
	libesedb_debug_print_column_group_of_bits(
	 test );
	libnotify_verbose_printf(
	 "\n" );

	endian_little_convert_32bit(
	 test,
	 definition_data );

	definition_data      += 4;
	definition_data_size -= 4;

	libnotify_verbose_printf(
	 "%s: codepage\t\t\t\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
	 function,
	 test,
	 test );

	if( ( definition_flags & 0x0008 ) == 0x0008 )
	{
		endian_little_convert_16bit(
		 test,
		 definition_data );

		definition_data      += 2;
		definition_data_size -= 2;

		libnotify_verbose_printf(
		 "%s: unknown1\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 test );

		libnotify_verbose_printf(
		 "%s: unknown2\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 *definition_data );

		definition_data      += 1;
		definition_data_size -= 1;
	}
	if( ( definition_flags & 0x0001 ) == 0x0001 )
	{
		libnotify_verbose_printf(
		 "%s: unknown3\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 *definition_data );

		definition_data      += 1;
		definition_data_size -= 1;
	}
	if( ( definition_flags & 0x0008 ) == 0x0008 )
	{
		libnotify_verbose_printf(
		 "%s: unknown4\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 *definition_data );

		definition_data      += 1;
		definition_data_size -= 1;
	}
	libnotify_verbose_printf(
	 "\n" );
#endif
	return( 1 );
}

