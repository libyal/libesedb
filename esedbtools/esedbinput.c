/*
 * Common input functions for the esedbtools
 *
 * Copyright (c) 2009-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "esedbinput.h"
#include "esedbtools_libesedb.h"
#include "export_handle.h"

/* Determines the codepage from a string
 * Returns 1 if successful, 0 if unsupported value or -1 on error
 */
int esedbinput_determine_ascii_codepage(
     const libcstring_system_character_t *string,
     int *ascii_codepage,
     liberror_error_t **error )
{
	static char *function = "esedbinput_determine_ascii_codepage";
	size_t string_length  = 0;
	int result            = 0;

	if( string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
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
	string_length = libcstring_system_string_length(
	                 string );

	if( string_length == 5 )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "ascii" ),
		     5 ) == 0 )
		{
			*ascii_codepage = LIBESEDB_CODEPAGE_ASCII;
			result          = 1;
		}
	}
#if defined( HAVE_ISO_CODEPAGES )
	if( ( string_length == 10 )
	 || ( string_length == 11 ) )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "iso" ),
		     3 ) == 0 )
		{
			if( ( string[ 3 ] != '-' )
			 && ( string[ 3 ] != '_' ) )
			{
			}
			else if( libcstring_system_string_compare(
				  &( string[ 4 ] ),
				  _LIBCSTRING_SYSTEM_STRING( "8859" ),
				  4 ) == 0 )
			{
				if( ( string[ 8 ] != '-' )
				 && ( string[ 8 ] != '_' ) )
				{
				}
				else if( string_length == 10 )
				{
					if( libcstring_system_string_compare(
					     &( string[ 9 ] ),
					     _LIBCSTRING_SYSTEM_STRING( "1" ),
					     1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_1;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "2" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_2;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "3" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_3;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "4" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_4;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "5" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_5;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "6" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_6;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "7" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_7;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "8" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_8;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "9" ),
						  1 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_9;
						result          = 1;
					}
				}
				else if( string_length == 11 )
				{
					if( libcstring_system_string_compare(
					     &( string[ 9 ] ),
					     _LIBCSTRING_SYSTEM_STRING( "10" ),
					     2 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_10;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "11" ),
						  2 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_11;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "13" ),
						  2 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_13;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "14" ),
						  2 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_14;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "15" ),
						  2 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_15;
						result          = 1;
					}
					else if( libcstring_system_string_compare(
						  &( string[ 9 ] ),
						  _LIBCSTRING_SYSTEM_STRING( "16" ),
						  2 ) == 0 )
					{
						*ascii_codepage = LIBESEDB_CODEPAGE_ISO_8859_16;
						result          = 1;
					}
				}
			}
		}
	}
#endif
	if( ( string_length == 11 )
	 || ( string_length == 12 ) )
	{
		if( libcstring_system_string_compare(
		     string,
		     _LIBCSTRING_SYSTEM_STRING( "windows" ),
		     7 ) == 0 )
		{
			if( ( string[ 7 ] != '-' )
			 && ( string[ 7 ] != '_' ) )
			{
			}
			else if( string_length == 11 )
			{
				if( libcstring_system_string_compare(
				     &( string[ 8 ] ),
				     _LIBCSTRING_SYSTEM_STRING( "874" ),
				     3 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_874;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
				          &( string[ 8 ] ),
				          _LIBCSTRING_SYSTEM_STRING( "932" ),
				          3 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_932;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
				          &( string[ 8 ] ),
				          _LIBCSTRING_SYSTEM_STRING( "936" ),
				          3 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_936;
					result          = 1;
				}
			}
			else if( string_length == 12 )
			{
				if( libcstring_system_string_compare(
				     &( string[ 8 ] ),
				     _LIBCSTRING_SYSTEM_STRING( "1250" ),
				     4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1250;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1251" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1251;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1252" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1252;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1253" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1253;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1253" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1253;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1254" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1254;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1255" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1255;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1256" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1256;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1257" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1257;
					result          = 1;
				}
				else if( libcstring_system_string_compare(
					  &( string[ 8 ] ),
					  _LIBCSTRING_SYSTEM_STRING( "1258" ),
					  4 ) == 0 )
				{
					*ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1258;
					result          = 1;
				}
			}
		}
	}
	return( result );
}

