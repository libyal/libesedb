/*
 * libesedb Input/Output (IO) handle
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
#include <endian.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_guid.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libfdatetime.h"
#include "libesedb_string.h"

#include "esedb_file_header.h"
#include "esedb_page_block.h"

const uint8_t esedb_file_signature[ 4 ] = { 0xef, 0xcd, 0xab, 0x89 };

/* Initialize an io handle
 * Make sure the value io_handle is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_initialize(
     libesedb_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libesedb_io_handle_initialize";

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( *io_handle == NULL )
	{
		*io_handle = (libesedb_io_handle_t *) memory_allocate(
		                                     sizeof( libesedb_io_handle_t ) );

		if( *io_handle == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create io handle.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *io_handle,
		     0,
		     sizeof( libesedb_io_handle_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear file.",
			 function );

			memory_free(
			 *io_handle );

			*io_handle = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees an exisisting io handle
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_free(
     libesedb_io_handle_t **io_handle,
     liberror_error_t **error )
{
	static char *function = "libesedb_io_handle_free";
	int result            = 1;

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		if( ( ( *io_handle )->handle_created_in_library != 0 )
		 && ( ( *io_handle )->file_io_handle != NULL )
		 && ( libbfio_handle_free(
		       &( ( *io_handle )->file_io_handle ),
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free file io handle.",
			 function );

			result = -1;
		}
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( result );
}

/* Opens an io handle
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_open(
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     int flags,
     liberror_error_t **error )
{
        static char *function = "libesedb_io_handle_open";

        if( io_handle == NULL )
        {
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
                 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
                 "%s: invalid io handle.",
                 function );

                return( -1 );
        }
	if( io_handle->file_io_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid io handle - file io handle already set.",
		 function );

		return( -1 );
	}
        if( file_io_handle == NULL )
        {
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
                 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
                 "%s: invalid file io handle.",
                 function );

                return( -1 );
        }
	io_handle->file_io_handle = file_io_handle;

	if( libbfio_handle_open(
	     io_handle->file_io_handle,
	     flags,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_IO,
                 LIBERROR_IO_ERROR_OPEN_FAILED,
                 "%s: unable to open file io handle.",
                 function );

                return( -1 );
	}
	return( 1 );
}

/* Closes an io handle
 * Returns 0 if successful or -1 on error
 */
int libesedb_io_handle_close(
     libesedb_io_handle_t *io_handle,
     liberror_error_t **error )
{
        static char *function = "libesedb_io_handle_close";

        if( io_handle == NULL )
        {
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
                 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
                 "%s: invalid io handle.",
                 function );

                return( -1 );
        }
#if defined( HAVE_DEBUG_OUTPUT )
	if( libesedb_debug_print_read_offsets(
	     io_handle->file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
		 "%s: unable to print the read offsets.",
		 function );
	}
#endif
	if( libbfio_handle_close(
	     io_handle->file_io_handle,
	     error ) != 0 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_IO,
                 LIBERROR_IO_ERROR_CLOSE_FAILED,
                 "%s: unable to close file io handle.",
                 function );

                return( -1 );
	}
	return( 0 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_read_file_header(
     libesedb_io_handle_t *io_handle,
     size_t *page_block_size,
     liberror_error_t **error )
{
	esedb_file_header_t file_header;

	static char *function = "libesedb_io_handle_read_file_header";
	ssize_t read_count    = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint32_t test         = 0;
#endif

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( io_handle->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid io handle - missing file io handle.",
		 function );

		return( -1 );
	}
	if( page_block_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page block size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: reading file header at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
	 function,
	 0,
	 0 );
#endif

	if( libbfio_handle_seek_offset(
	     io_handle->file_io_handle,
	     0,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek file header offset: %" PRIu64 ".",
		 function,
		 0 );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              io_handle->file_io_handle,
	              (uint8_t *) &file_header,
	              sizeof( esedb_file_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( esedb_file_header_t ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: file header:\n",
	 function );
	libnotify_verbose_print_data(
	 (uint8_t *) &file_header,
	 sizeof( esedb_file_header_t ) );
#endif

	if( memory_compare(
	     file_header.signature,
	     esedb_file_signature,
	     4 ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported file signature.",
		 function );

		return( -1 );
	}
	/* TODO */

	endian_little_convert_32bit(
	 io_handle->format_version,
	 file_header.format_version );

	endian_little_convert_32bit(
	 io_handle->format_revision,
	 file_header.format_revision );

	endian_little_convert_32bit(
	 *page_block_size,
	 file_header.page_size );

#if defined( HAVE_VERBOSE_OUTPUT )
	endian_little_convert_32bit(
	 test,
	 file_header.unknown1 );
	libnotify_verbose_printf(
	 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 function,
	 test,
	 test );

	endian_little_convert_32bit(
	 test,
	 file_header.signature );
	libnotify_verbose_printf(
	 "%s: signature\t\t\t\t: 0x%08" PRIx32 "\n",
	 function,
	 test );

	libnotify_verbose_printf(
	 "%s: format version\t\t\t: 0x%08" PRIx32 "\n",
	 function,
	 io_handle->format_version );

	endian_little_convert_32bit(
	 test,
	 file_header.unknown2 );
	libnotify_verbose_printf(
	 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 function,
	 test,
	 test );

	libnotify_verbose_printf(
	 "%s: unknown3:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.unknown3,
	 8 );

	libnotify_verbose_printf(
	 "%s: database signature:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.database_signature,
	 28 );
	endian_little_convert_32bit(
	 test,
	 file_header.database_state );
	libnotify_verbose_printf(
	 "%s: database state\t\t\t: 0x%08" PRIx32 "\n",
	 function,
	 test );

	libnotify_verbose_printf(
	 "%s: consistent position:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.consistent_postition,
	 8 );
	libesedb_debug_print_log_time(
	 file_header.consistent_time,
	 8,
	 "consistent time\t\t\t\t",
	 NULL );

	libesedb_debug_print_log_time(
	 file_header.attach_time,
	 8,
	 "attach time\t\t\t\t",
	 NULL );
	libnotify_verbose_printf(
	 "%s: attach position:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.attach_postition,
	 8 );

	libesedb_debug_print_log_time(
	 file_header.detach_time,
	 8,
	 "detach time\t\t\t\t",
	 NULL );
	libnotify_verbose_printf(
	 "%s: detach position:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.detach_postition,
	 8 );

	libnotify_verbose_printf(
	 "%s: unknown signature:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.unknown_signature,
	 28 );

	libnotify_verbose_printf(
	 "%s: previous full backup:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.previous_full_backup,
	 24 );
	libnotify_verbose_printf(
	 "%s: previous incremental backup:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.previous_incremental_backup,
	 24 );
	libnotify_verbose_printf(
	 "%s: current full backup:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.current_full_backup,
	 24 );

	endian_little_convert_32bit(
	 test,
	 file_header.unknown4 );
	libnotify_verbose_printf(
	 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 function,
	 test,
	 test );
	endian_little_convert_32bit(
	 test,
	 file_header.unknown5 );
	libnotify_verbose_printf(
	 "%s: unknown5\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 function,
	 test,
	 test );
	endian_little_convert_32bit(
	 test,
	 file_header.unknown6 );
	libnotify_verbose_printf(
	 "%s: unknown6\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 function,
	 test,
	 test );

	endian_little_convert_32bit(
	 test,
	 file_header.index_update_major_version );
	libnotify_verbose_printf(
	 "%s: index update major version\t\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_32bit(
	 test,
	 file_header.index_update_minor_version );
	libnotify_verbose_printf(
	 "%s: index update minor version\t\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_32bit(
	 test,
	 file_header.index_update_build_number );
	libnotify_verbose_printf(
	 "%s: index update build number\t\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_32bit(
	 test,
	 file_header.index_update_service_pack_number );
	libnotify_verbose_printf(
	 "%s: index update service pack number\t: %" PRIu32 "\n",
	 function,
	 test );

	libnotify_verbose_printf(
	 "%s: format revision\t\t\t: 0x%08" PRIx32 "\n",
	 function,
	 io_handle->format_revision );
	libnotify_verbose_printf(
	 "%s: page size\t\t\t\t: %" PRIu32 "\n",
	 function,
	 *page_block_size );

	libnotify_verbose_printf(
	 "%s: unknown7:\n",
	 function );
	libnotify_verbose_print_data(
	 file_header.unknown7,
	 84 );

	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libesedb_io_handle_read_page_block(
     libesedb_io_handle_t *io_handle,
     off64_t page_block_offset,
     size_t page_block_size,
     liberror_error_t **error )
{
	uint8_t *page_block_data     = NULL;
	uint8_t *page_tag_data       = NULL;
	uint8_t *page_values_data    = NULL;
	uint8_t *page_value_data     = NULL;
	static char *function        = "libesedb_io_handle_read_page_block";
	ssize_t read_count           = 0;
	uint32_t page_flags          = 0;
	uint16_t available_data_size = 0;
	uint16_t available_page_tag  = 0;
	uint16_t page_tag_number     = 0;
	uint16_t page_value_offset   = 0;
	uint16_t page_value_size     = 0;
	uint16_t key_size            = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint32_t test                = 0;
#endif

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( io_handle->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid io handle - missing file io handle.",
		 function );

		return( -1 );
	}
	if( page_block_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid page block size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: reading file header at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
	 function,
	 page_block_offset,
	 page_block_offset );
#endif

	if( libbfio_handle_seek_offset(
	     io_handle->file_io_handle,
	     page_block_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek page block offset: %" PRIu64 ".",
		 function,
		 page_block_offset );

		return( -1 );
	}
	page_block_data = (uint8_t *) memory_allocate(
	                               page_block_size );

	if( page_block_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page blocks data.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              io_handle->file_io_handle,
	              page_block_data,
	              page_block_size,
	              error );

	if( read_count != (ssize_t) page_block_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page block data.",
		 function );

		memory_free(
		 page_block_data );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: page block header:\n",
	 function );
	libnotify_verbose_print_data(
	 page_block_data,
	 sizeof( esedb_page_block_header_t ) );
#endif

	endian_little_convert_16bit(
	 available_data_size,
	 ( (esedb_page_block_header_t *) page_block_data )->available_data_size );

	endian_little_convert_16bit(
	 available_page_tag,
	 ( (esedb_page_block_header_t *) page_block_data )->available_page_tag );

	endian_little_convert_32bit(
	 page_flags,
	 ( (esedb_page_block_header_t *) page_block_data )->page_flags );

#if defined( HAVE_VERBOSE_OUTPUT )

	libnotify_verbose_printf(
	 "%s: current page number\t\t\t\t: %" PRIu64 "\n",
	 function,
	 ( page_block_offset / page_block_size ) - 1 );

	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_block_header_t *) page_block_data )->xor_checksum );
	libnotify_verbose_printf(
	 "%s: xor checksum\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 function,
	 test,
	 test );
	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_block_header_t *) page_block_data )->page_number );

	if( io_handle->format_revision >= 0x0c )
	{
		endian_little_convert_32bit(
		 test,
		 ( (esedb_page_block_header_t *) page_block_data )->ecc_checksum );
		libnotify_verbose_printf(
		 "%s: ecc checksum\t\t\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 test,
		 test );
		endian_little_convert_32bit(
		 test,
		 ( (esedb_page_block_header_t *) page_block_data )->page_number );
	}
	else
	{
		libnotify_verbose_printf(
		 "%s: page number\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 test );
	}
	libnotify_verbose_printf(
	 "%s: modification time:\n",
	 function );
	libnotify_verbose_print_data(
	 ( (esedb_page_block_header_t *) page_block_data )->modification_time,
	 8 );

	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_block_header_t *) page_block_data )->previous_page );
	libnotify_verbose_printf(
	 "%s: previous page number\t\t\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_block_header_t *) page_block_data )->next_page );
	libnotify_verbose_printf(
	 "%s: next page number\t\t\t\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_block_header_t *) page_block_data )->father_object_identifier );
	libnotify_verbose_printf(
	 "%s: father object identifier\t\t\t: %" PRIu32 "\n",
	 function,
	 test );

	libnotify_verbose_printf(
	 "%s: available data size\t\t\t\t: %" PRIu32 "\n",
	 function,
	 available_data_size );
	endian_little_convert_16bit(
	 test,
	 ( (esedb_page_block_header_t *) page_block_data )->available_uncommitted_data_size );
	libnotify_verbose_printf(
	 "%s: available uncommitted data size\t\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_16bit(
	 test,
	 ( (esedb_page_block_header_t *) page_block_data )->available_data_offset );
	libnotify_verbose_printf(
	 "%s: available data offset\t\t\t: %" PRIu32 "\n",
	 function,
	 test );
	libnotify_verbose_printf(
	 "%s: available page tag\t\t\t\t: %" PRIu32 "\n",
	 function,
	 available_page_tag );

	liblnk_debug_print_page_flags(
	 page_flags,
	 NULL );
#endif

	page_values_data = &( page_block_data[ sizeof( esedb_page_block_header_t ) ] );
	page_tag_data    = &( page_block_data[ page_block_size - 2 ] );

	for( page_tag_number = 0;
	     page_tag_number < available_page_tag;
	     page_tag_number++ )
	{
		endian_little_convert_16bit(
		 page_value_offset,
		 page_tag_data );

		page_tag_data -= 2;

		endian_little_convert_16bit(
		 page_value_size,
		 page_tag_data );

		page_tag_data -= 2;

		page_value_data = &( page_values_data[ page_value_offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: page tag: %03d offset\t\t\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_value_offset );

		libnotify_verbose_printf(
		 "%s: page tag: %03d size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_value_size );

		if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) == LIBESEDB_PAGE_FLAG_IS_ROOT )
		{
			if( page_tag_number == 0 )
			{
				libnotify_verbose_printf(
				 "%s: page tag: %03d value:\n",
				 function,
				 page_tag_number );
				libnotify_verbose_print_data(
				 page_value_data,
				 page_value_size );

				endian_little_convert_32bit(
				 test,
				 page_value_data );

				page_value_data += 4;

				libnotify_verbose_printf(
				 "%s: page tag: %03d back reference\t\t: 0x%08" PRIx32 "\n",
				 function,
				 page_tag_number,
				 test );

				/* Skip the next value */
				page_value_data += 4;

				endian_little_convert_32bit(
				 test,
				 page_value_data );

				page_value_data += 4;

				libnotify_verbose_printf(
				 "%s: page tag: %03d unknown amount\t\t: %" PRIu32 "\n",
				 function,
				 page_tag_number,
				 test );

				endian_little_convert_32bit(
				 test,
				 page_value_data );

				libnotify_verbose_printf(
				 "%s: page tag: %03d intermediate page number\t: %" PRIu32 "\n",
				 function,
				 page_tag_number,
				 test );
			}
			else
			{
				endian_little_convert_16bit(
				 key_size,
				 page_value_data );

				page_value_data += 2;

				libnotify_verbose_printf(
				 "%s: page tag: %03d highest key size\t\t: %" PRIu16 "\n",
				 function,
				 page_tag_number,
				 key_size );

				libnotify_verbose_printf(
				 "%s: page tag: %03d highest key value:\n",
				 function,
				 page_tag_number );
				libnotify_verbose_print_data(
				 page_value_data,
				 key_size );

				page_value_data += key_size;

				endian_little_convert_32bit(
				 test,
				 page_value_data );

				libnotify_verbose_printf(
				 "%s: page tag: %03d child page number\t\t: %" PRIu32 "\n",
				 function,
				 page_tag_number,
				 test );
			}
			libnotify_verbose_printf(
			 "\n" );
		}
		else if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == LIBESEDB_PAGE_FLAG_IS_LEAF )
		{
			if( page_tag_number == 0 )
			{
				libnotify_verbose_printf(
				 "%s: page tag: %03d key value:\n",
				 function,
				 page_tag_number );
				libnotify_verbose_print_data(
				 page_value_data,
				 page_value_size );
			}
			else
			{
				endian_little_convert_16bit(
				 key_size,
				 page_value_data );

				page_value_data += 2;

				libnotify_verbose_printf(
				 "%s: page tag: %03d key size\t\t\t: %" PRIu16 "\n",
				 function,
				 page_tag_number,
				 key_size );

				if( ( ( page_flags & LIBESEDB_PAGE_FLAG_IS_PARENT ) == LIBESEDB_PAGE_FLAG_IS_PARENT )
				 && ( key_size == 4 ) )
				{
					endian_big_convert_32bit(
					 test,
					 page_value_data );

					if( page_tag_number == 1 )
					{
						libnotify_verbose_printf(
						 "%s: page tag: %03d intermediate page number\t: %" PRIu32 "\n",
						 function,
						 page_tag_number,
						 test );
					}
					else
					{
						libnotify_verbose_printf(
						 "%s: page tag: %03d child page number\t\t: %" PRIu32 "\n",
						 function,
						 page_tag_number,
						 test );
					}
				}
				else
				{
					libnotify_verbose_printf(
					 "%s: page tag: %03d key value:\n",
					 function,
					 page_tag_number );
					libnotify_verbose_print_data(
					 page_value_data,
					 key_size );
				}
				page_value_data += key_size;

				page_value_size -= 2 + key_size;

				if( ( ( page_flags & LIBESEDB_PAGE_FLAG_IS_PARENT ) == LIBESEDB_PAGE_FLAG_IS_PARENT )
				 && ( page_value_size == 4 ) )
				{
					endian_little_convert_32bit(
					 test,
					 page_value_data );

					libnotify_verbose_printf(
					 "%s: page tag: %03d back reference\t\t: 0x%08" PRIx32 "\n",
					 function,
					 page_tag_number,
					 test );

					libnotify_verbose_printf(
					 "\n" );
				}
				else
				{
					libnotify_verbose_printf(
					 "%s: page tag: %03d value:\n",
					 function,
					 page_tag_number );
					libnotify_verbose_print_data(
					 page_value_data,
					 page_value_size );
				}
			}
		}
		else
		{
			libnotify_verbose_printf(
			 "%s: page tag: %03d value:\n",
			 function,
			 page_tag_number );
			libnotify_verbose_print_data(
			 page_value_data,
			 page_value_size );
		}

#endif
	}
	memory_free(
	 page_block_data );

	return( 1 );
}

