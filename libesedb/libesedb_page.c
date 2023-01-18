/*
 * Page functions
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
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_page.h"
#include "libesedb_page_header.h"
#include "libesedb_page_value.h"

#include "esedb_page.h"

/* Creates a page
 * Make sure the value page is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_initialize(
     libesedb_page_t **page,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_initialize";

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( *page != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page value already set.",
		 function );

		return( -1 );
	}
	*page = memory_allocate_structure(
	         libesedb_page_t );

	if( *page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *page,
	     0,
	     sizeof( libesedb_page_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear page.",
		 function );

		memory_free(
		 *page );

		*page = NULL;

		return( -1 );
	}
	if( libesedb_page_header_initialize(
	     &( ( *page )->header ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create header.",
		 function );

		goto on_error;
	}
	if( libcdata_array_initialize(
	     &( ( *page )->values_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *page != NULL )
	{
		if( ( *page )->header != NULL )
		{
			libesedb_page_header_free(
			 &( ( *page )->header ),
			 NULL );
		}
		memory_free(
		 *page );

		*page = NULL;
	}
	return( -1 );
}

/* Frees a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_free(
     libesedb_page_t **page,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_free";
	int result            = 1;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( *page != NULL )
	{
		if( libesedb_page_header_free(
		     &( ( *page )->header ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free header.",
			 function );

			result = -1;
		}
		if( libcdata_array_free(
		     &( ( *page )->values_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_page_value_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free values array.",
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

/* Calculates the page checksums
 * Returns 1 if successful, 0 if page is empty or -1 on error
 */
int libesedb_page_calculate_checksums(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     const uint8_t *page_data,
     size_t page_data_size,
     uint32_t *ecc32_checksum,
     uint32_t *xor32_checksum,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_calculate_checksums";

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( page_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page data.",
		 function );

		return( -1 );
	}
	if( ( page_data_size < 4 )
	 || ( page_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid page data size value out of bounds.",
		 function );

		return( -1 );
	}
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
/* TODO for now don't bother calculating checksums for uninitialized pages */

	if( ( page_data[ 0 ] == 0 )
	 && ( page_data[ 1 ] == 0 )
	 && ( page_data[ 2 ] == 0 )
	 && ( page_data[ 3 ] == 0 ) )
	{
		return( 0 );
	}
	if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
	 && ( io_handle->page_size >= 16384 ) )
	{
/* TODO calculate checksum */
	}
	else if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_NEW_RECORD_FORMAT )
	      && ( ( page->header->flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) != 0 ) )
	{
		if( libesedb_checksum_calculate_little_endian_ecc32(
		     ecc32_checksum,
		     xor32_checksum,
		     page_data,
		     page_data_size,
		     8,
		     page->page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unable to calculate ECC-32 and XOR-32 checksum.",
			 function );

			return( -1 );
		}
	}
	else
	{
		if( libesedb_checksum_calculate_little_endian_xor32(
		     xor32_checksum,
		     &( page_data[ 4 ] ),
		     page_data_size - 4,
		     0x89abcdef,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unable to calculate XOR-32 checksum.",
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
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     const uint8_t *page_data,
     size_t page_data_size,
     uint16_t number_of_page_tags,
     libcerror_error_t **error )
{
	libesedb_page_value_t *page_value = NULL;
	const uint8_t *page_tags_data     = NULL;
	static char *function             = "libesedb_page_read_tags";
	size_t page_tags_data_size        = 0;
	uint16_t page_tag_offset          = 0;
	uint16_t page_tag_size            = 0;
	uint16_t page_tags_index          = 0;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
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
	if( page_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page data.",
		 function );

		return( -1 );
	}
	if( ( page_data_size < 2 )
	 || ( page_data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid page data size value out of bounds.",
		 function );

		return( -1 );
	}
	page_tags_data_size = 4 * number_of_page_tags;

	if( page_tags_data_size > page_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid number of page tags value out of bounds.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: page tags:\n",
		 function );
		libcnotify_print_data(
		 &( page_data[ page_data_size - page_tags_data_size ] ),
		 page_tags_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( libcdata_array_resize(
	     page->values_array,
	     number_of_page_tags,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_page_value_free,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_RESIZE_FAILED,
		 "%s: unable to resize page values array.",
		 function );

		goto on_error;
	}
	/* Read the page tags back to front
	 */
	page_tags_data = &( page_data[ page_data_size - 2 ] );

	for( page_tags_index = 0;
	     page_tags_index < number_of_page_tags;
	     page_tags_index++ )
	{
		if( libesedb_page_value_initialize(
		     &page_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create page value.",
			 function );

			goto on_error;
		}
		byte_stream_copy_to_uint16_little_endian(
		 page_tags_data,
		 page_tag_offset );

		page_tags_data -= 2;

		byte_stream_copy_to_uint16_little_endian(
		 page_tags_data,
		 page_tag_size );

		page_tags_data -= 2;

		if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
		 && ( io_handle->page_size >= 16384 ) )
		{
			page_value->flags  = 0;
			page_value->offset = page_tag_offset & 0x7fff;
			page_value->size   = page_tag_size & 0x7fff;;
		}
		else
		{
			page_value->flags  = page_tag_offset >> 13;
			page_value->offset = page_tag_offset & 0x1fff;
			page_value->size   = page_tag_size & 0x1fff;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page tag: %03" PRIu16 " offset\t\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 page_tags_index,
			 page_value->offset,
			 page_tag_offset );

			libcnotify_printf(
			 "%s: page tag: %03" PRIu16 " size\t\t\t\t: %" PRIu16 " (0x%04" PRIx16 ")\n",
			 function,
			 page_tags_index,
			 page_value->size,
			 page_tag_size );

			if( ( io_handle->format_revision < LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
			 && ( io_handle->page_size < 16384 ) )
			{
				libcnotify_printf(
				 "%s: page tag: %03" PRIu16 " flags\t\t\t\t: 0x%02" PRIx8 "",
				 function,
				 page_tags_index,
				 page_value->flags );
				libesedb_debug_print_page_tag_flags(
				 page_value->flags );
				libcnotify_printf(
				 "\n" );
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libcdata_array_set_entry_by_index(
		     page->values_array,
		     (int) page_tags_index,
		     (intptr_t *) page_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page value: %" PRIu16 ".",
			 function,
			 page_tags_index );

			goto on_error;
		}
		page_value = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );

on_error:
	if( page_value != NULL )
	{
		libesedb_page_value_free(
		 &page_value,
		 NULL );
	}
	libcdata_array_empty(
	 page->values_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_page_value_free,
	 NULL );

	return( -1 );
}

/* Reads the page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read_values(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     uint8_t *page_data,
     size_t page_data_size,
     size_t page_values_data_offset,
     libcerror_error_t **error )
{
	libesedb_page_value_t *page_value = NULL;
	uint8_t *page_values_data         = NULL;
	static char *function             = "libesedb_page_read_values";
	size_t page_tags_data_size        = 0;
	size_t page_values_data_size      = 0;
	uint16_t number_of_page_tags      = 0;
	uint16_t page_tags_index          = 0;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( page_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page data.",
		 function );

		return( -1 );
	}
	if( page_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid page data size value out of bounds.",
		 function );

		return( -1 );
	}
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
	if( (size_t) page_values_data_offset >= page_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid page values data offset value out of bounds.",
		 function );

		return( -1 );
	}
	number_of_page_tags = page->header->available_page_tag;

	if( number_of_page_tags == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_read_tags(
	     page,
	     io_handle,
	     page_data,
	     page_data_size,
	     number_of_page_tags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page tags.",
		 function );

		goto on_error;
	}
	/* The offsets in the page tags are relative after the page header
	 */
	page_values_data      = &( page_data[ page_values_data_offset ] );
	page_values_data_size = page_data_size - page_values_data_offset;
	page_tags_data_size   = 4 * number_of_page_tags;

	if( page_values_data_size < page_tags_data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid page values data size value out of bounds.",
		 function );

		goto on_error;
	}
	page_values_data_size -= page_tags_data_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: page values data:\n",
		 function );
		libcnotify_print_data(
		 page_values_data,
		 page_values_data_size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	for( page_tags_index = 0;
	     page_tags_index < number_of_page_tags;
	     page_tags_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     page->values_array,
		     page_tags_index,
		     (intptr_t **) &page_value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_tags_index );

			goto on_error;
		}
		if( page_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid page value.",
			 function );

			goto on_error;
		}
		if( page_value->offset >= page_values_data_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: unsupported page tags value offset value out of bounds.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " offset: % 5" PRIu16 ", size: % 5" PRIu16 " (page value data size: %" PRIu16 ")\n",
				 function,
				 page_tags_index,
				 page_value->offset,
				 page_value->size,
				 page_values_data_size );
			}
#endif
			goto on_error;
		}
		if( (size_t) page_value->size > ( page_values_data_size - page_value->offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: unsupported page tags value size value out of bounds.",
			 function );

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " offset: % 5" PRIu16 ", size: % 5" PRIu16 " (page value data size: %" PRIu16 ")\n",
				 function,
				 page_tags_index,
				 page_value->offset,
				 page_value->size,
				 page_values_data_size );
			}
#endif
			goto on_error;
		}
		if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
		 && ( io_handle->page_size >= 16384 ) )
		{
			if( page_value->size >= 2 )
			{
				/* The page tags flags are stored in the upper byte of the first 16-bit value
				 */
				page_value->flags = page_values_data[ page_value->offset + 1 ] >> 5;

				page_values_data[ page_value->offset + 1 ] &= 0x1f;
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " offset: % 5" PRIu16 ", size: % 5" PRIu16 ", flags: 0x%02" PRIx8 "",
			 function,
			 page_tags_index,
			 page_value->offset,
			 page_value->size,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );
			libcnotify_printf(
			 "\n" );
		}
#endif
		page_value->data    = &( page_values_data[ page_value->offset ] );
		page_value->offset += page_values_data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %" PRIu16 " data:\n",
			 function,
			 page_tags_index );
			libcnotify_print_data(
			 page_value->data,
			 page_value->size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		"\n" );
	}
#endif
	return( 1 );

on_error:
	libcdata_array_empty(
	 page->values_array,
	 (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_page_value_free,
	 NULL );

	return( -1 );
}

/* Reads a page and its values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read_file_io_handle(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	static char *function              = "libesedb_page_read_file_io_handle";
	ssize_t read_count                 = 0;
	uint32_t calculated_ecc32_checksum = 0;
	uint32_t calculated_xor32_checksum = 0;
	int result                         = 0;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( page->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page data already set.",
		 function );

		return( -1 );
	}
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
	page->page_number = (uint32_t) ( ( file_offset - io_handle->page_size ) / io_handle->page_size );
	page->offset      = file_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: current page number\t\t\t: %" PRIu32 "\n",
		 function,
		 page->page_number );

		libcnotify_printf(
		 "\n" );
	}
#endif

	page->data = (uint8_t *) memory_allocate(
	                          (size_t) io_handle->page_size );

	if( page->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page data.",
		 function );

		goto on_error;
	}
	page->data_size = (size_t) io_handle->page_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading page: %" PRIu32 " at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 page->page_number,
		 page->offset,
		 page->offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              page->data,
	              page->data_size,
	              page->offset,
	              error );

	if( read_count != (ssize_t) page->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page: %" PRIu32 " data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 page->page_number,
		 page->offset,
		 page->offset );

		goto on_error;
	}
	if( libesedb_page_header_read_data(
	     page->header,
	     io_handle,
	     page->data,
	     page->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page header.",
		 function );

		goto on_error;
	}
	result = libesedb_page_calculate_checksums(
	          page,
	          io_handle,
	          page->data,
	          page->data_size,
	          &calculated_ecc32_checksum,
	          &calculated_xor32_checksum,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unable to calculate page checksums.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( page->header->xor32_checksum != calculated_xor32_checksum )
		{
#ifdef TODO
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CRC_MISMATCH,
			 "%s: mismatch in page XOR-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
			 function,
			 page->header->xor32_checksum,
			 calculated_xor32_checksum );

			goto on_error;
#else
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in page XOR-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
				 function,
				 page->header->xor32_checksum,
				 calculated_xor32_checksum );
			}
#endif
		}
		if( page->header->ecc32_checksum != calculated_ecc32_checksum )
		{
#ifdef TODO
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_INPUT,
			 LIBCERROR_INPUT_ERROR_CRC_MISMATCH,
			 "%s: mismatch in page ECC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).",
			 function,
			 page->header->ecc32_checksum,
			 calculated_ecc32_checksum );

			goto on_error;
#else
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in page ECC-32 checksum ( 0x%08" PRIx32 " != 0x%08" PRIx32 " ).\n",
				 function,
				 page->header->ecc32_checksum,
				 calculated_ecc32_checksum );
			}
#endif
		}
	}
	if( libesedb_page_read_values(
	     page,
	     io_handle,
	     page->data,
	     page->data_size,
	     page->header->data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page values.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( page->data != NULL )
	{
		memory_free(
		 page->data );

		page->data = NULL;
	}
	return( -1 );
}

/* Checks if the page is valid root page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_validate_root_page(
     libesedb_page_t *page,
     libcerror_error_t **error )
{
	static char *function         = "libesedb_page_validate_root_page";
	uint32_t page_flags           = 0;
	uint32_t required_page_flags  = 0;
	uint32_t supported_page_flags = 0;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_header_get_flags(
	     page->header,
	     &page_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page flags from header.",
		 function );

		return( -1 );
	}
	required_page_flags = LIBESEDB_PAGE_FLAG_IS_ROOT;

	if( ( page_flags & required_page_flags ) != required_page_flags )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page_flags );

		return( -1 );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) != 0 )
	{
		return( 1 );
	}
	supported_page_flags = required_page_flags
	                     | LIBESEDB_PAGE_FLAG_IS_LEAF
	                     | LIBESEDB_PAGE_FLAG_IS_PARENT
	                     | LIBESEDB_PAGE_FLAG_IS_INDEX
	                     | LIBESEDB_PAGE_FLAG_IS_SPACE_TREE
	                     | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                     | LIBESEDB_PAGE_FLAG_0x0400
	                     | LIBESEDB_PAGE_FLAG_0x0800
	                     | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT
	                     | LIBESEDB_PAGE_FLAG_IS_SCRUBBED
	                     | LIBESEDB_PAGE_FLAG_0x8000
	                     | LIBESEDB_PAGE_FLAG_0x10000;

	if( ( page_flags & ~( supported_page_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page_flags );

		return( -1 );
	}
	return( 1 );
}

/* Checks if the page is valid space tree page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_validate_space_tree_page(
     libesedb_page_t *page,
     libcerror_error_t **error )
{
	static char *function         = "libesedb_page_validate_space_tree_page";
	uint32_t next_page_number     = 0;
	uint32_t page_flags           = 0;
	uint32_t previous_page_number = 0;
	uint32_t required_page_flags  = 0;
	uint32_t supported_page_flags = 0;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_header_get_flags(
	     page->header,
	     &page_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page flags from header.",
		 function );

		return( -1 );
	}
	required_page_flags = LIBESEDB_PAGE_FLAG_IS_SPACE_TREE;

	if( ( page_flags & required_page_flags ) != required_page_flags )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page_flags );

		return( -1 );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) != 0 )
	{
		return( 1 );
	}
	supported_page_flags = required_page_flags
	                     | LIBESEDB_PAGE_FLAG_IS_ROOT
	                     | LIBESEDB_PAGE_FLAG_IS_LEAF
	                     | LIBESEDB_PAGE_FLAG_IS_PARENT
	                     | LIBESEDB_PAGE_FLAG_IS_INDEX
	                     | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                     | LIBESEDB_PAGE_FLAG_0x0400
	                     | LIBESEDB_PAGE_FLAG_0x0800
	                     | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT
	                     | LIBESEDB_PAGE_FLAG_IS_SCRUBBED
	                     | LIBESEDB_PAGE_FLAG_0x8000
	                     | LIBESEDB_PAGE_FLAG_0x10000;

	if( ( page_flags & ~( supported_page_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page_flags );

		return( -1 );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == 0 )
	{
		if( libesedb_page_header_get_previous_page_number(
		     page->header,
		     &previous_page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous page number.",
			 function );

			return( -1 );
		}
		if( previous_page_number != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported previous page number: %" PRIu32 ".",
			 function,
			 previous_page_number );

			return( -1 );
		}
		if( libesedb_page_header_get_next_page_number(
		     page->header,
		     &next_page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next page number.",
			 function );

			return( -1 );
		}
		if( next_page_number != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported next page number: %" PRIu32 ".",
			 function,
			 next_page_number );

			return( -1 );
		}
	}
	return( 1 );
}

/* Checks if the page is valid (regular) page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_validate_page(
     libesedb_page_t *page,
     libcerror_error_t **error )
{
	static char *function         = "libesedb_page_validate_page";
	uint32_t next_page_number     = 0;
	uint32_t page_flags           = 0;
	uint32_t previous_page_number = 0;
	uint32_t supported_page_flags = 0;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_header_get_flags(
	     page->header,
	     &page_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page flags from header.",
		 function );

		return( -1 );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) != 0 )
	{
		return( 1 );
	}
	supported_page_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	                     | LIBESEDB_PAGE_FLAG_IS_LEAF
	                     | LIBESEDB_PAGE_FLAG_IS_PARENT
	                     | LIBESEDB_PAGE_FLAG_IS_INDEX
	                     | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                     | LIBESEDB_PAGE_FLAG_0x0400
	                     | LIBESEDB_PAGE_FLAG_0x0800
	                     | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT
	                     | LIBESEDB_PAGE_FLAG_IS_SCRUBBED
	                     | LIBESEDB_PAGE_FLAG_0x8000
	                     | LIBESEDB_PAGE_FLAG_0x10000;

	if( ( page_flags & ~( supported_page_flags ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page_flags );

		return( -1 );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == 0 )
	{
		if( libesedb_page_header_get_previous_page_number(
		     page->header,
		     &previous_page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve previous page number.",
			 function );

			return( -1 );
		}
		if( previous_page_number != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported previous page number: %" PRIu32 ".",
			 function,
			 previous_page_number );

			return( -1 );
		}
		if( libesedb_page_header_get_next_page_number(
		     page->header,
		     &next_page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve next page number.",
			 function );

			return( -1 );
		}
		if( next_page_number != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported next page number: %" PRIu32 ".",
			 function,
			 next_page_number );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the previous page number
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_previous_page_number(
     libesedb_page_t *page,
     uint32_t *previous_page_number,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_get_previous_page_number";

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_header_get_previous_page_number(
	     page->header,
	     previous_page_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve previous page number from header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the next page number
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_next_page_number(
     libesedb_page_t *page,
     uint32_t *next_page_number,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_get_next_page_number";

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_header_get_next_page_number(
	     page->header,
	     next_page_number,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve next page number from header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the father data page object identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_father_data_page_object_identifier(
     libesedb_page_t *page,
     uint32_t *father_data_page_object_identifier,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_get_father_data_page_object_identifier";

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_header_get_father_data_page_object_identifier(
	     page->header,
	     father_data_page_object_identifier,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve father data page object identifier from header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_flags(
     libesedb_page_t *page,
     uint32_t *flags,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_get_flags";

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_header_get_flags(
	     page->header,
	     flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve flags from header.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_number_of_values(
     libesedb_page_t *page,
     uint16_t *number_of_values,
     libcerror_error_t **error )
{
	static char *function     = "libesedb_page_get_number_of_values";
	int page_number_of_values = 0;

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( number_of_values == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of values.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     page->values_array,
	     &page_number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		return( -1 );
	}
	if( page_number_of_values > (int) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of page values value exceeds maximum.",
		 function );

		return( -1 );
	}
	*number_of_values = (uint16_t) page_number_of_values;

	return( 1 );
}

/* Retrieves the page value at the index
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_get_value_by_index(
     libesedb_page_t *page,
     uint16_t value_index,
     libesedb_page_value_t **page_value,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_get_value_by_index";

	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     page->values_array,
	     (int) value_index,
	     (intptr_t **) page_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

