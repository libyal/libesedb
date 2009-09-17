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
		 "%s: invalid page_tree.",
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
		 "%s: invalid page_tree.",
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
	static char *function                       = "libesedb_page_tree_read";
	uint32_t father_object_identifier           = 0;
	uint32_t root_father_object_identifier      = 0;
	uint32_t space_tree_page_number             = 0;
	uint32_t next_space_tree_page_number        = 0;
	uint32_t next_backup_space_tree_page_number = 0;

	if( libesedb_page_tree_read_father_data_page(
	     page_tree,
	     io_handle,
	     father_data_page_number,
	     &root_father_object_identifier,
	     &space_tree_page_number,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read father data page: %" PRIu32 ".",
		 function,
		 father_data_page_number );

		return( -1 );
	}
	while( ( space_tree_page_number > 0 )
	    && ( space_tree_page_number < 0xfffffff0UL ) )
	{
		if( libesedb_page_tree_read_space_tree_page(
		     page_tree,
		     io_handle,
		     space_tree_page_number,
		     &father_object_identifier,
		     &next_space_tree_page_number,
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
		if( root_father_object_identifier != father_object_identifier )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch in father object identifier (%" PRIu32 " != %" PRIu32 ").",
			 function,
			 root_father_object_identifier,
			 father_object_identifier );

			return( -1 );
		}
		if( libesedb_page_tree_read_space_tree_page(
		     page_tree,
		     io_handle,
		     space_tree_page_number + 1,
		     &father_object_identifier,
		     &next_backup_space_tree_page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read backup space tree page: %" PRIu32 ".",
			 function,
			 space_tree_page_number + 1 );

			return( -1 );
		}
		if( root_father_object_identifier != father_object_identifier )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch in father object identifier (%" PRIu32 " != %" PRIu32 ").",
			 function,
			 root_father_object_identifier,
			 father_object_identifier );

			return( -1 );
		}
		space_tree_page_number = next_space_tree_page_number;
	}
	return( 1 );
}

/* Reads a father data page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_father_data_page(
     libesedb_page_tree_t *page_tree,
     libesedb_io_handle_t *io_handle,
     uint32_t father_data_page_number,
     uint32_t *father_object_identifier,
     uint32_t *first_space_tree_page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page                   = NULL;
	libesedb_page_value_t *page_value       = NULL;
	static char *function                   = "libesedb_page_tree_read_father_data_page";
	uint32_t child_father_object_identifier = 0;
	uint32_t child_page_number              = 0;
	uint32_t next_leaf_page_number          = 0;
	uint16_t amount_of_page_values          = 0;
	uint16_t page_value_iterator            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_value_data                = NULL;
	uint32_t extent_space                   = 0;
	uint32_t initial_amount_of_pages        = 0;
	uint16_t page_key_size                  = 0;
	uint16_t page_value_size                = 0;
	uint32_t test                           = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page_tree.",
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
	if( father_object_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid father object identifier.",
		 function );

		return( -1 );
	}
	if( first_space_tree_page_number == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid first space tree page number.",
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
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: page is not a root page.",
		 function );

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

		return( -1 );
	}
	*father_object_identifier = page->father_object_identifier;

	endian_little_convert_32bit(
	 *first_space_tree_page_number,
	 ( (esedb_root_page_header_t *) page_value->data )->space_tree_page_number );

	/* TODO handle the root page header */

#if defined( HAVE_DEBUG_OUTPUT )
	endian_little_convert_32bit(
	 initial_amount_of_pages,
	 ( (esedb_root_page_header_t *) page_value->data )->initial_amount_of_pages );
	libnotify_verbose_printf(
	 "%s: header initial amount of pages\t: %" PRIu32 "\n",
	 function,
	 initial_amount_of_pages );

	endian_little_convert_32bit(
	 test,
	 ( (esedb_root_page_header_t *) page_value->data )->parent_father_data_page_number );
	libnotify_verbose_printf(
	 "%s: header parent FDP number\t\t: %" PRIu32 "\n",
	 function,
	 test );

	endian_little_convert_32bit(
	 extent_space,
	 ( (esedb_root_page_header_t *) page_value->data )->extent_space );
	libnotify_verbose_printf(
	 "%s: header extent space\t\t\t: %" PRIu32 "\n",
	 function,
	 extent_space );

	libnotify_verbose_printf(
	 "%s: header space tree page number\t\t: %" PRIu32 " (0x%08" PRIx32 ")\n",
	 function,
	 *first_space_tree_page_number,
	 *first_space_tree_page_number );

	libnotify_verbose_printf(
	 "%s: primary extent\t\t\t: %" PRIu32 "-%c\n",
	 function,
	 initial_amount_of_pages,
	 ( extent_space == 0 ? 's' : 'm' ) );

	libnotify_verbose_printf(
	 "\n" );
#endif

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

			libesedb_page_free(
			 &page,
			 NULL );

			return( -1 );
		}
		if( ( page_value->flags & 0x07 ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported page value flags: 0x%02" PRIx8 ".",
			 function,
			 page_value->flags );

			libesedb_page_free(
			 &page,
			 NULL );

			return( -1 );
		}
		/* TODO handle leaf page values */

		page_value_data = page_value->data;
		page_value_size = page_value->size;

#if defined( HAVE_DEBUG_OUTPUT )
		endian_little_convert_16bit(
		 page_key_size,
		 page_value_data );

		page_value_data += 2;

		libnotify_verbose_printf(
		 "%s: value: %03d highest key size\t\t: %" PRIu16 "\n",
		 function,
		 page_value_iterator,
		 page_key_size );

		libnotify_verbose_printf(
		 "%s: value: %03d highest key value\t\t: ",
		 function,
		 page_value_iterator );

		while( page_key_size > 0 )
		{
			libnotify_verbose_printf(
			 "%02" PRIx8 "",
			 *page_value_data );

			page_value_data++;
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
		if( libesedb_page_tree_read_leaf_page(
		     page_tree,
		     io_handle,
		     child_page_number,
		     &child_father_object_identifier,
		     &next_leaf_page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read leaf page: %" PRIu32 ".",
			 function,
			 child_page_number );

			libesedb_page_free(
			 &page,
			 NULL );

			return( -1 );
		}
		if( child_father_object_identifier != *father_object_identifier )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: mismatch in father object identifier (%" PRIu32 " != %" PRIu32 ").",
			 function,
			 child_father_object_identifier,
			 *father_object_identifier );

			libesedb_page_free(
			 &page,
			 NULL );

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
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

/* Reads a space tree page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_space_tree_page(
     libesedb_page_tree_t *page_tree,
     libesedb_io_handle_t *io_handle,
     uint32_t space_tree_page_number,
     uint32_t *father_object_identifier,
     uint32_t *next_space_tree_page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_space_tree_page";
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
		 "%s: invalid page_tree.",
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
	if( father_object_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid father object identifier.",
		 function );

		return( -1 );
	}
	if( next_space_tree_page_number == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next space tree page number.",
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
	     space_tree_page_number,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page: %" PRIu32 ".",
		 function,
		 space_tree_page_number );

		libesedb_page_free(
		 &page,
		 NULL );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_SPACE_TREE ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: page is not a space tree page.",
		 function );

		libesedb_page_free(
		 &page,
		 NULL );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: page is not a root page.",
		 function );

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

		return( -1 );
	}
	*father_object_identifier = page->father_object_identifier;

	*next_space_tree_page_number = 0;

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

		libesedb_page_free(
		 &page,
		 NULL );

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

			libesedb_page_free(
			 &page,
			 NULL );

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

			libesedb_page_free(
			 &page,
			 NULL );

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

			libesedb_page_free(
			 &page,
			 NULL );

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

			libesedb_page_free(
			 &page,
			 NULL );

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

/* Reads a leaf page
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_tree_read_leaf_page(
     libesedb_page_tree_t *page_tree,
     libesedb_io_handle_t *io_handle,
     uint32_t leaf_page_number,
     uint32_t *father_object_identifier,
     uint32_t *next_leaf_page_number,
     liberror_error_t **error )
{
	libesedb_page_t *page             = NULL;
	libesedb_page_value_t *page_value = NULL;
	static char *function             = "libesedb_page_tree_read_leaf_page";
	uint16_t amount_of_page_values    = 0;
	uint16_t page_value_iterator      = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint8_t *page_value_data          = NULL;
	uint16_t column_definition_type   = 0;
	uint16_t page_key_size            = 0;
	uint16_t page_value_size          = 0;
	uint16_t page_value_type          = 0;
	uint32_t test                     = 0;
#endif

	if( page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page_tree.",
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
	if( father_object_identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid father object identifier.",
		 function );

		return( -1 );
	}
	if( next_leaf_page_number == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next leaf page number.",
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
	     leaf_page_number,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page: %" PRIu32 ".",
		 function,
		 leaf_page_number );

		libesedb_page_free(
		 &page,
		 NULL );

		return( -1 );
	}
	if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: page is not a leaf page.",
		 function );

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

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

		libesedb_page_free(
		 &page,
		 NULL );

		return( -1 );
	}
	*father_object_identifier = page->father_object_identifier;

	*next_leaf_page_number = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: header:\n",
	 function,
	 page_value_iterator );
	libnotify_verbose_print_data(
	 page_value->data,
	 page_value->size );
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

			libesedb_page_free(
			 &page,
			 NULL );

			return( -1 );
		}
		/* TODO handle the leaf page values */

		page_value_data = page_value->data;
		page_value_size = page_value->size;

#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: value: %03d value:\n",
		 function,
		 page_value_iterator );
		libnotify_verbose_print_data(
		 page_value_data,
		 page_value->size );

		if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_INDEX ) == LIBESEDB_PAGE_FLAG_IS_INDEX )
		{
			page_value_size = page_value->size;

			endian_little_convert_16bit(
			 page_value_type,
			 page_value_data );

			page_value_data += 2;
			page_value_size -= 2;

			libnotify_verbose_printf(
			 "%s: value: %03d index value type\t\t: 0x%04" PRIx32 "\n",
			 function,
			 page_value_iterator,
			 page_value_type );

			if( ( page_value->flags & 0x04 ) == 0x04 )
			{
				endian_little_convert_16bit(
				 page_key_size,
				 page_value_data );

				page_value_data += 2;
				page_value_size -= 2;

				libnotify_verbose_printf(
				 "%s: value: %03d index key size\t\t\t: %" PRIu16 "\n",
				 function,
				 page_value_iterator,
				 page_key_size );

				libnotify_verbose_printf(
				 "%s: value: %03d index key value:\n",
				 function,
				 page_value_iterator );
				libnotify_verbose_print_data(
				 page_value_data,
				 page_key_size );

				page_value_data += page_key_size;
				page_value_size -= page_key_size;
			}
			libnotify_verbose_printf(
			 "%s: value: %03d index value:\n",
			 function,
			 page_value_iterator );
			libnotify_verbose_print_data(
			 page_value_data,
			 page_value_size );
		}
		else if( ( page->flags & LIBESEDB_PAGE_FLAG_IS_LONG_VALUE ) == LIBESEDB_PAGE_FLAG_IS_LONG_VALUE )
		{
			endian_little_convert_16bit(
			 test,
			 page_value_data );

			page_value_data += 2;

			libnotify_verbose_printf(
			 "%s: value: %03d long value type\t\t: 0x%04" PRIx32 "\n",
			 function,
			 page_value_iterator,
			 test );

			libnotify_verbose_printf(
			 "\n" );
		}
		else
		{
			page_value_size = page_value->size;

			endian_little_convert_16bit(
			 page_value_type,
			 page_value_data );

			page_value_data += 2;
			page_value_size -= 2;

			libnotify_verbose_printf(
			 "%s: value: %03d table value type\t\t: 0x%04" PRIx32 "\n",
			 function,
			 page_value_iterator,
			 page_value_type );

			if( ( page_value->flags & 0x04 ) == 0x04 )
			{
				endian_little_convert_16bit(
				 page_key_size,
				 page_value_data );

				page_value_data += 2;
				page_value_size -= 2;

				libnotify_verbose_printf(
				 "%s: value: %03d table key size\t\t\t: %" PRIu16 "\n",
				 function,
				 page_value_iterator,
				 page_key_size );

				libnotify_verbose_printf(
				 "%s: value: %03d table key value:\n",
				 function,
				 page_value_iterator );
				libnotify_verbose_print_data(
				 page_value_data,
				 page_key_size );

				page_value_data += page_key_size;
				page_value_size -= page_key_size;

				endian_little_convert_16bit(
				 test,
				 page_value_data );

				page_value_data += 2;
				page_value_size -= 2;

				libnotify_verbose_printf(
				 "%s: value: %03d unknown flags\t\t\t: 0x%04" PRIx32 "\n",
				 function,
				 page_value_iterator,
				 test );

				endian_little_convert_16bit(
				 test,
				 page_value_data );

				page_value_data += 2;
				page_value_size -= 2;

				libnotify_verbose_printf(
				 "%s: value: %03d unknown size\t\t\t: %" PRIu32 "\n",
				 function,
				 page_value_iterator,
				 test );

				if( ( page_value_type == 0x0004 )
				 || ( page_value_type == 0x0007 )
				 || ( page_value_type == 0x000b )
				 || ( page_value_type == 0x000c ) )
				{
					endian_little_convert_32bit(
					 test,
					 page_value_data );

					page_value_data += 4;
					page_value_size -= 4;

					libnotify_verbose_printf(
					 "%s: value: %03d unknown\t\t\t: 0x%08" PRIx32 "\n",
					 function,
					 page_value_iterator,
					 test );

					endian_little_convert_16bit(
					 column_definition_type,
					 page_value_data );

					page_value_data += 2;
					page_value_size -= 2;

					libnotify_verbose_printf(
					 "%s: value: %03d definition type\t\t: 0x%04" PRIx32 "\n",
					 function,
					 page_value_iterator,
					 column_definition_type );

					if( ( column_definition_type == 1 )
					 || ( column_definition_type == 2 ) )
					{
						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;

						libnotify_verbose_printf(
						 "%s: value: %03d column identifier\t\t: %" PRIu32 "\n",
						 function,
						 page_value_iterator,
						 test );

						endian_little_convert_32bit(
						 test,
						 page_value_data );

						page_value_data += 4;
						page_value_size -= 4;

						if( column_definition_type == 1 )
						{
							libnotify_verbose_printf(
							 "%s: value: %03d unknown\t\t\t: %" PRIu32 "\n",
							 function,
							 page_value_iterator,
							 test );

							page_key_size = 14;
						}
						else if( column_definition_type == 2 )
						{
							libnotify_verbose_printf(
							 "%s: value: %03d column type\t\t\t: %" PRIu32 "\n",
							 function,
							 page_value_iterator,
							 test );

							if( ( test >= 0x01 )
							 && ( test <= 0x04 ) )
							{
								page_key_size = 17;
							}
							else if( test == 0x07 )
							{
								page_key_size = 13;
							}
							else if( test == 0x08 )
							{
								page_key_size = 17;
							}
							else if( ( test >= 0x09 )
							      && ( test <= 0x0c ) )
							{
								page_key_size = 13;
							}
							else
							{
								page_key_size = 0;
							}
						}
						if( page_key_size > 0 )
						{
							libnotify_verbose_printf(
							 "%s: value: %03d definition data:\n",
							 function,
							 page_value_iterator );
							libnotify_verbose_print_data(
							 page_value_data,
							 page_key_size );

							page_value_data += page_key_size;
							page_value_size -= page_key_size;
						}
					}
				}
			}
			libnotify_verbose_printf(
			 "%s: value: %03d table value:\n",
			 function,
			 page_value_iterator );
			libnotify_verbose_print_data(
			 page_value_data,
			 page_value_size );
		}
#endif
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
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( 1 );
}

