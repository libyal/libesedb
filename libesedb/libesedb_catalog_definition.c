/*
 * Catalog definition functions
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
#include <byte_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_codepage.h"
#include "libesedb_column_type.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_lcid.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libuna.h"
#include "libesedb_unused.h"

#include "esedb_page_values.h"

#if !defined( LIBESEDB_ATTRIBUTE_FALLTHROUGH )
#if defined( __GNUC__ ) && __GNUC__ >= 7
#define LIBESEDB_ATTRIBUTE_FALLTHROUGH	__attribute__ ((fallthrough))
#else
#define LIBESEDB_ATTRIBUTE_FALLTHROUGH
#endif
#endif

/* Creates a catalog definition
 * Make sure the value catalog_definition is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_initialize(
     libesedb_catalog_definition_t **catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_initialize";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( *catalog_definition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid catalog definition value already set.",
		 function );

		return( -1 );
	}
	*catalog_definition = memory_allocate_structure(
	                       libesedb_catalog_definition_t );

	if( *catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create catalog definition.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *catalog_definition,
	     0,
	     sizeof( libesedb_catalog_definition_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear catalog definition.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *catalog_definition != NULL )
	{
		memory_free(
		 *catalog_definition );

		*catalog_definition = NULL;
	}
	return( -1 );
}

/* Frees a catalog definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_free(
     libesedb_catalog_definition_t **catalog_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_free";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( *catalog_definition != NULL )
	{
		if( ( *catalog_definition )->name != NULL )
		{
			memory_free(
			 ( *catalog_definition )->name );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( *catalog_definition )->name_string != NULL )
		{
			memory_free(
			 ( *catalog_definition )->name_string );
		}
#endif
		if( ( *catalog_definition )->template_name != NULL )
		{
			memory_free(
			 ( *catalog_definition )->template_name );
		}
		if( ( *catalog_definition )->default_value != NULL )
		{
			memory_free(
			 ( *catalog_definition )->default_value );
		}
		memory_free(
		 *catalog_definition );

		*catalog_definition = NULL;
	}
	return( 1 );
}

/* Reads the catalog definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_read_data(
     libesedb_catalog_definition_t *catalog_definition,
     const uint8_t *data,
     size_t data_size,
     int ascii_codepage LIBESEDB_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	const uint8_t *fixed_size_data_type_value_data      = NULL;
	const uint8_t *variable_size_data_type_value_data   = NULL;
	static char *function                               = "libesedb_catalog_definition_read_data";
	size_t remaining_data_size                          = 0;
	size_t variable_size_data_type_value_data_offset    = 0;
	uint16_t calculated_variable_size_data_types_offset = 0;
	uint16_t data_type_number                           = 0;
	uint16_t data_type_size                             = 0;
	uint16_t previous_variable_size_data_type_size      = 0;
	uint16_t variable_size_data_type_size               = 0;
	uint16_t variable_size_data_types_offset            = 0;
	uint8_t last_fixed_size_data_type                   = 0;
	uint8_t last_variable_size_data_type                = 0;
	uint8_t number_of_variable_size_data_types          = 0;
	uint8_t variable_size_data_type_iterator            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	system_character_t *value_string                    = 0;
	size_t value_string_size                            = 0;
	uint32_t value_32bit                                = 0;
	uint16_t record_offset                              = 0;
	uint16_t value_16bit                                = 0;
	int result                                          = 0;
#endif

	LIBESEDB_UNREFERENCED_PARAMETER( ascii_codepage )

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
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
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( esedb_data_definition_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data definition:\n",
		 function );
		libcnotify_print_data(
		 data,
		 data_size,
		 0 );
	}
#endif
	last_fixed_size_data_type    = ( (esedb_data_definition_header_t *) data )->last_fixed_size_data_type;
	last_variable_size_data_type = ( (esedb_data_definition_header_t *) data )->last_variable_size_data_type;

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_data_definition_header_t *) data )->variable_size_data_types_offset,
	 variable_size_data_types_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: last fixed size data type\t\t\t: %" PRIu8 "\n",
		 function,
		 last_fixed_size_data_type );

		libcnotify_printf(
		 "%s: last variable size data type\t\t\t: %" PRIu8 "\n",
		 function,
		 last_variable_size_data_type );

		libcnotify_printf(
		 "%s: variable size data types offset\t\t\t: %" PRIu16 "\n",
		 function,
		 variable_size_data_types_offset );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	/* As far as the documentation states
	 * the column data FIELD structure is 16 bytes of size
	 */
	if( last_fixed_size_data_type < 5 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: last fixed size data type too small.",
		 function );

		return( -1 );
	}
	if( last_fixed_size_data_type > 12 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported last fixed size data type: %" PRIu8 ".",
		 function,
		 last_fixed_size_data_type );

		return( -1 );
	}
	if( last_variable_size_data_type > 127 )
	{
		number_of_variable_size_data_types = last_variable_size_data_type - 127;
	}
	calculated_variable_size_data_types_offset += sizeof( esedb_data_definition_header_t );

	/* Use a fall through to determine the size of the fixed size data types
	 */
	switch( last_fixed_size_data_type )
	{
		case 12:
			calculated_variable_size_data_types_offset += 4;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 11:
			calculated_variable_size_data_types_offset += 2;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 10:
			calculated_variable_size_data_types_offset += 4;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 9:
			calculated_variable_size_data_types_offset += 2;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 8:
			if( last_variable_size_data_type > 127 )
			{
				calculated_variable_size_data_types_offset += 1 * number_of_variable_size_data_types;
			}

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 7:
			calculated_variable_size_data_types_offset += 4;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 6:
			calculated_variable_size_data_types_offset += 4;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 5:
			calculated_variable_size_data_types_offset += 4;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 4:
			calculated_variable_size_data_types_offset += 4;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 3:
			calculated_variable_size_data_types_offset += 4;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 2:
			calculated_variable_size_data_types_offset += 2;

		LIBESEDB_ATTRIBUTE_FALLTHROUGH;
		case 1:
			calculated_variable_size_data_types_offset += 4;
			break;
	}
	if( ( variable_size_data_types_offset < sizeof( esedb_data_definition_header_t ) )
	 || ( variable_size_data_types_offset > data_size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: variable size data types offset value out of bounds.",
		 function );

		return( -1 );
	}
	fixed_size_data_type_value_data = &( data[ sizeof( esedb_data_definition_header_t ) ] );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->father_data_page_object_identifier,
	 catalog_definition->father_data_page_object_identifier );

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->type,
	 catalog_definition->type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->identifier,
	 catalog_definition->identifier );

	if( catalog_definition->type == LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->column_type,
		 catalog_definition->column_type );
	}
	else
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->father_data_page_number,
		 catalog_definition->father_data_page_number );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->space_usage,
	 catalog_definition->size );

	if( catalog_definition->type == LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->codepage,
		 catalog_definition->codepage );
	}
	if( last_fixed_size_data_type >= 10 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->lc_map_flags,
		 catalog_definition->lcmap_flags );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		data_type_number = 1;

		libcnotify_printf(
		 "%s: (%03" PRIu16 ") father data page (FDP) object identifier\t: %" PRIu32 "\n",
		 function,
		 data_type_number++,
		 catalog_definition->father_data_page_object_identifier );

		libcnotify_printf(
		 "%s: (%03" PRIu16 ") type\t\t\t\t\t: 0x%04" PRIx16 " ",
		 function,
		 data_type_number++,
		 catalog_definition->type );
		libesedb_debug_print_page_value_definition_type(
		 catalog_definition->type );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: (%03" PRIu16 ") identifier\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_type_number++,
		 catalog_definition->identifier );

		if( catalog_definition->type == LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
		{
			libcnotify_printf(
			 "%s: (%03" PRIu16 ") column type\t\t\t\t: %" PRIu32 " (%s) %s\n",
			 function,
			 data_type_number++,
			 catalog_definition->column_type,
			 libesedb_column_type_get_identifier(
			  catalog_definition->column_type ),
			 libesedb_column_type_get_description(
			  catalog_definition->column_type ) );
		}
		else
		{
			libcnotify_printf(
			 "%s: (%03" PRIu16 ") father data page (FDP) number\t\t: %" PRIu32 "\n",
			 function,
			 data_type_number++,
			 catalog_definition->father_data_page_number );
		}
		libcnotify_printf(
		 "%s: (%03" PRIu16 ") space usage\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_type_number++,
		 catalog_definition->size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->flags,
		 value_32bit );

		if( last_fixed_size_data_type >= 6 )
		{
			if( catalog_definition->type == LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
			{
				libcnotify_printf(
				 "%s: (%03" PRIu16 ") flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 data_type_number++,
				 value_32bit );
				libesedb_debug_print_column_group_of_bits(
				 value_32bit );
				libcnotify_printf(
				 "\n" );
			}
			else if( catalog_definition->type == LIBESEDB_CATALOG_DEFINITION_TYPE_INDEX )
			{
				libcnotify_printf(
				 "%s: (%03" PRIu16 ") flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 data_type_number++,
				 value_32bit );
				libesedb_debug_print_index_group_of_bits(
				 value_32bit );
				libcnotify_printf(
				 "\n" );
			}
			else
			{
				libcnotify_printf(
				 "%s: (%03" PRIu16 ") flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 data_type_number++,
				 value_32bit );
			}
		}
		if( last_fixed_size_data_type >= 7 )
		{
			if( catalog_definition->type == LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
			{
				libcnotify_printf(
				 "%s: (%03" PRIu16 ") codepage\t\t\t\t\t: %" PRIu32 "",
				 function,
				 data_type_number++,
				 catalog_definition->codepage );

				if( catalog_definition->codepage != 0 )
				{
					libcnotify_printf(
					 " (%s) %s",
					 libesedb_codepage_get_identifier(
					  catalog_definition->codepage ),
					 libesedb_codepage_get_description(
					  catalog_definition->codepage ) );
				}
				libcnotify_printf(
				 "\n" );
			}
			else if( catalog_definition->type == LIBESEDB_CATALOG_DEFINITION_TYPE_INDEX )
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->locale_identifier,
				 value_32bit );

				libcnotify_printf(
				 "%s: (%03" PRIu16 ") locale identifier\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
				 function,
				 data_type_number++,
				 value_32bit,
				 libesedb_lcid_language_tag_get_identifier(
				  (uint16_t) value_32bit ),
				 libesedb_lcid_language_tag_get_description(
				  (uint16_t) value_32bit ) );
			}
			else
			{
				byte_stream_copy_to_uint32_little_endian(
				 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->number_of_pages,
				 value_32bit );

				libcnotify_printf(
				 "%s: (%03" PRIu16 ") number of pages\t\t\t\t: %" PRIu32 "\n",
				 function,
				 data_type_number++,
				 value_32bit );
			}
		}
		if( last_fixed_size_data_type >= 8 )
		{
			libcnotify_printf(
			 "%s: (%03" PRIu16 ") root flag\t\t\t\t\t: 0x%02" PRIx8 "\n",
			 function,
			 data_type_number++,
			 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->root_flag );
		}
		if( last_fixed_size_data_type >= 9 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->record_offset,
			 record_offset );

			libcnotify_printf(
			 "%s: (%03" PRIu16 ") record offset\t\t\t\t: %" PRIu16 "\n",
			 function,
			 data_type_number++,
			 record_offset );
		}
		if( last_fixed_size_data_type >= 10 )
		{
			libcnotify_printf(
			 "%s: (%03" PRIu16 ") locale map (LCMAP) flags\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_type_number++,
			 catalog_definition->lcmap_flags );
			libesedb_debug_print_lcmap_flags(
			 catalog_definition->lcmap_flags );
		}
		if( last_fixed_size_data_type >= 11 )
		{
			byte_stream_copy_to_uint16_little_endian(
			 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->key_most,
			 value_16bit );

			libcnotify_printf(
			 "%s: (%03" PRIu16 ") key most\t\t\t\t: 0x04%" PRIx16 "\n",
			 function,
			 data_type_number++,
			 value_16bit );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( variable_size_data_types_offset > calculated_variable_size_data_types_offset )
		{
			libcnotify_printf(
			 "%s: fixed size data types trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ calculated_variable_size_data_types_offset ] ),
			 variable_size_data_types_offset - calculated_variable_size_data_types_offset,
			 0 );
		}
	}
#endif
	if( number_of_variable_size_data_types > 0 )
	{
		variable_size_data_type_value_data_offset = variable_size_data_types_offset + ( number_of_variable_size_data_types * 2 );

		if( variable_size_data_type_value_data_offset > data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: variable size data type value data offset exceeds data.",
			 function );

			return( -1 );
		}
		variable_size_data_type_value_data = &( data[ variable_size_data_type_value_data_offset ] );
		remaining_data_size                = data_size - variable_size_data_type_value_data_offset;

		data_type_number = 128;

		for( variable_size_data_type_iterator = 0;
		     variable_size_data_type_iterator < number_of_variable_size_data_types;
		     variable_size_data_type_iterator++ )
		{
			if( variable_size_data_types_offset > ( data_size - 2 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: variable size data types offset value out of bounds.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint16_little_endian(
			 &( data[ variable_size_data_types_offset ] ),
			 variable_size_data_type_size );

			variable_size_data_types_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: (%03" PRIu16 ") variable size data type size\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
				 function,
				 data_type_number,
				 variable_size_data_type_size,
				 ( ( variable_size_data_type_size & 0x8000 ) != 0 ) ? 0 : ( variable_size_data_type_size & 0x7fff ) - previous_variable_size_data_type_size );
			}
#endif
			/* The MSB signifies that the variable size data type is empty
			 */
			if( ( variable_size_data_type_size & 0x8000 ) != 0 )
			{
				data_type_size = 0;
			}
			else
			{
				data_type_size = variable_size_data_type_size - previous_variable_size_data_type_size;

				if( ( previous_variable_size_data_type_size > remaining_data_size )
				 || ( data_type_size > ( remaining_data_size - previous_variable_size_data_type_size ) ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid data type size value out of bounds.",
					 function );

					return( -1 );
				}
			}
			switch( data_type_number )
			{
				case 128:
					if( data_type_size > 0 )
					{
						catalog_definition->name = (uint8_t *) memory_allocate(
										        sizeof( uint8_t ) * data_type_size );

						if( catalog_definition->name == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
							 "%s: unable to create name.",
							 function );

							return( -1 );
						}
						catalog_definition->name_size = (size_t) data_type_size;

						if( memory_copy(
						     catalog_definition->name,
						     &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						     catalog_definition->name_size ) == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
							 "%s: unable to set name.",
							 function );

							memory_free(
							 catalog_definition->name );

							catalog_definition->name      = NULL;
							catalog_definition->name_size = 0;

							return( -1 );
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
							result = libuna_utf16_string_size_from_byte_stream(
							          catalog_definition->name,
							          catalog_definition->name_size,
							          ascii_codepage,
							          &value_string_size,
							          error );
#else
							result = libuna_utf8_string_size_from_byte_stream(
							          catalog_definition->name,
							          catalog_definition->name_size,
							          ascii_codepage,
							          &value_string_size,
							          error );
#endif

							if( result != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
								 "%s: unable to determine size of name string.",
								 function );

								return( -1 );
							}
							catalog_definition->name_string = system_string_allocate(
							                                   value_string_size );

							if( catalog_definition->name_string == NULL )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_MEMORY,
								 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
								 "%s: unable to create name string.",
								 function );

								return( -1 );
							}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
							result = libuna_utf16_string_copy_from_byte_stream(
							          (libuna_utf16_character_t *) catalog_definition->name_string,
							          value_string_size,
							          catalog_definition->name,
							          catalog_definition->name_size,
							          ascii_codepage,
							          error );
#else
							result = libuna_utf8_string_copy_from_byte_stream(
							          (libuna_utf8_character_t *) catalog_definition->name_string,
							          value_string_size,
							          catalog_definition->name,
							          catalog_definition->name_size,
							          ascii_codepage,
							          error );
#endif

							if( result != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_CONVERSION,
								 LIBCERROR_CONVERSION_ERROR_GENERIC,
								 "%s: unable to set name string.",
								 function );

								memory_free(
								 catalog_definition->name_string );

								catalog_definition->name_string = NULL;

								return( -1 );
							}
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") name\t\t\t\t\t: %" PRIs_SYSTEM "\n",
							 function,
							 data_type_number,
							 catalog_definition->name_string );
						}
#endif
					}
#if defined( HAVE_DEBUG_OUTPUT )
					else if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu8 ") name\t\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
#endif
					break;

#if defined( HAVE_DEBUG_OUTPUT )
				case 129:
					if( libcnotify_verbose != 0 )
					{
						if( data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") stats:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 data_type_size,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") stats\t\t\t\t\t: <NULL>\n",
							 function,
							 data_type_number );
						}
					}
					break;
#endif

				case 130:
					if( data_type_size > 0 )
					{
						catalog_definition->template_name = (uint8_t *) memory_allocate(
						                                                 sizeof( uint8_t ) * data_type_size );

						if( catalog_definition->template_name == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
							 "%s: unable to create template name.",
							 function );

							return( -1 );
						}
						catalog_definition->template_name_size = (size_t) data_type_size;

						if( memory_copy(
						     catalog_definition->template_name,
						     &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						     catalog_definition->template_name_size ) == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
							 "%s: unable to set template name.",
							 function );

							memory_free(
							 catalog_definition->template_name );

							catalog_definition->template_name      = NULL;
							catalog_definition->template_name_size = 0;

							return( -1 );
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
							result = libuna_utf16_string_size_from_byte_stream(
							          catalog_definition->template_name,
							          catalog_definition->template_name_size,
							          ascii_codepage,
							          &value_string_size,
							          error );
#else
							result = libuna_utf8_string_size_from_byte_stream(
							          catalog_definition->template_name,
							          catalog_definition->template_name_size,
							          ascii_codepage,
							          &value_string_size,
							          error );
#endif

							if( result != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
								 "%s: unable to determine size of template name string.",
								 function );

								return( -1 );
							}
							value_string = system_string_allocate(
							                value_string_size );

							if( value_string == NULL )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_MEMORY,
								 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
								 "%s: unable to create template name string.",
								 function );

								return( -1 );
							}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
							result = libuna_utf16_string_copy_from_byte_stream(
							          (libuna_utf16_character_t *) value_string,
							          value_string_size,
							          catalog_definition->template_name,
							          catalog_definition->template_name_size,
							          ascii_codepage,
							          error );
#else
							result = libuna_utf8_string_copy_from_byte_stream(
							          (libuna_utf8_character_t *) value_string,
							          value_string_size,
							          catalog_definition->template_name,
							          catalog_definition->template_name_size,
							          ascii_codepage,
							          error );
#endif

							if( result != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_CONVERSION,
								 LIBCERROR_CONVERSION_ERROR_GENERIC,
								 "%s: unable to set template name string.",
								 function );

								memory_free(
								 value_string );

								return( -1 );
							}
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") template name\t\t\t\t: %" PRIs_SYSTEM "\n",
							 function,
							 data_type_number,
							 value_string );

							memory_free(
							 value_string );
						}
#endif
					}
#if defined( HAVE_DEBUG_OUTPUT )
					else if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu8 ") template name\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
#endif
					break;

				case 131:
					if( data_type_size > 0 )
					{
						catalog_definition->default_value = (uint8_t *) memory_allocate(
												 sizeof( uint8_t ) * data_type_size );

						if( catalog_definition->default_value == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
							 "%s: unable to create default value.",
							 function );

							return( -1 );
						}
						catalog_definition->default_value_size = (size_t) data_type_size;

						if( memory_copy(
						     catalog_definition->default_value,
						     &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						     catalog_definition->default_value_size ) == NULL )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_MEMORY,
							 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
							 "%s: unable to set default value.",
							 function );

							memory_free(
							 catalog_definition->default_value );

							catalog_definition->default_value      = NULL;
							catalog_definition->default_value_size = 0;

							return( -1 );
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") default value:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 catalog_definition->default_value,
							 catalog_definition->default_value_size,
							 0 );
						}
#endif
					}
#if defined( HAVE_DEBUG_OUTPUT )
					else if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu8 ") default value\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
#endif
					break;

#if defined( HAVE_DEBUG_OUTPUT )
				case 132:
					if( libcnotify_verbose != 0 )
					{
						if( data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") KeyFldIDs:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 data_type_size,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") KeyFldIDs\t\t\t\t: <NULL>\n",
							 function,
							 data_type_number );
						}
					}
					break;

				case 133:
					if( libcnotify_verbose != 0 )
					{
						if( data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") VarSegMac:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 data_type_size,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") VarSegMac\t\t\t\t\t: <NULL>\n",
							 function,
							 data_type_number );
						}
					}
					break;

				case 134:
					if( libcnotify_verbose != 0 )
					{
						if( data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") ConditionalColumns:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 data_type_size,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") ConditionalColumns\t\t\t\t: <NULL>\n",
							 function,
							 data_type_number );
						}
					}
					break;

				case 135:
					if( libcnotify_verbose != 0 )
					{
						if( data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") TupleLimits:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 data_type_size,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") TupleLimits\t\t\t\t: <NULL>\n",
							 function,
							 data_type_number );
						}
					}
					break;

				case 136:
					if( libcnotify_verbose != 0 )
					{
						if( data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") Version:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 data_type_size,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu8 ") Version\t\t\t\t: <NULL>\n",
							 function,
							 data_type_number );
						}
					}
					break;
#endif

				default:
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu16 ") variable size data type:\n",
							 function,
							 data_type_number );
							libcnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 data_type_size,
							 0 );
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu16 ") variable size data type\t\t: <NULL>\n",
							 function,
							 data_type_number );
						}
					}
#endif
					break;
			}
			if( data_type_size > 0 )
			{
				previous_variable_size_data_type_size = variable_size_data_type_size;
			}
			data_type_number++;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Retrieves the catalog definition identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_identifier(
     libesedb_catalog_definition_t *catalog_definition,
     uint32_t *identifier,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_identifier";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = catalog_definition->identifier;

	return( 1 );
}

/* Retrieves the catalog definition column type
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_column_type(
     libesedb_catalog_definition_t *catalog_definition,
     uint32_t *column_type,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_column_type";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( column_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column type.",
		 function );

		return( -1 );
	}
	*column_type = catalog_definition->column_type;

	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded string string of the catalog definition name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf8_name_size(
     libesedb_catalog_definition_t *catalog_definition,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf8_name_size";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_size_from_byte_stream(
	     catalog_definition->name,
	     catalog_definition->name_size,
	     ascii_codepage,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded string of the catalog definition name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf8_name(
     libesedb_catalog_definition_t *catalog_definition,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf8_name";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_byte_stream(
	     utf8_string,
	     utf8_string_size,
	     catalog_definition->name,
	     catalog_definition->name_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded string of the catalog definition name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf16_name_size(
     libesedb_catalog_definition_t *catalog_definition,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf16_name_size";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_size_from_byte_stream(
	     catalog_definition->name,
	     catalog_definition->name_size,
	     ascii_codepage,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded string of the catalog definition name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf16_name(
     libesedb_catalog_definition_t *catalog_definition,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf16_name";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_byte_stream(
	     utf16_string,
	     utf16_string_size,
	     catalog_definition->name,
	     catalog_definition->name_size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the UTF-8 encoded string string of the catalog definition template name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf8_template_name_size(
     libesedb_catalog_definition_t *catalog_definition,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf8_template_name_size";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( catalog_definition->template_name == NULL )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_byte_stream(
		     catalog_definition->template_name,
		     catalog_definition->template_name_size,
		     ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 encoded string of the catalog definition template name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf8_template_name(
     libesedb_catalog_definition_t *catalog_definition,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf8_template_name";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( catalog_definition->template_name != NULL )
	{
		if( libuna_utf8_string_copy_from_byte_stream(
		     utf8_string,
		     utf8_string_size,
		     catalog_definition->template_name,
		     catalog_definition->template_name_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the size of the UTF-16 encoded string of the catalog definition template name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf16_template_name_size(
     libesedb_catalog_definition_t *catalog_definition,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf16_template_name_size";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( catalog_definition->template_name == NULL )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_byte_stream(
		     catalog_definition->template_name,
		     catalog_definition->template_name_size,
		     ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 encoded string of the catalog definition template name
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_catalog_definition_get_utf16_template_name(
     libesedb_catalog_definition_t *catalog_definition,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_get_utf16_template_name";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( catalog_definition->template_name != NULL )
	{
		if( libuna_utf16_string_copy_from_byte_stream(
		     utf16_string,
		     utf16_string_size,
		     catalog_definition->template_name,
		     catalog_definition->template_name_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Compares the name of the table definition with a name
 * Returns 1 if equal, 0 if not or -1 on error
 */
int libesedb_catalog_definition_compare_name(
     libesedb_catalog_definition_t *catalog_definition,
     const uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_compare_name";

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( catalog_definition->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog definition - missing name.",
		 function );

		return( -1 );
	}
	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( ( name_size == 0 )
	 || ( name_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( name_size == catalog_definition->name_size )
	{
		if( memory_compare(
		     catalog_definition->name,
		     name,
		     name_size ) == 0 )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Compares the name of the table definition with an UTF-8 encoded string
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libesedb_catalog_definition_compare_name_with_utf8_string(
     libesedb_catalog_definition_t *catalog_definition,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_compare_name_with_utf8_string";
	int result            = 0;

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( catalog_definition->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog definition - missing name.",
		 function );

		return( -1 );
	}
/* TODO use ascii codepage */
	result = libuna_utf8_string_compare_with_byte_stream(
		  utf8_string,
		  utf8_string_length,
		  catalog_definition->name,
		  catalog_definition->name_size,
		  LIBUNA_CODEPAGE_WINDOWS_1252,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to compare UTF-8 string with catalog definition name.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Compares the name of the table definition with an UTF-16 encoded string
 * Returns LIBUNA_COMPARE_LESS, LIBUNA_COMPARE_EQUAL, LIBUNA_COMPARE_GREATER if successful or -1 on error
 */
int libesedb_catalog_definition_compare_name_with_utf16_string(
     libesedb_catalog_definition_t *catalog_definition,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcerror_error_t **error )
{
	static char *function = "libesedb_catalog_definition_compare_name_with_utf16_string";
	int result            = 0;

	if( catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid catalog definition.",
		 function );

		return( -1 );
	}
	if( catalog_definition->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid catalog definition - missing name.",
		 function );

		return( -1 );
	}
/* TODO use ascii codepage */
	result = libuna_utf16_string_compare_with_byte_stream(
		  utf16_string,
		  utf16_string_length,
		  catalog_definition->name,
		  catalog_definition->name_size,
		  LIBUNA_CODEPAGE_WINDOWS_1252,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: unable to compare UTF-16 string with catalog definition name.",
		 function );

		return( -1 );
	}
	return( result );
}

