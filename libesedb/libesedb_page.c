/*
 * Page functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_array_type.h"
#include "libesedb_checksum.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_page.h"

#include "esedb_page.h"

/* Frees the page value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_value_free(
     intptr_t *page_value,
     liberror_error_t **error )
{
	if( page_value != NULL )
	{
		/* The data is referenced and freed elsewhere
		 */
		memory_free(
		 page_value );
	}
	return( 1 );
}

/* Frees the page tags value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tags_value_free(
     intptr_t *page_tags_value,
     liberror_error_t **error )
{
	if( page_tags_value != NULL )
	{
		memory_free(
		 page_tags_value );
	}
	return( 1 );
}

/* Creates a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_initialize(
     libesedb_page_t **page,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_initialize";

	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( *page == NULL )
	{
		*page = (libesedb_page_t *) memory_allocate(
		                             sizeof( libesedb_page_t ) );

		if( *page == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create page.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *page,
		     0,
		     sizeof( libesedb_page_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear page.",
			 function );

			memory_free(
			 *page );

			*page = NULL;

			return( -1 );
		}
		if( libesedb_array_initialize(
		     &( ( *page )->values_array ),
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create page values array.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_free(
     libesedb_page_t **page,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_free";
	int result            = 1;

	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( *page != NULL )
	{
		if( libesedb_array_free(
		     &( ( *page )->values_array ),
		     &libesedb_page_value_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the page values array.",
			 function );

			result = -1;
		}
		if( ( *page )->data != NULL )
		{
			memory_free(
			 ( *page )->data );
		}
		memory_free(
		 *page );

		*page = NULL;
	}
	return( result );
}

/* Reads a page and its values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint32_t page_number,
     liberror_error_t **error )
{
	libesedb_array_t *page_tags_array  = NULL;
	uint8_t *page_values_data          = NULL;
	static char *function              = "libesedb_page_read";
	size_t page_values_data_size       = 0;
	ssize_t read_count                 = 0;
	off64_t page_offset                = 0;
	uint32_t calculated_ecc32_checksum = 0;
	uint32_t calculated_xor32_checksum = 0;
	uint32_t stored_ecc32_checksum     = 0;
	uint32_t stored_page_number        = 0;
	uint32_t stored_xor32_checksum     = 0;
	uint16_t available_data_size       = 0;
	uint16_t available_page_tag        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit               = 0;
	uint16_t value_16bit               = 0;
#endif

	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( page->data != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page data already set.",
		 function );

		return( -1 );
	}
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
	page_offset = ( (off64_t) page_number + 1 ) * (off64_t) io_handle->page_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: reading page: %" PRIu32 " at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
		 function,
		 page_number,
		 page_offset,
		 page_offset );
	}
#endif

	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     page_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek page offset: %" PRIu64 ".",
		 function,
		 page_offset );

		return( -1 );
	}
	page->data = (uint8_t *) memory_allocate(
	                          (size_t) io_handle->page_size );

	if( page->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page data.",
		 function );

		return( -1 );
	}
	page->data_size = (size_t) io_handle->page_size;

	read_count = libbfio_handle_read(
	              file_io_handle,
	              page->data,
	              page->data_size,
	              error );

	if( read_count != (ssize_t) page->data_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page data.",
		 function );

		return( -1 );
	}
	page_values_data      = page->data;
	page_values_data_size = page->data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page header:\n",
		 function );
		libnotify_print_data(
		 page_values_data,
		 sizeof( esedb_page_header_t ) );
	}
#endif

	page->page_number = page_number;

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_page_header_t *) page_values_data )->available_data_size,
	 available_data_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) page_values_data )->previous_page,
	 page->previous_page_number );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) page_values_data )->next_page,
	 page->next_page_number );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) page_values_data )->father_data_page_object_identifier,
	 page->father_data_page_object_identifier );

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_page_header_t *) page_values_data )->available_page_tag,
	 available_page_tag );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) page_values_data )->page_flags,
	 page->flags );

	/* Make sure to read after the page flags
	 */
	if( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
	{
		/* TODO handle checksum */
	}
	else
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_page_header_t *) page_values_data )->xor_checksum,
		 stored_xor32_checksum );

		if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_NEW_RECORD_FORMAT )
		 && ( ( page->flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) != 0 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (esedb_page_header_t *) page_values_data )->ecc_checksum,
			 stored_ecc32_checksum );
		}
		else
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (esedb_page_header_t *) page_values_data )->page_number,
			 stored_page_number );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: current page number\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 page_number );

		if( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
		{
			/* TODO handle checksum */

			byte_stream_copy_to_uint64_little_endian(
			 page_values_data,
			 value_64bit );
			libnotify_printf(
			 "%s: checksum\t\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		else
		{
			libnotify_printf(
			 "%s: XOR checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 stored_xor32_checksum );

			if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_NEW_RECORD_FORMAT )
			 && ( ( page->flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) != 0 ) )
			{
				libnotify_printf(
				 "%s: ECC checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 stored_ecc32_checksum );
			}
			else
			{
				libnotify_printf(
				 "%s: page number\t\t\t\t\t\t: %" PRIu32 "\n",
				 function,
				 stored_page_number );
			}
		}
		libnotify_printf(
		 "%s: database modification time:\n",
		 function );
		libnotify_print_data(
		 ( (esedb_page_header_t *) page_values_data )->database_modification_time,
		 8 );

		libnotify_printf(
		 "%s: previous page number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 page->previous_page_number );
		libnotify_printf(
		 "%s: next page number\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 page->next_page_number );
		libnotify_printf(
		 "%s: father data page (FDP) object identifier\t\t: %" PRIu32 "\n",
		 function,
		 page->father_data_page_object_identifier );

		libnotify_printf(
		 "%s: available data size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 available_data_size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (esedb_page_header_t *) page_values_data )->available_uncommitted_data_size,
		 value_16bit );
		libnotify_printf(
		 "%s: available uncommitted data size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (esedb_page_header_t *) page_values_data )->available_data_offset,
		 value_16bit );
		libnotify_printf(
		 "%s: available data offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libnotify_printf(
		 "%s: available page tag\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 available_page_tag );

		libnotify_printf(
		 "%s: page flags\t\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 page->flags );
		libesedb_debug_print_page_flags(
		 page->flags );
		libnotify_printf(
		 "\n" );
	}
#endif

	/* TODO for now don't bother calculating a checksum for uninitialized pages */

	if( ( page_values_data[ 0 ] != 0 )
	 || ( page_values_data[ 1 ] != 0 )
	 || ( page_values_data[ 2 ] != 0 )
	 || ( page_values_data[ 3 ] != 0 ) )
	{
		if( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
		{
			/* TODO handle checksum */
		}
		else if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_NEW_RECORD_FORMAT )
		      && ( ( page->flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) != 0 ) )
		{
			if( libesedb_checksum_calculate_little_endian_ecc32(
			     &calculated_ecc32_checksum,
			     &calculated_xor32_checksum,
			     page_values_data,
			     page_values_data_size,
			     8,
			     page_number,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unable to calculate ECC-32 and XOR-32 checksum.",
				 function );

				return( -1 );
			}
		}
		else
		{
			if( libesedb_checksum_calculate_little_endian_xor32(
			     &calculated_xor32_checksum,
			     &( page_values_data[ 4 ] ),
			     page_values_data_size - 4,
			     0x89abcdef,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unable to calculate XOR-32 checksum.",
				 function );

				return( -1 );
			}
		}
		if( stored_xor32_checksum != calculated_xor32_checksum )
		{
#ifdef TODO
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_CRC_MISMATCH,
			 "%s: mismatch in page XOR-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
			 function,
			 stored_xor32_checksum,
			 calculated_xor32_checksum );

			return( -1 );
#else
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: mismatch in page XOR-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
				 function,
				 stored_xor32_checksum,
				 calculated_xor32_checksum );
			}
#endif
		}
		if( stored_ecc32_checksum != calculated_ecc32_checksum )
		{
#ifdef TODO
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_CRC_MISMATCH,
			 "%s: mismatch in page ECC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
			 function,
			 stored_ecc32_checksum,
			 calculated_ecc32_checksum );

			return( -1 );
#else
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: mismatch in page ECC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
				 function,
				 stored_ecc32_checksum,
				 calculated_ecc32_checksum );
			}
#endif
		}
	}
	page_values_data      += sizeof( esedb_page_header_t );
	page_values_data_size -= sizeof( esedb_page_header_t );

	if( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: extended page header:\n",
			 function );
			libnotify_print_data(
			 page_values_data,
			 40 );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) page_values_data )->checksum1,
			 value_64bit );
			libnotify_printf(
			 "%s: checksum1\t\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) page_values_data )->checksum2,
			 value_64bit );
			libnotify_printf(
			 "%s: checksum2\t\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) page_values_data )->checksum3,
			 value_64bit );
			libnotify_printf(
			 "%s: checksum3\t\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) page_values_data )->page_number,
			 value_64bit );
			libnotify_printf(
			 "%s: page number\t\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libnotify_printf(
			 "%s: unknown1:\n",
			 function );
			libnotify_print_data(
			 ( (esedb_extended_page_header_t *) page_values_data )->unknown1,
			 8 );
		}
#endif

		page_values_data      += sizeof( esedb_extended_page_header_t );
		page_values_data_size -= sizeof( esedb_extended_page_header_t );
	}
	if( available_page_tag > 0 )
	{
		/* Create the page tags array
		 */
		if( libesedb_array_initialize(
		     &page_tags_array,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create page tags array.",
			 function );

			return( -1 );
		}
		if( libesedb_page_read_tags(
		     page_tags_array,
		     io_handle,
		     available_page_tag,
		     page->data,
		     page->data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page tags.",
			 function );

			libesedb_array_free(
			 &page_tags_array,
			 &libesedb_page_tags_value_free,
			 NULL );

			return( -1 );
		}
		/* The offsets in the page tags are relative after the page header
		 */
		if( libesedb_page_read_values(
		     page->values_array,
		     io_handle,
		     page_tags_array,
		     page_values_data,
		     page_values_data_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page values.",
			 function );

			libesedb_array_free(
			 &page_tags_array,
			 &libesedb_page_tags_value_free,
			 NULL );

			return( -1 );
		}
		if( libesedb_array_free(
		     &page_tags_array,
		     &libesedb_page_tags_value_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the page tags array.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Reads the page tags
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read_tags(
     libesedb_array_t *page_tags_array,
     libesedb_io_handle_t *io_handle,
     uint16_t number_of_page_entries,
     uint8_t *page_data,
     size_t page_data_size,
     liberror_error_t **error )
{
	libesedb_page_tags_value_t *page_tags_value = NULL;
	uint8_t *page_tags_data                     = NULL;
	static char *function                       = "libesedb_page_read_tags";
	uint16_t page_tag_iterator                  = 0;
	uint16_t page_tag_offset                    = 0;
	uint16_t page_tag_size                      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	size_t page_tags_data_size                  = 0;
#endif

	if( page_tags_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tags array.",
		 function );

		return( -1 );
	}
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
	if( page_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page data.",
		 function );

		return( -1 );
	}
	if( page_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid page data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Resize the page tags array
	 */
	if( libesedb_array_resize(
	     page_tags_array,
	     number_of_page_entries,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize page tags array.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		page_tags_data_size = 4 * number_of_page_entries;

		libnotify_printf(
		 "%s: page tags:\n",
		 function );
		libnotify_print_data(
		 &( page_data[ page_data_size - page_tags_data_size ] ),
		 page_tags_data_size );
	}
#endif

	/* Read the page tags back to front
	 */
	page_tags_data = &( page_data[ page_data_size - 2 ] );

	for( page_tag_iterator = 0;
	     page_tag_iterator < number_of_page_entries;
	     page_tag_iterator++ )
	{
		page_tags_value = (libesedb_page_tags_value_t *) memory_allocate(
		                                                  sizeof( libesedb_page_tags_value_t ) );

		if( page_tags_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create page tag value.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint16_little_endian(
		 page_tags_data,
		 page_tag_offset );

		page_tags_data -= 2;

		byte_stream_copy_to_uint16_little_endian(
		 page_tags_data,
		 page_tag_size );

		page_tags_data -= 2;

		if( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
		{
			page_tags_value->flags  = 0;
			page_tags_value->offset = page_tag_offset & 0x7fff;
			page_tags_value->size   = page_tag_size & 0x7fff;;
		}
		else
		{
			page_tags_value->flags  = page_tag_offset >> 13;
			page_tags_value->offset = page_tag_offset & 0x1fff;
			page_tags_value->size   = page_tag_size;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page tag: %03" PRIu16 " offset\t\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 page_tag_iterator,
			 page_tags_value->offset,
			 page_tag_offset );

			libnotify_printf(
			 "%s: page tag: %03" PRIu16 " size\t\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 page_tag_iterator,
			 page_tags_value->size,
			 page_tag_size );

			if( io_handle->format_revision < LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
			{
				libnotify_printf(
				 "%s: page tag: %03" PRIu16 " flags\t\t\t\t: 0x%02" PRIx8 "",
				 function,
				 page_tag_iterator,
				 page_tags_value->flags );
				libesedb_debug_print_page_tag_flags(
				 page_tags_value->flags );
				libnotify_printf(
				 "\n" );
			}
		}
#endif

		if( libesedb_array_set_entry(
		     page_tags_array,
		     page_tag_iterator,
		     (intptr_t *) page_tags_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page tag: %" PRIu16 ".",
			 function,
			 page_tag_iterator );

			memory_free(
			 page_tags_value );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads the page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read_values(
     libesedb_array_t *page_values_array,
     libesedb_io_handle_t *io_handle,
     libesedb_array_t *page_tags_array,
     uint8_t *page_values_data,
     size_t page_values_data_size,
     liberror_error_t **error )
{
	libesedb_page_tags_value_t *page_tags_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	static char *function                       = "libesedb_page_read_values";
	uint16_t page_tag_iterator                  = 0;

	if( page_values_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page values array.",
		 function );

		return( -1 );
	}
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
	if( page_tags_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tags array.",
		 function );

		return( -1 );
	}
	if( page_values_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page values data.",
		 function );

		return( -1 );
	}
	if( page_values_data_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid page values data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Resize the page values array
	 */
	if( libesedb_array_resize(
	     page_values_array,
	     page_tags_array->number_of_entries,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize page values array.",
		 function );

		return( -1 );
	}
	for( page_tag_iterator = 0;
	     page_tag_iterator < page_tags_array->number_of_entries;
	     page_tag_iterator++ )
	{
		if( libesedb_array_get_entry(
		     page_tags_array,
		     page_tag_iterator,
		     (intptr_t **) &page_tags_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page tag: %" PRIu16 ".",
			 function,
			 page_tag_iterator );

			return( -1 );
		}
		if( page_tags_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid page tags value.",
			 function );

			return( -1 );
		}
		page_value = (libesedb_page_value_t *) memory_allocate(
		                                        sizeof( libesedb_page_value_t ) );

		if( page_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create page value.",
			 function );

			return( -1 );
		}
		if( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
		{
			/* TODO does this also apply to non leaf pages ? */

			/* The page tags flags are stored in the upper byte of the first 16-bit value
			 */
			page_tags_value->flags = page_values_data[ page_tags_value->offset + 1 ] >> 5;

			page_values_data[ page_tags_value->offset + 1 ] &= 0x1f;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " offset: % 5" PRIu16 ", size: % 5" PRIu16 ", flags: 0x%02" PRIx8 "",
			 function,
			 page_tag_iterator,
			 page_tags_value->offset,
			 page_tags_value->size,
			 page_tags_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_tags_value->flags );
			libnotify_printf(
			 "\n" );
		}
#endif

		/* TODO check sanity of offset and size */

		page_value->data  = &( page_values_data[ page_tags_value->offset ] );
		page_value->size  = page_tags_value->size;
		page_value->flags = page_tags_value->flags;

		if( libesedb_array_set_entry(
		     page_values_array,
		     page_tag_iterator,
		     (intptr_t *) page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page value: %" PRIu16 ".",
			 function,
			 page_tag_iterator );

			memory_free(
			 page_value );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		"\n" );
	}
#endif

	return( 1 );
}

/* Retrieves the number of page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_number_of_values(
     libesedb_page_t *page,
     uint16_t *number_of_values,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_get_number_of_values";

	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( page->values_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page - missing values array.",
		 function );

		return( -1 );
	}
	if( number_of_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
	if( page->values_array->number_of_entries > (int) UINT16_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of page values value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_values = (uint16_t) page->values_array->number_of_entries;

	return( 1 );
}

/* Retrieves the page value at the index
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_value(
     libesedb_page_t *page,
     uint16_t value_index,
     libesedb_page_value_t **page_value,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_get_value";

	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( page->values_array == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page - missing values array.",
		 function );

		return( -1 );
	}
	if( libesedb_array_get_entry(
	     page->values_array,
	     value_index,
	     (intptr_t **) page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

