/*
 * Page tree functions
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

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_page.h"
#include "libesedb_page_tree.h"

#include "esedb_page_values.h"

/* Creates a page_tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_initialize(
     libesedb_page_tree_t **page_tree,
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
			 "%s: unable to create page_tree.",
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
			 "%s: unable to clear page_tree.",
			 function );

			memory_free(
			 *page_tree );

			*page_tree = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees page_tree
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_free(
     libesedb_page_tree_t **page_tree,
     liberror_error_t **error )
{
	static char *function = "libesedb_page_tree_free";

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
	if( *page_tree != NULL )
	{
		memory_free(
		 *page_tree );

		*page_tree = NULL;
	}
	return( 1 );
}

/* Reads a page tree and its values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read(
     libesedb_page_tree_t *page_tree,
     libesedb_io_handle_t *io_handle,
     uint32_t father_data_page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page = NULL;
	static char *function = "libesedb_page_tree_read";

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
		 "%s: invalid io handle.",
		 function );

		return( -1 );
	}
	if( libesedb_page_initialize(
	     &page,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create page.",
		 function );

		return( -1 );
	}
	if( libesedb_page_read(
	     page,
	     io_handle,
	     father_data_page_number,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page: %" PRIu32 ".",
		 function,
		 father_data_page_number );

		libesedb_page_free(
		 &page,
		 NULL );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == LIBESEDB_PAGE_FLAG_IS_LEAF )
	{
		if( libesedb_page_tree_read_leaf_page_values(
		     page_tree,
		     page,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read leaf page values.",
			 function );

			return( -1 );
		}
	}
	else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) == LIBESEDB_PAGE_FLAG_IS_ROOT )
	{
		if( libesedb_page_tree_read_father_data_page_values(
		     page_tree,
		     page,
		     io_handle,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read father data page values.",
			 function );

			return( -1 );
		}
	}
	if( libesedb_page_free(
	     &page,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free page.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the father data page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_father_data_page_values(
     libesedb_page_tree_t *page_tree,
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     liberror_error_t **error )
{
	libesedb_page_t *sub_page                = NULL;
	libesedb_page_value_t *page_value        = NULL;
	static char *function                    = "libesedb_page_tree_read_father_data_page_values";
	uint32_t child_page_number               = 0;
	uint32_t previous_child_page_number      = 0;
	uint32_t previous_next_child_page_number = 0;
	uint32_t required_flags                  = 0;
	uint32_t space_tree_page_number          = 0;
	uint32_t supported_flags                 = 0;
	uint16_t amount_of_page_values           = 0;
	uint16_t page_value_iterator             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_value_data                 = NULL;
	uint32_t extent_space                    = 0;
	uint32_t initial_amount_of_pages         = 0;
	uint16_t page_key_size                   = 0;
	uint16_t page_value_size                 = 0;
	uint32_t test                            = 0;
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
	required_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	               | LIBESEDB_PAGE_FLAG_IS_PARENT;

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
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_IS_PRIMARY
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
	if( page->number_previous != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported previous page number: %" PRIu32 ".",
		 function,
		 page->number_previous );

		return( -1 );
	}
	if( page->number_next != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported next page number: %" PRIu32 ".",
		 function,
		 page->number_next );

		return( -1 );
	}
	if( libesedb_page_get_amount_of_values(
	     page,
	     &amount_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of page values.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_iterator,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_iterator );

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
	endian_little_convert_32bit(
	 space_tree_page_number,
	 ( (esedb_father_data_page_header_t *) page_value->data )->space_tree_page_number );

	/* TODO handle the root page header */

#if defined( HAVE_DEBUG_OUTPUT )
	endian_little_convert_32bit(
	 initial_amount_of_pages,
	 ( (esedb_father_data_page_header_t *) page_value->data )->initial_amount_of_pages );
	libnotify_verbose_printf(
	 "%s: header initial amount of pages\t: %" PRIu32 "\n",
	 function,
	 initial_amount_of_pages );

	endian_little_convert_32bit(
	 test,
	 ( (esedb_father_data_page_header_t *) page_value->data )->parent_father_data_page_number );
	libnotify_verbose_printf(
	 "%s: header parent FDP number\t: %" PRIu32 "\n",
	 function,
	 test );

	endian_little_convert_32bit(
	 extent_space,
	 ( (esedb_father_data_page_header_t *) page_value->data )->extent_space );
	libnotify_verbose_printf(
	 "%s: header extent space\t\t: %" PRIu32 "\n",
	 function,
	 extent_space );

	libnotify_verbose_printf(
	 "%s: header space tree page number\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
	 function,
	 space_tree_page_number,
	 space_tree_page_number );

	libnotify_verbose_printf(
	 "%s: primary extent\t\t\t: %" PRIu32 "-%c\n",
	 function,
	 initial_amount_of_pages,
	 ( extent_space == 0 ? 's' : 'm' ) );

	libnotify_verbose_printf(
	 "\n" );
#endif

	/* Read the space tree pages
	 */
	if( ( space_tree_page_number > 0 )
	 && ( space_tree_page_number < 0xfffffff0UL ) )
	{
		/* Read the owned pages space tree page
		 */
		if( libesedb_page_initialize(
		     &sub_page,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create space tree page.",
			 function );

			return( -1 );
		}
		if( libesedb_page_read(
		     sub_page,
		     io_handle,
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

			libesedb_page_free(
			 &sub_page,
			 NULL );

			return( -1 );
		}
		if( page->father_object_identifier != sub_page->father_object_identifier )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch in father object identifier (%" PRIu32 " != %" PRIu32 ").",
			 function,
			 page->father_object_identifier,
			 sub_page->father_object_identifier );

			libesedb_page_free(
			 &sub_page,
			 NULL );

			return( -1 );
		}
		if( libesedb_page_tree_read_space_tree_page_values(
		     page_tree,
		     sub_page,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read space tree page values.",
			 function );

			libesedb_page_free(
			 &sub_page,
			 NULL );

			return( -1 );
		}
		if( libesedb_page_free(
		     &sub_page,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free space tree page.",
			 function );

			return( -1 );
		}

		/* Read the available pages space tree page
		 */
		space_tree_page_number += 1;

		if( libesedb_page_initialize(
		     &sub_page,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create space tree page.",
			 function );

			return( -1 );
		}
		if( libesedb_page_read(
		     sub_page,
		     io_handle,
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

			libesedb_page_free(
			 &sub_page,
			 NULL );

			return( -1 );
		}
		if( page->father_object_identifier != sub_page->father_object_identifier )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch in father object identifier (%" PRIu32 " != %" PRIu32 ").",
			 function,
			 page->father_object_identifier,
			 sub_page->father_object_identifier );

			libesedb_page_free(
			 &sub_page,
			 NULL );

			return( -1 );
		}
		if( libesedb_page_tree_read_space_tree_page_values(
		     page_tree,
		     sub_page,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read space tree page values.",
			 function );

			libesedb_page_free(
			 &sub_page,
			 NULL );

			return( -1 );
		}
		if( libesedb_page_free(
		     &sub_page,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free space tree page.",
			 function );

			return( -1 );
		}
	}
	/* Read the page values
	 */
	for( page_value_iterator = 1;
	     page_value_iterator < amount_of_page_values;
	     page_value_iterator++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_iterator,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_iterator );

			return( -1 );
		}
		/* TODO handle leaf page values */

		page_value_data = page_value->data;
		page_value_size = page_value->size;

		if( ( page_value->flags & 0x7 ) != 0 )
		{
			libnotify_verbose_printf(
			 "%s: PAGE TAG FLAGS MARKER: 0x%02" PRIx8 "\n",
			 function,
			 page_value->flags );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		endian_little_convert_16bit(
		 page_key_size,
		 page_value_data );

		page_value_data += 2;
		page_value_size -= 2;

		libnotify_verbose_printf(
		 "%s: value: %03d highest key size\t\t: %" PRIu16 "\n",
		 function,
		 page_value_iterator,
		 page_key_size );

		libnotify_verbose_printf(
		 "%s: value: %03d highest key value\t\t: ",
		 function,
		 page_value_iterator );

		if( page_key_size > page_value_size )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_OUT_OF_RANGE,
			 "%s: page key size exceeds page value size.",
			 function );

			return( -1 );
		}
		while( page_key_size > 0 )
		{
			if( ( ( *page_value_data >= 'A' )
			  && ( *page_value_data <= 'Z' ) )
			 || ( ( *page_value_data >= 'a' )
			  && ( *page_value_data <= 'z' ) ) )
			{
				libnotify_verbose_printf(
				 "%c",
				 (char) *page_value_data );
			}
			else
			{
				libnotify_verbose_printf(
				 "\\x%02" PRIx8 "",
				 *page_value_data );
			}
			page_value_data++;
			page_value_size--;
			page_key_size--;
		}
		libnotify_verbose_printf(
		 "\n" );

		endian_little_convert_32bit(
		 child_page_number,
		 page_value_data );

		libnotify_verbose_printf(
		 "%s: value: %03d child page number\t\t: %" PRIu32 "\n",
		 function,
		 page_value_iterator,
		 child_page_number );
		libnotify_verbose_printf(
		 "\n" );
#endif

		/* TODO can an upper bound be determined ?
		 */
		if( child_page_number < 0x117f02 )
		{
			if( libesedb_page_initialize(
			     &sub_page,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create child page.",
				 function );

				return( -1 );
			}
			if( libesedb_page_read(
			     sub_page,
			     io_handle,
			     child_page_number,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read child page: %" PRIu32 ".",
				 function,
				 child_page_number );

				libesedb_page_free(
				 &sub_page,
				 NULL );

				return( -1 );
			}
			if( page->father_object_identifier != sub_page->father_object_identifier )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: mismatch in father object identifier (%" PRIu32 " != %" PRIu32 ").",
				 function,
				 page->father_object_identifier,
				 sub_page->father_object_identifier );

				libesedb_page_free(
				 &sub_page,
				 NULL );

				return( -1 );
			}
			if( page_value_iterator > 1 )
			{
				if( sub_page->number != previous_next_child_page_number )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: mismatch in child page number (%" PRIu32 " != %" PRIu32 ").",
					 function,
					 previous_next_child_page_number,
					 sub_page->number );

					libesedb_page_free(
					 &sub_page,
					 NULL );

					return( -1 );
				}
				if( sub_page->number_previous != previous_child_page_number )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").",
					 function,
					 previous_child_page_number,
					 sub_page->number_previous );

					libesedb_page_free(
					 &sub_page,
					 NULL );

					return( -1 );
				}
			}
			if( page_value_iterator == 1 )
			{
				if( sub_page->number_previous != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: mismatch in previous child page number (%" PRIu32 " != %" PRIu32 ").",
					 function,
					 0,
					 sub_page->number_previous );

					libesedb_page_free(
					 &sub_page,
					 NULL );

					return( -1 );
				}
			}
			if( page_value_iterator == ( amount_of_page_values - 1 ) )
			{
				if( sub_page->number_next != 0 )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
					 "%s: mismatch in next child page number (%" PRIu32 " != %" PRIu32 ").",
					 function,
					 0,
					 sub_page->number_previous );

					libesedb_page_free(
					 &sub_page,
					 NULL );

					return( -1 );
				}
			}
			previous_child_page_number      = sub_page->number;
			previous_next_child_page_number = sub_page->number_next;

			if( libesedb_page_tree_read_leaf_page_values(
			     page_tree,
			     sub_page,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_IO,
				 LIBERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read leaf page values.",
				 function );

				libesedb_page_free(
				 &sub_page,
				 NULL );

				return( -1 );
			}
			if( libesedb_page_free(
			     &sub_page,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free child page.",
				 function );

				return( -1 );
			}
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

/* Reads the space tree page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_space_tree_page_values(
     libesedb_page_tree_t *page_tree,
     libesedb_page_t *page,
     liberror_error_t **error )
{
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_space_tree_page_values";
	uint32_t required_flags           = 0;
	uint32_t supported_flags          = 0;
	uint16_t amount_of_page_values    = 0;
	uint16_t page_key_size            = 0;
	uint16_t page_value_iterator      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t test                     = 0;
	uint32_t total_amount_of_pages    = 0;
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
	required_flags = LIBESEDB_PAGE_FLAG_IS_ROOT
	               | LIBESEDB_PAGE_FLAG_IS_LEAF
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
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_IS_PRIMARY
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
	if( page->number_previous != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported previous page number: %" PRIu32 ".",
		 function,
		 page->number_previous );

		return( -1 );
	}
	if( page->number_next != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported next page number: %" PRIu32 ".",
		 function,
		 page->number_next );

		return( -1 );
	}
	if( libesedb_page_get_amount_of_values(
	     page,
	     &amount_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of page values.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_iterator,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_iterator );

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
	libnotify_verbose_printf(
	 "%s: header:\n",
	 function,
	 page_value_iterator );
	libnotify_verbose_print_data(
	 page_value->data,
	 page_value->size );
#endif

	if( ( page_value->data[  0 ] != 0 )
	 || ( page_value->data[  1 ] != 0 )
	 || ( page_value->data[  2 ] != 0 )
	 || ( page_value->data[  3 ] != 0 )
	 || ( page_value->data[  4 ] != 0 )
	 || ( page_value->data[  5 ] != 0 )
	 || ( page_value->data[  6 ] != 0 )
	 || ( page_value->data[  7 ] != 0 )
	 || ( page_value->data[  8 ] != 0 )
	 || ( page_value->data[  9 ] != 0 )
	 || ( page_value->data[ 10 ] != 0 )
	 || ( page_value->data[ 11 ] != 0 )
	 || ( page_value->data[ 12 ] != 0 )
	 || ( page_value->data[ 13 ] != 0 )
	 || ( page_value->data[ 14 ] != 0 )
	 || ( page_value->data[ 15 ] != 0 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported header.",
		 function );

		return( -1 );
	}
	/* TODO handle the space tree page header */

	for( page_value_iterator = 1;
	     page_value_iterator < amount_of_page_values;
	     page_value_iterator++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_iterator,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_iterator );

			return( -1 );
		}
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
		if( page_value->size != sizeof( esedb_space_tree_page_entry_t ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported page value size: %" PRIzd ".",
			 function,
			 page_value->size );

			return( -1 );
		}
		endian_little_convert_16bit(
		 page_key_size,
		 ( (esedb_space_tree_page_entry_t *) page_value->data )->key_size );

		if( page_key_size != 4 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported page key size: %" PRIu16 ".",
			 function,
			 page_key_size);

			return( -1 );
		}
		/* TODO handle the space tree page values */

#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: value: %03d key size\t\t: %" PRIu16 "\n",
		 function,
		 page_value_iterator,
		 page_key_size );

		endian_little_convert_32bit(
		 test,
		 ( (esedb_space_tree_page_entry_t *) page_value->data )->last_page_number );
		libnotify_verbose_printf(
		 "%s: value: %03d key value\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
		 function,
		 page_value_iterator,
		 test,
		 test );

		endian_little_convert_32bit(
		 test,
		 ( (esedb_space_tree_page_entry_t *) page_value->data )->amount_of_pages );
		libnotify_verbose_printf(
		 "%s: value: %03d amount of pages\t: %" PRIu32 "\n",
		 function,
		 page_value_iterator,
		 test );

		libnotify_verbose_printf(
		 "\n" );

		if( ( page_value->flags & 0x02 ) == 0 )
		{
			total_amount_of_pages += test;
		}
#endif
	}

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: total amount of pages\t\t: %" PRIu32 "\n",
	 function,
	 total_amount_of_pages );

	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

/* Reads the leaf page values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_leaf_page_values(
     libesedb_page_tree_t *page_tree,
     libesedb_page_t *page,
     liberror_error_t **error )
{
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_leaf_page_values";
	uint32_t required_flags           = 0;
	uint32_t supported_flags          = 0;
	uint16_t amount_of_page_values    = 0;
	uint16_t page_value_iterator      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_value_data          = NULL;
	uint32_t column_type              = 0;
	uint16_t definition_flags         = 0;
	uint16_t definition_size          = 0;
	uint16_t definition_type          = 0;
	uint16_t page_key_size            = 0;
	uint16_t page_value_size          = 0;
	uint16_t page_value_unknown1      = 0;
	uint32_t test                     = 0;
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
	required_flags = LIBESEDB_PAGE_FLAG_IS_LEAF;

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
	                | LIBESEDB_PAGE_FLAG_IS_ROOT
	                | LIBESEDB_PAGE_FLAG_IS_INDEX
	                | LIBESEDB_PAGE_FLAG_IS_LONG_VALUE
	                | LIBESEDB_PAGE_FLAG_IS_PRIMARY
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
	if( libesedb_page_get_amount_of_values(
	     page,
	     &amount_of_page_values,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of page values.",
		 function );

		return( -1 );
	}
	if( libesedb_page_get_value(
	     page,
	     page_value_iterator,
	     &page_value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve page value: %" PRIu16 ".",
		 function,
		 page_value_iterator );

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
	page_value_data = page_value->data;
	page_value_size = page_value->size;

	libnotify_verbose_printf(
	 "%s: header (record key)\t\t\t\t: ",
	 function,
	 page_value_iterator );

	while( page_value_size > 0 )
	{
		if( ( *page_value_data >= 'A' )
		 && ( *page_value_data <= 'Z' ) )
		{
			libnotify_verbose_printf(
			 "%c",
			 (char) *page_value_data );
		}
		else
		{
			libnotify_verbose_printf(
			 "\\x%02" PRIx8 "",
			 *page_value_data );
		}
		page_value_data++;
		page_value_size--;
	}
	libnotify_verbose_printf(
	 "\n" );
	libnotify_verbose_printf(
	 "\n" );
#endif

	/* TODO handle the leaf page header */

	for( page_value_iterator = 1;
	     page_value_iterator < amount_of_page_values;
	     page_value_iterator++ )
	{
		if( libesedb_page_get_value(
		     page,
		     page_value_iterator,
		     &page_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve page value: %" PRIu16 ".",
			 function,
			 page_value_iterator );

			return( -1 );
		}
		/* TODO handle the leaf page values */

		page_value_data = page_value->data;
		page_value_size = page_value->size;

#if defined( HAVE_DEBUG_OUTPUT )
/*
		libnotify_verbose_printf(
		 "%s: value: %03d value:\n",
		 function,
		 page_value_iterator );
		libnotify_verbose_print_data(
		 page_value_data,
		 page_value->size );
*/

		if( ( page_value->flags & 0x04 ) == 0x04 )
		{
			endian_little_convert_16bit(
			 page_value_unknown1,
			 page_value_data );

			page_value_data += 2;
			page_value_size -= 2;

			libnotify_verbose_printf(
			 "%s: value: %03d unknown1\t\t\t\t: 0x%04" PRIx32 "\n",
			 function,
			 page_value_iterator,
			 page_value_unknown1 );
		}
		endian_little_convert_16bit(
		 page_key_size,
		 page_value_data );

		page_value_data += 2;
		page_value_size -= 2;

		libnotify_verbose_printf(
		 "%s: value: %03d key size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 page_value_iterator,
		 page_key_size );

		libnotify_verbose_printf(
		 "%s: value: %03d key value\t\t\t\t: ",
		 function,
		 page_value_iterator );

		while( page_key_size > 0 )
		{
			if( ( ( *page_value_data >= 'A' )
			  && ( *page_value_data <= 'Z' ) )
			 || ( ( *page_value_data >= 'a' )
			  && ( *page_value_data <= 'z' ) ) )
			{
				libnotify_verbose_printf(
				 "%c",
				 (char) *page_value_data );
			}
			else
			{
				libnotify_verbose_printf(
				 "\\x%02" PRIx8 "",
				 *page_value_data );
			}
			page_value_data++;
			page_value_size--;
			page_key_size--;
		}
		libnotify_verbose_printf(
		 "\n" );

		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_INDEX ) == LIBESEDB_PAGE_FLAG_IS_INDEX )
		{
			libnotify_verbose_printf(
			 "%s: value: %03d index value\t\t\t: ",
			 function,
			 page_value_iterator );

			while( page_value_size > 0 )
			{
				if( ( *page_value_data >= 'A' )
				  && ( *page_value_data <= 'Z' ) )
				{
					libnotify_verbose_printf(
					 "%c",
					 (char) *page_value_data );
				}
				else
				{
					libnotify_verbose_printf(
					 "\\x%02" PRIx8 "",
					 *page_value_data );
				}
				page_value_data++;
				page_value_size--;
			}
			libnotify_verbose_printf(
			 "\n" );
			libnotify_verbose_printf(
			 "\n" );
		}
		else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LONG_VALUE ) == LIBESEDB_PAGE_FLAG_IS_LONG_VALUE )
		{
			libnotify_verbose_printf(
			 "%s: value: %03d long value:\n",
			 function,
			 page_value_iterator );
			libnotify_verbose_print_data(
			 page_value_data,
			 page_value_size );
		}
		else
		{
			endian_little_convert_16bit(
			 definition_flags,
			 page_value_data );

			page_value_data += 2;
			page_value_size -= 2;

			libnotify_verbose_printf(
			 "%s: value: %03d definition flags\t\t\t: 0x%04" PRIx32 "\n",
			 function,
			 page_value_iterator,
			 definition_flags );

			endian_little_convert_16bit(
			 definition_size,
			 page_value_data );

			page_value_data += 2;
			page_value_size -= 2;

			libnotify_verbose_printf(
			 "%s: value: %03d definition size\t\t\t: %" PRIu32 "\n",
			 function,
			 page_value_iterator,
			 definition_size );

			definition_size -= 4;

			if( ( definition_flags == 0x8007 )
			 || ( definition_flags == 0x8008 )
			 || ( definition_flags == 0x8009 )
			 || ( definition_flags == 0x840a ) )
			{
				libnotify_verbose_printf(
				 "%s: value: %03d definition data:\n",
				 function,
				 page_value_iterator );
				libnotify_verbose_print_data(
				 page_value_data,
				 definition_size );

				endian_little_convert_32bit(
				 test,
				 page_value_data );

				page_value_data += 4;
				page_value_size -= 4;
				definition_size -= 4;

				libnotify_verbose_printf(
				 "%s: value: %03d parent FDP object identifier\t: %" PRIu32 "\n",
				 function,
				 page_value_iterator,
				 test );

				endian_little_convert_16bit(
				 definition_type,
				 page_value_data );

				page_value_data += 2;
				page_value_size -= 2;
				definition_size -= 2;

				libnotify_verbose_printf(
				 "%s: value: %03d definition type\t\t\t: 0x%04" PRIx16 " ",
				 function,
				 page_value_iterator,
				 definition_type );
				libesedb_debug_print_page_value_definition_type(
				 definition_type );
				libnotify_verbose_printf(
				 "\n" );

				if( definition_type == 0x0001 )
				{
					endian_little_convert_32bit(
					 test,
					 page_value_data );

					page_value_data += 4;
					page_value_size -= 4;
					definition_size -= 4;

					libnotify_verbose_printf(
					 "%s: value: %03d FDP object identifier\t\t: %" PRIu32 "\n",
					 function,
					 page_value_iterator,
					 test );

					if( definition_flags == 0x8008 )
					{
						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d parent FDP page number\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d table density percentage\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d unknown1_1\t\t\t\t: 0x%08" PRIx32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d intial amount of pages\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_16bit(
						 test,
						 page_value_data );

						page_value_data += 2;
						page_value_size -= 2;
						definition_size -= 2;

						libnotify_verbose_printf(
						 "%s: value: %03d unknown1_2\t\t\t\t: 0x%04" PRIx32 "\n",
						 function,
						 page_value_iterator,
						 test );
					}
				}
				else if( definition_type == 0x0002 )
				{
					endian_little_convert_32bit(
					 test,
					 page_value_data );

					page_value_data += 4;
					page_value_size -= 4;
					definition_size -= 4;

					libnotify_verbose_printf(
					 "%s: value: %03d column identifier\t\t\t: %" PRIu32 "\n",
					 function,
					 page_value_iterator,
					 test );

					if( ( definition_flags == 0x8007 )
					 || ( definition_flags == 0x8009 ) )
					{
						endian_little_convert_32bit(
						 column_type,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d column type\t\t\t: %" PRIu32 " ",
						 function,
						 page_value_iterator,
						 column_type );
						libesedb_debug_print_column_type(
						 column_type );
						libnotify_verbose_printf(
						 "\n" );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d maximum size\t\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d column group of bits\t\t: ",
						 function,
						 page_value_iterator );
						libesedb_debug_print_column_group_of_bits(
						 test );
						libnotify_verbose_printf(
						 "\n" );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d codepage\t\t\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
						 function,
						 page_value_iterator,
						 test,
						 test );

						if( ( definition_flags & 0x0008 ) == 0x0008 )
						{
							endian_little_convert_16bit(
							 test,
							 page_value_data );

							page_value_data += 2;
							page_value_size -= 2;
							definition_size -= 2;

							libnotify_verbose_printf(
							 "%s: value: %03d unknown2_1\t\t\t\t: %" PRIu32 "\n",
							 function,
							 page_value_iterator,
							 test );

							libnotify_verbose_printf(
							 "%s: value: %03d unknown2_2\t\t\t\t: 0x%02" PRIx8 "\n",
							 function,
							 page_value_iterator,
							 *page_value_data );

							page_value_data += 1;
							page_value_size -= 1;
							definition_size -= 1;
						}
						if( ( definition_flags & 0x0001 ) == 0x0001 )
						{
							libnotify_verbose_printf(
							 "%s: value: %03d unknown2_3\t\t\t\t: 0x%02" PRIx8 "\n",
							 function,
							 page_value_iterator,
							 *page_value_data );

							page_value_data += 1;
							page_value_size -= 1;
							definition_size -= 1;
						}
						if( ( definition_flags & 0x0008 ) == 0x0008 )
						{
							libnotify_verbose_printf(
							 "%s: value: %03d unknown2_4\t\t\t\t: 0x%02" PRIx8 "\n",
							 function,
							 page_value_iterator,
							 *page_value_data );

							page_value_data += 1;
							page_value_size -= 1;
							definition_size -= 1;
						}
					}
				}
				else if( definition_type == 0x0003 )
				{
					if( definition_flags == 0x840a )
					{
						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d FDP object identifier\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d parent FDP page number\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d unknown3_1\t\t\t\t: 0x%08" PRIx32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d unknown3_2\t\t\t\t: 0x%08" PRIx32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d unknown3_3\t\t\t\t: 0x%08" PRIx32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;
					}
				}
				else if( definition_type == 0x0004 )
				{
					if( definition_flags == 0x8007 )
					{
						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d FDP object identifier\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d parent FDP page number\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d initial amount of pages\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d unknown4_1\t\t\t\t: 0x%08" PRIx32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;
						definition_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d extent space\t\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						libnotify_verbose_printf(
						 "%s: value: %03d unknown4_2\t\t\t\t: 0x%02" PRIx8 "\n",
						 function,
						 page_value_iterator,
						 *page_value_data );

						page_value_data += 1;
						page_value_size -= 1;
						definition_size -= 1;
					}
				}
			}
			if( definition_size > 0 )
			{
				libnotify_verbose_printf(
				 "%s: value: %03d definition data:\n",
				 function,
				 page_value_iterator );
				libnotify_verbose_print_data(
				 page_value_data,
				 definition_size );

				page_value_data += definition_size;
				page_value_size -= definition_size;
			}
			libnotify_verbose_printf(
			 "%s: value: %03d value data:\n",
			 function,
			 page_value_iterator );
			libnotify_verbose_print_data(
			 page_value_data,
			 page_value_size );
		}
#endif
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

