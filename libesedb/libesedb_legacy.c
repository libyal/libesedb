/*
 * Legacy functions
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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
#include <types.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_file.h"
#include "libesedb_legacy.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfvalue.h"
#include "libesedb_multi_value.h"
#include "libesedb_record.h"
#include "libesedb_table.h"
#include "libesedb_types.h"

#if !defined( HAVE_LOCAL_LIBESEDB )

/* Retrieves the amount of tables
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_amount_of_tables(
     libesedb_file_t *file,
     int *amount_of_tables,
     libcerror_error_t **error )
{
	return( libesedb_file_get_number_of_tables(
	         file,
	         amount_of_tables,
	         error ) );
}

/* Retrieves the amount of values of the multi value
 * Returns 1 if successful or -1 on error
 */
int libesedb_multi_value_get_amount_of_values(
     libesedb_multi_value_t *multi_value,
     int *amount_of_values,
     libcerror_error_t **error )
{
	return( libesedb_multi_value_get_number_of_values(
	         multi_value,
	         amount_of_values,
	         error ) );
}

/* Retrieves the amount of values in the record of the referenced record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_amount_of_values(
     libesedb_record_t *record,
     int *amount_of_values,
     libcerror_error_t **error )
{
	return( libesedb_record_get_number_of_values(
	         record,
	         amount_of_values,
	         error ) );
}

/* Retrieves the double precision floating point value of a specific entry
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_get_value_floating_point(
     libesedb_record_t *record,
     int value_entry,
     double *value_floating_point,
     libcerror_error_t **error )
{
	libesedb_catalog_definition_t *column_catalog_definition = NULL;
	libesedb_internal_record_t *internal_record              = NULL;
	libfvalue_value_t *record_value                          = NULL;
	static char *function                                    = "libesedb_record_get_value_floating_point";
	uint32_t column_type                                     = 0;
	int result                                               = 0;

	if( record == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( libesedb_record_get_column_catalog_definition(
	     internal_record,
	     value_entry,
	     &column_catalog_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve column catalog definition.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_definition_get_column_type(
	     column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_FLOAT_32BIT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     internal_record->values_array,
	     value_entry,
	     (intptr_t **) &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value: %d from values array.",
		 function,
		 value_entry );

		return( -1 );
	}
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if value: %d has data.",
		 function,
		 value_entry );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_copy_to_double(
		     record_value,
		     0,
		     value_floating_point,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to floating point value.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the amount of columns in the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_columns(
     libesedb_table_t *table,
     int *amount_of_columns,
     libcerror_error_t **error )
{
	return( libesedb_table_get_number_of_columns(
	         table,
	         amount_of_columns,
	         LIBESEDB_GET_COLUMN_FLAG_IGNORE_TEMPLATE_TABLE,
	         error ) );
}

/* Retrieves the amount of indexes
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_indexes(
     libesedb_table_t *table,
     int *amount_of_indexes,
     libcerror_error_t **error )
{
	return( libesedb_table_get_number_of_indexes(
	         table,
	         amount_of_indexes,
	         error ) );
}

/* Retrieves the amount of records in the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_records(
     libesedb_table_t *table,
     int *amount_of_records,
     libcerror_error_t **error )
{
	return( libesedb_table_get_number_of_records(
	         table,
	         amount_of_records,
	         error ) );
}

#endif /* !defined( HAVE_LOCAL_LIBESEDB ) */

