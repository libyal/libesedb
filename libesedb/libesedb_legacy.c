/*
 * Legacy functions
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
#include <types.h>

#include <liberror.h>

#include "libesedb_file.h"
#include "libesedb_legacy.h"
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
     liberror_error_t **error )
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
     liberror_error_t **error )
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
     liberror_error_t **error )
{
	return( libesedb_record_get_number_of_values(
	         record,
	         amount_of_values,
	         error ) );
}

/* Retrieves the amount of columns in the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_columns(
     libesedb_table_t *table,
     int *amount_of_columns,
     liberror_error_t **error )
{
	return( libesedb_table_get_number_of_columns(
	         table,
	         amount_of_columns,
	         error ) );
}

/* Retrieves the amount of indexes
 * Returns 1 if successful or -1 on error
 */
int libesedb_table_get_amount_of_indexes(
     libesedb_table_t *table,
     int *amount_of_indexes,
     liberror_error_t **error )
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
     liberror_error_t **error )
{
	return( libesedb_table_get_number_of_records(
	         table,
	         amount_of_records,
	         error ) );
}

#endif /* !defined( HAVE_LOCAL_LIBESEDB ) */

