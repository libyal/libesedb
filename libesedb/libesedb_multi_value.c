/*
 * Multi value functions
 *
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
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_definitions.h"
#include "libesedb_multi_value.h"
#include "libesedb_value_type.h"
#include "libesedb_types.h"

/* Initializes the multi value and its values
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_initialize(
     libesedb_multi_value_t **multi_value,
     liberror_error_t **error )
{
	libesedb_internal_multi_value_t *internal_multi_value = NULL;
	static char *function                                 = "libesedb_multi_value_initialize";

	if( multi_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	if( *multi_value == NULL )
	{
		internal_multi_value = (libesedb_internal_multi_value_t *) memory_allocate(
		                                                            sizeof( libesedb_internal_multi_value_t ) );

		if( internal_multi_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create internal multi value.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_multi_value,
		     0,
		     sizeof( libesedb_internal_multi_value_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear internal multi value.",
			 function );

			memory_free(
			 internal_multi_value );

			return( -1 );
		}
		*multi_value = (libesedb_multi_value_t *) internal_multi_value;
	}
	return( 1 );
}

/* Frees a multi value
 * Detaches the reference from the file if necessary
 * Return 1 if successful or -1 on error
 */
int libesedb_multi_value_free(
     libesedb_multi_value_t **multi_value,
     liberror_error_t **error )
{
	static char *function = "libesedb_multi_value_free";

	if( multi_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	if( *multi_value != NULL )
	{
		if( ( (libesedb_internal_multi_value_t *) *multi_value )->value_data != NULL )
		{
			memory_free(
			 ( (libesedb_internal_multi_value_t *) *multi_value )->value_data );
		}
		if( ( (libesedb_internal_multi_value_t *) *multi_value )->value_offset != NULL )
		{
			memory_free(
			 ( (libesedb_internal_multi_value_t *) *multi_value )->value_offset );
		}
		if( ( (libesedb_internal_multi_value_t *) *multi_value )->value_size != NULL )
		{
			memory_free(
			 ( (libesedb_internal_multi_value_t *) *multi_value )->value_size );
		}
		memory_free(
		 *multi_value );

		*multi_value = NULL;
	}
	return( 1 );
}

/* Retrieves the amount of values of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_amount_of_values(
     libesedb_multi_value_t *multi_value,
     int *amount_of_values,
     liberror_error_t **error )
{
	libesedb_internal_multi_value_t *internal_multi_value = NULL;
	static char *function                                 = "libesedb_multi_value_get_amount_of_entries";

	if( multi_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	internal_multi_value = (libesedb_internal_multi_value_t *) multi_value;

	if( amount_of_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of values.",
		 function );

		return( -1 );
	}
	*amount_of_values = (int) internal_multi_value->amount_of_values;

	return( 1 );
}

/* Retrieves a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value(
     libesedb_multi_value_t *multi_value,
     int value_index,
     uint32_t *column_type,
     uint8_t **value_data,
     size_t *value_data_size,
     liberror_error_t **error )
{
	libesedb_internal_multi_value_t *internal_multi_value = NULL;
	static char *function                                 = "libesedb_multi_value_get_value";
	uint32_t value_offset                                 = 0;

	if( multi_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid multi value.",
		 function );

		return( -1 );
	}
	internal_multi_value = (libesedb_internal_multi_value_t *) multi_value;

	if( column_type == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column type.",
		 function );

		return( -1 );
	}
	if( ( value_index < 0 )
	 || ( value_index >= (int) internal_multi_value->amount_of_values ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid value index.",
		 function );

		return( -1 );
	}
	value_offset = internal_multi_value->value_offset[ value_index ];

	*column_type     = internal_multi_value->column_type;
	*value_data_size = internal_multi_value->value_size[ value_index ];

	if( internal_multi_value->value_size[ value_index ] == 0 )
	{
		*value_data = NULL;
	}
	else
	{
		*value_data = &( internal_multi_value->value_data[ value_offset ] );
	}
	return( 1 );
}

/* Retrieves the 32-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_32bit(
     libesedb_multi_value_t *multi_value,
     int value_index,
     uint32_t *value,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_32bit";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libesedb_value_type_copy_to_32bit(
		     value_data,
		     value_data_size,
		     value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set 32-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_64bit(
     libesedb_multi_value_t *multi_value,
     int value_index,
     uint64_t *value,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_64bit";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libesedb_value_type_copy_to_64bit(
		     value_data,
		     value_data_size,
		     value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set 64-bit value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the 64-bit filetime value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_filetime(
     libesedb_multi_value_t *multi_value,
     int value_index,
     uint64_t *filetime,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_filetime";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libesedb_value_type_copy_to_64bit(
		     value_data,
		     value_data_size,
		     filetime,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set filetime value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string size of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_utf8_string_size(
     libesedb_multi_value_t *multi_value,
     int value_index,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_utf8_string_size";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
		 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string column type: 0x%04" PRIx32 ".",
			 function,
			 column_type );

			return( -1 );
		}
		if( libesedb_value_type_get_utf8_string_size(
		     value_data,
		     value_data_size,
		     ( (libesedb_internal_multi_value_t *) multi_value )->codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 string value of a specific value of the multi value
 * The string is formatted in UTF-8
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_utf8_string(
     libesedb_multi_value_t *multi_value,
     int value_index,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_utf8_string";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
		 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string column type: 0x%04" PRIx32 ".",
			 function,
			 column_type );

			return( -1 );
		}
		if( libesedb_value_type_copy_to_utf8_string(
		     value_data,
		     value_data_size,
		     ( (libesedb_internal_multi_value_t *) multi_value )->codepage,
		     utf8_string,
		     utf8_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string size of a specific value of the multi value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_utf16_string_size(
     libesedb_multi_value_t *multi_value,
     int value_index,
     size_t *utf16_string_size,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_utf16_string_size";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
		 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string column type: 0x%04" PRIx32 ".",
			 function,
			 column_type );

			return( -1 );
		}
		if( libesedb_value_type_get_utf16_string_size(
		     value_data,
		     value_data_size,
		     ( (libesedb_internal_multi_value_t *) multi_value )->codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 string value of a specific value of the multi value
 * The string is formatted in UTF-16
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_utf16_string(
     libesedb_multi_value_t *multi_value,
     int value_index,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_utf16_string";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
		 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported string column type: 0x%04" PRIx32 ".",
			 function,
			 column_type );

			return( -1 );
		}
		if( libesedb_value_type_copy_to_utf16_string(
		     value_data,
		     value_data_size,
		     ( (libesedb_internal_multi_value_t *) multi_value )->codepage,
		     utf16_string,
		     utf16_string_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of a binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_binary_data_size(
     libesedb_multi_value_t *multi_value,
     int value_index,
     size_t *size,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_binary_data_size";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libesedb_value_type_get_binary_data_size(
		     value_data,
		     value_data_size,
		     size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set binary data size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the binary data value of a specific value of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_value_binary_data(
     libesedb_multi_value_t *multi_value,
     int value_index,
     uint8_t *binary_data,
     size_t size,
     liberror_error_t **error )
{
	uint8_t *value_data    = NULL;
	static char *function  = "libesedb_multi_value_get_value_binary_data";
	size_t value_data_size = 0;
	uint32_t column_type   = 0;
	int result             = 0;

	result = libesedb_multi_value_get_value(
	          multi_value,
	          value_index,
	          &column_type,
	          &value_data,
	          &value_data_size,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value.",
		 function );
	}
	else if( result == 1 )
	{
		if( libesedb_value_type_copy_to_binary_data(
		     value_data,
		     value_data_size,
		     binary_data,
		     size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_CONVERSION,
			 LIBERROR_CONVERSION_ERROR_GENERIC,
			 "%s: unable to set binary data.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

