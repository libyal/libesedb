/*
 * Checkcum functions
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
#include <byte_stream.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_checksum.h"

/* The ECC-32 mask lookup table
 */
const uint32_t libesedb_checksum_ecc32_mask_lookup_table[ 256 ] = {
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00,
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00,
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0xff0000ff, 0x00ffff00, 0x00ffff00, 0xff0000ff,
	0x00ffff00, 0xff0000ff, 0xff0000ff, 0x00ffff00
};

/* Calculates the little-endian ECC-32 of a buffer
 * Use a initial value to calculate a new ECC-32
 * Returns 1 if successful or -1 on error
 */
int libesedb_checksum_calculate_little_endian_ecc32(
     uint32_t *checksum_value,
     uint8_t *buffer,
     size_t size,
     uint32_t initial_value,
     liberror_error_t **error )
{
	static char *function          = "libesedb_checksum_calculate_little_endian_ecc32";
	size_t buffer_iterator         = 0;
	size_t buffer_alignment        = 0;
	uint32_t bitmask               = 0xff800000UL;
	uint32_t bit_iterator          = 0;
	uint32_t lookup_mask           = 0;
	uint32_t value_32bit           = 0;
	uint32_t xor32_value           = 0;
	uint32_t xor32_vertical_first  = 0;
	uint32_t xor32_vertical_second = 0;
	uint32_t xor32_vertical_third  = 0;
	uint32_t xor32_vertical_fourth = 0;
	uint8_t mask_table_index       = 0;

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
	buffer_alignment = (size_t) buffer % 16;

	if( ( buffer_alignment % 4 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: buffer is not 32-bit aligned.",
		 function );

		return( -1 );
	}
	/* TODO intial value ? */
	*checksum_value = 0;

	for( buffer_iterator = 0;
	     buffer_iterator < size;
	     buffer_iterator += 4 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 buffer,
		 value_32bit );

		if( buffer_alignment == 0 )
		{
			xor32_vertical_first ^= value_32bit;
		}
		else if( buffer_alignment == 4 )
		{
			xor32_vertical_second ^= value_32bit;
		}
		else if( buffer_alignment == 8 )
		{
			xor32_vertical_third ^= value_32bit;
		}
		else
		{
			xor32_vertical_fourth ^= value_32bit;
		}
		xor32_value      ^= value_32bit;
		buffer           += 4;
		buffer_alignment += 4;

		if( buffer_alignment >= 16 )
		{
			mask_table_index  = ( xor32_value & 0xff );
			xor32_value     >>= 8;
			mask_table_index ^= ( xor32_value & 0xff );
			xor32_value     >>= 8;
			mask_table_index ^= ( xor32_value & 0xff );
			xor32_value     >>= 8;
			mask_table_index ^= ( xor32_value & 0xff );

			lookup_mask  = libesedb_checksum_ecc32_mask_lookup_table[ mask_table_index ];
			lookup_mask &= bitmask;

			*checksum_value ^= lookup_mask;

			bitmask         -= 0x007fff80UL;
			buffer_alignment = 0;
			xor32_value      = 0;
		}
	}
	/* Determine the checksum part of the combination of
	 * the first and second vertical XOR
	 */
	xor32_value  = xor32_vertical_first;
	xor32_value ^= xor32_vertical_second;

	mask_table_index  = ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );

	lookup_mask  = libesedb_checksum_ecc32_mask_lookup_table[ mask_table_index ];
	lookup_mask &= 0x00400000UL;

	*checksum_value ^= lookup_mask;

	/* Determine the checksum part of the combination of
	 * the first and third vertical XOR
	 */
	xor32_value  = xor32_vertical_first;
	xor32_value ^= xor32_vertical_third;

	mask_table_index  = ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );

	lookup_mask  = libesedb_checksum_ecc32_mask_lookup_table[ mask_table_index ];
	lookup_mask &= 0x00200000UL;

	*checksum_value ^= lookup_mask;

	/* Determine the checksum part of the combination of
	 * the second and fourth vertical XOR
	 */
	xor32_value  = xor32_vertical_second;
	xor32_value ^= xor32_vertical_fourth;

	mask_table_index  = ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );

	lookup_mask  = libesedb_checksum_ecc32_mask_lookup_table[ mask_table_index ];
	lookup_mask &= 0x00000020UL;

	*checksum_value ^= lookup_mask;

	/* Determine the checksum part of the combination of
	 * the third and fourth vertical XOR
	 */
	xor32_value  = xor32_vertical_third;
	xor32_value ^= xor32_vertical_fourth;

	mask_table_index  = ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );
	xor32_value     >>= 8;
	mask_table_index ^= ( xor32_value & 0xff );

	lookup_mask  = libesedb_checksum_ecc32_mask_lookup_table[ mask_table_index ];
	lookup_mask &= 0x00000040UL;

	*checksum_value ^= lookup_mask;

	/* Determine the vertical XOR
	 */
	xor32_value  = xor32_vertical_first;
	xor32_value ^= xor32_vertical_second;
	xor32_value ^= xor32_vertical_third;
	xor32_value ^= xor32_vertical_fourth;

	bitmask = 0x0fff0000UL;

	for( bit_iterator = 1;
	     bit_iterator != 0;
	     bit_iterator <<= 2 )
	{
		/* TODO */
	}
	/* TODO */

	return( 1 );
}

/* Calculates the little-endian XOR-32 of a buffer
 * Use a initial value to calculate a new XOR-32
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
		byte_stream_copy_to_uint32_little_endian(
		 buffer,
		 value_32bit );

		*checksum_value ^= value_32bit;

		buffer += 4;
	}
	return( 1 );
}

