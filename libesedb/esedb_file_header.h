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
	/* The checksum
	 * A XOR-32 checksum calcalted over the bytes
	 * from offset 8 to 4096
	 * with an initial value of 0x89abcdef
	 * Consists of 4 bytes
	 */
	uint8_t checksum[ 4 ];

	/* The file signature
	 * Consists of 4 bytes
	 * Consists of: 0xef 0xcd 0xab 0x89
	 */
	uint8_t signature[ 4 ];

	/* The file format version
	 * Consists of 4 bytes
	 */
	uint8_t format_version[ 4 ];

	/* The file type
	 * Consists of 4 bytes
	 */
	uint8_t file_type[ 4 ];

	/* The database time
	 * Consists of 8 bytes
	 * Contains a database time structure
	 */
	uint8_t database_time[ 8 ];

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

	/* The log signature
	 * Consists of 28 bytes
	 * Contains a log signature structure
	 */
	uint8_t log_signature[ 28 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown5[ 4 ];

	/* Previous full backup
	 * Consists of 24 bytes
	 * Contains a backup information structure
	 */
	uint8_t previous_full_backup[ 24 ];

	/* Previous incremental backup
	 * Consists of 24 bytes
	 * Contains a backup information structure
	 */
	uint8_t previous_incremental_backup[ 24 ];

	/* Current full backup
	 * Consists of 24 bytes
	 * Contains a backup information structure
	 */
	uint8_t current_full_backup[ 24 ];

	/* Last object identifier
	 * Consists of 4 bytes
	 */
	uint8_t last_object_identifier[ 4 ];

	/* Index update major version
	 * Consists of 4 bytes
	 */
	uint8_t index_update_major_version[ 4 ];

	/* Index update minor version
	 * Consists of 4 bytes
	 */
	uint8_t index_update_minor_version[ 4 ];

	/* Index update build number
	 * Consists of 4 bytes
	 */
	uint8_t index_update_build_number[ 4 ];

	/* Index update service pack number
	 * Consists of 4 bytes
	 */
	uint8_t index_update_service_pack_number[ 4 ];

	/* The file format revision
	 * Consists of 4 bytes
	 */
	uint8_t format_revision[ 4 ];

	/* Page size
	 * Consists of 4 bytes
	 */
	uint8_t page_size[ 4 ];

	/* Unknown
	 * Consists of 100 bytes
	 */
	uint8_t unknown7[ 100 ];

	/* The file format version on creation
	 * Consists of 4 bytes
	 */
	uint8_t creation_format_version[ 4 ];

	/* The file format revision on creation
	 * Consists of 4 bytes
	 */
	uint8_t creation_format_revision[ 4 ];

	/* Unknown
	 * Consists of 320 bytes
	 */
	uint8_t unknown8[ 320 ];
};

#if defined( __cplusplus )
}
#endif

#endif

