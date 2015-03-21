/*
 * Export functions
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
#include <file_stream.h>
#include <memory.h>
#include <types.h>

#include "esedbtools_libcstring.h"
#include "esedbtools_libcerror.h"
#include "esedbtools_libesedb.h"
#include "export.h"

/* Exports binary data
 */
void export_binary_data(
      const uint8_t *data,
      size_t data_size,
      FILE *stream )
{
	if( data != NULL )
	{
		while( data_size > 0 )
		{
			fprintf(
			 stream,
			 "%02" PRIx8 "",
			 *data );

			data      += 1;
			data_size -= 1;
		}
	}
}

/* Exports narrow character text
 * Converts \\, \t, \n, \r, \0 into their string representation
 */
void export_narrow_text(
      const char *string,
      size_t string_size,
      FILE *stream )
{
	if( string != NULL )
	{
		while( string_size > 0 )
		{
			if( *string == 0 )
			{
				if( string_size > 1 )
				{
					fprintf(
					 stream,
					 "\\0" );
				}
			}
			else if( *string == '\\' )
			{
				fprintf(
				 stream,
				 "\\\\" );
			}
			else if( *string == '\n' )
			{
				fprintf(
				 stream,
				 "\\n" );
			}
			else if( *string == '\r' )
			{
				fprintf(
				 stream,
				 "\\r" );
			}
			else if( *string == '\t' )
			{
				fprintf(
				 stream,
				 "\\t" );
			}
			else
			{
				fprintf(
				 stream,
				 "%c",
				 *string );
			}
			string      += 1;
			string_size -= 1;
		}
	}
}

/* Exports system character text
 * Converts \\, \t, \n, \r, \0 into their string representation
 */
void export_text(
      const libcstring_system_character_t *string,
      size_t string_size,
      FILE *stream )
{
	if( string != NULL )
	{
		while( string_size > 0 )
		{
			if( *string == 0 )
			{
				if( string_size > 1 )
				{
					fprintf(
					 stream,
					 "\\0" );
				}
			}
			else if( *string == (libcstring_system_character_t) '\\' )
			{
				fprintf(
				 stream,
				 "\\\\" );
			}
			else if( *string == (libcstring_system_character_t) '\n' )
			{
				fprintf(
				 stream,
				 "\\n" );
			}
			else if( *string == (libcstring_system_character_t) '\r' )
			{
				fprintf(
				 stream,
				 "\\r" );
			}
			else if( *string == (libcstring_system_character_t) '\t' )
			{
				fprintf(
				 stream,
				 "\\t" );
			}
			else
			{
				fprintf(
				 stream,
				 "%" PRIc_LIBCSTRING_SYSTEM "",
				 *string );
			}
			string      += 1;
			string_size -= 1;
		}
	}
}

/* Retrieves the long value data from a long value
 * Returns 1 if successful or -1 on error
 */
int export_get_long_value_data(
     libesedb_long_value_t *long_value,
     uint8_t **long_value_data,
     size_t *long_value_data_size,
     libcerror_error_t **error )
{
	static char *function              = "export_get_long_value_data";
	size64_t safe_long_value_data_size = 0;

	if( long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	if( long_value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value data.",
		 function );

		return( -1 );
	}
	if( *long_value_data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid long value data value already set.",
		 function );

		return( -1 );
	}
	if( long_value_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value data size.",
		 function );

		return( -1 );
	}
	if( libesedb_long_value_get_data_size(
	     long_value,
	     &safe_long_value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve long value data size.",
		 function );

		goto on_error;
	}
	if( safe_long_value_data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid long value data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	if( safe_long_value_data_size == 0 )
	{
		*long_value_data      = NULL;
		*long_value_data_size = 0;
	}
	*long_value_data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * (size_t) safe_long_value_data_size );

	if( *long_value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create long value data.",
		 function );

		goto on_error;
	}
	if( libesedb_long_value_get_data(
	     long_value,
	     *long_value_data,
	     (size_t) safe_long_value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve long value data.",
		 function );

		goto on_error;
	}
	*long_value_data_size = (size_t) safe_long_value_data_size;

	return( 1 );

on_error:
	if( long_value_data != NULL )
	{
		memory_free(
		 long_value_data );
	}
	return( -1 );
}

