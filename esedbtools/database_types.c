/*
 * Database types
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "database_types.h"
#include "esedbtools_libcdata.h"
#include "esedbtools_libcerror.h"

const system_character_t *database_type_descriptions[ ] = {
	_SYSTEM_STRING( "Unknown" ),
	_SYSTEM_STRING( "Exchange" ),
	_SYSTEM_STRING( "System Resource Usage Monitor (SRUM)" ),
	_SYSTEM_STRING( "MSIE WebCache" ),
	_SYSTEM_STRING( "Windows Search" ),
	_SYSTEM_STRING( "Windows Security" ),
};

database_type_descriptor_t database_type_descriptors[ ] = {
	{ DATABASE_TYPE_EXCHANGE, {
		_SYSTEM_STRING( "Folders" ),
		_SYSTEM_STRING( "Global" ),
		_SYSTEM_STRING( "Mailbox" ),
		_SYSTEM_STRING( "Msg" ),
		_SYSTEM_STRING( "PerUserRead" ),
		 NULL } },
	{ DATABASE_TYPE_SRUM, {
		_SYSTEM_STRING( "SruDbIdMapTable" ),
		_SYSTEM_STRING( "{D10CA2FE-6FCF-4F6D-848E-B2E99266FA86}" ),
		_SYSTEM_STRING( "{D10CA2FE-6FCF-4F6D-848E-B2E99266FA89}" ),
		_SYSTEM_STRING( "{FEE4E14F-02A9-4550-B5CE-5FA2DA202E37}" ),
		_SYSTEM_STRING( "{973F5D5C-1D90-4944-BE8E-24B94231A174}" ),
		_SYSTEM_STRING( "{FEE4E14F-02A9-4550-B5CE-5FA2DA202E37}LT" ),
		_SYSTEM_STRING( "{DD6636C4-8929-4683-974E-22C046A43763}" ),
		NULL } },
	{ DATABASE_TYPE_WEBCACHE, {
		_SYSTEM_STRING( "Containers" ),
		_SYSTEM_STRING( "LeakFiles" ),
		_SYSTEM_STRING( "Partitions" ),
		NULL } },
	{ DATABASE_TYPE_WINDOWS_SEARCH, {
		_SYSTEM_STRING( "SystemIndex_0A" ),
		_SYSTEM_STRING( "SystemIndex_Gthr" ),
		NULL } },
	{ DATABASE_TYPE_WINDOWS_SECURITY, {
		_SYSTEM_STRING( "SmTblSection" ),
		_SYSTEM_STRING( "SmTblVersion" ),
		 NULL } },
	{ DATABASE_TYPE_UNKNOWN, {
		 NULL } }
};

/* Determines the database type
 * Returns 1 if successful or -1 on error
 */
int database_type_determine(
     int *database_type,
     libcdata_array_t *table_names,
     libcerror_error_t **error )
{
	int database_types[ NUMBER_OF_DATABASE_TYPES ];

	database_type_descriptor_t *database_type_descriptor = NULL;
	const system_character_t *known_table_name           = NULL;
	system_character_t *table_name                       = NULL;
	static char *function                                = "export_handle_determine_database_type";
	size_t known_table_name_length                       = 0;
	size_t table_name_length                             = 0;
	int database_type_index                              = 0;
	int known_table_name_index                           = 0;
	int number_of_tables                                 = 0;
	int table_index                                      = 0;

	if( database_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid database type.",
		 function );

		return( -1 );
	}
	for( database_type_index = 0;
	     database_type_index < NUMBER_OF_DATABASE_TYPES;
	     database_type_index++ )
	{
		database_types[ database_type_index ] = 0;
	}
	if( libcdata_array_get_number_of_entries(
	     table_names,
	     &number_of_tables,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of tables.",
		 function );

		return( -1 );
	}
	for( table_index = 0;
	     table_index < number_of_tables;
	     table_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     table_names,
		     table_index,
		     (intptr_t **) &table_name,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve the name of table: %d from array.",
			 function,
			 table_index );

			return( -1 );
		}
		if( table_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing name of table: %d.",
			 function,
			 table_index );

			return( -1 );
		}
		table_name_length = system_string_length(
		                     table_name );

		database_type_index = 0;

		database_type_descriptor = &( database_type_descriptors[ database_type_index++ ] );

		while( database_type_descriptor->database_type != DATABASE_TYPE_UNKNOWN )
		{
			known_table_name_index = 0;

			known_table_name = database_type_descriptor->known_table_names[ known_table_name_index++ ];

			while( known_table_name != NULL )
			{
				known_table_name_length = system_string_length(
				                           known_table_name );

				if( ( known_table_name_length == table_name_length )
				 && ( system_string_compare(
				       known_table_name,
				       table_name,
				       known_table_name_length ) == 0 ) )
				{
					database_types[ database_type_descriptor->database_type ] += 1;
				}
				known_table_name = database_type_descriptor->known_table_names[ known_table_name_index++ ];
			}
			database_type_descriptor = &( database_type_descriptors[ database_type_index++ ] );
		}
	}
	*database_type = DATABASE_TYPE_UNKNOWN;

	for( database_type_index = 0;
	     database_type_index < NUMBER_OF_DATABASE_TYPES;
	     database_type_index++ )
	{
		if( database_types[ *database_type ] < database_types[ database_type_index ] )
		{
			*database_type = database_type_index;
		}
	}
	return( 1 );
}

