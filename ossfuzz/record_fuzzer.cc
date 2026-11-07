/*
 * OSS-Fuzz target for libesedb record type
 *
 * Copyright (C) 2011-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libesedb.h"

#if !defined( LIBESEDB_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBESEDB_EXTERN \
int libesedb_file_open_file_io_handle(
      libesedb_file_t *file,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libesedb_error_t **error );

#endif /* !defined( LIBESEDB_HAVE_BFIO ) */

int LLVMFuzzerTestOneInput(
      const uint8_t *data,
      size_t size )
{
	uint8_t value_data[ 256 ];

	libbfio_handle_t *file_io_handle = NULL;
	libesedb_file_t *file            = NULL;
	libesedb_record_t *record        = NULL;
	libesedb_table_t *table          = NULL;
	size_t utf8_column_name_size     = 0;
	size_t value_data_size           = 0;
	uint32_t column_identifier       = 0;
	uint32_t column_type             = 0;
	uint8_t value_data_flags         = 0;
	int number_of_records            = 0;
	int number_of_tables             = 0;
	int number_of_values             = 0;
	int value_index                  = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libesedb_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libesedb_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBESEDB_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libesedb_file;
	}
	if( libesedb_file_get_number_of_tables(
	     file,
	     &number_of_tables,
	     NULL ) != 1 )
	{
		goto on_error_libesedb_file;
	}
	if( number_of_tables > 0 )
	{
		if( libesedb_file_get_table(
		     file,
		     0,
		     &table,
		     NULL ) != 1 )
		{
			goto on_error_libesedb_file;
		}
		if( libesedb_table_get_number_of_records(
		     table,
		     &number_of_records,
		     NULL ) != 1 )
		{
			goto on_error_libesedb_table;
		}
		if( number_of_records > 0 )
		{
			if( libesedb_table_get_record(
			     table,
			     0,
			     &record,
			     NULL ) != 1 )
			{
				goto on_error_libesedb_table;
			}
			if( libesedb_record_get_number_of_values(
			     record,
			     &number_of_values,
			     NULL ) != 1 )
			{
				goto on_error_libesedb_record;
			}
			if( number_of_values > 0 )
			{
				for( value_index = 0;
				     value_index < 16;
				     value_index++ )
				{
					if( value_index >= number_of_values )
					{
						break;
					}
					if( libesedb_record_get_value_data_size(
					     record,
					     value_index,
					     &value_data_size,
					     NULL ) == 1
					 && value_data_size > 0 )
					{
						libesedb_record_get_value_data(
							 record,
							 value_index,
							 value_data,
							 256,
							 NULL );
					}
					libesedb_record_get_value_data_flags(
						 record,
						 value_index,
						 &value_data_flags,
						 NULL );

					libesedb_record_get_column_identifier(
						 record,
						 value_index,
						 &column_identifier,
						 NULL );

					libesedb_record_get_column_type(
						 record,
						 value_index,
						 &column_type,
						 NULL );

					libesedb_record_get_utf8_column_name_size(
						 record,
						 value_index,
						 &utf8_column_name_size,
						 NULL );
				}
			}
on_error_libesedb_record:
			libesedb_record_free(
			 &record,
			 NULL );
		}
on_error_libesedb_table:
		libesedb_table_free(
		 &table,
		 NULL );
	}
	libesedb_file_close(
	 file,
	 NULL );

on_error_libesedb_file:
	libesedb_file_free(
	 &file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
