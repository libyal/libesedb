/*
 * Item descriptor functions
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
#include <memory.h>
#include <types.h>

#include <liberror.h>
#include <libnotify.h>

#include "libesedb_array_type.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_page.h"

#include "esedb_page.h"

/* Frees page tags value
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
		memory_free(
		 *page );

		*page = NULL;
	}
	return( 1 );
}

/* Reads a page and its values
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     off64_t page_offset,
     size_t page_size,
     liberror_error_t **error )
{
	libesedb_array_t *page_tags_array           = NULL;
	libesedb_page_tags_value_t *page_tags_value = NULL;
	uint8_t *page_data                          = NULL;
	uint8_t *page_values_data                   = NULL;
	uint8_t *page_value_data                    = NULL;
	static char *function                       = "libesedb_page_read";
	ssize_t read_count                          = 0;
	uint32_t page_flags                         = 0;
	uint16_t available_data_size                = 0;
	uint16_t available_page_tag                 = 0;
	uint16_t page_key_size                      = 0;
	uint16_t page_tag_number                    = 0;
	uint16_t page_value_size                    = 0;

#if defined( HAVE_VERBOSE_OUTPUT )
	uint32_t test                               = 0;
#endif

/* TODO
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
*/
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
	if( page_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid page size value exceeds maximum.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: reading page at offset: %" PRIu64 " (0x%08" PRIx64 ")\n",
	 function,
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
	page_data = (uint8_t *) memory_allocate(
	                         page_size );

	if( page_data == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page data.",
		 function );

		return( -1 );
	}
	read_count = libbfio_handle_read(
	              io_handle->file_io_handle,
	              page_data,
	              page_size,
	              error );

	if( read_count != (ssize_t) page_size )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page data.",
		 function );

		memory_free(
		 page_data );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "%s: page header:\n",
	 function );
	libnotify_verbose_print_data(
	 page_data,
	 sizeof( esedb_page_header_t ) );
#endif

	endian_little_convert_16bit(
	 available_data_size,
	 ( (esedb_page_header_t *) page_data )->available_data_size );

	endian_little_convert_16bit(
	 available_page_tag,
	 ( (esedb_page_header_t *) page_data )->available_page_tag );

	endian_little_convert_32bit(
	 page_flags,
	 ( (esedb_page_header_t *) page_data )->page_flags );

#if defined( HAVE_VERBOSE_OUTPUT )
	libnotify_verbose_printf(
	 "%s: current page number\t\t: %" PRIu64 "\n",
	 function,
	 ( page_offset / page_size ) - 1 );

	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_header_t *) page_data )->xor_checksum );
	libnotify_verbose_printf(
	 "%s: xor checksum\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
	 function,
	 test,
	 test );
	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_header_t *) page_data )->page_number );

	if( io_handle->format_revision >= 0x0c )
	{
		endian_little_convert_32bit(
		 test,
		 ( (esedb_page_header_t *) page_data )->ecc_checksum );
		libnotify_verbose_printf(
		 "%s: ecc checksum\t\t: 0x%08" PRIx32 " (%" PRIu32 ")\n",
		 function,
		 test,
		 test );
		endian_little_convert_32bit(
		 test,
		 ( (esedb_page_header_t *) page_data )->page_number );
	}
	else
	{
		libnotify_verbose_printf(
		 "%s: page number\t\t\t: %" PRIu32 "\n",
		 function,
		 test );
	}
	libnotify_verbose_printf(
	 "%s: modification time:\n",
	 function );
	libnotify_verbose_print_data(
	 ( (esedb_page_header_t *) page_data )->modification_time,
	 8 );

	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_header_t *) page_data )->previous_page );
	libnotify_verbose_printf(
	 "%s: previous page number\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_header_t *) page_data )->next_page );
	libnotify_verbose_printf(
	 "%s: next page number\t\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_32bit(
	 test,
	 ( (esedb_page_header_t *) page_data )->father_object_identifier );
	libnotify_verbose_printf(
	 "%s: father object identifier\t: %" PRIu32 "\n",
	 function,
	 test );

	libnotify_verbose_printf(
	 "%s: available data size\t\t: %" PRIu32 "\n",
	 function,
	 available_data_size );
	endian_little_convert_16bit(
	 test,
	 ( (esedb_page_header_t *) page_data )->available_uncommitted_data_size );
	libnotify_verbose_printf(
	 "%s: available uncommitted data size\t: %" PRIu32 "\n",
	 function,
	 test );
	endian_little_convert_16bit(
	 test,
	 ( (esedb_page_header_t *) page_data )->available_data_offset );
	libnotify_verbose_printf(
	 "%s: available data offset\t: %" PRIu32 "\n",
	 function,
	 test );
	libnotify_verbose_printf(
	 "%s: available page tag\t\t: %" PRIu32 "\n",
	 function,
	 available_page_tag );

	libesedb_debug_print_page_flags(
	 page_flags,
	 NULL );
#endif

	if( libesedb_page_read_tags(
	     &page_tags_array,
	     available_page_tag,
	     page_data,
	     page_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page tags.",
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
	page_values_data = &( page_data[ sizeof( esedb_page_header_t ) ] );

	for( page_tag_number = 0;
	     page_tag_number < available_page_tag;
	     page_tag_number++ )
	{
		libnotify_verbose_printf(
		 "%s: page tag: %d\n",
		 function,
		 page_tag_number );

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

			libesedb_array_free(
			 &page_tags_array,
			 &libesedb_page_tags_value_free,
			 NULL );

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

			libesedb_array_free(
			 &page_tags_array,
			 &libesedb_page_tags_value_free,
			 NULL );

			return( -1 );
		}
		page_value_data = &( page_values_data[ page_tags_value->offset ] );

#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: page tag: %03d offset\t\t\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_tags_value->offset );

		libnotify_verbose_printf(
		 "%s: page tag: %03d size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_tags_value->size );

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
				 page_tags_value->size );

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
				 page_key_size,
				 page_value_data );

				page_value_data += 2;

				libnotify_verbose_printf(
				 "%s: page tag: %03d highest key size\t\t: %" PRIu16 "\n",
				 function,
				 page_tag_number,
				 page_key_size );

				libnotify_verbose_printf(
				 "%s: page tag: %03d highest key value:\n",
				 function,
				 page_tag_number );
				libnotify_verbose_print_data(
				 page_value_data,
				 page_key_size );

				page_value_data += page_key_size;

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
				 page_tags_value->size );
			}
			else
			{
				endian_little_convert_16bit(
				 page_key_size,
				 page_value_data );

				page_value_data += 2;

				libnotify_verbose_printf(
				 "%s: page tag: %03d key size\t\t\t: %" PRIu16 "\n",
				 function,
				 page_tag_number,
				 page_key_size );

				if( ( ( page_flags & LIBESEDB_PAGE_FLAG_IS_PARENT ) == LIBESEDB_PAGE_FLAG_IS_PARENT )
				 && ( page_key_size == 4 ) )
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
					 page_key_size );
				}
				page_value_data += page_key_size;

				page_value_size = page_tags_value->size - 2 - page_key_size;

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
			 page_tags_value->size );
		}

#endif
	}
	memory_free(
	 page_data );

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
	return( 1 );
}

/* Reads the page tags
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_read_tags(
     libesedb_array_t **page_tags_array,
     uint16_t amount_of_page_entries,
     uint8_t *page_data,
     size_t page_data_size,
     liberror_error_t **error )
{
	libesedb_page_tags_value_t *page_tags_value = NULL;
	uint8_t *page_tags_data                     = NULL;
	static char *function                       = "libesedb_page_read_tags";
	uint16_t page_tag_number                    = 0;
	int result                                  = 1;

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
	if( *page_tags_array != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: page tags array already set.",
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
	/* Create the page tags array
	 */
	if( libesedb_array_initialize(
	     page_tags_array,
	     amount_of_page_entries,
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

			result = -1;

			break;
		}
		endian_little_convert_16bit(
		 page_tags_value->offset,
		 page_tags_data );

		page_tags_data -= 2;

		endian_little_convert_16bit(
		 page_tags_value->size,
		 page_tags_data );

		page_tags_data -= 2;

#if defined( HAVE_DEBUG_OUTPUT )
		libnotify_verbose_printf(
		 "%s: page tag: %03d offset\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_tags_value->offset );
		libnotify_verbose_printf(
		 "%s: page tag: %03d size\t: %" PRIu16 "\n",
		 function,
		 page_tag_number,
		 page_tags_value->size );
#endif
		if( libesedb_array_set_entry(
		     *page_tags_array,
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

			result = -1;

			break;
		}
	}
	if( result != 1 )
	{
		if( libesedb_array_free(
		     page_tags_array,
		     &libesedb_page_tags_value_free,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the page tags array.",
			 function );
		}
		*page_tags_array = NULL;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	libnotify_verbose_printf(
	 "\n" );
#endif

	return( result );
}

