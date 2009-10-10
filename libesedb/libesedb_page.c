/*
 * Page functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_array_type.h"
#include "libesedb_checksum.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
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
		/* The referenced data is freed elsewhere
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
     uint32_t page_number,
     liberror_error_t **error )
{
	libesedb_array_t *page_tags_array  = NULL;
	static char *function              = "libesedb_page_read";
	ssize_t read_count                 = 0;
	off64_t page_offset                = 0;
	uint32_t calculated_xor32_checksum = 0;
	uint32_t stored_xor32_checksum     = 0;
	uint16_t available_data_size       = 0;
	uint16_t available_page_tag        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit               = 0;
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
	page_offset = ( page_number + 1 ) * io_handle->page_size;

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: reading page: %" PRIu32 " at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
	 function,
	 page_number,
	 page_offset,
	 page_offset );
#endif

	if( libbfio_handle_seek_offset(
	     io_handle->file_io_handle,
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
	              io_handle->file_io_handle,
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
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: page header:\n",
	 function );
	libnotify_verbose_print_data(
	 page->data,
	 sizeof( esedb_page_header_t ) );
#endif

	/* TODO for now don't bother calculating a checksum for uninitialized pages */

	if( ( page->data[ 0 ] != 0 )
	 || ( page->data[ 1 ] != 0 )
	 || ( page->data[ 2 ] != 0 )
	 || ( page->data[ 3 ] != 0 ) )
	{
		if( io_handle->format_revision >= 0x0c )
		{
			if( libesedb_checksum_calculate_little_endian_xor32(
			     &calculated_xor32_checksum,
			     &( ( page->data )[ 8 ] ),
			     page->data_size - 8,
			     page_number,
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
		else
		{
			if( libesedb_checksum_calculate_little_endian_xor32(
			     &calculated_xor32_checksum,
			     &( ( page->data )[ 4 ] ),
			     page->data_size - 4,
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
		endian_little_convert_32bit(
		 stored_xor32_checksum,
		 ( (esedb_page_header_t *) page->data )->xor_checksum );

		if( stored_xor32_checksum != calculated_xor32_checksum )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_INPUT,
			 LIBERROR_INPUT_ERROR_CRC_MISMATCH,
			 "%s: mismatch in page XOR checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
			 function,
			 stored_xor32_checksum,
			 calculated_xor32_checksum );

			return( -1 );
		}
	}
	page->page_number = page_number;

	endian_little_convert_16bit(
	 available_data_size,
	 ( (esedb_page_header_t *) page->data )->available_data_size );

	endian_little_convert_32bit(
	 page->previous_page_number,
	 ( (esedb_page_header_t *) page->data )->previous_page );
	endian_little_convert_32bit(
	 page->next_page_number,
	 ( (esedb_page_header_t *) page->data )->next_page );
	endian_little_convert_32bit(
	 page->father_data_page_object_identifier,
	 ( (esedb_page_header_t *) page->data )->father_data_page_object_identifier );

	endian_little_convert_16bit(
	 available_page_tag,
	 ( (esedb_page_header_t *) page->data )->available_page_tag );

	endian_little_convert_32bit(
	 page->flags,
	 ( (esedb_page_header_t *) page->data )->page_flags );

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: current page number\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 page_number );

	libnotify_verbose_printf(
	 "%s: XOR checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
	 function,
	 stored_xor32_checksum );

	if( ( io_handle->format_revision >= 0x0c )
	 && ( ( page->flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) == LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) )
	{
		endian_little_convert_32bit(
		 value_32bit,
		 ( (esedb_page_header_t *) page->data )->ecc_checksum );
		libnotify_verbose_printf(
		 "%s: ECC checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
	}
	else
	{
		endian_little_convert_32bit(
		 value_32bit,
		 ( (esedb_page_header_t *) page->data )->page_number );
		libnotify_verbose_printf(
		 "%s: page number\t\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );
	}
	libnotify_verbose_printf(
	 "%s: database modification time:\n",
	 function );
	libnotify_verbose_print_data(
	 ( (esedb_page_header_t *) page->data )->database_modification_time,
	 8 );

	libnotify_verbose_printf(
	 "%s: previous page number\t\t\t\t: %" PRIu32 "\n",
	 function,
	 page->previous_page_number );
	libnotify_verbose_printf(
	 "%s: next page number\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 page->next_page_number );
	libnotify_verbose_printf(
	 "%s: father data page (FDP) object identifier\t\t: %" PRIu32 "\n",
	 function,
	 page->father_data_page_object_identifier );

	libnotify_verbose_printf(
	 "%s: available data size\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 available_data_size );
	endian_little_convert_16bit(
	 value_32bit,
	 ( (esedb_page_header_t *) page->data )->available_uncommitted_data_size );
	libnotify_verbose_printf(
	 "%s: available uncommitted data size\t\t\t: %" PRIu32 "\n",
	 function,
	 value_32bit );
	endian_little_convert_16bit(
	 value_32bit,
	 ( (esedb_page_header_t *) page->data )->available_data_offset );
	libnotify_verbose_printf(
	 "%s: available data offset\t\t\t\t: %" PRIu32 "\n",
	 function,
	 value_32bit );
	libnotify_verbose_printf(
	 "%s: available page tag\t\t\t\t\t: %" PRIu32 "\n",
	 function,
	 available_page_tag );

	libnotify_verbose_printf(
	 "%s: page flags\t\t\t\t\t\t: ",
	 function );
	libesedb_debug_print_page_flags(
	 page->flags );
	libnotify_verbose_printf(
	 "\n" );
#endif

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
		     page_tags_array,
		     &( page->data[ sizeof( esedb_page_header_t ) ] ),
		     page->data_size - sizeof( esedb_page_header_t ),
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
     uint16_t amount_of_page_entries,
     uint8_t *page_data,
     size_t page_data_size,
     liberror_error_t **error )
{
	libesedb_page_tags_value_t *page_tags_value = NULL;
	uint8_t *page_tags_data                     = NULL;
	static char *function                       = "libesedb_page_read_tags";
	uint16_t page_tag_number                    = 0;

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
	     amount_of_page_entries,
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
	page_tags_data_size = 4 * amount_of_page_entries;

	libnotify_verbose_printf(
	 "%s: page tags:\n",
	 function );
	libnotify_verbose_print_data(
	 &( page_data[ page_data_size - page_tags_data_size ] ),
	 page_tags_data_size );
#endif

	/* Read the page tags back to front
	 */
	page_tags_data = &( page_data[ page_data_size - 2 ] );

	for( page_tag_number = 0;
	     page_tag_number < amount_of_page_entries;
	     page_tag_number++ )
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
		endian_little_convert_16bit(
		 page_tags_value->offset,
		 page_tags_data );

		page_tags_data -= 2;

		endian_little_convert_16bit(
		 page_tags_value->size,
		 page_tags_data );

		page_tags_data -= 2;

		page_tags_value->flags   = page_tags_value->offset >> 13;
		page_tags_value->offset &= 0x1fff;

#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: page tag: %03d offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_tags_value->offset );
		libnotify_verbose_printf(
		 "%s: page tag: %03d size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_tags_value->size );
		libnotify_verbose_printf(
		 "%s: page tag: %03d flags\t\t\t\t: ",
		 function,
		 page_tag_number );
		libesedb_debug_print_page_tag_flags(
		 page_tags_value->flags );
		libnotify_verbose_printf(
		 "\n" );
#endif

		if( libesedb_array_set_entry(
		     page_tags_array,
		     page_tag_number,
		     (intptr_t *) page_tags_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page tag: %" PRIu16 ".",
			 function,
			 page_tag_number );

			memory_free(
			 page_tags_value );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

/* Reads the page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read_values(
     libesedb_array_t *page_values_array,
     libesedb_array_t *page_tags_array,
     uint8_t *page_values_data,
     size_t page_values_data_size,
     liberror_error_t **error )
{
	libesedb_page_tags_value_t *page_tags_value = NULL;
	libesedb_page_value_t *page_value           = NULL;
	static char *function                       = "libesedb_page_read_values";
	uint16_t page_tag_number                    = 0;

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
	     page_tags_array->amount_of_entries,
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
	for( page_tag_number = 0;
	     page_tag_number < page_tags_array->amount_of_entries;
	     page_tag_number++ )
	{
		if( libesedb_array_get_entry(
		     page_tags_array,
		     page_tag_number,
		     (intptr_t **) &page_tags_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page tag: %" PRIu16 ".",
			 function,
			 page_tag_number );

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

#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: page value: %03d offset: % 4" PRIu16 ", size: % 4" PRIu16 ", flags: ",
		 function,
		 page_tag_number,
		 page_tags_value->offset,
		 page_tags_value->size );
		libesedb_debug_print_page_tag_flags(
		 page_tags_value->flags );
		libnotify_verbose_printf(
		 "\n" );
#endif

		page_value->data  = &( page_values_data[ page_tags_value->offset ] );
		page_value->size  = page_tags_value->size;
		page_value->flags = page_tags_value->flags;

		if( libesedb_array_set_entry(
		     page_values_array,
		     page_tag_number,
		     (intptr_t *) page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page value: %" PRIu16 ".",
			 function,
			 page_tag_number );

			memory_free(
			 page_value );

			return( -1 );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	"\n" );
#endif

	return( 1 );
}

/* Retrieves the amount of page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_amount_of_values(
     libesedb_page_t *page,
     uint16_t *amount_of_values,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_get_amount_of_values";

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
	if( amount_of_values == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid amount of values.",
		 function );

		return( -1 );
	}
	if( page->values_array->amount_of_entries > (int) UINT16_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid amount of page values value exceeds maximum.",
		 function );

		return( -1 );
	}
	*amount_of_values = (uint16_t) page->values_array->amount_of_entries;

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

