/*
 * Common input functions for the esedbtools
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

/* If libtool DLL support is enabled set LIBESEDB_DLL_IMPORT
 * before including libesedb.h
 */
#if defined( _WIN32 ) && defined( DLL_EXPORT )
#define LIBESEDB_DLL_IMPORT
#endif

#include <libesedb.h>

/* Determines the codepage from an argument string
 * Returns 1 if successful or -1 on error
 */
int esedbinput_determine_ascii_codepage(
     const libcstring_system_character_t *argument,
     int *ascii_codepage,
     liberror_error_t **error )
{
	static char *function = "esedbinput_determine_ascii_codepage";
	int result            = -1;

	if( argument == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid argument string.",
		 function );

		return( -1 );
	}
	if( ascii_codepage == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ASCII codepage.",
		 function );

		return( -1 );
	}
	if( libcstring_system_string_compare(
	     argument,
	     _LIBCSTRING_SYSTEM_STRING( "ascii" ),
	     5 ) == 0 )
	{
		*ascii_codepage = LIBESEDB_CODEPAGE_ASCII;
		result          = 1;
	}
#if defined( HAVE_ISO_CODEPAGES )
	else if( libcstring_system_string_compare(
	          argument,
	          _LIBCSTRING_SYSTEM_STRING( "iso" ),
	          3 ) == 0 )
	{
		if( ( argument[ 3 ] != '-' )
		 && ( argument[ 3 ] != '_' ) )
		{
		}
		else if( libcstring_system_string_compare(
		          &( argument[ 4 ] ),
		          _LIBCSTRING_SYSTEM_STRING( "8859" ),
		          4 ) == 0 )
		{
			if( ( argument[ 8 ] != '-' )
			 && ( argument[ 8 ] != '_' ) )
			{
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "10" ),
				  2 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_10;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "11" ),
				  2 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_11;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "13" ),
				  2 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_13;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "14" ),
				  2 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_14;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "15" ),
				  2 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_15;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "16" ),
				  2 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_16;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "1" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_1;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "2" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_2;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "3" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_3;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "4" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_4;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "5" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_5;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "6" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_6;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "7" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_7;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "8" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_8;
				result          = 1;
			}
			else if( libcstring_system_string_compare(
				  &( argument[ 9 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "9" ),
				  1 ) == 0 )
			{
				*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_9;
				result          = 1;
			}
		}
	}
#endif
	else if( libcstring_system_string_compare(
	          argument,
	          _LIBCSTRING_SYSTEM_STRING( "windows" ),
	          7 ) == 0 )
	{
		if( ( argument[ 7 ] != '-' )
		 && ( argument[ 7 ] != '_' ) )
		{
		}
		else if( libcstring_system_string_compare(
		          &( argument[ 8 ] ),
		          _LIBCSTRING_SYSTEM_STRING( "874" ),
		          3 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_874;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
		          &( argument[ 8 ] ),
		          _LIBCSTRING_SYSTEM_STRING( "1250" ),
		          4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1250;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1251" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1251;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1252" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1252;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1253" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1253;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1253" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1253;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1254" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1254;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1255" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1255;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1256" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1256;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1257" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1257;
			result          = 1;
		}
		else if( libcstring_system_string_compare(
			  &( argument[ 8 ] ),
			  _LIBCSTRING_SYSTEM_STRING( "1258" ),
			  4 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1258;
			result          = 1;
		}
	}
	return( result );
}

