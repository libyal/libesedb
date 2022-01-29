/*
 * Column type functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libesedb_column_type.h"

#if defined( HAVE_DEBUG_OUTPUT )

libesedb_column_type_t libesedb_column_types[ ] = {
	{ 0x0000, "JET_coltypNil",		"Invalid" },
	{ 0x0001, "JET_coltypBit",		"Boolean" },
	{ 0x0002, "JET_coltypUnsignedByte",	"Integer 8-bit unsigned" },
	{ 0x0003, "JET_coltypShort",		"Integer 16-bit signed" },
	{ 0x0004, "JET_coltypLong",		"Integer 32-bit signed" },
	{ 0x0005, "JET_coltypCurrency",		"Currency (64-bit)" },
	{ 0x0006, "JET_coltypIEEESingle",	"Floating point single precision (32-bit)" },
	{ 0x0007, "JET_coltypIEEEDouble",	"Floating point double precision (64-bit)" },
	{ 0x0008, "JET_coltypDateTime",		"Application Time (64-bit)" },
	{ 0x0009, "JET_coltypBinary",		"Binary data" },
	{ 0x000a, "JET_coltypText",		"Text (extended ASCII or Unicode string)" },
	{ 0x000b, "JET_coltypLongBinary",	"Large binary data" },
	{ 0x000c, "JET_coltypLongText",		"Large text (extended ASCII or Unicode string)" },
	{ 0x000d, "JET_coltypSLV",		"Super long value" },
	{ 0x000e, "JET_coltypUnsignedLong",	"Integer 32-bit unsigned" },
	{ 0x000f, "JET_coltypLongLong",		"Integer 64-bit signed" },
	{ 0x0010, "JET_coltypGUID",		"GUID (128-bit)" },
	{ 0x0011, "JET_coltypUnsignedShort",	"Integer 16-bit unsigned" },

	{ (uint32_t) -1, "_UNKNOWN_", "Unknown" } };

/* Retrieves a string containing the column type identifier
 */
const char *libesedb_column_type_get_identifier(
             uint32_t column_type )
{
	int iterator = 0;

	while( ( libesedb_column_types[ iterator ] ).column_type != (uint32_t) -1 )
	{
		if( ( libesedb_column_types[ iterator ] ).column_type == column_type )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libesedb_column_types[ iterator ] ).identifier );
}

/* Retrieves a string containing the column type description
 */
const char *libesedb_column_type_get_description(
             uint32_t column_type )
{
	int iterator = 0;

	while( ( libesedb_column_types[ iterator ] ).column_type != (uint32_t) -1 )
	{
		if( ( libesedb_column_types[ iterator ] ).column_type == column_type )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libesedb_column_types[ iterator ] ).description );
}

#endif

