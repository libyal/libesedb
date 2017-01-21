/*
 * Page tree functions
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

#include "libesedb_data_definition.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_key.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"
#include "libesedb_unused.h"

#include "esedb_page_values.h"

/* Creates a page tree
 * Make sure the value page_tree is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_initialize(
     libesedb_page_tree_t **page_tree,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     uint32_t object_identifier,
     libesedb_table_definition_t *table_definition,
     libesedb_table_definition_t *template_table_definition,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_initialize";

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( *page_tree != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page tree value already set.",
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
	*page_tree = memory_allocate_structure(
	              libesedb_page_tree_t );

	if( *page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page tree.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *page_tree,
	     0,
	     sizeof( libesedb_page_tree_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear page tree.",
		 function );

		goto on_error;
	}
	( *page_tree )->io_handle                 = io_handle;
	( *page_tree )->pages_vector              = pages_vector;
	( *page_tree )->pages_cache               = pages_cache;
	( *page_tree )->object_identifier         = object_identifier;
	( *page_tree )->table_definition          = table_definition;
	( *page_tree )->template_table_definition = template_table_definition;

	return( 1 );

on_error:
	if( *page_tree != NULL )
	{
		memory_free(
		 *page_tree );

		*page_tree = NULL;
	}
	return( -1 );
}

/* Frees a page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_free(
     libesedb_page_tree_t **page_tree,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_free";
	int result            = 1;

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( *page_tree != NULL )
	{
		/* The io_handle, pages_vector, pages_cache, table_definition and template_table_definition references
		 * are freed elsewhere
		 */
		memory_free(
		 *page_tree );

		*page_tree = NULL;
	}
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
     libcerror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_root_page";
	off64_t element_data_offset       = 0;
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     page_tree->pages_cache,
	     page_offset,
	     &element_data_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: 0x%08" PRIx64 ".",
		 function,
		 page_number,
		 page_offset );

		return( -1 );
	}
	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
	required_flags = LIBESEDB_PAGE_FLAG_IS_ROOT;

	if( ( page->flags & required_flags ) != required_flags )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) != 0 )
	{
		return( 1 );
	}
	supported_flags = required_flags
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

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: 0.",
		 function );

		return( -1 );
	}
	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page value.",
		 function );

		return( -1 );
	}
	if( page_value->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page value - missing data.",
		 function );

		return( -1 );
	}
	if( page_value->size != 16 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported size of page value.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_root_page_header_t *) page_value->data )->space_tree_page_number,
	 space_tree_page_number );

/* TODO handle the root page header */

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->initial_number_of_pages,
		 initial_number_of_pages );
		libcnotify_printf(
		 "%s: (header) initial number of pages\t: %" PRIu32 "\n",
		 function,
		 initial_number_of_pages );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->parent_father_data_page_number,
		 value_32bit );
		libcnotify_printf(
		 "%s: (header) parent FDP number\t\t: %" PRIu32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_root_page_header_t *) page_value->data )->extent_space,
		 extent_space );
		libcnotify_printf(
		 "%s: (header) extent space\t\t: %" PRIu32 "\n",
		 function,
		 extent_space );

		libcnotify_printf(
		 "%s: (header) space tree page number\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 space_tree_page_number,
		 space_tree_page_number );

		libcnotify_printf(
		 "%s: (header) primary extent\t\t: %" PRIu32 "-%c\n",
		 function,
		 initial_number_of_pages,
		 ( extent_space == 0 ? 's' : 'm' ) );

		libcnotify_printf(
		 "\n" );
	}
#endif
	/* Read the space tree pages
	 */
	if( extent_space > 0 )
	{
		if( space_tree_page_number >= 0xff000000UL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
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
     libcerror_error_t **error )
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_by_index(
	     page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     page_tree->pages_cache,
	     (int) page_number - 1,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 ".",
		 function,
		 page_number );

		return( -1 );
	}
	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( page_tree->object_identifier != page->father_data_page_object_identifier )
		{
			libcnotify_printf(
			 "%s: mismatch in father data page object identifier (tree: %" PRIu32 " != page: %" PRIu32 ").",
			 function,
			 page_tree->object_identifier,
			 page->father_data_page_object_identifier );
		}
	}
#endif
	required_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	               | LIBESEDB_PAGE_FLAG_IS_SPACE_TREE;

	if( ( page->flags & required_flags ) != required_flags )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing required page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) != 0 )
	{
		return( 1 );
	}
	supported_flags = required_flags
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

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		return( -1 );
	}
	if( page->previous_page_number != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported previous page number: %" PRIu32 ".",
		 function,
		 page->previous_page_number );

		return( -1 );
	}
	if( page->next_page_number != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
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
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_index );

		return( -1 );
	}
	if( page_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page value.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: page value: %03" PRIu16 " data:\n",
		 function,
		 page_value_index );
		libcnotify_print_data(
		 page_value->data,
		 (size_t) page_value->size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

		libcnotify_printf(
		 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
		 function,
		 page_value_index,
		 page_value->flags );
		libesedb_debug_print_page_tag_flags(
		 page_value->flags );

		libcnotify_printf(
		 "\n" );
	}
#endif
	page_value_data = page_value->data;
	page_value_size = page_value->size;

	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
		if( page_value_size != 16 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " data:\n",
			 function,
			 page_value_index );
			libcnotify_print_data(
			 page_value->data,
			 (size_t) page_value->size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );

			libcnotify_printf(
			 "\n" );
		}
#endif
		page_value_data = page_value->data;
		page_value_size = page_value->size;

		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
			if( ( page_value->flags & 0x05 ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page value flags: 0x%02" PRIx8 ".",
				 function,
				 page_value->flags );

				return( -1 );
			}
			if( page_value_size < 2 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: unsupported key size value out of bounds.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint16_little_endian(
			 page_value_data,
			 key_size );

			page_value_data += 2;
			page_value_size -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " key size\t\t: %" PRIu16 "\n",
				 function,
				 page_value_index,
				 key_size );
			}
#endif
			if( key_size > page_value_size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: unsupported key size value out of bounds.",
				 function );

				return( -1 );
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " key value\t\t: ",
				 function,
				 page_value_index );

				page_key_data = page_value_data;
				page_key_size = key_size;

				while( page_key_size > 0 )
				{
					libcnotify_printf(
					 "%02" PRIx8 " ",
					 *page_key_data );

					page_key_data++;
					page_key_size--;
				}
				libcnotify_printf(
				 "\n" );
			}
#endif
			page_value_data += key_size;
			page_value_size -= key_size;

			if( page_value_size < 4 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: unsupported key size value out of bounds.",
				 function );

				return( -1 );
			}
			byte_stream_copy_to_uint32_little_endian(
			 page_value_data,
			 number_of_pages );

			page_value_data += 4;
			page_value_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " number of pages\t: %" PRIu32 "\n",
				 function,
				 page_value_index,
				 number_of_pages );

				libcnotify_printf(
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
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: data:\n",
				 function );
				libcnotify_print_data(
				 page_value_data,
				 (size_t) page_value_size,
				 0 );
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
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read space tree page: %" PRIu32 ".",
				 function,
				 space_tree_page_number );

				return( -1 );
			}
#endif
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: total number of pages\t\t\t: %" PRIu32 "\n",
		 function,
		 total_number_of_pages );

		libcnotify_printf(
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
     libfdata_btree_node_t *node,
     libcerror_error_t **error )
{
	libesedb_key_t *key                      = NULL;
	libesedb_page_value_t *header_page_value = NULL;
	libesedb_page_t *page                    = NULL;
	libesedb_page_value_t *page_value        = NULL;
	uint8_t *page_value_data                 = NULL;
	static char *function                    = "libesedb_page_tree_read_page";
	off64_t element_data_offset              = 0;
	off64_t sub_node_data_offset             = 0;
	uint32_t child_page_number               = 0;
	uint32_t supported_flags                 = 0;
	uint16_t common_key_size                 = 0;
	uint16_t local_key_size                  = 0;
	uint16_t number_of_page_values           = 0;
	uint16_t page_value_index                = 0;
	uint16_t page_value_offset               = 0;
	uint16_t page_value_size                 = 0;
	int element_index                        = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_key_data                   = NULL;
	size_t page_key_size                     = 0;
#endif

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libfdata_vector_get_element_value_at_offset(
	     page_tree->pages_vector,
	     (intptr_t *) file_io_handle,
	     page_tree->pages_cache,
	     page_offset,
	     &element_data_offset,
	     (intptr_t **) &page,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page: %" PRIu32 " at offset: 0x%08" PRIx64 ".",
		 function,
		 page_number,
		 page_offset );

		goto on_error;
	}
	if( page == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing page.",
		 function );

		goto on_error;
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) != 0 )
	{
		return( 1 );
	}
	supported_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
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

	if( ( page->flags & ~supported_flags ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported page flags: 0x%08" PRIx32 ".",
		 function,
		 page->flags );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
	{
/* TODO uncache the root page?
 */
		if( libesedb_page_tree_read_root_page(
		     page_tree,
		     file_io_handle,
		     page_offset,
		     page_number,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read root page: %" PRIu32 ".",
			 function,
			 page_number );

			goto on_error;
		}
		/* Since the previous function reads the space tree
		 * page can be cached out and we must be sure to re-read it.
		 */
		if( libfdata_vector_get_element_value_at_offset(
		     page_tree->pages_vector,
		     (intptr_t *) file_io_handle,
		     page_tree->pages_cache,
		     page_offset,
		     &element_data_offset,
		     (intptr_t **) &page,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page: %" PRIu32 " at offset: 0x%08" PRIx64 ".",
			 function,
			 page_number,
			 page_offset );

			goto on_error;
		}
		if( page == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page.",
			 function );

			goto on_error;
		}
	}
#endif
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
	{
#ifdef TODO
/* TODO refactor */
			if( libcnotify_verbose != 0 )
			{
				if( page_value_index > 1 )
				{
					if( child_page->page_number != previous_next_child_page_number )
					{
						libcnotify_printf(
						 "%s: mismatch in child page number (%" PRIu32 " != %" PRIu32 ").\n",
						 function,
						 previous_next_child_page_number,
						 child_page->page_number );
					}
					if( child_page->previous_page_number != previous_child_page_number )
					{
						libcnotify_printf(
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
						libcnotify_printf(
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
						libcnotify_printf(
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported previous page number: %" PRIu32 ".",
			 function,
			 page->previous_page_number );

			goto on_error;
		}
		if( page->next_page_number != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported next page number: %" PRIu32 ".",
			 function,
			 page->next_page_number );

			goto on_error;
		}
	}
	if( libesedb_page_get_number_of_values(
	     page,
	     &number_of_page_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of page values.",
		 function );

		goto on_error;
	}
	if( number_of_page_values == 0 )
	{
		return( 1 );
	}
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
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
		if( page_value == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing page value: %" PRIu16 ".",
			 function,
			 page_value_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " data:\n",
			 function,
			 page_value_index );
			libcnotify_print_data(
			 page_value->data,
			 (size_t) page_value->size,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );

			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " page tag flags\t\t: 0x%02" PRIx8 "",
			 function,
			 page_value_index,
			 page_value->flags );
			libesedb_debug_print_page_tag_flags(
			 page_value->flags );

			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_IS_DEFUNCT ) != 0 )
		{
			continue;
		}
/* TODO are defunct data definition of any value recovering
 */
		page_value_data   = page_value->data;
		page_value_offset = page_value->offset;
		page_value_size   = page_value->size;

		if( libesedb_key_initialize(
		     &key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key.",
			 function );

			goto on_error;
		}
		if( key == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing key.",
			 function );

			goto on_error;
		}
		if( ( page_value->flags & LIBESEDB_PAGE_TAG_FLAG_HAS_COMMON_KEY_SIZE ) != 0 )
		{
			if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page flags - root flag is set.",
				 function );

				goto on_error;
			}
			if( libesedb_page_get_value(
			     page,
			     0,
			     &header_page_value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve page value: 0.",
				 function );

				goto on_error;
			}
			if( header_page_value == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: missing page value: 0.",
				 function );

				goto on_error;
			}
			byte_stream_copy_to_uint16_little_endian(
			 page_value_data,
			 common_key_size );

			page_value_data   += 2;
			page_value_offset += 2;
			page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " common key size\t\t: %" PRIu16 "\n",
				 function,
				 page_value_index,
				 common_key_size );
			}
#endif
			if( common_key_size > header_page_value->size )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: common key size exceeds header page value size.",
				 function );

				goto on_error;
			}
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " common key value\t\t: ",
				 function,
				 page_value_index );

				page_key_data = header_page_value->data;
				page_key_size = common_key_size;

				while( page_key_size > 0 )
				{
					if( libcnotify_verbose != 0 )
					{
						libcnotify_printf(
						 "%02" PRIx8 " ",
						 *page_key_data );
					}
					page_key_data++;
					page_key_size--;
				}
				libcnotify_printf(
				 "\n" );
			}
#endif
			if( libesedb_key_set_data(
			     key,
			     header_page_value->data,
			     common_key_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set common key data in key.",
				 function );

				goto on_error;
			}
		}
		byte_stream_copy_to_uint16_little_endian(
		 page_value_data,
		 local_key_size );

		page_value_data   += 2;
		page_value_offset += 2;
		page_value_size   -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " local key size\t\t: %" PRIu16 "\n",
			 function,
			 page_value_index,
			 local_key_size );
		}
#endif
		if( local_key_size > page_value_size )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: local key size exceeds page value size.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " local key value\t\t: ",
			 function,
			 page_value_index );

			page_key_data = page_value_data;
			page_key_size = local_key_size;

			while( page_key_size > 0 )
			{
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%02" PRIx8 " ",
					 *page_key_data );
				}
				page_key_data++;
				page_key_size--;
			}
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( libesedb_key_append_data(
		     key,
		     page_value_data,
		     local_key_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to append local key data to key.",
			 function );

			goto on_error;
		}
		page_value_data   += local_key_size;
		page_value_offset += local_key_size;
		page_value_size   -= local_key_size;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: page value: %03" PRIu16 " key value\t\t\t: ",
			 function,
			 page_value_index );

			page_key_data = key->data;
			page_key_size = key->data_size;

			while( page_key_size > 0 )
			{
				libcnotify_printf(
				 "%02" PRIx8 " ",
				 *page_key_data );

				page_key_data++;
				page_key_size--;
			}
			libcnotify_printf(
			 "\n" );
		}
#endif
		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) != 0 )
		{
#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "\n" );
			}
#endif
			key->type = LIBESEDB_KEY_TYPE_LEAF;

			if( libfdata_btree_node_append_leaf_value(
			     node,
			     &element_index,
			     (int) page_value_index,
			     page_offset + page_value_offset,
			     (size64_t) page_value_size,
			     0,
			     (intptr_t *) key,
			     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_key_free,
			     LIBFDATA_KEY_VALUE_FLAG_MANAGED,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append page: %" PRIu32 " value: %" PRIu16 " as leaf value.",
				 function,
				 page_number,
				 page_value_index );

				goto on_error;
			}
			key = NULL;
		}
		else
		{
			byte_stream_copy_to_uint32_little_endian(
			 page_value_data,
			 child_page_number );

			page_value_data += 4;
			page_value_size -= 4;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " child page number\t\t: %" PRIu32 "",
				 function,
				 page_value_index,
				 child_page_number );

				if( child_page_number == 0 )
				{
					libcnotify_printf(
					 " (invalid page number)\n" );
				}
				else if( child_page_number > page_tree->io_handle->last_page_number )
				{
					libcnotify_printf(
					 " (exceeds last page number: %" PRIu32 ")\n",
					 page_tree->io_handle->last_page_number );
				}
				libcnotify_printf(
				 "\n" );
				libcnotify_printf(
				 "\n" );
			}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
			if( ( libcnotify_verbose != 0 )
			 && ( page_value_size > 0 ) )
			{
				libcnotify_printf(
				 "%s: page value: %03" PRIu16 " trailing data:\n",
				 function,
				 page_value_index );
				libcnotify_print_data(
				 page_value_data,
				 page_value_size,
				 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
			}
#endif
			if( ( child_page_number > 0 )
			 && ( child_page_number <= page_tree->io_handle->last_page_number ) )
			{
				sub_node_data_offset  = child_page_number - 1;
				sub_node_data_offset *= page_tree->io_handle->page_size;

				key->type = LIBESEDB_KEY_TYPE_BRANCH;

				if( libfdata_btree_node_append_sub_node(
				     node,
				     &element_index,
				     0,
				     sub_node_data_offset,
				     (size64_t) page_tree->io_handle->page_size,
				     0,
				     (intptr_t *) key,
				     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_key_free,
				     LIBFDATA_KEY_VALUE_FLAG_MANAGED,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
					 "%s: unable to append page: %" PRIu32 " value: %" PRIu16 " as sub node.",
					 function,
					 page_number,
					 page_value_index );

					goto on_error;
				}
				key = NULL;
			}
		}
		if( key != NULL )
		{
			if( libesedb_key_free(
			     &key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key.",
				 function );

				goto on_error;
			}
		}
	}
	return( 1 );

on_error:
	if( key != NULL )
	{
		libesedb_key_free(
		 &key,
		 NULL );
	}
	return( -1 );
}

/* Reads a page tree node
 * Callback function for the page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_node(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libfdata_btree_node_t *node,
     int node_data_file_index LIBESEDB_ATTRIBUTE_UNUSED,
     off64_t node_data_offset,
     size64_t node_data_size,
     uint32_t node_data_flags LIBESEDB_ATTRIBUTE_UNUSED,
     intptr_t *key_value LIBESEDB_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBESEDB_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_tree_read_node";
	uint64_t page_number  = 0;

	LIBESEDB_UNREFERENCED_PARAMETER( node_data_file_index );
	LIBESEDB_UNREFERENCED_PARAMETER( node_data_flags );
	LIBESEDB_UNREFERENCED_PARAMETER( key_value );
	LIBESEDB_UNREFERENCED_PARAMETER( read_flags );

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle->page_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - invalid IO handle - missing page size.",
		 function );

		return( -1 );
	}
	if( node_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( node_data_size > (off64_t) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid node data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	page_number = (uint64_t)( ( node_data_offset / page_tree->io_handle->page_size ) + 1 );

	if( page_number > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid page number value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libesedb_page_tree_read_page(
	     page_tree,
	     file_io_handle,
	     node_data_offset,
	     (uint32_t) page_number,
	     node,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page: %" PRIu64 " at offset: 0x%08" PRIx64 ".",
		 function,
		 page_number,
		 node_data_offset );

		return( -1 );
	}
	return( 1 );
}

/* Reads a page tree leaf value
 * Callback function for the page tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_leaf_value(
     libesedb_page_tree_t *page_tree,
     libbfio_handle_t *file_io_handle,
     libfdata_btree_t *tree,
     libfcache_cache_t *cache,
     int leaf_value_index,
     int leaf_value_data_file_index,
     off64_t leaf_value_data_offset,
     size64_t leaf_value_data_size,
     uint32_t leaf_value_data_flags LIBESEDB_ATTRIBUTE_UNUSED,
     intptr_t *key_value,
     uint8_t read_flags LIBESEDB_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libesedb_data_definition_t *data_definition = NULL;
	static char *function                       = "libesedb_page_tree_read_leaf_value";
	off64_t page_offset                         = 0;
	uint64_t page_number                        = 0;

	LIBESEDB_UNREFERENCED_PARAMETER( leaf_value_data_flags );
	LIBESEDB_UNREFERENCED_PARAMETER( read_flags );

	if( page_tree == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page tree.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - missing IO handle.",
		 function );

		return( -1 );
	}
	if( page_tree->io_handle->page_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page tree - invalid IO handle - missing page size.",
		 function );

		return( -1 );
	}
	if( leaf_value_data_file_index < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid leaf value data file index value less than zero.",
		 function );

		return( -1 );
	}
	if( leaf_value_data_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid leaf value data offset value less than zero.",
		 function );

		return( -1 );
	}
	if( leaf_value_data_size > (off64_t) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid leaf value data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	page_offset  = leaf_value_data_offset / page_tree->io_handle->page_size;
	page_number  = (uint64_t) ( page_offset + 1 );
	page_offset *= page_tree->io_handle->page_size;

	if( page_number > (uint64_t) UINT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid page number value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libesedb_data_definition_initialize(
	     &data_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data definition.",
		 function );

		goto on_error;
	}
	if( data_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data definition.",
		 function );

		return( -1 );
	}
	/* leaf_value_data_file_index contains the page value index
	 */
	data_definition->page_value_index = (uint16_t) leaf_value_data_file_index;
	data_definition->page_offset      = page_offset;
	data_definition->page_number      = (uint32_t) page_number;
	data_definition->data_offset      = (uint16_t) ( leaf_value_data_offset - page_offset );
	data_definition->data_size        = (uint16_t) leaf_value_data_size;

	if( libfdata_btree_set_leaf_value_by_index(
	     tree,
	     (intptr_t *) file_io_handle,
	     cache,
	     leaf_value_index,
	     (intptr_t *) data_definition,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_data_definition_free,
	     LIBFDATA_BTREE_LEAF_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data definition as leaf value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_definition != NULL )
	{
		libesedb_data_definition_free(
		 &data_definition,
		 NULL );
	}
	return( -1 );
}

