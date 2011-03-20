/*
 * Page tree functions
 *
 * Copyright (c) 2009-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_page.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"
#include "libesedb_unused.h"
#include "libesedb_values_tree_value.h"

#include "esedb_page_values.h"

/* Creates a page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_initialize(
     libesedb_page_tree_t **page_tree,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfdata_cache_t *pages_cache,
     uint32_t object_identifier,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_initialize";

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *page_tree == NULL )
	{
		*page_tree = (libesedb_page_tree_t *) memory_allocate(
		                                       sizeof( libesedb_page_tree_t ) );

		if( *page_tree == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create page tree.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *page_tree,
		     0,
		     sizeof( libesedb_page_tree_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear page tree.",
			 function );

			memory_free(
			 *page_tree );

			*page_tree = NULL;

			return( -1 );
		}
		( *page_tree )->io_handle                 = io_handle;
		( *page_tree )->pages_vector              = pages_vector;
		( *page_tree )->pages_cache               = pages_cache;
		( *page_tree )->object_identifier         = object_identifier;
		( *page_tree )->table_definition          = table_definition;
		( *page_tree )->template_table_definition = template_table_definition;
	}
	return( 1 );
}

/* Frees page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_free(
     intptr_t *page_tree,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_free";
	int result            = 1;

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	/* The io_handle, pages_vector, pages_cache, table_definition and template_table_definition references
	 * are freed elsewhere
	 */
	memory_free(
	 page_tree );

	return( result );
}

/* Reads the root page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_root_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     off64_t page_offset,
     uint32_t page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_root_page";
	uint32_t extent_space             = 0;
	uint32_t required_flags           = 0;
	uint32_t supported_flags          = 0;
	uint32_t space_tree_page_number   = 0;
	uint16_t number_of_page_values    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t initial_number_of_pages  = 0;
	uint32_t value_32bit              = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     page_tree->pages_vector,
	     file_io_handle,
	     page_tree->pages_cache,
	     page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
		 function,
		 page_number,
		 page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	required_flags = LIBESEDB_PAGE_FLAG_IS_ROOT;

	if( ( page->flags & required_flags ) != required_flags )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	supported_flags = required_flags
	                | LIBESEDB_PAGE_FLAG_IS_LEAF
	                | LIBESEDB_PAGE_FLAG_IS_PARENT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_SPACE_TREE
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_0x0400
	                | LIBESEDB_PAGE_FLAG_0x0800
	                | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT;

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     0,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: 0.",
		 function );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value.",
		 function );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page value - missing data.",
		 function );

		return( -1 );
	}
	if( page_value->size != 16 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported size of page value.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_root_page_header_t *) page_value->data )->space_tree_page_number,
	 space_tree_page_number );

	/* TODO handle the root page header */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->initial_number_of_pages,
		 initial_number_of_pages );
		libnotify_printf(
		 "%s: (header) initial number of pages\t: %" PRIu32 "\n",
		 function,
		 initial_number_of_pages );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->parent_father_data_page_number,
		 value_32bit );
		libnotify_printf(
		 "%s: (header) parent FDP number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->extent_space,
		 extent_space );
		libnotify_printf(
		 "%s: (header) extent space\t\t: %" PRIu32 "\n",
		 function,
		 extent_space );

		libnotify_printf(
		 "%s: (header) space tree page number\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 space_tree_page_number,
		 space_tree_page_number );

		libnotify_printf(
		 "%s: (header) primary extent\t\t: %" PRIu32 "-%c\n",
		 function,
		 initial_number_of_pages,
		 ( extent_space == 0 ? 's' : 'm' ) );

		libnotify_printf(
		 "\n" );
	}
#endif

	/* Read the space tree pages
	 */
	if( extent_space > 0 )
	{
		if( space_tree_page_number >= 0xff000000UL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported space tree page number.",
			 function,
			 space_tree_page_number );

			return( -1 );
		}
		if( space_tree_page_number > 0 )
		{
			/* Read the owned pages space tree page
			 */
			if( libesedb_page_tree_read_space_tree_page(
			     page_tree,
			     file_io_handle,
			     space_tree_page_number,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read space tree page: %" PRIu32 ".",
				 function,
				 space_tree_page_number );

				return( -1 );
			}
			/* Read the available pages space tree page
			 */
			space_tree_page_number += 1;

			if( libesedb_page_tree_read_space_tree_page(
			     page_tree,
			     file_io_handle,
			     space_tree_page_number,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read space tree page: %" PRIu32 ".",
				 function,
				 space_tree_page_number );

				return( -1 );
			}
		}
	}
	return( 1 );
}

/* Reads the space tree page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_space_tree_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     uint32_t page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	uint8_t *page_value_data          = NULL;
	static char *function             = "libesedb_page_tree_read_space_tree_page";
	uint32_t number_of_pages          = 0;
	uint32_t required_flags           = 0;
	uint32_t supported_flags          = 0;
	uint32_t total_number_of_pages    = 0;
	uint16_t number_of_page_values    = 0;
	uint16_t key_size                 = 0;
	uint16_t page_value_index         = 0;
	uint16_t page_value_size          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data            = NULL;
	size_t page_key_size              = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     file_io_handle,
	     page_tree->pages_cache,
	     (int) page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 page_number );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	if( page_tree->object_identifier != page->father_data_page_object_identifier )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in father data page object identifier (tree: %" PRIu32 " != page: %" PRIu32 ").",
		 function,
		 page_tree->object_identifier,
		 page->father_data_page_object_identifier );

		return( -1 );
	}
	required_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	               | LIBESEDB_PAGE_FLAG_IS_SPACE_TREE;

	if( ( page->flags & required_flags ) != required_flags )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	supported_flags = required_flags
	                | LIBESEDB_PAGE_FLAG_IS_LEAF
	                | LIBESEDB_PAGE_FLAG_IS_PARENT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_0x0400
	                | LIBESEDB_PAGE_FLAG_0x0800
	                | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT;

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( page->previous_page_number != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported previous page number: %" PRIu32 ".",
		 function,
		 page->previous_page_number );

		return( -1 );
	}
	if( page->next_page_number != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported next page number: %" PRIu32 ".",
		 function,
		 page->next_page_number );

		return( -1 );
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " data:\n",
		 function,
		 page_value_index );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );

		libnotify_printf(
		 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );

		libnotify_printf(
		 "\n" );
	}
#endif
	page_value_data = page_value->data;
	page_value_size = page_value->size;

	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		if( page_value_size == 16 )
		{
			if( ( page_value_data[  0 ] != 0 )
			 || ( page_value_data[  1 ] != 0 )
			 || ( page_value_data[  2 ] != 0 )
			 || ( page_value_data[  3 ] != 0 )
			 || ( page_value_data[  4 ] != 0 )
			 || ( page_value_data[  5 ] != 0 )
			 || ( page_value_data[  6 ] != 0 )
			 || ( page_value_data[  7 ] != 0 )
			 || ( page_value_data[  8 ] != 0 )
			 || ( page_value_data[  9 ] != 0 )
			 || ( page_value_data[ 10 ] != 0 )
			 || ( page_value_data[ 11 ] != 0 )
			 || ( page_value_data[ 12 ] != 0 )
			 || ( page_value_data[ 13 ] != 0 )
			 || ( page_value_data[ 14 ] != 0 )
			 || ( page_value_data[ 15 ] != 0 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported header.",
				 function );

				return( -1 );
			}
		}
		else if( page_value_size != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported header size.",
			 function );

			return( -1 );
		}
	}
	/* TODO handle the space tree page header */

	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " data:\n",
			 function,
			 page_value_index );
			libnotify_print_data(
			 page_value->data,
			 (size_t) page_value->size );

			libnotify_printf(
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );

			libnotify_printf(
			 "\n" );
		}
#endif
		page_value_data = page_value->data;
		page_value_size = page_value->size;

		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
			if( ( page_value->flags & 0x05 ) != 0 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page value flags: 0x%02" PRIx8 ".",
				 function,
				 page_value->flags );

				return( -1 );
			}
			if( (size_t) page_value_size != sizeof( esedb_space_tree_page_entry_t ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page value size: %" PRIu16 ".",
				 function,
				 page_value_size );

				return( -1 );
			}
			/* TODO handle the space tree page values */

			byte_stream_copy_to_uint16_little_endian(
			 page_value_data,
			 key_size );

			page_value_data += 2;
			page_value_size -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: page value: %03" PRIu16 " key size\t\t: %" PRIu16 "\n",
				 function,
				 page_value_index,
				 key_size );
			}
#endif
			if( key_size != 4 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported key size: %" PRIu16 ".",
				 function,
				 key_size);

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: page value: %03" PRIu16 " key value\t\t: ",
				 function,
				 page_value_index );

				page_key_data = page_value_data;
				page_key_size = key_size;

				while( page_key_size > 0 )
				{
					libnotify_printf(
					 "%02" PRIx8 " ",
					 *page_key_data );

					page_key_data++;
					page_key_size--;
				}
				libnotify_printf(
				 "\n" );
			}
#endif
			page_value_data += key_size;
			page_value_size -= key_size;

			byte_stream_copy_to_uint32_little_endian(
			 page_value_data,
			 number_of_pages );

			page_value_data += 4;
			page_value_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: page value: %03" PRIu16 " number of pages\t: %" PRIu32 "\n",
				 function,
				 page_value_index,
				 number_of_pages );

				libnotify_printf(
				 "\n" );

			}
#endif
			if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) == 0 )
			{
				total_number_of_pages += number_of_pages;
			}
		}
		else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_PARENT ) != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%s: data:\n",
				 function );
				libnotify_print_data(
				 page_value_data,
				 (size_t) page_value_size );
			}
#endif
#ifdef TODO
			/* TODO handle parent space tree pages */
			if( libesedb_page_tree_read_space_tree_page(
			     page_tree,
			     file_io_handle,
			     space_tree_page_number,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read space tree page: %" PRIu32 ".",
				 function,
				 space_tree_page_number );

				return( -1 );
			}
#endif
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: total number of pages\t\t\t: %" PRIu32 "\n",
		 function,
		 total_number_of_pages );

		libnotify_printf(
		 "\n" );
	}
#endif
	return( 1 );
}

/* Reads a page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_page(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     off64_t page_offset,
     uint32_t page_number,
     libfdata_tree_node_t *value_tree_node,
     libesedb_values_tree_value_t *values_tree_value,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_page";
	off64_t node_data_offset          = 0;
	uint32_t supported_flags          = 0;
	uint16_t number_of_page_values    = 0;
	uint16_t page_value_index         = 0;
	int sub_node_index                = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data            = NULL;
	size_t page_key_size              = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid values tree value.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     page_tree->pages_vector,
	     file_io_handle,
	     page_tree->pages_cache,
	     page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset %" PRIi64 ".",
		 function,
		 page_number,
		 page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	supported_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	                | LIBESEDB_PAGE_FLAG_IS_LEAF
	                | LIBESEDB_PAGE_FLAG_IS_PARENT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_0x0400
	                | LIBESEDB_PAGE_FLAG_0x0800
	                | LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT;

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
#ifdef TODO
		/* TODO refactor */
			if( libnotify_verbose != 0 )
			{
				if( page_value_index > 1 )
				{
					if( child_page->page_number != previous_next_child_page_number )
					{
						libnotify_printf(
						 "%s: mismatch in child page number (%" PRIu32 " != %" PRIu32 ").\n",
						 function,
						 previous_next_child_page_number,
						 child_page->page_number );
					}
					if( child_page->previous_page_number != previous_child_page_number )
					{
						libnotify_printf(
						 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").\n",
						 function,
						 previous_child_page_number,
						 child_page->previous_page_number );
					}
				}
/* TODO need the actual values for the following checks
				if( page_value_index == 1 )
				{
					if( child_page->previous_page_number != 0 )
					{
						libnotify_printf(
						 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").",
						 function,
						 0,
						 child_page->previous_page_number );
					}
				}
				if( page_value_index == ( number_of_page_values - 1 ) )
				{
					if( child_page->next_page_number != 0 )
					{
						libnotify_printf(
						 "%s: mismatch in next child page number (%" PRIu32 " != %" PRIu32 ").",
						 function,
						 0,
						 child_page->previous_page_number );
					}
				}
*/
			}
			previous_child_page_number      = child_page->page_number;
			previous_next_child_page_number = child_page->next_page_number;
#endif

	}
	else
	{
		if( page->previous_page_number != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported previous page number: %" PRIu32 ".",
			 function,
			 page->previous_page_number );

			return( -1 );
		}
		if( page->next_page_number != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported next page number: %" PRIu32 ".",
			 function,
			 page->next_page_number );

			return( -1 );
		}
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " value:\n",
		 function,
		 page_value_index );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );

		libnotify_printf(
		 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );
		libnotify_printf(
		 "\n" );
	}
#endif
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "\n" );
		}
#endif
		/* TODO */
	}
#if defined( HAVE_DEBUG_OUTPUT )
	else if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " common page key\t\t: ",
		 function,
		 page_value_index );

		page_key_data = page_value->data;
		page_key_size = page_value->size;

		while( page_key_size > 0 )
		{
			libnotify_printf(
			 "%02" PRIx8 " ",
			 *page_key_data );

			page_key_data++;
			page_key_size--;
		}
		libnotify_printf(
		 "\n" );
		libnotify_printf(
		 "\n" );
	}
#endif
	for( page_value_index = 1;
	     page_value_index < number_of_page_values;
	     page_value_index++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_index,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			return( -1 );
		}
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) == 0 )
		{
			node_data_offset = page_offset + page_value->offset;

			if( libfdata_tree_node_append_sub_node(
			     value_tree_node,
			     &sub_node_index,
			     node_data_offset,
			     (size64_t) page_value_index,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append page: %" PRIu32 " value: %" PRIu16 " as sub node.",
				 function,
				 page_number,
				 page_value_index );

				return( -1 );
			}
		}
		/* TODO are defunct data definition of any value recovering
		 */
	}
	return( 1 );
}

/* Reads a page value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_page_value(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     off64_t page_offset,
     uint32_t page_number,
     uint16_t page_value_index,
     libfdata_tree_node_t *value_tree_node,
     libesedb_values_tree_value_t *values_tree_value,
     liberror_error_t **error )
{
	libesedb_page_value_t *header_page_value = NULL;
	libesedb_page_t *page                    = NULL;
	libesedb_page_value_t *page_value        = NULL;
	uint8_t *page_value_data                 = NULL;
	static char *function                    = "libesedb_page_tree_read_page_value";
	off64_t sub_nodes_offset                 = 0;
	uint32_t child_page_number               = 0;
	uint16_t common_key_size                 = 0;
	uint16_t local_key_size                  = 0;
	uint16_t number_of_page_values           = 0;
	uint16_t page_value_offset               = 0;
	uint16_t page_value_size                 = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                   = NULL;
	size_t page_key_size                     = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree values.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     page_tree->pages_vector,
	     file_io_handle,
	     page_tree->pages_cache,
	     page_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
		 function,
		 page_number,
		 page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
		if( libesedb_page_tree_read_root_page(
		     page_tree,
		     file_io_handle,
		     page_offset,
		     page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read root page: %" PRIu32 ".",
			 function,
			 page_number );

			return( -1 );
		}
		if( libfdata_vector_get_element_value_at_offset(
		     page_tree->pages_vector,
		     file_io_handle,
		     page_tree->pages_cache,
		     page_offset,
		     (intptr_t **) &page,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page: %" PRIu32 " at offset: %" PRIi64 ".",
			 function,
			 page_number,
			 page_offset );

			return( -1 );
		}
		if( page == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page.",
			 function );

			return( -1 );
		}
	}
#endif
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		return( -1 );
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_index,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " data:\n",
		 function,
		 page_value_index );
		libnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size );

		libnotify_printf(
		 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );

		libnotify_printf(
		 "\n" );
	}
#endif
	page_value_data = page_value->data;
	page_value_size = page_value->size;

	if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
	{
		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported page flags - root flag is set.",
			 function );

			return( -1 );
		}
		if( libesedb_page_get_value(
		     page,
		     0,
		     &header_page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: 0.",
			 function );

			return( -1 );
		}
		if( header_page_value == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page value: 0.",
			 function );

			return( -1 );
		}
		byte_stream_copy_to_uint16_little_endian(
		 page_value_data,
		 common_key_size );

		page_value_data   += 2;
		page_value_offset += 2;
		page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " common key size\t\t: %" PRIu16 "\n",
			 function,
			 page_value_index,
			 common_key_size );
		}
#endif
		if( common_key_size > header_page_value->size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: common key size exceeds header page value size.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " common key value\t\t: ",
			 function,
			 page_value_index );

			page_key_data = header_page_value->data;
			page_key_size = common_key_size;

			while( page_key_size > 0 )
			{
				if( libnotify_verbose != 0 )
				{
					libnotify_printf(
					 "%02" PRIx8 " ",
					 *page_key_data );
				}
				page_key_data++;
				page_key_size--;
			}
			libnotify_printf(
			 "\n" );
		}
#endif
		if( libesedb_values_tree_value_set_key_common(
		     values_tree_value,
		     header_page_value->data,
		     common_key_size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set common key in page tree values.",
			 function );

			return( -1 );
		}
	}
	byte_stream_copy_to_uint16_little_endian(
	 page_value_data,
	 local_key_size );

	page_value_data   += 2;
	page_value_offset += 2;
	page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " local key size\t\t: %" PRIu16 "\n",
		 function,
		 page_value_index,
		 local_key_size );
	}
#endif
	if( local_key_size > page_value_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: local key size exceeds page value size.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " local key value\t\t: ",
		 function,
		 page_value_index );

		page_key_data = page_value_data;
		page_key_size = local_key_size;

		while( page_key_size > 0 )
		{
			if( libnotify_verbose != 0 )
			{
				libnotify_printf(
				 "%02" PRIx8 " ",
				 *page_key_data );
			}
			page_key_data++;
			page_key_size--;
		}
		libnotify_printf(
		 "\n" );
	}
#endif
	if( libesedb_values_tree_value_set_key_local(
	     values_tree_value,
	     page_value_data,
	     local_key_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set local key in page tree values.",
		 function );

		return( -1 );
	}
	page_value_data   += local_key_size;
	page_value_offset += local_key_size;
	page_value_size   -= local_key_size;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "%s: page value: %03" PRIu16 " key value\t\t\t: ",
		 function,
		 page_value_index );

		page_key_data = values_tree_value->key;
		page_key_size = values_tree_value->key_size;

		while( page_key_size > 0 )
		{
			libnotify_printf(
			 "%02" PRIx8 " ",
			 *page_key_data );

			page_key_data++;
			page_key_size--;
		}
		libnotify_printf(
		 "\n" );
	}
#endif
	values_tree_value->data_offset = page_value_offset;
	values_tree_value->data_size   = page_value_size;

	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_INDEX ) != 0 )
		{
			values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_INDEX;
		}
		else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LONG_VALUE ) != 0 )
		{
			values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_LONG_VALUE;
		}
		else
		{
			values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_RECORD;
		}
		if( libfdata_tree_node_set_leaf(
		     value_tree_node,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to make value tree node a leaf.",
			 function );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "\n" );
		}
#endif
	}
	else
	{
		byte_stream_copy_to_uint32_little_endian(
		 page_value_data,
		 child_page_number );

		page_value_data += 4;
		page_value_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " child page number\t\t: %" PRIu32 "",
			 function,
			 page_value_index,
			 child_page_number );

			if( child_page_number == 0 )
			{
				libnotify_printf(
				 " (invalid page number)\n" );
			}
			else if( child_page_number > page_tree->io_handle->last_page_number )
			{
				libnotify_printf(
				 " (exceeds last page number: %" PRIu32 ")\n",
				 page_tree->io_handle->last_page_number );
			}
			libnotify_printf(
			 "\n" );
			libnotify_printf(
			 "\n" );
		}
#endif
		if( ( child_page_number > 0 )
		 && ( child_page_number <= page_tree->io_handle->last_page_number ) )
		{
			sub_nodes_offset  = child_page_number - 1;
			sub_nodes_offset *= page_tree->io_handle->page_size;

			if( libfdata_tree_node_set_sub_nodes_range(
			     value_tree_node,
			     sub_nodes_offset,
			     0,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set child page number: %" PRIu32 " as sub nodes range.",
				 function,
				 child_page_number );

				return( -1 );
			}
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libnotify_verbose != 0 )
		 && ( page_value_size > 0 ) )
		{
			libnotify_printf(
			 "%s: page value: %03" PRIu16 " trailing data:\n",
			 function,
			 page_value_index );
			libnotify_print_data(
			 page_value_data,
			 page_value_size );
		}
#endif
		values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_NODE;
	}
	return( 1 );
}

/* Reads a page tree node value
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_node_value(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfdata_cache_t *cache,
     off64_t node_data_offset,
     size64_t node_data_size,
     uint8_t read_flags LIBESEDB_ATTRIBUTE_UNUSED,
     liberror_error_t **error )
{
	libesedb_page_tree_t *page_tree                 = NULL;
	libesedb_values_tree_value_t *values_tree_value = NULL;
	static char *function                           = "libesedb_page_tree_read_node_value";
	uint64_t page_number                            = 0;
	int result                                      = 0;

	LIBESEDB_UNREFERENCED_PARAMETER( read_flags );

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree IO handle.",
		 function );

		return( -1 );
	}
	page_tree = (libesedb_page_tree_t *) io_handle;

	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle->page_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - invalid IO handle - missing page size.",
		 function );

		return( -1 );
	}
	if( node_data_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( node_data_size > (off64_t) UINT16_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	page_number = (uint64_t)( ( node_data_offset / page_tree->io_handle->page_size ) + 1 );

	if( page_number > (uint64_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid page number value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libesedb_values_tree_value_initialize(
	     &values_tree_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create values tree value.",
		 function );

		return( -1 );
	}
	values_tree_value->page_offset      = node_data_offset;
	values_tree_value->page_number      = (uint32_t) page_number;
	values_tree_value->page_value_index = (uint16_t) node_data_size;

	result = libfdata_tree_node_is_root(
	          node,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if node is the root.",
		 function );

		libesedb_values_tree_value_free(
		 (intptr_t *) values_tree_value,
		NULL );

		return( -1 );
	}
	else if( result != 0 )
	{
		/* The values tree root node is virtual
		 */
		if( libfdata_tree_node_set_sub_nodes_range(
		     node,
		     node_data_offset,
		     0,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set page: %" PRIu64 " as sub node range.",
			 function,
			 page_number );

			libesedb_values_tree_value_free(
			 (intptr_t *) values_tree_value,
			NULL );

			return( -1 );
		}
		values_tree_value->type = LIBESEDB_VALUES_TREE_VALUE_TYPE_NODE;
	}
	else
	{
		/* node_data_size contains the page value index
		 */
		if( libesedb_page_tree_read_page_value(
		     page_tree,
		     file_io_handle,
		     node_data_offset,
		     (uint32_t) page_number,
		     (uint16_t) node_data_size,
		     node,
		     values_tree_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page: %" PRIu64 " value: %" PRIu64 ".",
			 function,
			 page_number,
			 node_data_size );

			libesedb_values_tree_value_free(
			 (intptr_t *) values_tree_value,
			NULL );

			return( -1 );
		}
	}
	if( libfdata_tree_node_set_node_value(
	     node,
	     cache,
	     (intptr_t *) values_tree_value,
	     &libesedb_values_tree_value_free,
	     LIBFDATA_TREE_NODE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set values tree value as node value.",
		 function );

		libesedb_values_tree_value_free(
		 (intptr_t *) values_tree_value,
		NULL );

		return( -1 );
	}
	return( 1 );
}

/* Reads a page tree sub nodes
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_sub_nodes(
     intptr_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfdata_cache_t *cache,
     off64_t sub_nodes_offset,
     size64_t sub_nodes_size LIBESEDB_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBESEDB_ATTRIBUTE_UNUSED,
     liberror_error_t **error )
{
	libesedb_page_tree_t *page_tree                 = NULL;
	libesedb_values_tree_value_t *values_tree_value = NULL;
	static char *function                           = "libesedb_page_tree_read_sub_nodes";
	uint64_t page_number                            = 0;

	LIBESEDB_UNREFERENCED_PARAMETER( sub_nodes_size );
	LIBESEDB_UNREFERENCED_PARAMETER( read_flags );

	if( io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree IO handle.",
		 function );

		return( -1 );
	}
	page_tree = (libesedb_page_tree_t *) io_handle;

	if( page_tree->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle->page_size == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - invalid IO handle - missing page size.",
		 function );

		return( -1 );
	}
	if( sub_nodes_offset < 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid sub nodes offset value less than zero.",
		 function );

		return( -1 );
	}
	page_number = (uint64_t) ( ( sub_nodes_offset / page_tree->io_handle->page_size ) + 1 );

	if( page_number > (uint64_t) UINT32_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid page number value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfdata_tree_node_get_node_value(
	     node,
	     file_io_handle,
	     cache,
	     (intptr_t **) &values_tree_value,
	     0,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve node value.",
		 function );

		return( -1 );
	}
	if( values_tree_value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing values tree value.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_read_page(
	     page_tree,
	     file_io_handle,
	     sub_nodes_offset,
	     (uint32_t) page_number,
	     node,
	     values_tree_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page: %" PRIu64 " at offset: %" PRIi64 ".",
		 function,
		 page_number,
		 sub_nodes_offset );

		return( -1 );
	}
	return( 1 );
}

