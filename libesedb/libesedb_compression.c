/*
 * Compression functions
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
#include <byte_stream.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_compression.h"
#include "libesedb_libuna.h"

/* Retrieves the UTF-8 string size of decompressed 7-bit compressed data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_7bit_get_utf8_string_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf8_string_size,
     liberror_error_t **error )
{
	static char *function        = "libesedb_compression_7bit_get_utf8_string_size";
	size_t character_size        = 0;
	size_t compressed_data_index = 0;
	uint16_t character_value     = 0;
	uint16_t value_16bit         = 0;
	uint8_t bit_index            = 0;
	uint8_t character_index      = 0;

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
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
	if( ( ( compressed_data[ 0 ] & 0xf0 ) != 0x00 )
	 && ( ( compressed_data[ 0 ] & 0xf0 ) != 0x10 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	if( ( compressed_data[ 0 ] & 0xf0 ) == 0x10 )
	{
		*utf8_string_size = ( ( ( compressed_data_size - 1 ) * 8 ) / 7 ) + 1;
	}
	else
	{
		*utf8_string_size = 1;

		for( compressed_data_index = 1;
		     compressed_data_index < compressed_data_size;
		     compressed_data_index++ )
		{
			value_16bit |= (uint16_t) compressed_data[ compressed_data_index ] << bit_index;

			if( character_index == 1 )
			{
				character_value |= ( value_16bit & 0x7f ) << 8;
			}
			else
			{
				character_value = value_16bit & 0x7f;
			}
			value_16bit >>= 7;

			bit_index++;

			if( character_index == 0 )
			{
				character_index++;
			}
			else
			{
				if( libuna_unicode_character_size_to_utf8(
				     (libuna_unicode_character_t) character_value,
				     &character_size,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_CONVERSION,
					 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
					 "%s: unable to unable to determine size of Unicode character in UTF-8.",
					 function );

					return( -1 );
				}
				*utf8_string_size += character_size;

				character_index = 0;
			}
			if( bit_index == 7 )
			{
				if( character_index == 1 )
				{
					character_value |= ( value_16bit & 0x7f ) << 8;
				}
				else
				{
					character_value = value_16bit & 0x7f;
				}
				value_16bit >>= 7;

				bit_index = 0;

				if( character_index == 0 )
				{
					character_index++;
				}
				else
				{
					if( libuna_unicode_character_size_to_utf8(
					     (libuna_unicode_character_t) character_value,
					     &character_size,
					     error ) != 1 )
					{
						liberror_error_set(
						 error,
						 LIBERROR_ERROR_DOMAIN_CONVERSION,
						 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
						 "%s: unable to unable to determine size of Unicode character in UTF-8.",
						 function );

						return( -1 );
					}
					*utf8_string_size += character_size;

					character_index = 0;
				}
			}
		}
		if( character_index != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid compressed data - missing data.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Copies decompressed 7-bit compressed data to an UTF-8 string
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_7bit_copy_to_utf8_string(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     liberror_error_t **error )
{
	static char *function        = "libesedb_compression_7bit_copy_to_utf8_string";
	size_t compressed_data_index = 0;
	size_t string_index          = 0;
	uint16_t character_value     = 0;
	uint16_t value_16bit         = 0;
	uint8_t bit_index            = 0;
	uint8_t character_index      = 0;
	uint8_t is_ascii             = 0;

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
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
	if( utf8_string_size < ( ( ( compressed_data_size - 1 ) * 8 ) / 7 ) + 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-8 string size value too small.",
		 function );

		return( -1 );
	}
	if( ( ( compressed_data[ 0 ] & 0xf0 ) != 0x00 )
	 && ( ( compressed_data[ 0 ] & 0xf0 ) != 0x10 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	is_ascii = ( compressed_data[ 0 ] & 0xf0 ) >> 4;

	for( compressed_data_index = 1;
	     compressed_data_index < compressed_data_size;
	     compressed_data_index++ )
	{
		value_16bit |= (uint16_t) compressed_data[ compressed_data_index ] << bit_index;

		if( character_index == 1 )
		{
			character_value |= ( value_16bit & 0x7f ) << 8;
		}
		else
		{
			character_value = value_16bit & 0x7f;
		}
		value_16bit >>= 7;

		bit_index++;

		if( is_ascii != 0 )
		{
			utf8_string[ string_index++ ] = (uint8_t) character_value;
		}
		else if( character_index == 0 )
		{
			character_index++;
		}
		else
		{
			if( libuna_unicode_character_copy_to_utf8(
			     (libuna_unicode_character_t) character_value,
			     utf8_string,
			     utf8_string_size,
			     &string_index,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_CONVERSION,
				 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
				 "%s: unable to copy Unicode character to UTF-8.",
				 function );

				return( -1 );
			}
			character_index = 0;
		}
		if( bit_index == 7 )
		{
			if( character_index == 1 )
			{
				character_value |= ( value_16bit & 0x7f ) << 8;
			}
			else
			{
				character_value = value_16bit & 0x7f;
			}
			value_16bit >>= 7;

			bit_index = 0;

			if( is_ascii != 0 )
			{
				utf8_string[ string_index++ ] = (uint8_t) character_value;
			}
			else if( character_index == 0 )
			{
				character_index++;
			}
			else
			{
				if( libuna_unicode_character_copy_to_utf8(
				     (libuna_unicode_character_t) character_value,
				     utf8_string,
				     utf8_string_size,
				     &string_index,
				     error ) != 1 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_CONVERSION,
					 LIBERROR_CONVERSION_ERROR_OUTPUT_FAILED,
					 "%s: unable to copy Unicode character to UTF-8.",
					 function );

					return( -1 );
				}
				character_index = 0;
			}
		}
	}
	if( value_16bit != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported remainder: 0x%02" PRIx16 ".",
		 function,
		 value_16bit );

		return( -1 );
	}
	if( ( is_ascii == 0 )
	 && ( character_index != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid compressed data - missing data.",
		 function );

		return( -1 );
	}
	utf8_string[ string_index++ ] = 0;

	return( 1 );
}

/* Retrieves the UTF-16 string size of decompressed 7-bit compressed data
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_7bit_get_utf16_string_size(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     size_t *utf16_string_size,
     liberror_error_t **error )
{
	static char *function = "libesedb_compression_7bit_get_utf16_string_size";

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string size.",
		 function );

		return( -1 );
	}
	if( ( ( compressed_data[ 0 ] & 0xf0 ) != 0x00 )
	 && ( ( compressed_data[ 0 ] & 0xf0 ) != 0x10 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	*utf16_string_size = ( ( ( compressed_data_size - 1 ) * 8 ) / 7 ) + 1;

	return( 1 );
}

/* Copies decompressed 7-bit compressed data to an UTF-16 string
 * Returns 1 on success or -1 on error
 */
int libesedb_compression_7bit_copy_to_utf16_string(
     const uint8_t *compressed_data,
     size_t compressed_data_size,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     liberror_error_t **error )
{
	static char *function        = "libesedb_compression_7bit_copy_to_utf16_string";
	size_t compressed_data_index = 0;
	size_t string_index          = 0;
	uint16_t character_value     = 0;
	uint16_t value_16bit         = 0;
	uint8_t bit_index            = 0;
	uint8_t character_index      = 0;
	uint8_t is_ascii             = 0;

	if( compressed_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid compressed data.",
		 function );

		return( -1 );
	}
	if( compressed_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid compressed data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( utf16_string_size < ( ( ( compressed_data_size - 1 ) * 8 ) / 7 ) + 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: UTF-16 string size value too small.",
		 function );

		return( -1 );
	}
	if( ( ( compressed_data[ 0 ] & 0xf0 ) != 0x00 )
	 && ( ( compressed_data[ 0 ] & 0xf0 ) != 0x10 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported leading byte: 0x%02" PRIx8 ".",
		 function,
		 compressed_data[ 0 ] );

		return( -1 );
	}
	is_ascii = ( compressed_data[ 0 ] & 0xf0 ) >> 4;

	for( compressed_data_index = 1;
	     compressed_data_index < compressed_data_size;
	     compressed_data_index++ )
	{
		value_16bit |= (uint16_t) compressed_data[ compressed_data_index ] << bit_index;

		if( character_index == 1 )
		{
			character_value |= ( value_16bit & 0x7f ) << 8;
		}
		else
		{
			character_value = value_16bit & 0x7f;
		}
		value_16bit >>= 7;

		bit_index++;

		if( is_ascii != 0 )
		{
			utf16_string[ string_index++ ] = character_value;
		}
		else if( character_index == 0 )
		{
			character_index++;
		}
		else
		{
			utf16_string[ string_index++ ] = character_value;

			character_index = 0;
		}
		if( bit_index == 7 )
		{
			if( character_index == 1 )
			{
				character_value |= ( value_16bit & 0x7f ) << 8;
			}
			else
			{
				character_value = value_16bit & 0x7f;
			}
			value_16bit >>= 7;

			bit_index = 0;

			if( is_ascii != 0 )
			{
				utf16_string[ string_index++ ] = character_value;
			}
			else if( character_index == 0 )
			{
				character_index++;
			}
			else
			{
				utf16_string[ string_index++ ] = character_value;

				character_index = 0;
			}
		}
	}
	if( value_16bit != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported remainder: 0x%02" PRIx16 ".",
		 function,
		 value_16bit );

		return( -1 );
	}
	if( ( is_ascii == 0 )
	 && ( character_index != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid compressed data - missing data.",
		 function );

		return( -1 );
	}
	utf16_string[ string_index++ ] = 0;

	return( 1 );
}

