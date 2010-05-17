/*
 * Data definition functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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
#include <byte_stream.h>
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
#include "libesedb_libuna.h"
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
	if( ( (libesedb_data_definition_t *) data_definition )->key != NULL )
	{
		memory_free(
		 ( (libesedb_data_definition_t *) data_definition )->key );
	}
	if( ( (libesedb_data_definition_t *) data_definition )->values_array != NULL )
	{
		if( libesedb_array_free(
		     &( ( (libesedb_data_definition_t *) data_definition )->values_array ),
		     &libesedb_data_type_definition_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
			 function );

			result = -1;
		}
	}
	memory_free(
	 data_definition );

	return( result );
}

/* Sets the common key in the data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_set_common_key(
     libesedb_data_definition_t *data_definition,
     uint8_t *common_key,
     size_t common_key_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_data_definition_set_common_key";

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
	if( data_definition->key != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data definition - key already set.",
		 function );

		return( -1 );
	}
	if( common_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid common key.",
		 function );

		return( -1 );
	}
	if( common_key_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid common key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( common_key_size > 0 )
	{
		data_definition->key = (uint8_t *) memory_allocate(
						    sizeof( uint8_t ) * common_key_size );

		if( data_definition->key == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create key.",
			 function );

			return( -1 );
		}
		if( memory_copy(
		     data_definition->key,
		     common_key,
		     sizeof( uint8_t ) * common_key_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy common key.",
			 function );

			return( -1 );
		}
		data_definition->key_size = common_key_size;
	}
	return( 1 );
}

/* Sets the local key in the data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_set_local_key(
     libesedb_data_definition_t *data_definition,
     uint8_t *local_key,
     size_t local_key_size,
     liberror_error_t **error )
{
	void *reallocation    = NULL;
	static char *function = "libesedb_data_definition_set_local_key";

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
	if( local_key == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid local key.",
		 function );

		return( -1 );
	}
	if( local_key_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid local key size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( local_key_size > 0 )
	{
		reallocation = memory_reallocate(
		                data_definition->key,
		                sizeof( uint8_t ) * ( data_definition->key_size + local_key_size ) );

		if( reallocation == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to resize key.",
			 function );

			return( -1 );
		}
		data_definition->key = (uint8_t *) reallocation;

		if( memory_copy(
		     &( data_definition->key[ data_definition->key_size ] ),
		     local_key,
		     sizeof( uint8_t ) * local_key_size ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy local key.",
			 function );

			return( -1 );
		}
		data_definition->key_size += local_key_size;
	}
	return( 1 );
}

/* Reads the record data definition
 * Uses the definition data in the catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_record(
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
	static char *function                                    = "libesedb_data_definition_read_record";
	size_t remaining_definition_data_size                    = 0;
	uint16_t previous_tagged_data_type_offset                = 0;
	uint16_t previous_variable_size_data_type_size           = 0;
	uint16_t tagged_data_type_offset_data_size               = 0;
	uint16_t tagged_data_type_identifier                     = 0;
	uint16_t tagged_data_type_offset                         = 0;
	uint16_t tagged_data_type_size                           = 0;
	uint16_t variable_size_data_type_size                    = 0;
	uint16_t variable_size_data_types_offset                 = 0;
	uint8_t current_variable_size_data_type                  = 0;
	uint8_t last_fixed_size_data_type                        = 0;
	uint8_t last_variable_size_data_type                     = 0;
	uint8_t number_of_variable_size_data_types               = 0;
	uint8_t tagged_data_types_format                         = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX;
	int column_catalog_definition_iterator                   = 0;
	int number_of_column_catalog_definitions                 = 0;

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
	if( data_definition->values_array != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data definition - values array already set.",
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
	if( ( io_handle->format_version == 0x620 )
	 && ( io_handle->format_revision <= 2 ) )
	{
		tagged_data_types_format = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR;
	}
	last_fixed_size_data_type    = ( (esedb_data_definition_header_t *) definition_data )->last_fixed_size_data_type;
	last_variable_size_data_type = ( (esedb_data_definition_header_t *) definition_data )->last_variable_size_data_type;

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_data_definition_header_t *) definition_data )->variable_size_data_types_offset,
	 variable_size_data_types_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: last fixed size data type\t\t\t\t: %" PRIu8 "\n",
		 function,
		 last_fixed_size_data_type );

		libnotify_printf(
		 "%s: last variable size data type\t\t\t\t: %" PRIu8 "\n",
		 function,
		 last_variable_size_data_type );

		libnotify_printf(
		 "%s: variable size data types offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 variable_size_data_types_offset );
	}
#endif

	if( libesedb_list_get_number_of_elements(
	     column_catalog_definition_list,
	     &number_of_column_catalog_definitions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of column catalog definitions.",
		 function );

		return( -1 );
	}
	if( libesedb_array_initialize(
	     &( data_definition->values_array ),
	     number_of_column_catalog_definitions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		return( -1 );
	}
	if( last_variable_size_data_type > 127 )
	{
		number_of_variable_size_data_types = last_variable_size_data_type - 127;
	}
	list_element                       = column_catalog_definition_list->first;
	fixed_size_data_type_value_data    = &( definition_data[ sizeof( esedb_data_definition_header_t ) ] );
	current_variable_size_data_type    = 127;
	variable_size_data_type_size_data  = &( definition_data[ variable_size_data_types_offset ] );
	variable_size_data_type_value_data = &( variable_size_data_type_size_data[ number_of_variable_size_data_types * 2 ] );

	for( column_catalog_definition_iterator = 0;
	     column_catalog_definition_iterator < number_of_column_catalog_definitions;
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
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: column definition identifier\t\t\t\t: %03" PRIu32 "\n",
			 function,
			 column_catalog_definition->identifier );
			libnotify_printf(
			 "%s: column definition name\t\t\t\t\t: %s\n",
			 function,
			 column_catalog_definition->name );
			libnotify_printf(
			 "%s: column definition type\t\t\t\t\t: %s (%s)\n",
			 function,
			 libesedb_column_type_get_description(
			  column_catalog_definition->column_type ),
			 libesedb_column_type_get_identifier(
			  column_catalog_definition->column_type ) );
		}
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
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
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
		else if( current_variable_size_data_type < last_variable_size_data_type )
		{
			while( current_variable_size_data_type < column_catalog_definition->identifier )
			{
				byte_stream_copy_to_uint16_little_endian(
				 variable_size_data_type_size_data,
				 variable_size_data_type_size );

				variable_size_data_type_size_data += 2;

				current_variable_size_data_type++;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: (%03" PRIu16 ") variable size data type size\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
					 function,
					 current_variable_size_data_type,
					 variable_size_data_type_size,
					 ( ( variable_size_data_type_size & 0x8000 ) != 0 ) ? 0 : ( variable_size_data_type_size & 0x7fff ) - previous_variable_size_data_type_size );
				}
#endif

				if( current_variable_size_data_type == column_catalog_definition->identifier )
				{
					/* The MSB signifies that the variable size data type is empty
					 */
					if( ( variable_size_data_type_size & 0x8000 ) == 0 )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: (%03" PRIu32 ") variable size data type:\n",
							 function,
							 column_catalog_definition->identifier );
							libnotify_print_data(
							 &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
							 variable_size_data_type_size - previous_variable_size_data_type_size );
						}
#endif

						if( libesedb_data_type_definition_set_data(
						     data_type_definition,
						     &( variable_size_data_type_value_data[ previous_variable_size_data_type_size ] ),
						     variable_size_data_type_size - previous_variable_size_data_type_size,
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
					else if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: (%03" PRIu32 ") variable size data type\t\t\t\t: <NULL>\n",
						 function,
						 column_catalog_definition->identifier );
					}
#endif
				}
				if( current_variable_size_data_type >= last_variable_size_data_type )
				{
					break;
				}
			}
		}
		else
		{
			variable_size_data_type_size &= 0x7fff;

			if( tagged_data_types_format == LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR )
			{
				if( tagged_data_type_value_data == NULL )
				{
					tagged_data_type_value_data    = &( variable_size_data_type_value_data[ variable_size_data_type_size ] );
					remaining_definition_data_size = definition_data_size - (size_t) ( tagged_data_type_value_data - definition_data );

					byte_stream_copy_to_uint16_little_endian(
					 tagged_data_type_value_data,
					 tagged_data_type_identifier );

					tagged_data_type_value_data += 2;

					byte_stream_copy_to_uint16_little_endian(
					 tagged_data_type_value_data,
					 tagged_data_type_size );

					tagged_data_type_value_data += 2;

					remaining_definition_data_size -= 4;
				}
				if( ( remaining_definition_data_size > 0 )
				 && ( column_catalog_definition->identifier == tagged_data_type_identifier ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type identifier\t\t\t\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_identifier );
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type size\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_size,
						 tagged_data_type_size & 0x5fff );
					}
#endif

					if( ( tagged_data_type_size & 0x8000 ) == 0x8000 )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type flags\t\t\t\t: 0x%02" PRIx8 "\n",
							 function,
							 column_catalog_definition->identifier,
							 *tagged_data_type_value_data );
							libesedb_debug_print_tagged_data_type_flags(
							 *tagged_data_type_value_data );
							libnotify_printf(
							 "\n" );
						}
#endif

						if( libesedb_data_type_definition_set_flags(
						     data_type_definition,
						     *tagged_data_type_value_data,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set tagged data type flags in tagged data type definition.",
							 function );

							libesedb_data_type_definition_free(
							 (intptr_t *) data_type_definition,
							 NULL );

							return( -1 );
						}
						tagged_data_type_value_data    += 1;
						tagged_data_type_size           = ( tagged_data_type_size & 0x5fff ) - 1;
						remaining_definition_data_size -= 1;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libnotify_verbose != 0 )
					{
						if( tagged_data_type_size > 0 )
						{
							libnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type:\n",
							 function,
							 column_catalog_definition->identifier );
							libnotify_print_data(
							 tagged_data_type_value_data,
							 tagged_data_type_size );
						}
						else
						{
							libnotify_printf(
							 "%s: (%03" PRIu32 ") tagged data type\t\t\t\t\t: <NULL>\n",
							 function,
							 column_catalog_definition->identifier );
						}
					}
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
						tagged_data_type_value_data    += tagged_data_type_size;
						remaining_definition_data_size -= tagged_data_type_size;
					}
					if( remaining_definition_data_size > 0 )
					{
						byte_stream_copy_to_uint16_little_endian(
						 tagged_data_type_value_data,
						 tagged_data_type_identifier );

						tagged_data_type_value_data += 2;

						byte_stream_copy_to_uint16_little_endian(
						 tagged_data_type_value_data,
						 tagged_data_type_size );

						tagged_data_type_value_data += 2;

						remaining_definition_data_size -= 4;
					}
				}
			}
			else if( tagged_data_types_format == LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX )
			{
				if( tagged_data_type_offset_data == NULL )
				{
					tagged_data_type_offset_data   = &( variable_size_data_type_value_data[ variable_size_data_type_size ] );
					tagged_data_type_value_data    = &( variable_size_data_type_value_data[ variable_size_data_type_size ] );
					remaining_definition_data_size = definition_data_size - (size_t) ( tagged_data_type_value_data - definition_data );

					if( remaining_definition_data_size > 0 )
					{
						byte_stream_copy_to_uint16_little_endian(
						 tagged_data_type_offset_data,
						 tagged_data_type_identifier );

						tagged_data_type_offset_data += 2;

						byte_stream_copy_to_uint16_little_endian(
						 tagged_data_type_offset_data,
						 tagged_data_type_offset );

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
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: tagged data type offset data size\t\t\t\t: %" PRIu16 "\n",
							 function,
							 tagged_data_type_offset_data_size );
							libnotify_printf(
							 "%s: tagged data type offset data:\n",
							 function );
							libnotify_print_data(
							 tagged_data_type_value_data,
							 tagged_data_type_offset_data_size + 4 );
						}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
					}
				}
				if( ( remaining_definition_data_size > 0 )
				 && ( column_catalog_definition->identifier == tagged_data_type_identifier ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type identifier\t\t\t\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_identifier );
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type offset\t\t\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_offset,
						 tagged_data_type_offset & 0x3fff );
					}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

					previous_tagged_data_type_offset = tagged_data_type_offset;

					if( tagged_data_type_offset_data_size > 0 )
					{
						byte_stream_copy_to_uint16_little_endian(
						 tagged_data_type_offset_data,
						 tagged_data_type_identifier );

						tagged_data_type_offset_data += 2;

						byte_stream_copy_to_uint16_little_endian(
						 tagged_data_type_offset_data,
						 tagged_data_type_offset );

						tagged_data_type_offset_data += 2;

						tagged_data_type_offset_data_size -= 4;
						remaining_definition_data_size    -= 4;
					}
					if( ( previous_tagged_data_type_offset & 0x3fff ) > ( tagged_data_type_offset & 0x3fff ) )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
						 "%s: invalid tagged data type offset value exceeds next tagged data type offset.",
						 function );

						return( -1 );
					}
					if( ( tagged_data_type_offset & 0x3fff ) > ( previous_tagged_data_type_offset & 0x3fff ) )
					{
						tagged_data_type_size = ( tagged_data_type_offset & 0x3fff ) - ( previous_tagged_data_type_offset & 0x3fff );
					}
					else
					{
						tagged_data_type_size = (uint16_t) remaining_definition_data_size;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type size\t\t\t\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_size );
					}
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

						if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
						 || ( ( previous_tagged_data_type_offset & 0x4000 ) != 0 ) )
						{
#if defined( HAVE_DEBUG_OUTPUT )
							if( libnotify_verbose != 0 )
							{
								libnotify_printf(
								 "%s: (%03" PRIu16 ") tagged data type flags\t\t\t\t: 0x%02" PRIx8 "\n",
								 function,
								 column_catalog_definition->identifier,
								 tagged_data_type_value_data[ previous_tagged_data_type_offset & 0x3fff ] );
								libesedb_debug_print_tagged_data_type_flags(
								 tagged_data_type_value_data[ previous_tagged_data_type_offset & 0x3fff ] );
								libnotify_printf(
								 "\n" );
							}
#endif

							if( libesedb_data_type_definition_set_flags(
							     data_type_definition,
							     tagged_data_type_value_data[ previous_tagged_data_type_offset & 0x3fff ],
							     error ) != 1 )
							{
								liberror_error_set(
								 error,
								 LIBERROR_ERROR_DOMAIN_RUNTIME,
								 LIBERROR_RUNTIME_ERROR_SET_FAILED,
								 "%s: unable to set tagged data type flags in tagged data type definition.",
								 function );

								libesedb_data_type_definition_free(
								 (intptr_t *) data_type_definition,
								 NULL );

								return( -1 );
							}
							previous_tagged_data_type_offset += 1;
							tagged_data_type_size            -= 1;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type:\n",
							 function,
							 column_catalog_definition->identifier );
							libnotify_print_data(
							 &( tagged_data_type_value_data[ previous_tagged_data_type_offset & 0x3fff ] ),
							 tagged_data_type_size );
						}
#endif
						previous_tagged_data_type_offset &= 0x3fff;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					/* TODO are zero size tagged data type values handled correctly?
					 */
					else if( libnotify_verbose != 0 )
					{
						libnotify_printf(
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
		     data_definition->values_array,
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
	if( libnotify_verbose != 0 )
	{
		if( fixed_size_data_type_value_data < &( definition_data[ variable_size_data_types_offset ] ) )
		{
			libnotify_printf(
			 "%s: fixed size data types trailing data:\n",
			 function );
			libnotify_print_data(
			 fixed_size_data_type_value_data,
			 &( definition_data[ variable_size_data_types_offset ] ) - fixed_size_data_type_value_data );
		}
		libnotify_printf(
		 "\n" );
	}
#endif

	return( 1 );
}

/* Reads the long value data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_long_value(
     libesedb_data_definition_t *data_definition,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error )
{
	static char *function            = "libesedb_data_definition_read_long_value";
	uint32_t number_of_data_segments = 0;

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
	if( data_definition->values_array != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data definition - values array already set.",
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
	if( definition_data_size != 8 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data definition size: %" PRIzd ".",
		 function,
		 definition_data_size );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 definition_data,
	 number_of_data_segments );

	definition_data += 4;

	byte_stream_copy_to_uint16_little_endian(
	 definition_data,
	 data_definition->size );

	definition_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: long value last data segment\t\t\t: %" PRIu32 "\n",
		 function,
		 number_of_data_segments );
		libnotify_printf(
		 "%s: long value number total data size\t\t: %" PRIu32 "\n",
		 function,
		 data_definition->size );
		libnotify_printf(
		 "\n" );
	}
#endif
	if( number_of_data_segments == 0 )
	{
		number_of_data_segments = 1;
	}
	if( libesedb_array_initialize(
	     &( data_definition->values_array ),
	     (int) number_of_data_segments,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the long value segment data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_long_value_segment(
     libesedb_data_definition_t *data_definition,
     uint32_t data_segment_number,
     uint8_t *definition_data,
     size_t definition_data_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	static char *function                                 = "libesedb_data_definition_read_long_value_segment";

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
	if( data_definition->values_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data definition - missing values array.",
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
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: long value data segment\t\t: %" PRIu32 "\n",
		 function,
		 data_segment_number );
		libnotify_printf(
		 "\n" );
	}
#endif

	if( libesedb_data_type_definition_initialize(
	     &data_type_definition,
	     NULL,
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
	if( libesedb_data_type_definition_set_data(
	     data_type_definition,
	     definition_data,
	     definition_data_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data in long value segment data type definition: %" PRIu32 ".",
		 function,
		 data_segment_number );

		libesedb_data_type_definition_free(
		 (intptr_t *) data_type_definition,
		 NULL );

		return( -1 );
	}
	if( libesedb_array_set_entry(
	     data_definition->values_array,
	     (int) data_segment_number,
	     (intptr_t *) data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to set long value segment data type definition: %" PRIu32 ".",
		 function,
		 data_segment_number );

		libesedb_data_type_definition_free(
		 (intptr_t *) data_type_definition,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

