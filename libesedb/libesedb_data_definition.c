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

#include "libesedb_codepage.h"
#include "libesedb_column_type.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_data_definition.h"
#include "libesedb_lcid.h"
#include "libesedb_list_type.h"
#include "libesedb_value_type.h"

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

/* Frees the data definition
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

/* Reads the data definition from the definition data using the predefined catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_catalog(
     libesedb_data_definition_t *data_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error )
{
	uint8_t *fixed_size_data_type_value_data           = NULL;
	uint8_t *variable_size_data_type_size_data         = NULL;
	uint8_t *variable_size_data_type_value_data        = NULL;
	static char *function                               = "libesedb_data_definition_read_catalog";
	uint32_t ascii_codepage                             = 0;
	uint16_t calculated_variable_size_data_types_offset = 0;
	uint16_t data_type_number                           = 0;
	uint16_t previous_variable_size_data_type_size      = 0;
	uint16_t record_offset                              = 0;
	uint16_t variable_size_data_type_size               = 0;
	uint16_t variable_size_data_types_offset            = 0;
	uint8_t amount_of_variable_size_data_types          = 0;
	uint8_t last_fixed_size_data_type                   = 0;
	uint8_t last_variable_size_data_type                = 0;
	uint8_t variable_size_data_type_iterator            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                                = 0;
	uint16_t value_16bit                                = 0;
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
	 variable_size_data_types_offset,
	 ( (esedb_data_definition_header_t *) definition_data )->variable_size_data_types_offset );

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
	 "%s: variable size data types offset\t\t\t: %" PRIu16 "\n",
	 function,
	 variable_size_data_types_offset );
#endif

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
	if( last_variable_size_data_type > 127 )
	{
		amount_of_variable_size_data_types = last_variable_size_data_type - 127;
	}
	calculated_variable_size_data_types_offset += sizeof( esedb_data_definition_header_t );

	/* Use a fall through to determine the size of the fixed size data types
	 */
	switch( last_fixed_size_data_type )
	{
		case 11:
			calculated_variable_size_data_types_offset += 2;
		case 10:
			calculated_variable_size_data_types_offset += 4;
		case 9:
			calculated_variable_size_data_types_offset += 2;
		case 8:
			if( last_variable_size_data_type > 127 )
			{
				calculated_variable_size_data_types_offset += 1 * amount_of_variable_size_data_types;
			}
		case 7:
			calculated_variable_size_data_types_offset += 4;
		case 6:
			calculated_variable_size_data_types_offset += 4;
		case 5:
			calculated_variable_size_data_types_offset += 4;
		case 4:
			calculated_variable_size_data_types_offset += 4;
		case 3:
			calculated_variable_size_data_types_offset += 4;
		case 2:
			calculated_variable_size_data_types_offset += 2;
		case 1:
			calculated_variable_size_data_types_offset += 4;
			break;
	}
	if( variable_size_data_types_offset > definition_data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: variable size data types offset exceeds definition data.",
		 function );

		return( -1 );
	}
	fixed_size_data_type_value_data = &( definition_data[ sizeof( esedb_data_definition_header_t ) ] );

	endian_little_convert_32bit(
	 data_definition->father_data_page_object_identifier,
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->father_data_page_object_identifier );

	endian_little_convert_16bit(
	 data_definition->type,
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->type );

	endian_little_convert_32bit(
	 data_definition->identifier,
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->identifier );

	if( data_definition->type == LIBESEDB_DATA_DEFINITION_TYPE_COLUMN )
	{
		endian_little_convert_32bit(
		 data_definition->column_type,
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->column_type );
	}
	else
	{
		endian_little_convert_32bit(
		 data_definition->father_data_page_number,
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->father_data_page_number );
	}
	endian_little_convert_32bit(
	 data_definition->size,
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->space_usage );

#if defined( HAVE_DEBUG_OUTPUT )
	data_type_number = 1;

	libnotify_verbose_printf(
	 "%s: (%03" PRIu16 ") father data page (FDP) object identifier\t: %" PRIu32 "\n",
	 function,
	 data_type_number++,
	 data_definition->father_data_page_object_identifier );

	libnotify_verbose_printf(
	 "%s: (%03" PRIu16 ") type\t\t\t\t\t: 0x%04" PRIx16 " ",
	 function,
	 data_type_number++,
	 data_definition->type );
	libesedb_debug_print_page_value_definition_type(
	 data_definition->type );
	libnotify_verbose_printf(
	 "\n" );

	libnotify_verbose_printf(
	 "%s: (%03" PRIu16 ") identifier\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 data_type_number++,
	 data_definition->identifier );

	if( data_definition->type == LIBESEDB_DATA_DEFINITION_TYPE_COLUMN )
	{
		libnotify_verbose_printf(
		 "%s: (%03" PRIu16 ") column type\t\t\t\t: %" PRIu32 " (%s) %s\n",
		 function,
		 data_type_number++,
		 data_definition->column_type,
		 libesedb_column_type_get_identifier(
		  data_definition->column_type ),
		 libesedb_column_type_get_description(
		  data_definition->column_type ) );
	}
	else
	{
		libnotify_verbose_printf(
		 "%s: (%03" PRIu16 ") father data page (FDP) number\t\t: %" PRIu32 "\n",
		 function,
		 data_type_number++,
		 data_definition->father_data_page_number );
	}
	libnotify_verbose_printf(
	 "%s: (%03" PRIu16 ") space usage\t\t\t\t: %" PRIu32 "\n",
	 function,
	 data_type_number++,
	 data_definition->size );

	endian_little_convert_32bit(
	 value_32bit,
	 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->flags );

	if( last_fixed_size_data_type >= 6 )
	{
		if( data_definition->type == LIBESEDB_DATA_DEFINITION_TYPE_COLUMN )
		{
			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") flags\t\t\t\t\t: ",
			 function,
			 data_type_number++ );
			libesedb_debug_print_column_group_of_bits(
			 value_32bit );
			libnotify_verbose_printf(
			 "\n" );
		}
		else if( data_definition->type == LIBESEDB_DATA_DEFINITION_TYPE_INDEX )
		{
			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") flags\t\t\t\t\t: ",
			 function,
			 data_type_number++ );
			libesedb_debug_print_index_group_of_bits(
			 value_32bit );
			libnotify_verbose_printf(
			 "\n" );
		}
		else
		{
			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 data_type_number++,
			 value_32bit );
		}
	}
	if( last_fixed_size_data_type >= 7 )
	{
		if( data_definition->type == LIBESEDB_DATA_DEFINITION_TYPE_COLUMN )
		{
			endian_little_convert_32bit(
			 ascii_codepage,
			 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->codepage );

			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") codepage\t\t\t\t\t: %" PRIu32 "",
			 function,
			 data_type_number++,
			 ascii_codepage );

			if( ascii_codepage != 0 )
			{
				libnotify_verbose_printf(
				 " (%s) %s",
				 libesedb_codepage_get_identifier(
				  ascii_codepage ),
				 libesedb_codepage_get_description(
				  ascii_codepage ) );
			}
			libnotify_verbose_printf(
			 "\n" );
		}
		else if( data_definition->type == LIBESEDB_DATA_DEFINITION_TYPE_INDEX )
		{
			endian_little_convert_32bit(
			 value_32bit,
			 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->locale_identifier );

			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") locale identifier\t\t\t\t: 0x%08" PRIx32 " (%s)\n",
			 function,
			 data_type_number++,
			 value_32bit,
			 libesedb_lcid_language_tag_get_identifier(
			  value_32bit ),
			 libesedb_lcid_language_tag_get_description(
			  value_32bit ) );
		}
		else
		{
			endian_little_convert_32bit(
			 value_32bit,
			 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->amount_of_pages );

			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") amount of pages\t\t\t\t: %" PRIu32 "\n",
			 function,
			 data_type_number++,
			 value_32bit );
		}
	}
	if( last_fixed_size_data_type >= 8 )
	{
		libnotify_verbose_printf(
		 "%s: (%03" PRIu16 ") root flag\t\t\t\t\t: 0x%02" PRIx8 "\n",
		 function,
		 data_type_number++,
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->root_flag );
	}
	if( last_fixed_size_data_type >= 9 )
	{
		endian_little_convert_16bit(
		 record_offset,
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->record_offset );

		libnotify_verbose_printf(
		 "%s: (%03" PRIu16 ") record offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 data_type_number++,
		 record_offset );
	}
	if( last_fixed_size_data_type >= 10 )
	{
		endian_little_convert_32bit(
		 value_32bit,
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->lc_map_flags );

		libnotify_verbose_printf(
		 "%s: (%03" PRIu16 ") LC map flags\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 data_type_number++,
		 value_32bit );
	}
	if( last_fixed_size_data_type >= 11 )
	{
		endian_little_convert_16bit(
		 value_16bit,
		 ( (esedb_data_definition_t *) fixed_size_data_type_value_data )->key_most );

		libnotify_verbose_printf(
		 "%s: (%03" PRIu16 ") key most\t\t\t\t\t: 0x04%" PRIx16 "\n",
		 function,
		 data_type_number++,
		 value_16bit );
	}
	libnotify_verbose_printf(
	 "\n" );
#endif

#if defined( HAVE_DEBUG_OUTPUT )
	if( variable_size_data_types_offset > calculated_variable_size_data_types_offset )
	{
		libnotify_verbose_printf(
		 "%s: trailing data:\n",
		 function );
		libnotify_verbose_print_data(
		 &( definition_data[ calculated_variable_size_data_types_offset ] ),
		 variable_size_data_types_offset - calculated_variable_size_data_types_offset );
	}
#endif

	if( amount_of_variable_size_data_types > 0 )
	{
		variable_size_data_type_size_data  = &( definition_data[ variable_size_data_types_offset ] );
		variable_size_data_type_value_data = &( variable_size_data_type_size_data[ amount_of_variable_size_data_types * 2 ] );

		/* The default codepage is 1252
		 */
		if( ascii_codepage == 0 )
		{
			ascii_codepage = LIBUNA_CODEPAGE_WINDOWS_1252;
		}
		data_type_number = 128;

		for( variable_size_data_type_iterator = 0;
		     variable_size_data_type_iterator < amount_of_variable_size_data_types;
		     variable_size_data_type_iterator++ )
		{
			endian_little_convert_16bit(
			 variable_size_data_type_size,
			 variable_size_data_type_size_data );

			variable_size_data_type_size_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") variable size data type size\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 data_type_number,
			 variable_size_data_type_size,
			 ( ( variable_size_data_type_size & 0x8000 ) != 0 ) ? 0 : ( variable_size_data_type_size & 0x7fff ) - previous_variable_size_data_type_size );
#endif

			switch( data_type_number )
			{
				case 128:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						if( libesedb_value_type_get_utf8_string_size(
						     &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						     (size_t) variable_size_data_type_size - previous_variable_size_data_type_size,
						     1,
						     ascii_codepage,
						     &( data_definition->name_size ),
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_GET_FAILED,
							 "%s: unable to determine size of name string.",
							 function );

							return( -1 );
						}
						data_definition->name = (uint8_t *) memory_allocate(
										     sizeof( uint8_t ) * data_definition->name_size );

						if( data_definition->name == NULL )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_MEMORY,
							 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
							 "%s: unable to create name string.",
							 function );

							data_definition->name_size = 0;

							return( -1 );
						}
						if( libesedb_value_type_copy_to_utf8_string(
						     &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						     (size_t) variable_size_data_type_size - previous_variable_size_data_type_size,
						     1,
						     ascii_codepage,
						     data_definition->name,
						     data_definition->name_size,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_CONVERSION,
							 LIBERROR_CONVERSION_ERROR_GENERIC,
							 "%s: unable to set name string.",
							 function );

							memory_free(
							 data_definition->name );

							data_definition->name      = NULL;
							data_definition->name_size = 0;

							return( -1 );
						}

#if defined( HAVE_DEBUG_OUTPUT )
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") name\t\t\t\t\t: %s\n",
						 function,
						 data_type_number,
						 data_definition->name );
#endif
					}
#if defined( HAVE_DEBUG_OUTPUT )
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") name\t\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
#endif
					break;

#if defined( HAVE_DEBUG_OUTPUT )
				case 129:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") stats:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") stats\t\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;

				case 130:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") temporary table:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") temporary table\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;

				case 131:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") default value:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") default value\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;

				case 132:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") KeyFldIDs:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") KeyFldIDs\t\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;

				case 133:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") VarSegMac:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") VarSegMac\t\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;

				case 134:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") ConditionalColumns:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") ConditionalColumns\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;

				case 135:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") TupleLimits:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") TupleLimits\t\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;

				case 136:
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") Version:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu8 ") TupleLimits\t\t\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
					break;
#endif

				default:
#if defined( HAVE_DEBUG_OUTPUT )
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu16 ") variable size data type:\n",
						 function,
						 data_type_number );
						libnotify_verbose_print_data(
						 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						 variable_size_data_type_size - previous_variable_size_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu16 ") variable size data type\t\t\t: <NULL>\n",
						 function,
						 data_type_number );
					}
#endif
					break;
			}
			/* The MSB signifies that the variable size data type is empty
			 */
			if( ( variable_size_data_type_size & 0x8000 ) == 0 )
			{
				previous_variable_size_data_type_size = variable_size_data_type_size;
			}
			data_type_number++;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

/* Reads the data definition from the definition data using the predefined catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read(
     libesedb_data_definition_t *data_definition,
     libesedb_list_t *column_data_definition_list,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error )
{
	libesedb_data_definition_t *column_data_definition = NULL;
	libesedb_list_element_t *list_element              = NULL;
	uint8_t *fixed_size_data_type_value_data           = NULL;
	uint8_t *variable_size_data_type_size_data         = NULL;
	uint8_t *variable_size_data_type_value_data        = NULL;
	static char *function                              = "libesedb_data_definition_read";
	size_t remaining_definition_data_size              = 0;
	uint32_t ascii_codepage                            = 0;
	uint16_t previous_variable_size_data_type_size     = 0;
	uint16_t record_offset                             = 0;
	uint16_t variable_size_data_type_size              = 0;
	uint16_t variable_size_data_types_offset           = 0;
	uint8_t amount_of_variable_size_data_types         = 0;
	uint8_t last_fixed_size_data_type                  = 0;
	uint8_t last_variable_size_data_type               = 0;
	uint8_t variable_size_data_type_iterator           = 0;

	int list_element_iterator                          = 0;

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
	if( column_data_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid colum data definition list.",
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
	 variable_size_data_types_offset,
	 ( (esedb_data_definition_header_t *) definition_data )->variable_size_data_types_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: last fixed size data type\t\t\t\t: %" PRIu8 "\n",
	 function,
	 last_fixed_size_data_type );

	libnotify_verbose_printf(
	 "%s: last variable size data type\t\t\t\t: %" PRIu8 "\n",
	 function,
	 last_variable_size_data_type );

	libnotify_verbose_printf(
	 "%s: variable size data types offset\t\t\t\t: %" PRIu16 "\n",
	 function,
	 variable_size_data_types_offset );
#endif

	if( last_variable_size_data_type > 127 )
	{
		amount_of_variable_size_data_types = last_variable_size_data_type - 127;
	}
	list_element                       = column_data_definition_list->first;
	fixed_size_data_type_value_data    = &( definition_data[ sizeof( esedb_data_definition_header_t ) ] );
	variable_size_data_type_size_data  = &( definition_data[ variable_size_data_types_offset ] );
	variable_size_data_type_value_data = &( variable_size_data_type_size_data[ amount_of_variable_size_data_types * 2 ] );

	for( list_element_iterator = 0;
	     list_element_iterator < column_data_definition_list->amount_of_elements;
	     list_element_iterator++ )
	{
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for element: %d.",
			 function,
			 list_element_iterator + 1 );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing column data definition for list element: %d.",
			 function,
			 list_element_iterator + 1 );

			return( -1 );
		}
		column_data_definition = (libesedb_data_definition_t *) list_element->value;

		if( column_data_definition->type != LIBESEDB_DATA_DEFINITION_TYPE_COLUMN )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data definition type: %" PRIu16 " for list element: %d.",
			 function,
			 column_data_definition->type,
			 list_element_iterator + 1 );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: column definition identifier\t\t\t\t: %03" PRIu32 "\n",
		 function,
		 column_data_definition->identifier );
		libnotify_verbose_printf(
		 "%s: column definition name\t\t\t\t\t: %s\n",
		 function,
		 column_data_definition->name );
		libnotify_verbose_printf(
		 "%s: column definition type\t\t\t\t\t: %s (%s)\n",
		 function,
		 libesedb_column_type_get_description(
		  column_data_definition->column_type ),
		 libesedb_column_type_get_identifier(
		  column_data_definition->column_type ) );
#endif

		if( column_data_definition->identifier <= last_fixed_size_data_type )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			libnotify_verbose_printf(
			 "%s: (%03" PRIu32 ") fixed size data type size\t\t\t\t: %" PRIu32 "\n",
			 function,
			 column_data_definition->identifier,
		 	 column_data_definition->size );

			if( libesedb_debug_print_column_value(
			     column_data_definition->column_type,
			     fixed_size_data_type_value_data,
			     column_data_definition->size,
			     LIBUNA_CODEPAGE_ASCII,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print column value.",
				 function );

				return( -1 );
			}
#endif
			fixed_size_data_type_value_data += column_data_definition->size;
		}
		else if( ( column_data_definition->identifier >= 128 )
		      && ( column_data_definition->identifier <= last_variable_size_data_type ) )
		{
			endian_little_convert_16bit(
			 variable_size_data_type_size,
			 variable_size_data_type_size_data );

			variable_size_data_type_size_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") variable size data type size\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 column_data_definition->identifier,
			 variable_size_data_type_size,
			 ( ( variable_size_data_type_size & 0x8000 ) != 0 ) ? 0 : ( variable_size_data_type_size & 0x7fff ) - previous_variable_size_data_type_size );
#endif
			/* The MSB signifies that the variable size data type is empty
			 */
			if( ( variable_size_data_type_size & 0x8000 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				libnotify_verbose_printf(
				 "%s: (%03" PRIu32 ") variable size data type:\n",
				 function,
				 column_data_definition->identifier );
				libnotify_verbose_print_data(
				 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
				 variable_size_data_type_size - previous_variable_size_data_type_size );
#endif

				previous_variable_size_data_type_size = variable_size_data_type_size;
			}
			else
			{
#if defined( HAVE_DEBUG_OUTPUT )
				libnotify_verbose_printf(
				 "%s: (%03" PRIu32 ") variable size data type\t\t\t\t: <NULL>\n",
				 function,
				 column_data_definition->identifier );
#endif
			}
		}
#ifdef XXX
		else
		{
			endian_little_convert_16bit(
			 variable_size_data_type_size,
			 variable_size_data_type_size_data );

			variable_size_data_type_size_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			libnotify_verbose_printf(
			 "%s: (%03" PRIu16 ") tagged data type size\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 column_data_definition->identifier,
			 variable_size_data_type_size,
			 ( ( variable_size_data_type_size & 0x8000 ) != 0 ) ? 0 : ( variable_size_data_type_size & 0x7fff ) - previous_variable_size_data_type_size );
#endif
			/* The MSB signifies that the variable size data type is empty
			 */
			if( ( variable_size_data_type_size & 0x8000 ) == 0 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				libnotify_verbose_printf(
				 "%s: (%03" PRIu32 ") tagged data type:\n",
				 function,
				 column_data_definition->identifier );
				libnotify_verbose_print_data(
				 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
				 variable_size_data_type_size - previous_variable_size_data_type_size );
#endif

				previous_variable_size_data_type_size = variable_size_data_type_size;
			}
			else
			{
#if defined( HAVE_DEBUG_OUTPUT )
				libnotify_verbose_printf(
				 "%s: (%03" PRIu32 ") tagged data type\t\t\t\t\t: <NULL>\n",
				 function,
				 column_data_definition->identifier );
#endif
			}
		}
#endif
		list_element = list_element->next;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( fixed_size_data_type_value_data < &( definition_data[ variable_size_data_types_offset ] ) )
	{
		libnotify_verbose_printf(
		 "%s: fixed size data types trailing data:\n",
		 function );
		libnotify_verbose_print_data(
		 fixed_size_data_type_value_data,
		 &( definition_data[ variable_size_data_types_offset ] ) - fixed_size_data_type_value_data );
	}
	uint8_t *tagged_data_type_offset_data      = NULL;
	uint8_t *tagged_data_type_value_data       = NULL;
	uint16_t tagged_data_type_offset_data_size = 0;
	uint16_t previous_tagged_data_type_offset  = 0;
	uint16_t tagged_data_type_identifier       = 0;
	uint16_t tagged_data_type_offset           = 0;
	uint16_t tagged_data_type_size             = 0;

	tagged_data_type_offset_data   = &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] );
	tagged_data_type_value_data    = &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] );
	remaining_definition_data_size = definition_data_size - (size_t) ( tagged_data_type_offset_data - definition_data );

	if( remaining_definition_data_size > 0 )
	{
		libnotify_verbose_printf(
		 "%s: tagged data types data:\n",
		 function );
		libnotify_verbose_print_data(
		 tagged_data_type_offset_data,
		 remaining_definition_data_size );

		do
		{
			endian_little_convert_16bit(
			 tagged_data_type_identifier,
			 tagged_data_type_offset_data );

			tagged_data_type_offset_data += 2;

			endian_little_convert_16bit(
			 tagged_data_type_offset,
			 tagged_data_type_offset_data );

			tagged_data_type_offset_data += 2;

			tagged_data_type_size = ( tagged_data_type_offset & 0x3fff ) - previous_tagged_data_type_offset;

			if( previous_tagged_data_type_offset != 0 )
			{
				libnotify_verbose_printf(
				 "%s: tagged data type:\n",
				 function );
				libnotify_verbose_print_data(
				 &( tagged_data_type_value_data[ previous_tagged_data_type_offset ] ),
				 tagged_data_type_size );
			}
			libnotify_verbose_printf(
			 "%s: tagged data type identifier\t\t\t: %" PRIu16 "\n",
			 function,
			 tagged_data_type_identifier );
			libnotify_verbose_printf(
			 "%s: tagged data type offset\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 tagged_data_type_offset,
			 tagged_data_type_offset & 0x3fff );

			previous_tagged_data_type_offset = tagged_data_type_offset & 0x3fff;

			if( tagged_data_type_offset_data_size == 0 )
			{
				tagged_data_type_offset_data_size = previous_tagged_data_type_offset - 4;
			}
			else
			{
				tagged_data_type_offset_data_size -= 4;
			}
			remaining_definition_data_size -= tagged_data_type_size;
		}
		while( tagged_data_type_offset_data_size > 0 );

		if( previous_tagged_data_type_offset != 0 )
		{
			libnotify_verbose_printf(
			 "%s: tagged data type:\n",
			 function );
			libnotify_verbose_print_data(
			 &( tagged_data_type_value_data[ previous_tagged_data_type_offset ] ),
			 remaining_definition_data_size );
		}
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

