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
#include <types.h>

#include "esedbtools_libcstring.h"
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

