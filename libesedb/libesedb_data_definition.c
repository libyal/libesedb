/*
 * Data definition functions
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
#include <system_string.h>
#include <types.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_column_type.h"
#include "libesedb_data_definition.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_libfvalue.h"
#include "libesedb_page.h"
#include "libesedb_table_definition.h"
#include "libesedb_value_data_handle.h"

#include "esedb_page_values.h"

/* Creates a data definition
 * Make sure the value data_definition is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_initialize(
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_data_definition_initialize";

	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( *data_definition != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data definition value already set.",
		 function );

		return( -1 );
	}
	*data_definition = memory_allocate_structure(
	                    libesedb_data_definition_t );

	if( *data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data definition.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_definition,
	     0,
	     sizeof( libesedb_data_definition_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data definition.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_definition != NULL )
	{
		memory_free(
		 *data_definition );

		*data_definition = NULL;
	}
	return( -1 );
}

/* Frees a data definition
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_free(
     libesedb_data_definition_t **data_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_data_definition_free";

	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( *data_definition != NULL )
	{
		memory_free(
		 *data_definition );

		*data_definition = NULL;
	}
	return( 1 );
}

/* Reads the data
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_data(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_data_definition_read_data";
	off64_t element_data_offset       = 0;
	uint16_t data_offset              = 0;

	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
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
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) pages_cache,
	     data_definition->page_offset,
	     &element_data_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: 0x%08" PRIx64 ".",
		 function,
		 data_definition->page_number,
		 data_definition->page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value_by_index(
	     page,
	     data_definition->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		return( -1 );
	}
	data_offset = data_definition->data_offset - page_value->offset;

	if( ( data_definition->data_offset < page_value->offset )
	 || ( data_offset > page_value->size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data definition - data offset value out of bounds.",
		 function );

		return( -1 );
	}
	*data      = &( page_value->data[ data_offset ] );
	*data_size = (size_t) ( page_value->size - data_offset );

	return( 1 );
}

/* Reads the record
 * Uses the definition data in the catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_record(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libcdata_array_t *values_array,
     uint8_t *record_flags,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_page_t *page                                    = NULL;
	libesedb_page_value_t *page_value                        = NULL;
	libfvalue_data_handle_t *value_data_handle               = NULL;
	libfvalue_value_t *record_value                          = NULL;
	uint8_t *record_data                                     = NULL;
	uint8_t *tagged_data_type_offset_data                    = NULL;
	static char *function                                    = "libesedb_data_definition_read_record";
	size_t record_data_size                                  = 0;
	size_t remaining_definition_data_size                    = 0;
	off64_t element_data_offset                              = 0;
	uint16_t data_offset                                     = 0;
	uint16_t fixed_size_data_type_value_offset               = 0;
	uint16_t masked_previous_tagged_data_type_offset         = 0;
	uint16_t masked_tagged_data_type_offset                  = 0;
	uint16_t previous_tagged_data_type_offset                = 0;
	uint16_t previous_variable_size_data_type_size           = 0;
	uint16_t tagged_data_type_identifier                     = 0;
	uint16_t tagged_data_type_offset                         = 0;
	uint16_t tagged_data_type_offset_bitmask                 = 0x3fff;
	uint16_t tagged_data_type_offset_data_size               = 0;
	uint16_t tagged_data_type_size                           = 0;
	uint16_t tagged_data_type_value_offset                   = 0;
	uint16_t tagged_data_types_offset                        = 0;
	uint16_t variable_size_data_type_offset                  = 0;
	uint16_t variable_size_data_type_size                    = 0;
	uint16_t variable_size_data_type_value_offset            = 0;
	uint16_t variable_size_data_type_value_size              = 0;
	uint16_t variable_size_data_types_offset                 = 0;
	uint8_t current_variable_size_data_type                  = 0;
	uint8_t last_fixed_size_data_type                        = 0;
	uint8_t last_variable_size_data_type                     = 0;
	uint8_t number_of_variable_size_data_types               = 0;
	uint8_t record_value_type                                = 0;
	uint8_t tagged_data_types_format                         = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX;
	int column_catalog_definition_index                      = 0;
	int encoding                                             = 0;
	int number_of_column_catalog_definitions                 = 0;
	int number_of_table_column_catalog_definitions           = 0;
	int number_of_template_table_column_catalog_definitions  = 0;
	int record_value_codepage                                = 0;

	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( values_array == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid values array.",
		 function );

		return( -1 );
	}
	if( record_flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid record flags.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) pages_cache,
	     data_definition->page_offset,
	     &element_data_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: 0x%08" PRIx64 ".",
		 function,
		 data_definition->page_number,
		 data_definition->page_offset );

		goto on_error;
	}
	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		goto on_error;
	}
	if( libesedb_page_get_value_by_index(
	     page,
	     data_definition->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		goto on_error;
	}
	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		goto on_error;
	}
	if( page_value->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		goto on_error;
	}
	data_offset = data_definition->data_offset - page_value->offset;

	if( ( data_definition->data_offset < page_value->offset )
	 || ( data_offset > page_value->size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data definition - data offset value out of bounds.",
		 function );

		goto on_error;
	}
	record_data      = &( page_value->data[ data_offset ] );
	record_data_size = page_value->size - data_offset;

	if( record_data_size < sizeof( esedb_data_definition_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( io_handle->format_version == 0x620 )
	 && ( io_handle->format_revision <= 2 ) )
	{
		tagged_data_types_format = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR;
	}
	if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
	 && ( io_handle->page_size >= 16384 ) )
	{
		tagged_data_type_offset_bitmask = 0x7fff;
	}
	last_fixed_size_data_type    = ( (esedb_data_definition_header_t *) record_data )->last_fixed_size_data_type;
	last_variable_size_data_type = ( (esedb_data_definition_header_t *) record_data )->last_variable_size_data_type;

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_data_definition_header_t *) record_data )->variable_size_data_types_offset,
	 variable_size_data_types_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: last fixed size data type\t\t\t: %" PRIu8 "\n",
		 function,
		 last_fixed_size_data_type );

		libcnotify_printf(
		 "%s: last variable size data type\t\t: %" PRIu8 "\n",
		 function,
		 last_variable_size_data_type );

		libcnotify_printf(
		 "%s: variable size data types offset\t\t: %" PRIu16 "\n",
		 function,
		 variable_size_data_types_offset );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( template_table_definition != NULL )
	{
		if( libesedb_table_definition_get_number_of_column_catalog_definitions(
		     template_table_definition,
		     &number_of_template_table_column_catalog_definitions,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of template table column catalog definitions.",
			 function );

			goto on_error;
		}
	}
	if( libesedb_table_definition_get_number_of_column_catalog_definitions(
	     table_definition,
	     &number_of_table_column_catalog_definitions,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table column catalog definitions.",
		 function );

		goto on_error;
	}
	number_of_column_catalog_definitions = number_of_table_column_catalog_definitions;

	if( template_table_definition != NULL )
	{
		if( number_of_table_column_catalog_definitions > number_of_template_table_column_catalog_definitions )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of table column catalog definitions value exceeds number in template table.",
			 function );

			goto on_error;
		}
		number_of_column_catalog_definitions += number_of_template_table_column_catalog_definitions;
	}
	if( libcdata_array_resize(
	     values_array,
	     number_of_column_catalog_definitions,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libfvalue_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize values array.",
		 function );

		goto on_error;
	}
	if( last_variable_size_data_type > 127 )
	{
		number_of_variable_size_data_types = last_variable_size_data_type - 127;
	}
	fixed_size_data_type_value_offset    = (uint16_t) sizeof( esedb_data_definition_header_t );
	current_variable_size_data_type      = 127;
	variable_size_data_type_offset       = variable_size_data_types_offset;
	variable_size_data_type_value_offset = variable_size_data_types_offset + ( number_of_variable_size_data_types * 2 );

	for( column_catalog_definition_index = 0;
	     column_catalog_definition_index < number_of_column_catalog_definitions;
	     column_catalog_definition_index++ )
	{
		if( ( template_table_definition != NULL )
		 && ( column_catalog_definition_index < number_of_template_table_column_catalog_definitions ) )
		{
			if( libesedb_table_definition_get_column_catalog_definition_by_index(
			     template_table_definition,
			     column_catalog_definition_index,
			     &column_catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve column catalog definition: %d from template table.",
				 function,
				 column_catalog_definition_index );

				goto on_error;
			}
		}
		else
		{
			if( libesedb_table_definition_get_column_catalog_definition_by_index(
			     table_definition,
			     column_catalog_definition_index,
			     &column_catalog_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve column catalog definition: %d from table.",
				 function,
				 column_catalog_definition_index );

				goto on_error;
			}
		}
		if( column_catalog_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing column catalog definition: %d.",
			 function,
			 column_catalog_definition_index );

			goto on_error;
		}
		if( column_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported column catalog definition type: %" PRIu16 " for list element: %d.",
			 function,
			 column_catalog_definition->type,
			 column_catalog_definition_index );

			goto on_error;
		}
		if( ( template_table_definition != NULL )
		 && ( column_catalog_definition_index == number_of_template_table_column_catalog_definitions ) )
		{
			if( column_catalog_definition->identifier != 256 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: only tagged data types supported in tables using a template table.",
				 function );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: column definition identifier\t\t: %03" PRIu32 "\n",
			 function,
			 column_catalog_definition->identifier );
			libcnotify_printf(
			 "%s: column definition name\t\t\t: %" PRIs_SYSTEM "\n",
			 function,
			 column_catalog_definition->name_string );
			libcnotify_printf(
			 "%s: column definition type\t\t\t: %s (%s)\n",
			 function,
			 libesedb_column_type_get_description(
			  column_catalog_definition->column_type ),
			 libesedb_column_type_get_identifier(
			  column_catalog_definition->column_type ) );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

/* TODO refactor to value type */

		switch( column_catalog_definition->column_type )
		{
			case LIBESEDB_COLUMN_TYPE_NULL:
				/* JET_coltypNil seems to be able to contain data */
				record_value_type = LIBFVALUE_VALUE_TYPE_BINARY_DATA;
				break;

			case LIBESEDB_COLUMN_TYPE_BOOLEAN:
				record_value_type = LIBFVALUE_VALUE_TYPE_BOOLEAN;
				break;

			case LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED:
				record_value_type = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_8BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED:
				record_value_type = LIBFVALUE_VALUE_TYPE_INTEGER_16BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED:
				record_value_type = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_16BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED:
				record_value_type = LIBFVALUE_VALUE_TYPE_INTEGER_32BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED:
				record_value_type = LIBFVALUE_VALUE_TYPE_UNSIGNED_INTEGER_32BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_CURRENCY:
			case LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED:
				record_value_type = LIBFVALUE_VALUE_TYPE_INTEGER_64BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_FLOAT_32BIT:
				record_value_type = LIBFVALUE_VALUE_TYPE_FLOATING_POINT_32BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT:
				record_value_type = LIBFVALUE_VALUE_TYPE_FLOATING_POINT_64BIT;
				break;

			case LIBESEDB_COLUMN_TYPE_DATE_TIME:
				record_value_type = LIBFVALUE_VALUE_TYPE_FILETIME;
				break;

			case LIBESEDB_COLUMN_TYPE_GUID:
				record_value_type = LIBFVALUE_VALUE_TYPE_GUID;
				break;

			case LIBESEDB_COLUMN_TYPE_BINARY_DATA:
			case LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA:
				record_value_type = LIBFVALUE_VALUE_TYPE_BINARY_DATA;
				break;

			case LIBESEDB_COLUMN_TYPE_TEXT:
			case LIBESEDB_COLUMN_TYPE_LARGE_TEXT:
				record_value_type = LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM;
				break;

			case LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE:
/* TODO handle this value type */
				record_value_type = LIBFVALUE_VALUE_TYPE_UNDEFINED;
				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported column type: %" PRIu32 ".",
				 function,
				 column_catalog_definition->column_type );

				goto on_error;
		}
		if( libfvalue_data_handle_initialize(
		     &value_data_handle,
		     (int (*)(libfvalue_data_handle_t *, const uint8_t *, size_t, int, uint32_t, libcerror_error_t **)) &libesedb_value_data_handle_read_value_entries,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value data handle.",
			 function );

			goto on_error;
		}
		if( ( column_catalog_definition->column_type == LIBESEDB_COLUMN_TYPE_TEXT )
		 || ( column_catalog_definition->column_type == LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
		{
			record_value_codepage = (int) column_catalog_definition->codepage;

			/* If the codepage is not set use the default codepage
			 */
			if( record_value_codepage == 0 )
			{
				record_value_codepage = io_handle->ascii_codepage;
			}
			/* Codepage 1200 in the ESE database format is not strict UTF-16 little-endian
			 * it can be used for ASCII strings as well.
			 */
			if( record_value_codepage == 1200 )
			{
				record_value_codepage = LIBFVALUE_CODEPAGE_1200_MIXED;
			}
			encoding = record_value_codepage;
		}
		else
		{
			encoding = LIBFVALUE_ENDIAN_LITTLE;
		}
		if( column_catalog_definition->identifier <= 127 )
		{
			if( column_catalog_definition->identifier <= last_fixed_size_data_type )
			{
				if( column_catalog_definition->size > ( record_data_size - fixed_size_data_type_value_offset ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid data definition - size value out of bounds.",
					 function );

					goto on_error;
				}
#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: (%03" PRIu32 ") fixed size data type size\t\t: %" PRIu32 "\n",
					 function,
					 column_catalog_definition->identifier,
					 column_catalog_definition->size );
					libcnotify_print_data(
					 &( record_data[ fixed_size_data_type_value_offset ] ),
					 column_catalog_definition->size,
					 0 );
				}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

				if( libfvalue_data_handle_set_data(
				     value_data_handle,
				     &( record_data[ fixed_size_data_type_value_offset ] ),
				     column_catalog_definition->size,
				     encoding,
				     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set data in fixed size data type definition.",
					 function );

					goto on_error;
				}
				if( column_catalog_definition->size > (uint32_t) UINT16_MAX )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid common catalog definition size value exceeds maximum.",
					 function );

					goto on_error;
				}
				fixed_size_data_type_value_offset += (uint16_t) column_catalog_definition->size;
			}
		}
		else if( current_variable_size_data_type < last_variable_size_data_type )
		{
			while( current_variable_size_data_type < column_catalog_definition->identifier )
			{
				if( variable_size_data_type_offset > ( record_data_size - 2 ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
					 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid variable size data type offset value out of bounds.",
					 function );

					goto on_error;
				}
				byte_stream_copy_to_uint16_little_endian(
				 &( record_data[ variable_size_data_type_offset ] ),
				 variable_size_data_type_size );

				variable_size_data_type_offset += 2;

				current_variable_size_data_type++;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: (%03" PRIu16 ") variable size data type size\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
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
						if( variable_size_data_type_size < previous_variable_size_data_type_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid variable size data type size value out of bounds.",
							 function );

							goto on_error;
						}
						variable_size_data_type_value_size = variable_size_data_type_size - previous_variable_size_data_type_size;

						if( ( variable_size_data_type_value_size > record_data_size )
						 || ( variable_size_data_type_value_offset > ( record_data_size - variable_size_data_type_value_size ) ) )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid variable size data type value size value out of bounds.",
							 function );

							goto on_error;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu32 ") variable size data type:\n",
							 function,
							 column_catalog_definition->identifier );
							libcnotify_print_data(
							 &( record_data[ variable_size_data_type_value_offset ] ),
							 variable_size_data_type_value_size,
							 0 );
						}
#endif
						if( libfvalue_data_handle_set_data(
						     value_data_handle,
						     &( record_data[ variable_size_data_type_value_offset ] ),
						     variable_size_data_type_value_size,
						     encoding,
						     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in variable size data type definition.",
							 function );

							goto on_error;
						}
						variable_size_data_type_value_offset += variable_size_data_type_value_size;
						previous_variable_size_data_type_size = variable_size_data_type_size;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					else if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu32 ") variable size data type\t\t: <NULL>\n",
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
			if( tagged_data_types_format == LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR )
			{
				if( tagged_data_types_offset == 0 )
				{
					tagged_data_types_offset       = variable_size_data_type_value_offset;
					tagged_data_type_value_offset  = variable_size_data_type_value_offset;
					remaining_definition_data_size = record_data_size - (size_t) tagged_data_types_offset;

					byte_stream_copy_to_uint16_little_endian(
					 &( record_data[ tagged_data_type_value_offset ] ),
					 tagged_data_type_identifier );

					tagged_data_type_value_offset += 2;

					byte_stream_copy_to_uint16_little_endian(
					 &( record_data[ tagged_data_type_value_offset ] ),
					 tagged_data_type_size );

					tagged_data_type_value_offset += 2;

					remaining_definition_data_size -= 4;
				}
				if( ( remaining_definition_data_size > 0 )
				 && ( column_catalog_definition->identifier == tagged_data_type_identifier ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type identifier\t\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_identifier );
						libcnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type size\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_size,
						 tagged_data_type_size & 0x5fff );
					}
#endif
					if( ( tagged_data_type_size & 0x8000 ) != 0 )
					{
						if( tagged_data_type_value_offset >= record_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type offset value out of bounds.",
							 function );

							goto on_error;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type flags\t\t: 0x%02" PRIx8 "\n",
							 function,
							 column_catalog_definition->identifier,
							 record_data[ tagged_data_type_value_offset ] );
							libesedb_debug_print_tagged_data_type_flags(
							 record_data[ tagged_data_type_value_offset ] );
							libcnotify_printf(
							 "\n" );
						}
#endif
						if( libfvalue_data_handle_set_data_flags(
						     value_data_handle,
						     (uint32_t) record_data[ tagged_data_type_value_offset ],
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set tagged data type flags in tagged data type definition.",
							 function );

							goto on_error;
						}
						tagged_data_type_value_offset  += 1;
						tagged_data_type_size           = ( tagged_data_type_size & 0x5fff ) - 1;
						remaining_definition_data_size -= 1;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						if( tagged_data_type_size > 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type:\n",
							 function,
							 column_catalog_definition->identifier );

							if( tagged_data_type_value_offset < record_data_size )
							{
								libcnotify_print_data(
								 &( record_data[ tagged_data_type_value_offset ] ),
								 tagged_data_type_size,
								 0 );
							}
							else
							{
								libcnotify_printf(
								 "<NULL>\n\n" );
							}
						}
						else
						{
							libcnotify_printf(
							 "%s: (%03" PRIu32 ") tagged data type\t\t\t: <NULL>\n",
							 function,
							 column_catalog_definition->identifier );
						}
					}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
					if( tagged_data_type_size > 0 )
					{
						if( tagged_data_type_value_offset >= record_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type offset value out of bounds.",
							 function );

							goto on_error;
						}
						if( tagged_data_type_size > remaining_definition_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type size value exceeds remaining data size.",
							 function );

							goto on_error;
						}
						if( libfvalue_data_handle_set_data(
						     value_data_handle,
						     &( record_data[ tagged_data_type_value_offset ] ),
						     tagged_data_type_size,
						     encoding,
						     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in tagged data type definition.",
							 function );

							goto on_error;
						}
						remaining_definition_data_size -= tagged_data_type_size;
					}
					if( remaining_definition_data_size > 0 )
					{
						if( tagged_data_type_value_offset >= record_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type offset value out of bounds.",
							 function );

							goto on_error;
						}
						byte_stream_copy_to_uint16_little_endian(
						 &( record_data[ tagged_data_type_value_offset ] ),
						 tagged_data_type_identifier );

						tagged_data_type_value_offset += 2;

						byte_stream_copy_to_uint16_little_endian(
						 &( record_data[ tagged_data_type_value_offset ] ),
						 tagged_data_type_size );

						tagged_data_type_value_offset += 2;

						remaining_definition_data_size -= 4;
					}
				}
			}
			else if( tagged_data_types_format == LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX )
			{
				if( tagged_data_types_offset == 0 )
				{
					tagged_data_types_offset = variable_size_data_type_value_offset;

					/* Note that offset is allowed to be equal to the record data size here
					 */
					if( tagged_data_types_offset > record_data_size )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid tagged data types offset value out of bounds.",
						 function );

						goto on_error;
					}
					tagged_data_type_offset_data   = &( record_data[ tagged_data_types_offset ] );
					remaining_definition_data_size = record_data_size - (size_t) tagged_data_types_offset;

					if( remaining_definition_data_size > 0 )
					{
						if( record_data_size < 4 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data types offset data size value out of bounds.",
							 function );

							goto on_error;
						}
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
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type offset value out of bounds.",
							 function );

							goto on_error;
						}
						tagged_data_type_offset_data_size = ( tagged_data_type_offset & 0x3fff ) - 4;

						remaining_definition_data_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: tagged data type offset data size\t\t: %" PRIu16 "\n",
							 function,
							 tagged_data_type_offset_data_size );
							libcnotify_printf(
							 "%s: tagged data type offset data:\n",
							 function );
							libcnotify_print_data(
							 tagged_data_type_offset_data,
							 tagged_data_type_offset_data_size + 4,
							 0 );
						}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
					}
				}
				if( ( remaining_definition_data_size > 0 )
				 && ( column_catalog_definition->identifier == tagged_data_type_identifier ) )
				{
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type identifier\t\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_identifier );

						libcnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type offset\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_offset,
						 tagged_data_type_offset & tagged_data_type_offset_bitmask );
					}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

					previous_tagged_data_type_offset = tagged_data_type_offset;

					if( tagged_data_type_offset_data_size > 0 )
					{
						if( tagged_data_type_offset_data_size < 4 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data types offset data size value out of bounds.",
							 function );

							goto on_error;
						}
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
					masked_previous_tagged_data_type_offset = previous_tagged_data_type_offset & tagged_data_type_offset_bitmask;
					masked_tagged_data_type_offset          = tagged_data_type_offset & tagged_data_type_offset_bitmask;

					if( masked_previous_tagged_data_type_offset > masked_tagged_data_type_offset )
					{
						libcerror_error_set(
						 error,
						 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
						 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid tagged data type offset value exceeds next tagged data type offset.",
						 function );

						goto on_error;
					}
					if( masked_tagged_data_type_offset > masked_previous_tagged_data_type_offset )
					{
						tagged_data_type_size = masked_tagged_data_type_offset - masked_previous_tagged_data_type_offset;
					}
					else
					{
						tagged_data_type_size = (uint16_t) remaining_definition_data_size;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type size\t\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_size );
					}
#endif
					tagged_data_type_value_offset = tagged_data_types_offset + masked_previous_tagged_data_type_offset;

					if( tagged_data_type_size > 0 )
					{
						if( tagged_data_type_size > remaining_definition_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type size value exceeds remaining data size.",
							 function );

							goto on_error;
						}
						remaining_definition_data_size -= tagged_data_type_size;

						if( ( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
						  &&  ( io_handle->page_size >= 16384 ) )
						 || ( ( previous_tagged_data_type_offset & 0x4000 ) != 0 ) )
						{
							if( tagged_data_type_value_offset >= record_data_size )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
								 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
								 "%s: invalid tagged data type offset value out of bounds.",
								 function );

								goto on_error;
							}
							if( libfvalue_data_handle_set_data_flags(
							     value_data_handle,
							     (uint32_t) record_data[ tagged_data_type_value_offset ],
							     error ) != 1 )
							{
								libcerror_error_set(
								 error,
								 LIBCERROR_ERROR_DOMAIN_RUNTIME,
								 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
								 "%s: unable to set tagged data type flags in tagged data type definition.",
								 function );

								goto on_error;
							}
							tagged_data_type_value_offset += 1;
							tagged_data_type_size         -= 1;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libcnotify_verbose != 0 )
						{
							libcnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type:\n",
							 function,
							 column_catalog_definition->identifier );

							if( tagged_data_type_value_offset < record_data_size )
							{
								libcnotify_print_data(
								 &( record_data[ tagged_data_type_value_offset ] ),
								 tagged_data_type_size,
								 0 );
							}
							else
							{
								libcnotify_printf(
								 "<NULL>\n\n" );
							}
						}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */
					}
#if defined( HAVE_DEBUG_OUTPUT )
					/* TODO are zero size tagged data type values handled correctly?
					 */
					else if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%s: (%03" PRIu32 ") tagged data type\t\t\t: <NULL>\n",
						 function,
						 column_catalog_definition->identifier );
					}
#endif
					if( tagged_data_type_size > 0 )
					{
						if( tagged_data_type_value_offset >= record_data_size )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type offset value out of bounds.",
							 function );

							goto on_error;
						}
						if( tagged_data_type_size > ( record_data_size - tagged_data_type_value_offset ) )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type size value out of bounds.",
							 function );

							goto on_error;
						}
						if( libfvalue_data_handle_set_data(
						     value_data_handle,
						     &( record_data[ tagged_data_type_value_offset ] ),
						     tagged_data_type_size,
						     encoding,
						     LIBFVALUE_VALUE_DATA_FLAG_MANAGED,
						     error ) != 1 )
						{
							libcerror_error_set(
							 error,
							 LIBCERROR_ERROR_DOMAIN_RUNTIME,
							 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in tagged data type definition.",
							 function );

							goto on_error;
						}
					}
				}
			}
		}
		if( libfvalue_value_type_initialize_with_data_handle(
		     &record_value,
		     record_value_type,
		     value_data_handle,
		     LIBFVALUE_VALUE_FLAG_DATA_HANDLE_MANAGED,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record value.",
			 function );

			goto on_error;
		}
		value_data_handle = NULL;

		if( libcdata_array_set_entry_by_index(
		     values_array,
		     column_catalog_definition_index,
		     (intptr_t *) record_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set data type definition: %d.",
			 function,
			 column_catalog_definition_index );

			goto on_error;
		}
		record_value = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( fixed_size_data_type_value_offset < variable_size_data_types_offset )
		{
			libcnotify_printf(
			 "%s: fixed size data types trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( record_data[ fixed_size_data_type_value_offset ] ),
			 variable_size_data_types_offset - fixed_size_data_type_value_offset,
			 0 );
		}
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( record_value != NULL )
	{
		libfvalue_value_free(
		 &record_value,
		 NULL );
	}
	if( value_data_handle != NULL )
	{
		libfvalue_data_handle_free(
		 &value_data_handle,
		 NULL );
	}
	return( -1 );
}

/* Reads the long value
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_long_value(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	uint8_t *long_value_data          = NULL;
	static char *function             = "libesedb_data_definition_read_long_value";
	size_t long_value_data_size       = 0;
	off64_t element_data_offset       = 0;
	uint32_t value_32bit              = 0;
	uint16_t data_offset              = 0;

	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) pages_cache,
	     data_definition->page_offset,
	     &element_data_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: 0x%08" PRIx64 ".",
		 function,
		 data_definition->page_number,
		 data_definition->page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value_by_index(
	     page,
	     data_definition->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		return( -1 );
	}
	data_offset = data_definition->data_offset - page_value->offset;

	if( ( data_definition->data_offset < page_value->offset )
	 || ( data_offset > page_value->size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data definition - data offset value out of bounds.",
		 function );

		return( -1 );
	}
	long_value_data      = &( page_value->data[ data_offset ] );
	long_value_data_size = page_value->size - data_offset;

	if( long_value_data_size != 8 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported long values data size: %" PRIzd ".",
		 function,
		 long_value_data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: long value data:\n",
		 function );
		libcnotify_print_data(
		 long_value_data,
		 long_value_data_size,
		 0 );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 long_value_data,
	 value_32bit );

	long_value_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
	}
#endif

	byte_stream_copy_to_uint16_little_endian(
	 long_value_data,
	 value_32bit );

	long_value_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: last segment offset\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the long value segment
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_definition_read_long_value_segment(
     libesedb_data_definition_t *data_definition,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint32_t long_value_segment_offset,
     libfdata_list_t *data_segments_list,
     libcerror_error_t **error )
{
	libesedb_page_t *page                  = NULL;
	libesedb_page_value_t *page_value      = NULL;
	static char *function                  = "libesedb_data_definition_read_long_value_segment";
	off64_t element_data_offset            = 0;
	off64_t long_value_segment_data_offset = 0;
	size64_t data_size                     = 0;
	size_t long_value_segment_data_size    = 0;
	uint16_t data_offset                   = 0;
	int element_index                      = 0;

	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data definition.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) pages_cache,
	     data_definition->page_offset,
	     &element_data_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: 0x%08" PRIx64 ".",
		 function,
		 data_definition->page_number,
		 data_definition->page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value_by_index(
	     page,
	     data_definition->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 data_definition->page_value_index );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		return( -1 );
	}
	data_offset = data_definition->data_offset - page_value->offset;

	if( ( data_definition->data_offset < page_value->offset )
	 || ( data_offset > page_value->size ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data definition - data offset value out of bounds.",
		 function );

		return( -1 );
	}
	long_value_segment_data_size = page_value->size - data_offset;

	/* Note that the data stream will point to the file offset
	 * io_handle->pages_data_offset contains the offset relative from the start of the file to the page data
	 * data_definition->page_offset contains the offset relative from the start of the page data
	 * data_definition->data_offset contains the offset relative from the start of the page
	 */
	long_value_segment_data_offset = io_handle->pages_data_offset
	                               + data_definition->page_offset
	                               + data_definition->data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: long value segment with offset: %" PRIu32 " has data at offset: %" PRIi64 " (0x%08" PRIx64 ") of size: %" PRIzd "\n",
		 function,
		 long_value_segment_offset,
		 long_value_segment_data_offset,
		 long_value_segment_data_offset,
		 long_value_segment_data_size );
		libcnotify_printf(
		 "\n" );
	}
#endif
	if( libfdata_list_get_size(
	     data_segments_list,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size of data segments list.",
		 function );

		return( -1 );
	}
	if( long_value_segment_offset != (off64_t) data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported long value segment offset: %" PRIi64 " value must match end of previous segment: %" PRIzd ".",
		 function,
		 long_value_segment_offset,
		 data_size );

		return( -1 );
	}
	if( libfdata_list_append_element(
	     data_segments_list,
	     &element_index,
	     0,
	     long_value_segment_data_offset,
	     (size64_t) long_value_segment_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append long value segment at offset: 0x%08" PRIx64 " to data segments list.",
		 function,
		 long_value_segment_offset );

		return( -1 );
	}
	return( 1 );
}

