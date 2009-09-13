/*
 * The file header definition of an Extensible Storage Engine (ESE) Database File (EDB)
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _ESEDB_FILE_HEADER_H )
#define _ESEDB_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct esedb_file_header esedb_file_header_t;

struct esedb_file_header
{
	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The signature
	 * Consists of 4 bytes
	 * Consists of: 0xef 0xcd 0xab 0x89
	 */
	uint8_t signature[ 4 ];

	/* The file format version
	 * Consists of 4 bytes
	 */
	uint8_t format_version[ 4 ];

	/* The file format revision
	 * Consists of 4 bytes
	 */
	uint8_t format_revision[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown2[ 8 ];

	/* The database signature
	 * Consists of 28 bytes
	 * Contains a log signature structure
	 */
	uint8_t database_signature[ 28 ];

	/* The database state
	 * Consists of 4 bytes
	 */
	uint8_t database_state[ 4 ];

	/* The consistent position
	 * Consists of 8 bytes
	 * Contains a log position structure
	 */
	uint8_t consistent_postition[ 8 ];

	/* The consistent date and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t consistent_time[ 8 ];

	/* The attach date and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t attach_time[ 8 ];

	/* The attach position
	 * Consists of 8 bytes
	 * Contains a log position structure
	 */
	uint8_t attach_postition[ 8 ];

	/* The detach date and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t detach_time[ 8 ];

	/* The detach position
	 * Consists of 8 bytes
	 * Contains a log position structure
	 */
	uint8_t detach_postition[ 8 ];

};

#if defined( __cplusplus )
}
#endif

#endif

