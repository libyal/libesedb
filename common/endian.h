/*
 * Endian functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _ENDIAN_H )
#define _ENDIAN_H

#include "common.h"
#include "types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#define _ENDIAN_BIG      (uint8_t) 'b'
#define _ENDIAN_LITTLE   (uint8_t) 'l'

#define endian_big_convert_16bit( value, bytes ) \
	value = ( (uint16_t) ( bytes[ 0 ] ) << 8 ) \
	      | (uint16_t) ( bytes[ 1 ] )

#define endian_big_convert_32bit( value, bytes ) \
	value = ( (uint32_t) ( bytes[ 0 ] ) << 24 ) \
	      | ( (uint32_t) ( bytes[ 1 ] ) << 16 ) \
	      | ( (uint32_t) ( bytes[ 2 ] ) << 8 ) \
	      | (uint32_t) ( bytes[ 3 ] )

#define endian_big_convert_64bit( value, bytes ) \
	value = ( (uint64_t) ( bytes[ 0 ] ) << 56 ) \
	      | ( (uint64_t) ( bytes[ 1 ] ) << 48 ) \
	      | ( (uint64_t) ( bytes[ 2 ] ) << 40 ) \
	      | ( (uint64_t) ( bytes[ 3 ] ) << 32 ) \
	      | ( (uint64_t) ( bytes[ 4 ] ) << 24 ) \
	      | ( (uint64_t) ( bytes[ 5 ] ) << 16 ) \
	      | ( (uint64_t) ( bytes[ 6 ] ) << 8 ) \
	      | (uint64_t) ( bytes[ 7 ] )

#define endian_little_convert_16bit( value, bytes ) \
	value = ( (uint16_t) ( bytes[ 1 ] ) << 8 ) \
	      | (uint16_t) ( bytes[ 0 ] )

#define endian_little_convert_32bit( value, bytes ) \
	value = ( (uint32_t) ( bytes[ 3 ] ) << 24 ) \
	      | ( (uint32_t) ( bytes[ 2 ] ) << 16 ) \
	      | ( (uint32_t) ( bytes[ 1 ] ) << 8 ) \
	      | (uint32_t) ( bytes[ 0 ] )

#define endian_little_convert_64bit( value, bytes ) \
	value = ( (uint64_t) ( bytes[ 7 ] ) << 56 ) \
	      | ( (uint64_t) ( bytes[ 6 ] ) << 48 ) \
	      | ( (uint64_t) ( bytes[ 5 ] ) << 40 ) \
	      | ( (uint64_t) ( bytes[ 4 ] ) << 32 ) \
	      | ( (uint64_t) ( bytes[ 3 ] ) << 24 ) \
	      | ( (uint64_t) ( bytes[ 2 ] ) << 16 ) \
	      | ( (uint64_t) ( bytes[ 1 ] ) << 8 ) \
	      | (uint64_t) ( bytes[ 0 ] )

#define endian_big_revert_16bit( bytes, value ) \
	bytes[ 0 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( value & 0x0ff )

#define endian_big_revert_32bit( bytes, value ) \
	bytes[ 0 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 3 ] = (uint8_t) ( value & 0x0ff )

#define endian_big_revert_64bit( bytes, value ) \
	bytes[ 0 ] = (uint8_t) ( ( value >> 56 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 48 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 40 ) & 0x0ff ); \
	bytes[ 3 ] = (uint8_t) ( ( value >> 32 ) & 0x0ff ); \
	bytes[ 4 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 5 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 6 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 7 ] = (uint8_t) ( value & 0x0ff )

#define endian_little_revert_16bit( bytes, value ) \
	bytes[ 1 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 0 ] = (uint8_t) ( value & 0x0ff )

#define endian_little_revert_32bit( bytes, value ) \
	bytes[ 3 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 0 ] = (uint8_t) ( value & 0x0ff )

#define endian_little_revert_64bit( bytes, value ) \
	bytes[ 7 ] = (uint8_t) ( ( value >> 56 ) & 0x0ff ); \
	bytes[ 6 ] = (uint8_t) ( ( value >> 48 ) & 0x0ff ); \
	bytes[ 5 ] = (uint8_t) ( ( value >> 40 ) & 0x0ff ); \
	bytes[ 4 ] = (uint8_t) ( ( value >> 32 ) & 0x0ff ); \
	bytes[ 3 ] = (uint8_t) ( ( value >> 24 ) & 0x0ff ); \
	bytes[ 2 ] = (uint8_t) ( ( value >> 16 ) & 0x0ff ); \
	bytes[ 1 ] = (uint8_t) ( ( value >> 8 ) & 0x0ff ); \
	bytes[ 0 ] = (uint8_t) ( value & 0x0ff )

#if defined( __cplusplus )
}
#endif

#endif

