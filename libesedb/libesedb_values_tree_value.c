/*
 * Values tree value functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libesedb_array_type.h"
#include "libesedb_catalog_definition.h"
#include "libesedb_column_type.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libfdata.h"
#include "libesedb_libfvalue.h"
#include "libesedb_list_type.h"
#include "libesedb_page.h"
#include "libesedb_table_definition.h"
#include "libesedb_values_tree_value.h"

#include "esedb_page_values.h"

enum LIBESEDB_TAGGED_DATA_TYPES_FORMATS
{
	LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR,
	LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX,
};

/* Creates values tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_initialize(
     libesedb_values_tree_value_t **values_tree_value,
     liberror_error_t **error )
{
	static char *function = "libesedb_values_tree_value_initialize";

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( *values_tree_value == NULL )
	{
		*values_tree_value = (libesedb_values_tree_value_t *) memory_allocate(
		                                                       sizeof( libesedb_values_tree_value_t ) );

		if( *values_tree_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create values tree value.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *values_tree_value,
		     0,
		     sizeof( libesedb_values_tree_value_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear values tree value.",
			 function );

			memory_free(
			 *values_tree_value );

			*values_tree_value = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees the values tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_free(
     intptr_t *values_tree_value,
     liberror_error_t **error )
{
	static char *function = "libesedb_values_tree_value_free";
	int result            = 1;

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( ( ( libesedb_values_tree_value_t *) values_tree_value )->key != NULL )
	{
		memory_free(
		 ( (libesedb_values_tree_value_t *) values_tree_value )->key );
	}
	memory_free(
	 values_tree_value );

	return( result );
}

/* Sets the common part of the key in the values tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_set_key_common(
     libesedb_values_tree_value_t *values_tree_value,
     uint8_t *common_key,
     size_t common_key_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_values_tree_value_set_key_common";

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( values_tree_value->key != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid values tree value - key already set.",
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
		values_tree_value->key = (uint8_t *) memory_allocate(
		                                      sizeof( uint8_t ) * common_key_size );

		if( values_tree_value->key == NULL )
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
		     values_tree_value->key,
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
		values_tree_value->key_size = common_key_size;
	}
	return( 1 );
}

/* Sets the local part of the key in the values tree value
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_set_key_local(
     libesedb_values_tree_value_t *values_tree_value,
     uint8_t *local_key,
     size_t local_key_size,
     liberror_error_t **error )
{
	void *reallocation    = NULL;
	static char *function = "libesedb_values_tree_value_set_key_local";

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
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
		                values_tree_value->key,
		                sizeof( uint8_t ) * ( values_tree_value->key_size + local_key_size ) );

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
		values_tree_value->key = (uint8_t *) reallocation;

		if( memory_copy(
		     &( values_tree_value->key[ values_tree_value->key_size ] ),
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
		values_tree_value->key_size += local_key_size;
	}
	return( 1 );
}

/* Reads the data
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_read_data(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     uint8_t **data,
     size_t *data_size,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_values_tree_value_read_data";

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( ( values_tree_value->type != LIBESEDB_VALUES_TREE_VALUE_TYPE_INDEX )
	 && ( values_tree_value->type != LIBESEDB_VALUES_TREE_VALUE_TYPE_RECORD ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported values tree value type: 0x%02" PRIx8 ".",
		 function,
		 values_tree_value->type );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     file_io_handle,
	     pages_cache,
	     values_tree_value->page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
		 function,
		 values_tree_value->page_number,
		 values_tree_value->page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     values_tree_value->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		return( -1 );
	}
	if( values_tree_value->data_offset > page_value->size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid values tree value - data offset exceeds page value size.",
		 function );

		return( -1 );
	}
	*data      = &( page_value->data[ values_tree_value->data_offset ] );
	*data_size = (size_t) ( page_value->size - values_tree_value->data_offset );

	return( 1 );
}

/* Reads the record
 * Uses the definition data in the catalog definitions
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_read_record(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libesedb_array_t *values_array,
     liberror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition        = NULL;
	libesedb_list_element_t *column_catalog_definition_list_element = NULL;
	libesedb_page_t *page                                           = NULL;
	libesedb_page_value_t *page_value                               = NULL;
	libfvalue_value_t *record_value                                 = NULL;
	uint8_t *record_data                                            = NULL;
	uint8_t *tagged_data_type_offset_data                           = NULL;
	static char *function                                           = "libesedb_values_tree_value_read_record";
	off64_t record_data_offset                                      = 0;
	size_t record_data_size                                         = 0;
	size_t remaining_definition_data_size                           = 0;
	uint16_t fixed_size_data_type_value_offset                      = 0;
	uint16_t previous_tagged_data_type_offset                       = 0;
	uint16_t previous_variable_size_data_type_size                  = 0;
	uint16_t tagged_data_type_offset_data_size                      = 0;
	uint16_t tagged_data_type_identifier                            = 0;
	uint16_t tagged_data_type_offset                                = 0;
	uint16_t tagged_data_type_size                                  = 0;
	uint16_t tagged_data_types_offset                               = 0;
	uint16_t tagged_data_type_value_offset                          = 0;
	uint16_t variable_size_data_type_offset                         = 0;
	uint16_t variable_size_data_type_size                           = 0;
	uint16_t variable_size_data_type_value_offset                   = 0;
	uint16_t variable_size_data_types_offset                        = 0;
	uint8_t current_variable_size_data_type                         = 0;
	uint8_t last_fixed_size_data_type                               = 0;
	uint8_t last_variable_size_data_type                            = 0;
	uint8_t number_of_variable_size_data_types                      = 0;
	uint8_t record_value_type                                       = 0;
	uint8_t tagged_data_types_format                                = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_INDEX;
	int column_catalog_definition_iterator                          = 0;
	int number_of_column_catalog_definitions                        = 0;
	int number_of_table_column_catalog_definitions                  = 0;
	int number_of_template_table_column_catalog_definitions         = 0;
	int record_value_codepage                                       = 0;

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( values_tree_value->type != LIBESEDB_VALUES_TREE_VALUE_TYPE_RECORD )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported values tree value type: 0x%02" PRIx8 ".",
		 function,
		 values_tree_value->type );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table definition.",
		 function );

		return( -1 );
	}
	if( table_definition->column_catalog_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table definition - missing column catalog definition list.",
		 function );

		return( -1 );
	}
	if( ( template_table_definition != NULL )
	 && ( template_table_definition->column_catalog_definition_list == NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid template table definition - missing column catalog definition list.",
		 function );

		return( -1 );
	}
	if( values_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid values array.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     file_io_handle,
	     pages_cache,
	     values_tree_value->page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
		 function,
		 values_tree_value->page_number,
		 values_tree_value->page_offset );

		goto on_error;
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		goto on_error;
	}
	if( libesedb_page_get_value(
	     page,
	     values_tree_value->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		goto on_error;
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		goto on_error;
	}
	if( page_value->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		goto on_error;
	}
	if( values_tree_value->data_offset > page_value->size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid values tree value - data offset exceeds page value size.",
		 function );

		goto on_error;
	}
	record_data        = &( page_value->data[ values_tree_value->data_offset ] );
	record_data_size   = page_value->size - values_tree_value->data_offset;
	record_data_offset = values_tree_value->page_offset + page_value->offset + values_tree_value->data_offset;

	if( record_data_size < sizeof( esedb_data_definition_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid record data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( ( io_handle->format_version == 0x620 )
	 && ( io_handle->format_revision <= 2 ) )
	{
		tagged_data_types_format = LIBESEDB_TAGGED_DATA_TYPES_FORMAT_LINEAR;
	}
	last_fixed_size_data_type    = ( (esedb_data_definition_header_t *) record_data )->last_fixed_size_data_type;
	last_variable_size_data_type = ( (esedb_data_definition_header_t *) record_data )->last_variable_size_data_type;

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_data_definition_header_t *) record_data )->variable_size_data_types_offset,
	 variable_size_data_types_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: last fixed size data type\t\t: %" PRIu8 "\n",
		 function,
		 last_fixed_size_data_type );

		libnotify_printf(
		 "%s: last variable size data type\t\t: %" PRIu8 "\n",
		 function,
		 last_variable_size_data_type );

		libnotify_printf(
		 "%s: variable size data types offset\t\t: %" PRIu16 "\n",
		 function,
		 variable_size_data_types_offset );
	}
#endif
	if( ( template_table_definition != NULL )
	 && ( template_table_definition->column_catalog_definition_list != NULL ) )
	{
		if( libesedb_list_get_number_of_elements(
		     template_table_definition->column_catalog_definition_list,
		     &number_of_template_table_column_catalog_definitions,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of template table column catalog definitions.",
			 function );

			goto on_error;
		}
	}
	if( libesedb_list_get_number_of_elements(
	     table_definition->column_catalog_definition_list,
	     &number_of_table_column_catalog_definitions,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of table column catalog definitions.",
		 function );

		goto on_error;
	}
	number_of_column_catalog_definitions = number_of_table_column_catalog_definitions;

	if( ( template_table_definition != NULL )
	 && ( template_table_definition->column_catalog_definition_list != NULL ) )
	{
		if( number_of_table_column_catalog_definitions > number_of_template_table_column_catalog_definitions )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid number of table column catalog definitions value exceeds number in template table.",
			 function );

			goto on_error;
		}
		number_of_column_catalog_definitions += number_of_template_table_column_catalog_definitions;
	}
	if( libesedb_array_resize(
	     values_array,
	     number_of_column_catalog_definitions,
	     &libfvalue_value_free_as_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize values array.",
		 function );

		goto on_error;
	}
	if( last_variable_size_data_type > 127 )
	{
		number_of_variable_size_data_types = last_variable_size_data_type - 127;
	}
	if( ( template_table_definition != NULL )
	 && ( template_table_definition->column_catalog_definition_list != NULL ) )
	{
		column_catalog_definition_list_element = template_table_definition->column_catalog_definition_list->first_element;
	}
	else
	{
		column_catalog_definition_list_element = table_definition->column_catalog_definition_list->first_element;
	}
	fixed_size_data_type_value_offset    = (uint16_t) sizeof( esedb_data_definition_header_t );
	current_variable_size_data_type      = 127;
	variable_size_data_type_offset       = variable_size_data_types_offset;
	variable_size_data_type_value_offset = variable_size_data_types_offset + ( number_of_variable_size_data_types * 2 );

	for( column_catalog_definition_iterator = 0;
	     column_catalog_definition_iterator < number_of_column_catalog_definitions;
	     column_catalog_definition_iterator++ )
	{
		if( column_catalog_definition_list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected for column catalog definition list element: %d.",
			 function,
			 column_catalog_definition_iterator );

			goto on_error;
		}
		if( column_catalog_definition_list_element->value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing column catalog definition list element: %d.",
			 function,
			 column_catalog_definition_iterator );

			goto on_error;
		}
		column_catalog_definition = (libesedb_catalog_definition_t *) column_catalog_definition_list_element->value;

		if( column_catalog_definition->type != LIBESEDB_CATALOG_DEFINITION_TYPE_COLUMN )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported column catalog definition type: %" PRIu16 " for list element: %d.",
			 function,
			 column_catalog_definition->type,
			 column_catalog_definition_iterator );

			goto on_error;
		}
		if( ( template_table_definition != NULL )
		 && ( template_table_definition->column_catalog_definition_list != NULL )
		 && ( column_catalog_definition_iterator == number_of_template_table_column_catalog_definitions ) )
		{
			if( column_catalog_definition->identifier != 256 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: only tagged data types supported in tables using a template table.",
				 function );

				goto on_error;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: column definition identifier\t\t: %03" PRIu32 "\n",
			 function,
			 column_catalog_definition->identifier );
			libnotify_printf(
			 "%s: column definition name\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 function,
			 column_catalog_definition->name_string );
			libnotify_printf(
			 "%s: column definition type\t\t\t: %s (%s)\n",
			 function,
			 libesedb_column_type_get_description(
			  column_catalog_definition->column_type ),
			 libesedb_column_type_get_identifier(
			  column_catalog_definition->column_type ) );
		}
#endif
		switch( column_catalog_definition->column_type )
		{
			case LIBESEDB_COLUMN_TYPE_NULL:
				/* TODO handle this value type */
				record_value_type = LIBFVALUE_VALUE_TYPE_UNDEFINED;
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
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported column type: %" PRIu32 ".",
				 function,
				 column_catalog_definition->column_type );

				goto on_error;
		}
		if( libfvalue_value_initialize(
		     &record_value,
		     record_value_type,
		     LIBFVALUE_VALUE_FLAG_DATA_MANAGED | LIBFVALUE_VALUE_FLAG_METADATA_MANAGED,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record value.",
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
			if( libfvalue_value_set_codepage(
			     record_value,
			     record_value_codepage,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set value codepage.",
				 function );

				goto on_error;
			}
		}
		if( column_catalog_definition->identifier <= 127 )
		{
			if( column_catalog_definition->identifier <= last_fixed_size_data_type )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%s: (%03" PRIu32 ") fixed size data type size\t\t: %" PRIu32 "\n",
					 function,
					 column_catalog_definition->identifier,
					 column_catalog_definition->size );
					libnotify_print_data(
					 &( record_data[ fixed_size_data_type_value_offset ] ),
					 column_catalog_definition->size );
				}
#endif
				/* record_data_offset + fixed_size_data_type_value_offset, */
				if( libfvalue_value_set_data(
				     record_value,
				     &( record_data[ fixed_size_data_type_value_offset ] ),
				     column_catalog_definition->size,
				     LIBFVALUE_ENDIAN_LITTLE,
				     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set data in fixed size data type definition.",
					 function );

					goto on_error;
				}
				if( column_catalog_definition->size > (uint32_t) UINT16_MAX )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
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
				byte_stream_copy_to_uint16_little_endian(
				 &( record_data[ variable_size_data_type_offset ] ),
				 variable_size_data_type_size );

				variable_size_data_type_offset += 2;

				current_variable_size_data_type++;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
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
#if defined( HAVE_DEBUG_OUTPUT )
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: (%03" PRIu32 ") variable size data type:\n",
							 function,
							 column_catalog_definition->identifier );
							libnotify_print_data(
							 &( record_data[ variable_size_data_type_value_offset ] ),
							 variable_size_data_type_size - previous_variable_size_data_type_size );
						}
#endif
						/* record_data_offset + variable_size_data_type_value_offset, */
						if( libfvalue_value_set_data(
						     record_value,
						     &( record_data[ variable_size_data_type_value_offset ] ),
						     variable_size_data_type_size - previous_variable_size_data_type_size,
						     LIBFVALUE_ENDIAN_LITTLE,
						     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in variable size data type definition.",
							 function );

							goto on_error;
						}
						variable_size_data_type_value_offset += variable_size_data_type_size - previous_variable_size_data_type_size;
						previous_variable_size_data_type_size = variable_size_data_type_size;
					}
#if defined( HAVE_DEBUG_OUTPUT )
					else if( libnotify_verbose != 0 )
					{
						libnotify_printf(
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
					if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type identifier\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_identifier );
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type size\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_size,
						 tagged_data_type_size & 0x5fff );
					}
#endif

					if( ( tagged_data_type_size & 0x8000 ) != 0 )
					{
#if defined( HAVE_DEBUG_OUTPUT )
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type flags\t\t: 0x%02" PRIx8 "\n",
							 function,
							 column_catalog_definition->identifier,
							 record_data[ tagged_data_type_value_offset ] );
							libesedb_debug_print_tagged_data_type_flags(
							 record_data[ tagged_data_type_value_offset ] );
							libnotify_printf(
							 "\n" );
						}
#endif

						if( libfvalue_value_set_metadata(
						     record_value,
						     &( record_data[ tagged_data_type_value_offset ] ),
						     1,
						     LIBFVALUE_VALUE_METADATA_FLAG_NON_MANAGED,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set tagged data type flags in tagged data type definition.",
							 function );

							goto on_error;
						}
						tagged_data_type_value_offset  += 1;
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
							 &( record_data[ tagged_data_type_value_offset ] ),
							 tagged_data_type_size );
						}
						else
						{
							libnotify_printf(
							 "%s: (%03" PRIu32 ") tagged data type\t\t\t: <NULL>\n",
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
							 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type size value exceeds remaining data size.",
							 function );

							goto on_error;
						}
						/* record_data_offset + tagged_data_type_value_offset, */
						if( libfvalue_value_set_data(
						     record_value,
						     &( record_data[ tagged_data_type_value_offset ] ),
						     tagged_data_type_size,
						     LIBFVALUE_ENDIAN_LITTLE,
						     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in tagged data type definition.",
							 function );

							goto on_error;
						}
						remaining_definition_data_size -= tagged_data_type_size;
					}
					if( remaining_definition_data_size > 0 )
					{
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
					tagged_data_types_offset       = variable_size_data_type_value_offset;
					tagged_data_type_offset_data   = &( record_data[ tagged_data_types_offset ] );
					remaining_definition_data_size = record_data_size - (size_t) tagged_data_types_offset;

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
							 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type offset value out of bounds.",
							 function );

							goto on_error;
						}
						tagged_data_type_offset_data_size = ( tagged_data_type_offset & 0x3fff ) - 4;

						remaining_definition_data_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: tagged data type offset data size\t: %" PRIu16 "\n",
							 function,
							 tagged_data_type_offset_data_size );
							libnotify_printf(
							 "%s: tagged data type offset data:\n",
							 function );
							libnotify_print_data(
							 tagged_data_type_offset_data,
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
						 "%s: (%03" PRIu16 ") tagged data type identifier\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_identifier );
						libnotify_printf(
						 "%s: (%03" PRIu16 ") tagged data type offset\t\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
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
						 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
						 "%s: invalid tagged data type offset value exceeds next tagged data type offset.",
						 function );

						goto on_error;
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
						 "%s: (%03" PRIu16 ") tagged data type size\t\t: %" PRIu16 "\n",
						 function,
						 column_catalog_definition->identifier,
						 tagged_data_type_size );
					}
#endif
					tagged_data_type_value_offset = tagged_data_types_offset + ( previous_tagged_data_type_offset & 0x3fff );

					if( tagged_data_type_size > 0 )
					{
						if( tagged_data_type_size > remaining_definition_data_size )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
							 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
							 "%s: invalid tagged data type size value exceeds remaining data size.",
							 function );

							goto on_error;
						}
						remaining_definition_data_size -= tagged_data_type_size;

						if( ( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
						  &&  ( io_handle->page_size >= 16384 ) )
						 || ( ( previous_tagged_data_type_offset & 0x4000 ) != 0 ) )
						{
#if defined( HAVE_DEBUG_OUTPUT )
							if( libnotify_verbose != 0 )
							{
								libnotify_printf(
								 "%s: (%03" PRIu16 ") tagged data type flags\t\t: 0x%02" PRIx8 "\n",
								 function,
								 column_catalog_definition->identifier,
								 record_data[ tagged_data_type_value_offset ] );
								libesedb_debug_print_tagged_data_type_flags(
								 record_data[ tagged_data_type_value_offset ] );
								libnotify_printf(
								 "\n" );
							}
#endif
							if( libfvalue_value_set_metadata(
							     record_value,
							     &( record_data[ tagged_data_type_value_offset ] ),
							     1,
							     LIBFVALUE_VALUE_METADATA_FLAG_NON_MANAGED,
							     error ) != 1 )
							{
								liberror_error_set(
								 error,
								 LIBERROR_ERROR_DOMAIN_RUNTIME,
								 LIBERROR_RUNTIME_ERROR_SET_FAILED,
								 "%s: unable to set tagged data type flags in tagged data type definition.",
								 function );

								goto on_error;
							}
							tagged_data_type_value_offset += 1;
							tagged_data_type_size         -= 1;
						}
#if defined( HAVE_DEBUG_OUTPUT )
						if( libnotify_verbose != 0 )
						{
							libnotify_printf(
							 "%s: (%03" PRIu16 ") tagged data type:\n",
							 function,
							 column_catalog_definition->identifier );
							libnotify_print_data(
							 &( record_data[ tagged_data_type_value_offset ] ),
							 tagged_data_type_size );
						}
#endif
					}
#if defined( HAVE_DEBUG_OUTPUT )
					/* TODO are zero size tagged data type values handled correctly?
					 */
					else if( libnotify_verbose != 0 )
					{
						libnotify_printf(
						 "%s: (%03" PRIu32 ") tagged data type\t\t\t: <NULL>\n",
						 function,
						 column_catalog_definition->identifier );
					}
#endif
					if( tagged_data_type_size > 0 )
					{
						/* record_data_offset + tagged_data_type_value_offset, */
						if( libfvalue_value_set_data(
						     record_value,
						     &( record_data[ tagged_data_type_value_offset ] ),
						     tagged_data_type_size,
						     LIBFVALUE_ENDIAN_LITTLE,
						     LIBFVALUE_VALUE_DATA_FLAG_NON_MANAGED,
						     error ) != 1 )
						{
							liberror_error_set(
							 error,
							 LIBERROR_ERROR_DOMAIN_RUNTIME,
							 LIBERROR_RUNTIME_ERROR_SET_FAILED,
							 "%s: unable to set data in tagged data type definition.",
							 function );

							goto on_error;
						}
					}
				}
			}
		}
		if( libesedb_array_set_entry_by_index(
		     values_array,
		     column_catalog_definition_iterator,
		     (intptr_t *) record_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to set data type definition: %d.",
			 function,
			 column_catalog_definition_iterator );

			goto on_error;
		}
		record_value = NULL;

		if( ( column_catalog_definition_list_element->next_element == NULL )
		 && ( template_table_definition != NULL )
		 && ( template_table_definition->column_catalog_definition_list != NULL ) )
		{
			column_catalog_definition_list_element = table_definition->column_catalog_definition_list->first_element;
		}
		else
		{
			column_catalog_definition_list_element = column_catalog_definition_list_element->next_element;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		if( fixed_size_data_type_value_offset < variable_size_data_types_offset )
		{
			libnotify_printf(
			 "%s: fixed size data types trailing data:\n",
			 function );
			libnotify_print_data(
			 &( record_data[ fixed_size_data_type_value_offset ] ),
			 variable_size_data_types_offset - fixed_size_data_type_value_offset );
		}
		libnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( record_value != NULL )
	{
		libfvalue_value_free(
		 &record_value,
		 NULL );
	}
	return( -1 );
}

/* Reads the long value
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_read_long_value(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	uint8_t *long_value_data          = NULL;
	static char *function             = "libesedb_values_tree_value_read_long_value";
	size_t long_value_data_size       = 0;
	uint32_t value_32bit              = 0;

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( values_tree_value->type != LIBESEDB_VALUES_TREE_VALUE_TYPE_LONG_VALUE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported values tree value type: 0x%02" PRIx8 ".",
		 function,
		 values_tree_value->type );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     file_io_handle,
	     pages_cache,
	     values_tree_value->page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
		 function,
		 values_tree_value->page_number,
		 values_tree_value->page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     values_tree_value->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		return( -1 );
	}
	if( values_tree_value->data_offset > page_value->size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid values tree value - data offset exceeds page value size.",
		 function );

		return( -1 );
	}
	long_value_data      = &( page_value->data[ values_tree_value->data_offset ] );
	long_value_data_size = page_value->size - values_tree_value->data_offset;

	if( long_value_data_size != 8 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported long values data size: %" PRIzd ".",
		 function,
		 long_value_data_size );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: long value data:\n",
		 function );
		libnotify_print_data(
		 long_value_data,
		 long_value_data_size );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 long_value_data,
	 value_32bit );

	long_value_data += 4;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
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
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: last segment offset\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the long value segment
 * Returns 1 if successful or -1 on error
 */
int libesedb_values_tree_value_read_long_value_segment(
     libesedb_values_tree_value_t *values_tree_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     uint32_t long_value_segment_offset,
     libfdata_block_t *data_block,
     liberror_error_t **error )
{
	libesedb_page_t *page                  = NULL;
	libesedb_page_value_t *page_value      = NULL;
	static char *function                  = "libesedb_values_tree_value_read_long_value_segment";
	off64_t long_value_segment_data_offset = 0;
	size_t long_value_segment_data_size    = 0;
	size64_t data_size                     = 0;

	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( values_tree_value->type != LIBESEDB_VALUES_TREE_VALUE_TYPE_LONG_VALUE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported values tree value type: 0x%02" PRIx8 ".",
		 function,
		 values_tree_value->type );

		return( -1 );
	}
	if( data_block == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     pages_vector,
	     file_io_handle,
	     pages_cache,
	     values_tree_value->page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
		 function,
		 values_tree_value->page_number,
		 values_tree_value->page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     values_tree_value->page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 values_tree_value->page_value_index );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value data.",
		 function );

		return( -1 );
	}
	if( values_tree_value->data_offset > page_value->size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid values tree value - data offset exceeds page value size.",
		 function );

		return( -1 );
	}
	long_value_segment_data_size = page_value->size - values_tree_value->data_offset;

	/* Note that the data block will point to the file offset
	 * values_tree_value->page_offset contains the offset relative from the start of the page data
	 */
	long_value_segment_data_offset = io_handle->pages_data_offset + values_tree_value->page_offset
	                               + values_tree_value->data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: long value segment with offset: %" PRIu32 " has data at offset: %" PRIi64 " of size: %" PRIzd "\n",
		 function,
		 long_value_segment_offset,
		 long_value_segment_data_offset,
		 long_value_segment_data_size );
		libnotify_printf(
		 "\n" );
	}
#endif
	if( libfdata_block_get_size(
	     data_block,
	     &data_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data block size.",
		 function );

		return( -1 );
	}
	if( long_value_segment_offset != (off64_t) data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported long value segment offset: %" PRIi64 " value must match end of previous segment: %" PRIzd ".",
		 function,
		 long_value_segment_offset,
		 data_size );

		return( -1 );
	}
	if( libfdata_block_append_segment(
	     data_block,
	     long_value_segment_data_offset,
	     (size64_t) long_value_segment_data_size,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append long value segment at offset: %" PRIu32 " to data block.",
		 function,
		 long_value_segment_offset );

		return( -1 );
	}
	return( 1 );
}

