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
#include "libesedb_catalog_definition.h"
#include "libesedb_codepage.h"
#include "libesedb_column_type.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_data_definition.h"
#include "libesedb_data_type_definition.h"
#include "libesedb_io_handle.h"
#include "libesedb_lcid.h"
#include "libesedb_list_type.h"
#include "libesedb_value_type.h"

#include "esedb_page_values.h"

enum LIBESEDB_TAGGED_DATA_TYPES_FORMATS
{
	LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR,
	LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX,
};

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
	int result            = 1;

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
	if( libesedb_array_free(
	     &( ( (libesedb_data_definition_t *) data_definition )->data_type_definitions_array ),
	     &libesedb_data_type_definition_free,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free data type definitions array.",
		 function );

		result = -1;
	}
	memory_free(
	 data_definition );

	return( result );
}

/* Reads the data definition from the definition data using the predefined catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read(
     libesedb_data_definition_t *data_definition,
     libesedb_list_t *column_catalog_definition_list,
     libesedb_io_handle_t *io_handle,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_data_type_definition_t *data_type_definition    = NULL;
	libesedb_list_element_t *list_element                    = NULL;
	uint8_t *fixed_size_data_type_value_data                 = NULL;
	uint8_t *tagged_data_type_offset_data                    = NULL;
	uint8_t *tagged_data_type_value_data                     = NULL;
	uint8_t *variable_size_data_type_size_data               = NULL;
	uint8_t *variable_size_data_type_value_data              = NULL;
	static char *function                                    = "libesedb_data_definition_read";
	size_t remaining_definition_data_size                    = 0;
	uint16_t previous_tagged_data_type_offset                = 0;
	uint16_t previous_variable_size_data_type_size           = 0;
	uint16_t tagged_data_type_offset_data_size               = 0;
	uint16_t tagged_data_type_identifier                     = 0;
	uint16_t tagged_data_type_offset                         = 0;
	uint16_t tagged_data_type_size                           = 0;
	uint16_t variable_size_data_type_size                    = 0;
	uint16_t variable_size_data_types_offset                 = 0;
	uint8_t amount_of_variable_size_data_types               = 0;
	uint8_t last_fixed_size_data_type                        = 0;
	uint8_t last_variable_size_data_type                     = 0;
	uint8_t tagged_data_types_format                         = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX;
	int amount_of_column_catalog_definitions                 = 0;
	int column_catalog_definition_iterator                   = 0;

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
	if( data_definition->data_type_definitions_array != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data definition - data type definitions array already set.",
		 function );

		return( -1 );
	}
	if( column_catalog_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid colum data definition list.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
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
	if( ( io_handle->format_version = 0x620 )
	 && ( io_handle->format_revision <= 2 ) )
	{
		tagged_data_types_format = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR;
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

	if( libesedb_list_get_amount_of_elements(
	     column_catalog_definition_list,
	     &amount_of_column_catalog_definitions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of column catalog definitions.",
		 function );

		return( -1 );
	}
	if( libesedb_array_initialize(
	     &( data_definition->data_type_definitions_array ),
	     amount_of_column_catalog_definitions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data type definitions array.",
		 function );

		return( -1 );
	}
	if( last_variable_size_data_type > 127 )
	{
		amount_of_variable_size_data_types = last_variable_size_data_type - 127;
	}
	list_element                       = column_catalog_definition_list->first;
	fixed_size_data_type_value_data    = &( definition_data[ sizeof( esedb_data_definition_header_t ) ] );
	variable_size_data_type_size_data  = &( definition_data[ variable_size_data_types_offset ] );
	variable_size_data_type_value_data = &( variable_size_data_type_size_data[ amount_of_variable_size_data_types * 2 ] );

	for( column_catalog_definition_iterator = 0;
	     column_catalog_definition_iterator < amount_of_column_catalog_definitions;
	     column_catalog_definition_iterator++ )
	{
		if( list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for element: %d.",
			 function,
			 column_catalog_definition_iterator );

			return( -1 );
		}
		if( list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing column catalog definition for list element: %d.",
			 function,
			 column_catalog_definition_iterator );

			return( -1 );
		}
		column_catalog_definition = (libesedb_catalog_definition_t *) list_element->value;

		if( column_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data definition type: %" PRIu16 " for list element: %d.",
			 function,
			 column_catalog_definition->type,
			 column_catalog_definition_iterator );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: column definition identifier\t\t\t\t: %03" PRIu32 "\n",
		 function,
		 column_catalog_definition->identifier );
		libnotify_verbose_printf(
		 "%s: column definition name\t\t\t\t\t: %s\n",
		 function,
		 column_catalog_definition->name );
		libnotify_verbose_printf(
		 "%s: column definition type\t\t\t\t\t: %s (%s)\n",
		 function,
		 libesedb_column_type_get_description(
		  column_catalog_definition->column_type ),
		 libesedb_column_type_get_identifier(
		  column_catalog_definition->column_type ) );
#endif

		if( libesedb_data_type_definition_initialize(
		     &data_type_definition,
		     column_catalog_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data type definition.",
			 function );

			return( -1 );
		}
		if( column_catalog_definition->identifier <= 127 )
		{
			if( column_catalog_definition->identifier <= last_fixed_size_data_type )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				libnotify_verbose_printf(
				 "%s: (%03" PRIu32 ") fixed size data type size\t\t\t\t: %" PRIu32 "\n",
				 function,
				 column_catalog_definition->identifier,
			 	 column_catalog_definition->size );

				if( libesedb_debug_print_column_value(
				     column_catalog_definition->column_type,
				     fixed_size_data_type_value_data,
				     column_catalog_definition->size,
				     LIBUNA_CODEPAGE_ASCII,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
					 "%s: unable to print column value.",
					 function );

					libesedb_data_type_definition_free(
					 (intptr_t *) data_type_definition,
					 NULL );

					return( -1 );
				}
#endif

				if( libesedb_data_type_definition_set_data(
				     data_type_definition,
				     fixed_size_data_type_value_data,
				     column_catalog_definition->size,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set data in fixed size data type definition.",
					 function );

					libesedb_data_type_definition_free(
					 (intptr_t *) data_type_definition,
					 NULL );

					return( -1 );
				}
				fixed_size_data_type_value_data += column_catalog_definition->size;
			}
		}
		else if( ( column_catalog_definition->identifier >= 128 )
		      && ( column_catalog_definition->identifier <= 255 ) )
		{
			if( column_catalog_definition->identifier <= last_variable_size_data_type )
			{
				endian_little_convert_16bit(
				 variable_size_data_type_size,
				 variable_size_data_type_size_data );

				variable_size_data_type_size_data += 2;

#if defined( HAVE_DEBUG_OUTPUT )
				libnotify_verbose_printf(
				 "%s: (%03" PRIu16 ") variable size data type size\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
				 function,
				 column_catalog_definition->identifier,
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
					 column_catalog_definition->identifier );
					libnotify_verbose_print_data(
					 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
					 variable_size_data_type_size - previous_variable_size_data_type_size );
#endif

					if( libesedb_data_type_definition_set_data(
					     data_type_definition,
					     &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
					     variable_size_data_type_size,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_RUNTIME,
						 LIBERROR_RUNTIME_ERROR_SET_FAILED,
						 "%s: unable to set data in variable size data type definition.",
						 function );

						libesedb_data_type_definition_free(
						 (intptr_t *) data_type_definition,
						 NULL );

						return( -1 );
					}
					previous_variable_size_data_type_size = variable_size_data_type_size;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				else
				{
					libnotify_verbose_printf(
					 "%s: (%03" PRIu32 ") variable size data type\t\t\t\t: <NULL>\n",
					 function,
					 column_catalog_definition->identifier );
				}
#endif
			}
		}
		else
		{
			if( tagged_data_types_format == LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR )
			{
				if( tagged_data_type_offset_data == NULL )
				{
					tagged_data_type_value_data    = &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] );
					remaining_definition_data_size = definition_data_size - (size_t) ( tagged_data_type_value_data - definition_data );

					endian_little_convert_16bit(
					 tagged_data_type_identifier,
					 tagged_data_type_value_data );

					tagged_data_type_value_data += 2;

					endian_little_convert_16bit(
					 tagged_data_type_size,
					 tagged_data_type_value_data );

					tagged_data_type_value_data += 2;

					remaining_definition_data_size -= 4;
				}
				if( ( remaining_definition_data_size > 0 )
				 && ( column_catalog_definition->identifier == tagged_data_type_identifier ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					libnotify_verbose_printf(
					 "%s: (%03" PRIu16 ") tagged data type identifier\t\t\t: %" PRIu16 "\n",
					 function,
					 column_catalog_definition->identifier,
					 tagged_data_type_identifier );
					libnotify_verbose_printf(
					 "%s: (%03" PRIu16 ") tagged data type size\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
					 function,
					 column_catalog_definition->identifier,
					 tagged_data_type_size,
					 tagged_data_type_size & 0x3fff );

					libnotify_verbose_printf(
					 "%s: (%03" PRIu16 ") tag byte\t\t\t\t\t\t: 0x%02" PRIx8 "\n",
					 function,
					 column_catalog_definition->identifier,
					 *tagged_data_type_value_data );
#endif
					tagged_data_type_value_data += 1;
					tagged_data_type_size       -= 1;

#if defined( HAVE_DEBUG_OUTPUT )
					if( tagged_data_type_size > 0 )
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu16 ") tagged data type:\n",
						 function,
						 column_catalog_definition->identifier );
						libnotify_verbose_print_data(
						 tagged_data_type_value_data,
						 tagged_data_type_size );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu32 ") tagged data type\t\t\t\t\t: <NULL>\n",
						 function,
						 column_catalog_definition->identifier );
					}
#endif
					if( tagged_data_type_size > 0 )
					{
						if( libesedb_data_type_definition_set_data(
						     data_type_definition,
						     tagged_data_type_value_data,
						     tagged_data_type_size,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in tagged data type definition.",
							 function );

							libesedb_data_type_definition_free(
							 (intptr_t *) data_type_definition,
							 NULL );

							return( -1 );
						}
					}
					tagged_data_type_value_data += tagged_data_type_size;
				}
			}
			else if( tagged_data_types_format == LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX )
			{
				if( tagged_data_type_offset_data == NULL )
				{
					tagged_data_type_offset_data   = &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] );
					tagged_data_type_value_data    = &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] );
					remaining_definition_data_size = definition_data_size - (size_t) ( tagged_data_type_value_data - definition_data );

					if( remaining_definition_data_size > 0 )
					{
						endian_little_convert_16bit(
						 tagged_data_type_identifier,
						 tagged_data_type_offset_data );

						tagged_data_type_offset_data += 2;

						endian_little_convert_16bit(
						 tagged_data_type_offset,
						 tagged_data_type_offset_data );

						tagged_data_type_offset_data += 2;

						if( tagged_data_type_offset == 0 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
							 "%s: invalid tagged data type offset value out of range.",
							 function );

							return( -1 );
						}
						tagged_data_type_offset_data_size = ( tagged_data_type_offset & 0x3fff ) - 4;

						remaining_definition_data_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
						libnotify_verbose_printf(
						 "%s: tagged data type offset data size\t\t\t: %" PRIu16 "\n",
						 function,
						 tagged_data_type_offset_data_size );
						libnotify_verbose_printf(
						 "%s: tagged data type offset data:\n",
						 function );
						libnotify_verbose_print_data(
						 tagged_data_type_value_data,
						 tagged_data_type_offset_data_size + 4 );
#endif

					}
				}
				if( ( remaining_definition_data_size > 0 )
				 && ( column_catalog_definition->identifier == tagged_data_type_identifier ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					libnotify_verbose_printf(
					 "%s: (%03" PRIu16 ") tagged data type identifier\t\t\t: %" PRIu16 "\n",
					 function,
					 column_catalog_definition->identifier,
					 tagged_data_type_identifier );
					libnotify_verbose_printf(
					 "%s: (%03" PRIu16 ") tagged data type offset\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
					 function,
					 column_catalog_definition->identifier,
					 tagged_data_type_offset,
					 tagged_data_type_offset & 0x3fff );
#endif

					previous_tagged_data_type_offset = tagged_data_type_offset;

					if( tagged_data_type_offset_data_size > 0 )
					{
						endian_little_convert_16bit(
						 tagged_data_type_identifier,
						 tagged_data_type_offset_data );

						tagged_data_type_offset_data += 2;

						endian_little_convert_16bit(
						 tagged_data_type_offset,
						 tagged_data_type_offset_data );

						tagged_data_type_offset_data += 2;

						tagged_data_type_offset_data_size -= 4;
						remaining_definition_data_size    -= 4;
					}
					if( previous_tagged_data_type_offset > tagged_data_type_offset )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
						 "%s: invalid tagged data type offset value exceeds next tagged data type offset.",
						 function );

						return( -1 );
					}
					if( tagged_data_type_offset_data_size > 0 )
					{
						tagged_data_type_size = ( tagged_data_type_offset & 0x3fff ) - ( previous_tagged_data_type_offset & 0x3fff );
					}
					else
					{
						tagged_data_type_size = remaining_definition_data_size;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					libnotify_verbose_printf(
					 "%s: (%03" PRIu16 ") tagged data type size\t\t\t\t: %" PRIu16 "\n",
					 function,
					 column_catalog_definition->identifier,
					 tagged_data_type_size );
#endif

					if( tagged_data_type_size > 0 )
					{
						if( tagged_data_type_size > remaining_definition_data_size )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
							 "%s: invalid tagged data type size value exceeds remaining data size.",
							 function );

							return( -1 );
						}
						remaining_definition_data_size -= tagged_data_type_size;

						/* TODO ignoring the tag byte for now
						 */
						if( ( previous_tagged_data_type_offset & 0x4000 ) == 0x4000 )
						{
#if defined( HAVE_DEBUG_OUTPUT )
							libnotify_verbose_printf(
							 "%s: (%03" PRIu16 ") tag byte\t\t\t\t\t\t: 0x%02" PRIx8 "\n",
							 function,
							 column_catalog_definition->identifier,
							 tagged_data_type_value_data[ previous_tagged_data_type_offset & 0x3fff ] );
#endif

							previous_tagged_data_type_offset += 1;
							tagged_data_type_size            -= 1;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						libnotify_verbose_printf(
						 "%s: (%03" PRIu16 ") tagged data type:\n",
						 function,
						 column_catalog_definition->identifier );
						libnotify_verbose_print_data(
						 &( tagged_data_type_value_data[ previous_tagged_data_type_offset & 0x3fff ] ),
						 tagged_data_type_size );
#endif
						previous_tagged_data_type_offset &= 0x3fff;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					/* TODO are zero size tagged data type values handled correctly?
					 */
					else
					{
						libnotify_verbose_printf(
						 "%s: (%03" PRIu32 ") tagged data type\t\t\t\t\t: <NULL>\n",
						 function,
						 column_catalog_definition->identifier );
					}
#endif
					if( tagged_data_type_size > 0 )
					{
						if( libesedb_data_type_definition_set_data(
						     data_type_definition,
						     &( tagged_data_type_value_data[ previous_tagged_data_type_offset ] ),
						     tagged_data_type_size,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in tagged data type definition.",
							 function );

							libesedb_data_type_definition_free(
							 (intptr_t *) data_type_definition,
							 NULL );

							return( -1 );
						}
					}
				}
			}
		}
		if( libesedb_array_set_entry(
		     data_definition->data_type_definitions_array,
		     column_catalog_definition_iterator,
		     (intptr_t *) data_type_definition,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set data type definition: %d.",
			 function,
			 column_catalog_definition_iterator );

			libesedb_data_type_definition_free(
			 (intptr_t *) data_type_definition,
			 NULL );

			return( -1 );
		}
		data_type_definition = NULL;

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
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

