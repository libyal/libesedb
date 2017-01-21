/*
 * Input/Output (IO) handle functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <memory.h>
#include <types.h>

#include "libesedb_checksum.h"
#include "libesedb_codepage.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page.h"
#include "libesedb_unused.h"

#include "esedb_file_header.h"

const uint8_t esedb_file_signature[ 4 ] = { 0xef, 0xcd, 0xab, 0x89 };

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_initialize(
     libesedb_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libesedb_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libesedb_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libesedb_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_free(
     libesedb_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libesedb_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_clear(
     libesedb_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libesedb_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libesedb_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	io_handle->ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1252;

	return( 1 );
}

/* Sets the pages data range
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_set_pages_data_range(
     libesedb_io_handle_t *io_handle,
     size64_t file_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_io_handle_set_pages_data_range";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( io_handle->page_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid IO handle - missing page size.",
		 function );

		return( -1 );
	}
	io_handle->pages_data_offset = (off64_t) ( io_handle->page_size * 2 );
	io_handle->pages_data_size   = file_size - (size64_t) io_handle->pages_data_offset;
	io_handle->last_page_number  = (uint32_t) ( io_handle->pages_data_size / io_handle->page_size );

	return( 1 );
}

/* Reads the file (or database) header
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_read_file_header(
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	uint8_t *file_header_data          = NULL;
	static char *function              = "libesedb_io_handle_read_file_header";
	size_t read_size                   = 2048;
	ssize_t read_count                 = 0;
	uint32_t calculated_xor32_checksum = 0;
	uint32_t creation_format_revision  = 0;
	uint32_t creation_format_version   = 0;
	uint32_t format_revision           = 0;
	uint32_t format_version            = 0;
	uint32_t page_size                 = 0;
	uint32_t stored_xor32_checksum     = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit               = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
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
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: %" PRIi64 ".",
		 function,
		 file_offset );

		goto on_error;
	}
	file_header_data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * read_size );

	if( file_header_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file header data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              file_header_data,
	              read_size,
	              error );

	if( read_count != (ssize_t) read_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header data:\n",
		 function );
		libcnotify_print_data(
		 file_header_data,
		 sizeof( esedb_file_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     ( (esedb_file_header_t *) file_header_data )->signature,
	     esedb_file_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		goto on_error;
	}
	if( libesedb_checksum_calculate_little_endian_xor32(
	     &calculated_xor32_checksum,
	     &( file_header_data[ 4 ] ),
	     read_size - 4,
	     0x89abcdef,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GENERIC,
		 "%s: unable to calculate XOR-32 checksum.",
		 function );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) file_header_data )->checksum,
	 stored_xor32_checksum );

	if( stored_xor32_checksum != calculated_xor32_checksum )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_INPUT,
		 LIBCERROR_INPUT_ERROR_CHECKSUM_MISMATCH,
		 "%s: mismatch in file header checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
		 function,
		 stored_xor32_checksum,
		 calculated_xor32_checksum );

		goto on_error;
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) file_header_data )->format_version,
	 format_version );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) file_header_data )->file_type,
	 io_handle->file_type );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) file_header_data )->format_revision,
	 format_revision );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) file_header_data )->page_size,
	 page_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) file_header_data )->creation_format_version,
	 creation_format_version );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_file_header_t *) file_header_data )->creation_format_revision,
	 creation_format_revision );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: checksum\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 stored_xor32_checksum );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->signature,
		 value_32bit );
		libcnotify_printf(
		 "%s: signature\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: format version\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 format_version );

		libcnotify_printf(
		 "%s: file type\t\t\t\t: %" PRIu32 " (",
		 function,
		 io_handle->file_type );
		libesedb_debug_print_file_type(
		 io_handle->file_type );
		libcnotify_printf(
		 ")\n" );

		libcnotify_printf(
		 "%s: database time:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->database_time,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: database signature:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->database_signature,
		 28,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->database_state,
		 value_32bit );
		libcnotify_printf(
		 "%s: database state\t\t\t: %" PRIu32 " ",
		 function,
		 value_32bit );
		libesedb_debug_print_database_state(
		 value_32bit );
		libcnotify_printf(
		 "\n" );

		libcnotify_printf(
		 "%s: consistent position:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->consistent_postition,
		 8,
		 0 );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->consistent_time,
		 8,
		 "consistent time",
		 "\t\t\t\t",
		 NULL );

		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->attach_time,
		 8,
		 "attach time",
		 "\t\t\t\t",
		 NULL );
		libcnotify_printf(
		 "%s: attach position:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->attach_postition,
		 8,
		 0 );

		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->detach_time,
		 8,
		 "detach time",
		 "\t\t\t\t",
		 NULL );
		libcnotify_printf(
		 "%s: detach position:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->detach_postition,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: log signature:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->log_signature,
		 28,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "%s: previous full backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->previous_full_backup,
		 24,
		 0 );
		libcnotify_printf(
		 "%s: previous incremental backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->previous_incremental_backup,
		 24,
		 0 );
		libcnotify_printf(
		 "%s: current full backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->current_full_backup,
		 24,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->shadowing_disabled,
		 value_32bit );
		libcnotify_printf(
		 "%s: shadowing disabled\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->last_object_identifier,
		 value_32bit );
		libcnotify_printf(
		 "%s: last object identifier\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->index_update_major_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update major version\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->index_update_minor_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update minor version\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->index_update_build_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update build number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->index_update_service_pack_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: index update service pack number\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: format revision\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 format_revision );
		libcnotify_printf(
		 "%s: page size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 page_size );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->repair_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: repair count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->repair_time,
		 8,
		 "repair time",
		 "\t\t\t\t",
		 NULL );

		libcnotify_printf(
		 "%s: unknown2:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->unknown2,
		 28,
		 0 );

		libcnotify_printf(
		 "%s: scrub database time:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->scrub_database_time,
		 8,
		 0 );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->scrub_time,
		 8,
		 "scrub time",
		 "\t\t\t\t",
		 NULL );

		libcnotify_printf(
		 "%s: required log:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->required_log,
		 8,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->upgrade_exchange5_format,
		 value_32bit );
		libcnotify_printf(
		 "%s: upgrade Exchange 5.5 format\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->upgrade_free_pages,
		 value_32bit );
		libcnotify_printf(
		 "%s: upgrade free pages\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->upgrade_space_map_pages,
		 value_32bit );
		libcnotify_printf(
		 "%s: upgrade space map pages\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: current shadow volume backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->current_shadow_volume_backup,
		 24,
		 0 );

		libcnotify_printf(
		 "%s: creation format version\t\t: 0x%08" PRIx32 "\n",
		 function,
		 creation_format_version );
		libcnotify_printf(
		 "%s: creation format revision\t\t: 0x%08" PRIx32 "\n",
		 function,
		 creation_format_revision );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->unknown3,
		 16,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->old_repair_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old repair count\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->ecc_fix_success_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: ECC fix success count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->ecc_fix_success_time,
		 8,
		 "ECC fix success time",
		 "\t\t\t",
		 NULL );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->old_ecc_fix_success_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old ECC fix success count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->ecc_fix_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: ECC fix error count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->ecc_fix_error_time,
		 8,
		 "ECC fix error time",
		 "\t\t\t",
		 NULL );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->old_ecc_fix_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old ECC fix error count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->bad_checksum_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: bad checksum error count\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
		libesedb_debug_print_log_time(
		 ( (esedb_file_header_t *) file_header_data )->bad_checksum_error_time,
		 8,
		 "bad checksum error time",
		 "\t\t\t",
		 NULL );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->old_bad_checksum_error_count,
		 value_32bit );
		libcnotify_printf(
		 "%s: old bad checksum error count\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: committed log:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->committed_log,
		 4,
		 0 );

		libcnotify_printf(
		 "%s: previous shadow volume backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->previous_shadow_volume_backup,
		 24,
		 0 );
		libcnotify_printf(
		 "%s: previous differential backup:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->previous_differential_backup,
		 24,
		 0 );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->unknown4,
		 40,
		 0 );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->nls_major_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: NLS major version\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->nls_minor_version,
		 value_32bit );
		libcnotify_printf(
		 "%s: NLS minor version\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_file_header_t *) file_header_data )->unknown5,
		 148,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_file_header_t *) file_header_data )->unknown_flags,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown flags\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 value_32bit,
		 value_32bit );

		libcnotify_printf(
		 "\n" );
	}
#endif

	memory_free(
	 file_header_data );

	file_header_data = NULL;

	/* TODO add more values to internal structures */

	if( file_offset == 0 )
	{
		io_handle->format_version           = format_version;
		io_handle->format_revision          = format_revision;
		io_handle->page_size                = page_size;
		io_handle->creation_format_version  = creation_format_version;
		io_handle->creation_format_revision = creation_format_revision;
	}
	else
	{
		if( io_handle->format_version == 0 )
		{
			io_handle->format_version = format_version;
		}
		else if( io_handle->format_version != format_version )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in format version: 0x%" PRIx32 " and backup: 0x%" PRIx32 "\n",
				 function,
				 io_handle->format_version,
				 io_handle->format_version );
			}
#endif
		}
		if( io_handle->format_revision == 0 )
		{
			io_handle->format_revision = format_revision;
		}
		else if( io_handle->format_revision != format_revision )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in format revision: 0x%" PRIx32 " and backup: 0x%" PRIx32 "\n",
				 function,
				 io_handle->format_revision,
				 io_handle->format_revision );
			}
#endif
		}
		if( io_handle->page_size == 0 )
		{
			io_handle->page_size = page_size;
		}
		else if( io_handle->page_size != page_size )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in page size: 0x%04" PRIx32 " and backup: 0x%04" PRIx32 "\n",
				 function,
				 io_handle->page_size,
				 page_size );
			}
#endif
			/* The offset of the backup (database) file header
			 * is a good indication of the actual page size
			 */
			io_handle->page_size = (uint32_t) file_offset;
		}
	}
	return( 1 );

on_error:
	if( file_header_data != NULL )
	{
		memory_free(
		 file_header_data );
	}
	return( -1 );
}

/* Reads a page
 * Callback function for the page vector
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_read_page(
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_vector_t *vector,
     libfcache_cache_t *cache,
     int element_index,
     int element_file_index LIBESEDB_ATTRIBUTE_UNUSED,
     off64_t element_offset,
     size64_t element_size LIBESEDB_ATTRIBUTE_UNUSED,
     uint32_t element_flags LIBESEDB_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBESEDB_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libesedb_page_t *page = NULL;
	static char *function = "libesedb_io_handle_read_page";

	LIBESEDB_UNREFERENCED_PARAMETER( element_file_index );
	LIBESEDB_UNREFERENCED_PARAMETER( element_size );
	LIBESEDB_UNREFERENCED_PARAMETER( element_flags );
	LIBESEDB_UNREFERENCED_PARAMETER( read_flags );

	if( libesedb_page_initialize(
	     &page,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create page.",
		 function );

		goto on_error;
	}
	if( libesedb_page_read(
	     page,
	     io_handle,
	     file_io_handle,
	     element_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_set_element_value_by_index(
	     vector,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_index,
	     (intptr_t *) page,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_page_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set page as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( page != NULL )
	{
		libesedb_page_free(
		 &page,
		 NULL );
	}
	return( -1 );
}

