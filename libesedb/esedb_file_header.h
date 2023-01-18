/*
 * The file header definition of an Extensible Storage Engine (ESE) Database File (EDB)
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

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The log signature
	 * Consists of 28 bytes
	 * Contains a log signature structure
	 */
	uint8_t log_signature[ 28 ];

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

	/* Shadowing disabled
	 * Consists of 4 bytes
	 */
	uint8_t shadowing_disabled[ 4 ];

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

	/* The page size
	 * Consists of 4 bytes
	 */
	uint8_t page_size[ 4 ];

	/* The repair count
	 * Consists of 4 bytes
	 */
	uint8_t repair_count[ 4 ];

	/* The repair date and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t repair_time[ 8 ];

	/* Unknown
	 * Consists of 28 bytes
	 */
	uint8_t unknown2[ 28 ];

	/* The scrub database time
	 * Consists of 8 bytes
	 * Contains a database time structure
	 */
	uint8_t scrub_database_time[ 8 ];

	/* The scrub date and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t scrub_time[ 8 ];

	/* The required log
	 * Consists of 8 bytes
	 */
	uint8_t required_log[ 8 ];

	/* Upgrade Exchange 5.5 format
	 * Consists of 4 bytes
	 */
	uint8_t upgrade_exchange5_format[ 4 ];

	/* Upgrade free pages
	 * Consists of 4 bytes
	 */
	uint8_t upgrade_free_pages[ 4 ];

	/* Upgrade space map pages
	 * Consists of 4 bytes
	 */
	uint8_t upgrade_space_map_pages[ 4 ];

	/* Current shadow volume backup
	 * Consists of 24 bytes
	 * Contains a backup information structure
	 */
	uint8_t current_shadow_volume_backup[ 24 ];

	/* The file format version on creation
	 * Consists of 4 bytes
	 */
	uint8_t creation_format_version[ 4 ];

	/* The file format revision on creation
	 * Consists of 4 bytes
	 */
	uint8_t creation_format_revision[ 4 ];

	/* Unknown
	 * Consists of 16 bytes
	 */
	uint8_t unknown3[ 16 ];

	/* The old repair count
	 * Consists of 4 bytes
	 */
	uint8_t old_repair_count[ 4 ];

	/* The ECC fix success count
	 * Consists of 4 bytes
	 */
	uint8_t ecc_fix_success_count[ 4 ];

	/* The last ECC fix success rate and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t ecc_fix_success_time[ 8 ];

	/* The old ECC fix success count
	 * Consists of 4 bytes
	 */
	uint8_t old_ecc_fix_success_count[ 4 ];

	/* The ECC fix error count
	 * Consists of 4 bytes
	 */
	uint8_t ecc_fix_error_count[ 4 ];

	/* The last ECC fix error rate and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t ecc_fix_error_time[ 8 ];

	/* The old ECC fix error count
	 * Consists of 4 bytes
	 */
	uint8_t old_ecc_fix_error_count[ 4 ];

	/* The bad checksum error count
	 * Consists of 4 bytes
	 */
	uint8_t bad_checksum_error_count[ 4 ];

	/* The last bad checksum error rate and time
	 * Consists of 8 bytes
	 * Contains a log time structure
	 */
	uint8_t bad_checksum_error_time[ 8 ];

	/* The old bad checksum error count
	 * Consists of 4 bytes
	 */
	uint8_t old_bad_checksum_error_count[ 4 ];

	/* The committed log
	 * Consists of 4 bytes
	 */
	uint8_t committed_log[ 4 ];

	/* Previous shadow volume backup
	 * Consists of 24 bytes
	 * Contains a backup information structure
	 */
	uint8_t previous_shadow_volume_backup[ 24 ];

	/* Previous differential backup
	 * Consists of 24 bytes
	 * Contains a backup information structure
	 */
	uint8_t previous_differential_backup[ 24 ];

	/* Unknown
	 * Consists of 40 bytes
	 */
	uint8_t unknown4[ 40 ];

	/* NLS major version
	 * Consists of 4 bytes
	 */
	uint8_t nls_major_version[ 4 ];

	/* NLS minor version
	 * Consists of 4 bytes
	 */
	uint8_t nls_minor_version[ 4 ];

	/* Unknown
	 * Consists of 148 bytes
	 */
	uint8_t unknown5[ 148 ];

	/* Unknown flags
	 * Consists of 4 bytes
	 */
	uint8_t unknown_flags[ 4 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _ESEDB_FILE_HEADER_H ) */

