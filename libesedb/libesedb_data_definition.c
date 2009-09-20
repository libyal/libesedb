/*
 * Data definition functions
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

#include "libesedb_array_type.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_data_definition.h"
#include "libesedb_lcid.h"

#include "esedb_page_values.h"

/* Creates a data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_initialize(
     libesedb_data_definition_t **data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_data_definition_initialize";

	if( data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( *data_definition == NULL )
	{
		*data_definition = (libesedb_data_definition_t *) memory_allocate(
		                                                   sizeof( libesedb_data_definition_t ) );

		if( *data_definition == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data definition.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *data_definition,
		     0,
		     sizeof( libesedb_data_definition_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear data definition.",
			 function );

			memory_free(
			 *data_definition );

			*data_definition = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_free(
     intptr_t *data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_data_definition_free";

	if( data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( ( ( libesedb_data_definition_t *) data_definition )->name != NULL )
	{
		memory_free(
		 ( (libesedb_data_definition_t *) data_definition )->name );
	}
	memory_free(
	data_definition );

	return( 1 );
}

/* Reads the data definition from the definition data
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read(
     libesedb_data_definition_t *data_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error )
{
	static char *function                  = "libesedb_data_definition_read";
	uint16_t calculated_size_of_data_types = 0;
	uint16_t size_of_data_types            = 0;
	uint8_t last_fixed_size_data_type      = 0;
	uint8_t last_variable_size_data_type   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit        = 0;
#endif

	if( data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
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
	if( definition_data_size < sizeof( esedb_data_definition_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: definition data too small.",
		 function );

		return( -1 );
	}
	last_fixed_size_data_type    = ( (esedb_data_definition_header_t *) definition_data )->last_fixed_size_data_type;
	last_variable_size_data_type = ( (esedb_data_definition_header_t *) definition_data )->last_variable_size_data_type;

	endian_little_convert_16bit(
	 size_of_data_types,
	 ( (esedb_data_definition_header_t *) definition_data )->size_of_data_types );

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: last fixed size data type\t\t\t: %" PRIu8 "\n",
	 function,
	 last_fixed_size_data_type );

	libnotify_verbose_printf(
	 "%s: last variable size data type\t\t\t: %" PRIu8 "\n",
	 function,
	 last_variable_size_data_type );

	libnotify_verbose_printf(
	 "%s: size of data types\t\t\t\t: %" PRIu16 "\n",
	 function,
	 size_of_data_types );
#endif

	definition_data      += sizeof( esedb_data_definition_header_t );
	definition_data_size -= sizeof( esedb_data_definition_header_t );

	/* As far as the documentation states
	 * the column data FIELD structure is 16 bytes of size
	 */
	if( last_fixed_size_data_type < 5 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: last fixed size data type too small.",
		 function );

		return( -1 );
	}
	if( last_fixed_size_data_type > 10 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported last fixed size data type: %" PRIu8 ".",
		 function,
		 last_fixed_size_data_type );

		return( -1 );
	}
	/* TODO is this right ? */
	calculated_size_of_data_types += sizeof( esedb_data_definition_header_t );

	/* Use a fall through to determine the size of the fixed size data types
	 */
	switch( last_fixed_size_data_type )
	{
		case 10:
			calculated_size_of_data_types += 4;
		case 9:
			calculated_size_of_data_types += 1;
		case 8:
			calculated_size_of_data_types += 1;
		case 7:
			calculated_size_of_data_types += 4;
		case 6:
			calculated_size_of_data_types += 4;
		case 5:
			calculated_size_of_data_types += 4;
		case 4:
			calculated_size_of_data_types += 4;
		case 3:
			calculated_size_of_data_types += 4;
		case 2:
			calculated_size_of_data_types += 2;
		case 1:
			calculated_size_of_data_types += 4;
			break;
	}
	if( last_variable_size_data_type > 127 )
	{
		calculated_size_of_data_types += last_variable_size_data_type - 127;
	}
	if( size_of_data_types != calculated_size_of_data_types )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in stored and calculated size of data types (%" PRIu16 " != %" PRIu16 ")",
		 function,
		 size_of_data_types,
		 calculated_size_of_data_types );

		return( -1 );
	}
	if( definition_data_size < size_of_data_types )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: definition data too small.",
		 function );

		return( -1 );
	}
	endian_little_convert_32bit(
	 data_definition->father_data_page_object_identifier,
	 ( (esedb_data_definition_t *) definition_data )->father_data_page_object_identifier );

	endian_little_convert_16bit(
	 data_definition->type,
	 ( (esedb_data_definition_t *) definition_data )->type );

	endian_little_convert_32bit(
	 data_definition->identifier,
	 ( (esedb_data_definition_t *) definition_data )->identifier );

	if( data_definition->type == LIBESEDB_PAGE_VALUE_DATA_DEFINITION_TYPE_COLUMN )
	{
		endian_little_convert_32bit(
		 data_definition->column_type,
		 ( (esedb_data_definition_t *) definition_data )->column_type );
	}
	else
	{
		endian_little_convert_32bit(
		 data_definition->father_data_page_number,
		 ( (esedb_data_definition_t *) definition_data )->father_data_page_number );
	}

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: father data page (FDP) object identifier\t\t: %" PRIu32 "\n",
	 function,
	 data_definition->father_data_page_object_identifier );

	libnotify_verbose_printf(
	 "%s: type\t\t\t\t\t\t: 0x%04" PRIx16 " ",
	 function,
	 data_definition->type );
	libesedb_debug_print_page_value_definition_type(
	 data_definition->type );
	libnotify_verbose_printf(
	 "\n" );

	libnotify_verbose_printf(
	 "%s: identifier\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 data_definition->identifier );

	if( data_definition->type == LIBESEDB_PAGE_VALUE_DATA_DEFINITION_TYPE_COLUMN )
	{
		libnotify_verbose_printf(
		 "%s: column type\t\t\t\t\t: 0x%08" PRIx32 " ",
		 function,
		 data_definition->column_type );
		libesedb_debug_print_column_type(
		 data_definition->column_type );
		libnotify_verbose_printf(
		 "\n" );
	}
	else
	{
		libnotify_verbose_printf(
		 "%s: father data page (FDP) number\t\t\t: %" PRIu32 "\n",
		 function,
		 data_definition->father_data_page_number );
	}
	endian_little_convert_32bit(
	 value_32bit,
	 ( (esedb_data_definition_t *) definition_data )->space_usage );

	libnotify_verbose_printf(
	 "%s: space usage\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 value_32bit );

	endian_little_convert_32bit(
	 value_32bit,
	 ( (esedb_data_definition_t *) definition_data )->flags );

	if( last_fixed_size_data_type >= 6 )
	{
		if( data_definition->type == LIBESEDB_PAGE_VALUE_DATA_DEFINITION_TYPE_COLUMN )
		{
			libnotify_verbose_printf(
			 "%s: flags\t\t\t\t\t\t: ",
			 function );
			libesedb_debug_print_column_group_of_bits(
			 value_32bit );
			libnotify_verbose_printf(
			 "\n" );
		}
		else if( data_definition->type == LIBESEDB_PAGE_VALUE_DATA_DEFINITION_TYPE_INDEX )
		{
			libnotify_verbose_printf(
			 "%s: flags\t\t\t\t\t\t: ",
			 function );
			libesedb_debug_print_index_group_of_bits(
			 value_32bit );
			libnotify_verbose_printf(
			 "\n" );
		}
		else
		{
			libnotify_verbose_printf(
			 "%s: flags\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
	}
	if( last_fixed_size_data_type >= 7 )
	{
		if( data_definition->type == LIBESEDB_PAGE_VALUE_DATA_DEFINITION_TYPE_COLUMN )
		{
			endian_little_convert_32bit(
			 value_32bit,
			 ( (esedb_data_definition_t *) definition_data )->codepage );

			libnotify_verbose_printf(
			 "%s: codepage\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_32bit );
		}
		else if( data_definition->type == LIBESEDB_PAGE_VALUE_DATA_DEFINITION_TYPE_INDEX )
		{
			endian_little_convert_32bit(
			 value_32bit,
			 ( (esedb_data_definition_t *) definition_data )->locale_identifier );

			libnotify_verbose_printf(
			 "%s: locale identifier\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
			 function,
			 value_32bit,
			 libesedb_lcid_language_tag_get_identifier(
			  value_32bit ) );
		}
		else
		{
			endian_little_convert_32bit(
			 value_32bit,
			 ( (esedb_data_definition_t *) definition_data )->amount_of_pages );

			libnotify_verbose_printf(
			 "%s: amount of pages\t\t\t\t\t: %" PRIu32 "\n",
			 function,
			 value_32bit );
		}
	}
	if( last_fixed_size_data_type >= 8 )
	{
		libnotify_verbose_printf(
		 "%s: root flag\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (esedb_data_definition_t *) definition_data )->root_flag );
	}
	if( last_fixed_size_data_type >= 9 )
	{
		libnotify_verbose_printf(
		 "%s: record offset\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 ( (esedb_data_definition_t *) definition_data )->record_offset );
	}
	if( last_fixed_size_data_type >= 10 )
	{
		endian_little_convert_32bit(
		 value_32bit,
		 ( (esedb_data_definition_t *) definition_data )->lc_map_flags );

		libnotify_verbose_printf(
		 "%s: LC map flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
	}
	libnotify_verbose_printf(
	 "\n" );
#endif
	return( 1 );
}

