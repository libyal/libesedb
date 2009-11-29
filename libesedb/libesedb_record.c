/*
 * Record functions
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

#include "libesedb_data_definition.h"
#include "libesedb_data_type_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_long_value.h"
#include "libesedb_multi_value.h"
#include "libesedb_record.h"
#include "libesedb_table.h"
#include "libesedb_types.h"
#include "libesedb_value_type.h"

/* Creates a record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_initialize(
     libesedb_record_t **record,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                      = "libesedb_record_initialize";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record == NULL )
	{
		internal_record = (libesedb_internal_record_t *) memory_allocate(
		                                                  sizeof( libesedb_internal_record_t ) );

		if( internal_record == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create record.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_record,
		     0,
		     sizeof( libesedb_internal_record_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear record.",
			 function );

			memory_free(
			 internal_record );

			return( -1 );
		}
		*record = (libesedb_record_t *) internal_record;
	}
	return( 1 );
}

/* Frees record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_free(
     libesedb_record_t **record,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_free";
	int result                                  = 1;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		internal_record = (libesedb_internal_record_t *) *record;
		*record         = NULL;

		/* The internal_table and data_definition references
		 * are freed elsewhere
		 */
		if( libesedb_record_detach(
		     internal_record,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to detach internal record.",
			 function );

			return( -1 );
		}
		memory_free(
		 internal_record );
	}
	return( result );
}

/* Attaches the record to the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_attach(
     libesedb_internal_record_t *internal_record,
     libesedb_internal_table_t *internal_table,
     libesedb_data_definition_t *data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_record_attach";

	if( internal_record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal record.",
		 function );

		return( -1 );
	}
	if( internal_record->internal_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal record - already attached to table.",
		 function );

		return( -1 );
	}
	internal_record->internal_table  = internal_table;
	internal_record->data_definition = data_definition;

	return( 1 );
}

/* Detaches the record from its table reference
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_detach(
     libesedb_internal_record_t *internal_record,
     liberror_error_t **error )
{
	static char *function = "libesedb_record_detach";

	if( internal_record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal record.",
		 function );

		return( -1 );
	}
	if( internal_record->internal_table != NULL )
	{
		internal_record->internal_table  = NULL;
		internal_record->data_definition = NULL;
	}
	return( 1 );
}

/* Retrieves the amount of values in the record of the referenced record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_amount_of_values(
     libesedb_record_t *record,
     int *amount_of_values,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_get_amount_of_values";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_amount_of_entries(
	     internal_record->data_definition->values_array,
	     amount_of_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of data type definitions.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the column identifier of the specific entry from the referenced record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_column_identifier(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_identifier,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_column_identifier";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( column_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column identifier.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	*column_identifier = data_type_definition->column_catalog_definition->identifier;

	return( 1 );
}

/* Retrieves the column type of the specific entry from the referenced record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_column_type(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *column_type,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_column_type";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
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
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	*column_type = data_type_definition->column_catalog_definition->column_type;

	return( 1 );
}

/* Retrieves the UTF-8 string size of the column name of the specific entry from the referenced record
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_utf8_column_name_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_utf8_column_name_size";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( utf8_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string size.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	*utf8_string_size = data_type_definition->column_catalog_definition->name_size;

	return( 1 );
}

/* Retrieves the UTF-8 string of the column name of the specific entry from the referenced record
 * The string is formatted in UTF-8
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_utf8_column_name(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_utf8_column_name";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( utf8_string_size < data_type_definition->column_catalog_definition->name_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string is too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     utf8_string,
	     data_type_definition->column_catalog_definition->name,
	     data_type_definition->column_catalog_definition->name_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value of the specific entry from the referenced record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_value(
     libesedb_record_t *record,
     int value_entry,
     uint8_t **value_data,
     size_t *value_data_size,
     uint8_t *value_flags,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data size.",
		 function );

		return( -1 );
	}
	if( value_flags == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value flags.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	*value_data      = data_type_definition->data;
	*value_data_size = data_type_definition->data_size;
	*value_flags     = data_type_definition->flags;

	return( 1 );
}

/* Retrieves the boolean value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_boolean(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_boolean";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_BOOLEAN )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_8bit(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set boolean value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the 8-bit value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_8bit(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_8bit";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_8bit(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set 8-bit value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the 16-bit value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_16bit(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_16bit";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_16bit(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set 16-bit value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the 32-bit value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_32bit(
     libesedb_record_t *record,
     int value_entry,
     uint32_t *value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_32bit";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_32bit(
	     data_type_definition->data,
	     data_type_definition->data_size,
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
	return( 1 );
}

/* Retrieves the 64-bit value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_64bit(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_64bit";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_CURRENCY )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_64bit(
	     data_type_definition->data,
	     data_type_definition->data_size,
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
	return( 1 );
}

/* Retrieves the 64-bit filetime value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_filetime(
     libesedb_record_t *record,
     int value_entry,
     uint64_t *value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_filetime";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_DATE_TIME )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_64bit(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     value,
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
	return( 1 );
}

/* Retrieves the floating point value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_floating_point(
     libesedb_record_t *record,
     int value_entry,
     double *value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_floating_point";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_FLOAT_32BIT )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_floating_point(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_CONVERSION,
		 LIBERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set floating point value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of a specific entry from the referenced record
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf8_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_utf8_string_size";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_get_utf8_string_size(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     data_type_definition->column_catalog_definition->codepage,
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
	return( 1 );
}

/* Retrieves the UTF-8 string value of a specific entry from the referenced record
 * The string is formatted in UTF-8
 * The function uses a codepage if necessary, it tries to read the codepage in the column definition
 * if not available it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf8_string(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_utf8_string";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_utf8_string(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     data_type_definition->column_catalog_definition->codepage,
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
	return( 1 );
}

/* Retrieves the UTF-16 string size of a specific entry from the referenced record
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf16_string_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *utf16_string_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_utf16_string_size";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_get_utf16_string_size(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     data_type_definition->column_catalog_definition->codepage,
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
	return( 1 );
}

/* Retrieves the UTF-16 string value of a specific entry from the referenced record
 * The string is formatted in UTF-16
 * The function uses a codepage if necessary, it tries to read the codepage in the column definition
 * if not available it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_utf16_string(
     libesedb_record_t *record,
     int value_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_utf16_string";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_utf16_string(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     data_type_definition->column_catalog_definition->codepage,
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
	return( 1 );
}

/* Retrieves the binary data size of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_binary_data_size(
     libesedb_record_t *record,
     int value_entry,
     size_t *binary_data_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_binary_data_size";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_get_binary_data_size(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     binary_data_size,
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
	return( 1 );
}

/* Retrieves the binary data value of a specific entry from the referenced record
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_binary_data(
     libesedb_record_t *record,
     int value_entry,
     uint8_t *binary_data,
     size_t binary_data_size,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	static char *function                                 = "libesedb_record_get_value_binary_data";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_BINARY_DATA )
	 && ( data_type_definition->column_catalog_definition->column_type != LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition column type: %" PRIu32 ".",
		 function,
		 data_type_definition->column_catalog_definition->column_type );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_value_type_copy_to_binary_data(
	     data_type_definition->data,
	     data_type_definition->data_size,
	     binary_data,
	     binary_data_size,
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
	return( 1 );
}

/* Retrieves the long value of a specific entry from the referenced record
 * Creates a new long value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libesedb_record_get_long_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_long_value_t **long_value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_long_value_t *internal_long_value   = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	uint8_t *value_data                                   = NULL;
	static char *function                                 = "libesedb_record_get_long_value";
	uint16_t value_offset_iterator                        = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( long_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	if( *long_value != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: long value already set.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) != LIBESEDB_VALUE_FLAG_LONG_VALUE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition flags: 0x%02" PRIx8 ".",
		 function,
		 data_type_definition->flags );

		return( -1 );
	}
	if( ( data_type_definition->flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) == LIBESEDB_VALUE_FLAG_MULTI_VALUE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition flags: 0x%02" PRIx8 ".",
		 function,
		 data_type_definition->flags );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_long_value_initialize(
	     long_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create long value.",
		 function );

		return( -1 );
	}
	/* TODO
	 * get the long value indentifier
	 * find the corresponding long value page tree entry
	 */

	return( 1 );
}

/* Retrieves the multi value of a specific entry from the referenced record
 * Creates a new multi value
 * Returns 1 if successful, 0 if the item does not contain such value or -1 on error
 */
int libesedb_record_get_multi_value(
     libesedb_record_t *record,
     int value_entry,
     libesedb_multi_value_t **multi_value,
     liberror_error_t **error )
{
	libesedb_data_type_definition_t *data_type_definition = NULL;
	libesedb_internal_multi_value_t *internal_multi_value = NULL;
	libesedb_internal_record_t *internal_record           = NULL;
	uint8_t *value_data                                   = NULL;
	static char *function                                 = "libesedb_record_get_multi_value";
	uint16_t value_offset_iterator                        = 0;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
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
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: multi value already set.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     internal_record->data_definition->values_array,
	     value_entry,
	     (intptr_t **) &data_type_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data type definition: %d.",
		 function,
		 value_entry );

		return( -1 );
	}
	if( data_type_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition.",
		 function );

		return( -1 );
	}
	if( ( data_type_definition->flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != LIBESEDB_VALUE_FLAG_MULTI_VALUE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition flags: 0x%02" PRIx8 ".",
		 function,
		 data_type_definition->flags );

		return( -1 );
	}
	if( ( ( data_type_definition->flags & LIBESEDB_VALUE_FLAG_LONG_VALUE ) == LIBESEDB_VALUE_FLAG_LONG_VALUE )
	 || ( ( data_type_definition->flags & 0x10 ) == 0x10 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported data type definition flags: 0x%02" PRIx8 ".",
		 function,
		 data_type_definition->flags );

		return( -1 );
	}
	if( data_type_definition->column_catalog_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data type definition - missing column catalog definition.",
		 function );

		return( -1 );
	}
	if( data_type_definition->data == NULL )
	{
		return( 0 );
	}
	if( libesedb_multi_value_initialize(
	     multi_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create multi value.",
		 function );

		return( -1 );
	}
	internal_multi_value = (libesedb_internal_multi_value_t *) *multi_value;

	internal_multi_value->column_type     = data_type_definition->column_catalog_definition->column_type;
	internal_multi_value->value_data_size = data_type_definition->data_size;

	internal_multi_value->value_data = (uint8_t *) memory_allocate(
							internal_multi_value->value_data_size );

	if( internal_multi_value->value_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create multi value data.",
		 function );

		libesedb_multi_value_free(
		 multi_value,
		 NULL );

		return( -1 );
	}
	if( memory_copy(
	     internal_multi_value->value_data,
	     data_type_definition->data,
	     internal_multi_value->value_data_size ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set multi value data.",
		 function );

		libesedb_multi_value_free(
		 multi_value,
		 NULL );

		return( -1 );
	}
	value_data = internal_multi_value->value_data;

	/* The first 2 byte contain the offset to the first value
	 * there is an offset for every value
	 * therefore first offset / 2 = the amount of values
	 */
	byte_stream_copy_to_uint16_little_endian(
	 value_data,
	 internal_multi_value->amount_of_values );

	internal_multi_value->amount_of_values /= 2;

	if( internal_multi_value->amount_of_values > 0 )
	{
		internal_multi_value->value_offset = (uint16_t *) memory_allocate(
								   sizeof( uint16_t ) * internal_multi_value->amount_of_values );

		if( internal_multi_value->value_offset == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create multi value offsets.",
			 function );

			libesedb_multi_value_free(
			 multi_value,
			 NULL );

			return( -1 );
		}
		internal_multi_value->value_size = (size_t *) memory_allocate(
							       sizeof( size_t ) * internal_multi_value->amount_of_values );

		if( internal_multi_value->value_offset == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create multi value offsets.",
			 function );

			libesedb_multi_value_free(
			 multi_value,
			 NULL );

			return( -1 );
		}
		for( value_offset_iterator = 0;
		     value_offset_iterator < internal_multi_value->amount_of_values;
		     value_offset_iterator++ )
		{
			byte_stream_copy_to_uint16_little_endian(
			 value_data,
			 internal_multi_value->value_offset[ value_offset_iterator ] );

			value_data += 2;

			if( internal_multi_value->value_offset[ value_offset_iterator ] > internal_multi_value->value_data_size )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
				 "%s: value offset: %" PRIu32 " exceeds value data size: %" PRIzd ".",
				 function,
				 internal_multi_value->value_offset[ value_offset_iterator ],
				 internal_multi_value->value_data_size );

				libesedb_multi_value_free(
				 multi_value,
				 NULL );

				return( -1 );
			}
			if( value_offset_iterator > 0 )
			{
				internal_multi_value->value_size[ value_offset_iterator - 1 ] = internal_multi_value->value_offset[ value_offset_iterator ]
											      - internal_multi_value->value_offset[ value_offset_iterator - 1 ];
			}
		}
		internal_multi_value->value_size[ value_offset_iterator - 1 ] = internal_multi_value->value_data_size
									      - internal_multi_value->value_offset[ value_offset_iterator - 1 ];

		internal_multi_value->codepage = data_type_definition->column_catalog_definition->codepage;
	}
	return( 1 );
}

