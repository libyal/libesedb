/*
 * Character string functions
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
#include <types.h>

#include "esedbtools_libcerror.h"

/* Copies a string of a decimal value from a 64-bit value
 * Returns 1 if successful or -1 on error
 */
int esedbtools_system_string_copy_from_64_bit_in_decimal(
     system_character_t *string,
     size_t string_size,
     size_t *string_index,
     uint64_t value_64bit,
     libcerror_error_t **error )
{
	static char *function        = "esedbtools_system_string_copy_from_64_bit_in_decimal";
	uint64_t divider             = 0;
	uint8_t number_of_characters = 0;

	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string_index == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string index.",
		 function );

		return( -1 );
	}
	if( *string_index >= string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string index value out of bounds.",
		 function );

		return( -1 );
	}
	divider              = 1;
	number_of_characters = 1;

	while( ( value_64bit / divider ) >= 10 )
	{
		divider *= 10;

		number_of_characters += 1;
	}
	if( ( *string_index + number_of_characters ) >= string_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: string size too small.",
		 function );

		return( -1 );
	}
	while( divider > 1 )
	{
		string[ *string_index ] = (uint8_t) '0' + (uint8_t) ( value_64bit / divider );

		*string_index += 1;

		value_64bit %= divider;

		divider /= 10;
	}
	string[ *string_index ] = (uint8_t) '0' + (uint8_t) ( value_64bit / divider );

	*string_index += 1;

	return( 1 );
}

