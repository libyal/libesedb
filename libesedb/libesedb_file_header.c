/*
 * File header functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libesedb_checksum.h"
#include "libesedb_debug.h"
#include "libesedb_file_header.h"
#include "libesedb_io_handle.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"

#include "esedb_file_header.h"

/* Creates file header
 * Make sure the value file_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_header_initialize(
     libesedb_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libesedb_file_header_initialize";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file header value already set.",
		 function );

		return( -1 );
	}
	*file_header = memory_allocate_structure(
	                libesedb_file_header_t );

	if( *file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *file_header,
	     0,
	     sizeof( libesedb_file_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( -1 );
}

/* Frees file header
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_header_free(
     libesedb_file_header_t **file_header,
     libcerror_error_t **error )
{
	static char *function = "libesedb_file_header_free";

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( *file_header != NULL )
	{
		memory_free(
		 *file_header );

		*file_header = NULL;
	}
	return( 1 );
}

/* Reads the file header data
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_header_read_data(
     libesedb_file_header_t *file_header,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function              = "libesedb_file_header_read_data";
	uint32_t calculated_xor32_checksum = 0;
	uint32_t stored_xor32_checksum     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit               = 0;
#endif

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( esedb_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( esedb_file_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (esedb_file_header_t *) data )->signature,
	     esedb_file_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->checksum,
	 stored_xor32_checksum );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->format_version,
	 file_header->format_version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->file_type,
	 file_header->file_type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->database_state,
	 file_header->database_state );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->format_revision,
	 file_header->format_revision );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->page_size,
	 file_header->page_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->creation_format_version,
	 file_header->creation_format_version );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) data )->creation_format_revision,
	 file_header->creation_format_revision );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_xor32_checksum );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->signature,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: format version\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_header->format_version );

		libcnotify_printf(
		 "%s: file type\t\t\t\t: %" PRIu32 " (",
		 function,
		 file_header->file_type );
		libesedb_debug_print_file_type(
		 file_header->file_type );
		libcnotify_printf(
		 ")\n" );

		libcnotify_printf(
		 "%s: database time:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->database_time,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: database signature:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->database_signature,
		 28,
		 0 );

		libcnotify_printf(
		 "%s: database state\t\t\t\t: %" PRIu32 " ",
		 function,
		 file_header->database_state );
		libesedb_debug_print_database_state(
		 file_header->database_state );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: consistent position:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->consistent_postition,
		 8,
		 0 );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->consistent_time,
		 8,
		 "consistent time",
		 "\t\t\t\t",
		 NULL );

		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->attach_time,
		 8,
		 "attach time",
		 "\t\t\t\t",
		 NULL );
		libcnotify_printf(
		 "%s: attach position:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->attach_postition,
		 8,
		 0 );

		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->detach_time,
		 8,
		 "detach time",
		 "\t\t\t\t",
		 NULL );
		libcnotify_printf(
		 "%s: detach position:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->detach_postition,
		 8,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "%s: log signature:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->log_signature,
		 28,
		 0 );

		libcnotify_printf(
		 "%s: previous full backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->previous_full_backup,
		 24,
		 0 );
		libcnotify_printf(
		 "%s: previous incremental backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->previous_incremental_backup,
		 24,
		 0 );
		libcnotify_printf(
		 "%s: current full backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->current_full_backup,
		 24,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->shadowing_disabled,
		 value_32bit );
		libcnotify_printf(
		 "%s: shadowing disabled\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->last_object_identifier,
		 value_32bit );
		libcnotify_printf(
		 "%s: last object identifier\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->index_update_major_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update major version\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->index_update_minor_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update minor version\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->index_update_build_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update build number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->index_update_service_pack_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update service pack number\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: format revision\t\t\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 file_header->format_revision,
		 file_header->format_revision );
		libcnotify_printf(
		 "%s: page size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 file_header->page_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->repair_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: repair count\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->repair_time,
		 8,
		 "repair time",
		 "\t\t\t\t",
		 NULL );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->unknown2,
		 28,
		 0 );

		libcnotify_printf(
		 "%s: scrub database time:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->scrub_database_time,
		 8,
		 0 );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->scrub_time,
		 8,
		 "scrub time",
		 "\t\t\t\t",
		 NULL );

		libcnotify_printf(
		 "%s: required log:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->required_log,
		 8,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->upgrade_exchange5_format,
		 value_32bit );
		libcnotify_printf(
		 "%s: upgrade Exchange 5.5 format\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->upgrade_free_pages,
		 value_32bit );
		libcnotify_printf(
		 "%s: upgrade free pages\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->upgrade_space_map_pages,
		 value_32bit );
		libcnotify_printf(
		 "%s: upgrade space map pages\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: current shadow volume backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->current_shadow_volume_backup,
		 24,
		 0 );

		libcnotify_printf(
		 "%s: creation format version\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 file_header->creation_format_version );
		libcnotify_printf(
		 "%s: creation format revision\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 file_header->creation_format_revision,
		 file_header->creation_format_revision );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->unknown3,
		 16,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->old_repair_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old repair count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->ecc_fix_success_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: ECC fix success count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->ecc_fix_success_time,
		 8,
		 "ECC fix success time",
		 "\t\t\t",
		 NULL );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->old_ecc_fix_success_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old ECC fix success count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->ecc_fix_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: ECC fix error count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->ecc_fix_error_time,
		 8,
		 "ECC fix error time",
		 "\t\t\t",
		 NULL );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->old_ecc_fix_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old ECC fix error count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->bad_checksum_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: bad checksum error count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) data )->bad_checksum_error_time,
		 8,
		 "bad checksum error time",
		 "\t\t\t",
		 NULL );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->old_bad_checksum_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old bad checksum error count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: committed log:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->committed_log,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: previous shadow volume backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->previous_shadow_volume_backup,
		 24,
		 0 );
		libcnotify_printf(
		 "%s: previous differential backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->previous_differential_backup,
		 24,
		 0 );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->unknown4,
		 40,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->nls_major_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: NLS major version\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->nls_minor_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: NLS minor version\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) data )->unknown5,
		 148,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) data )->unknown_flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown flags\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( libesedb_checksum_calculate_little_endian_xor32(
	     &calculated_xor32_checksum,
	     &( data[ 4 ] ),
	     data_size - 4,
	     0x89abcdef,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to calculate XOR-32 checksum.",
		 function );

		return( -1 );
	}
	if( ( file_header->database_state != 2 )
	 && ( stored_xor32_checksum != calculated_xor32_checksum ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in file header checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
		 function,
		 stored_xor32_checksum,
		 calculated_xor32_checksum );

		return( -1 );
	}
/* TODO add more values to internal structures */

	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_header_read_file_io_handle(
     libesedb_file_header_t *file_header,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t file_header_data[ sizeof( esedb_file_header_t ) ];

	static char *function = "libesedb_file_header_read_file_io_handle";
	ssize_t read_count    = 0;

	if( file_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              file_header_data,
	              sizeof( esedb_file_header_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( esedb_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	if( libesedb_file_header_read_data(
	     file_header,
	     file_header_data,
	     sizeof( esedb_file_header_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

