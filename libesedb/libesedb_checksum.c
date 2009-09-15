/*
 * CRC functions
 * Based on RFC 1952
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#include "libesedb_checksum.h"

/* Calculates the little-endian XOR-32 of a buffer
 * Use a intial value to calculate a new XOR-32
 * Returns 1 if successful or -1 on error
 */
int libesedb_checksum_calculate_little_endian_xor32(
     uint32_t *checksum_value,
     uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     liberror_error_t **error )
{
	static char *function = "libesedb_checksum_calculate_little_endian_xor32";
	size_t buffer_iterator = 0;
	uint32_t value_32bit   = 0;

	if( checksum_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid checksum value.",
		 function );

		return( -1 );
	}
	if( buffer == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		return( -1 );
	}
	if( size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		return( -1 );
	}
	*checksum_value = initial_value;

	for( buffer_iterator = 0;
	     buffer_iterator < size;
	     buffer_iterator += 4 )
	{
		endian_little_convert_32bit(
		 value_32bit,
		 buffer );

		*checksum_value ^= value_32bit;

		buffer += 4;
	}
	return( 1 );
}

