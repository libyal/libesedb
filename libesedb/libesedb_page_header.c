/*
 * Page header functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_page_header.h"

#include "esedb_page.h"

/* Creates a page header
 * Make sure the value page_header is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_header_initialize(
     libesedb_page_header_t **page_header,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_header_initialize";

	if( page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page header.",
		 function );

		return( -1 );
	}
	if( *page_header != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid page header value already set.",
		 function );

		return( -1 );
	}
	*page_header = memory_allocate_structure(
	                libesedb_page_header_t );

	if( *page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create page header.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *page_header,
	     0,
	     sizeof( libesedb_page_header_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear page header.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *page_header != NULL )
	{
		memory_free(
		 *page_header );

		*page_header = NULL;
	}
	return( -1 );
}

/* Frees a page header
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_header_free(
     libesedb_page_header_t **page_header,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_header_free";

	if( page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page header.",
		 function );

		return( -1 );
	}
	if( *page_header != NULL )
	{
		memory_free(
		 *page_header );

		*page_header = NULL;
	}
	return( 1 );
}

/* Reads a page header
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_header_read_data(
     libesedb_page_header_t *page_header,
     libesedb_io_handle_t *io_handle,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function        = "libesedb_page_header_read_data";
	size_t data_offset           = 0;
	size_t minimum_data_size     = 0;
	uint16_t available_data_size = 0;
	uint8_t has_extended_header  = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit         = 0;
	uint16_t value_16bit         = 0;
#endif

	if( page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page header.",
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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	minimum_data_size = sizeof( esedb_page_header_t );

	if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_EXTENDED_PAGE_HEADER )
	 && ( io_handle->page_size >= 16384 ) )
	{
		minimum_data_size  += sizeof( esedb_extended_page_header_t );
		has_extended_header = 1;
	}
	if( ( data_size < minimum_data_size )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: page header:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( esedb_page_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_page_header_t *) data )->available_data_size,
	 available_data_size );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) data )->previous_page,
	 page_header->previous_page_number );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) data )->next_page,
	 page_header->next_page_number );
	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) data )->father_data_page_object_identifier,
	 page_header->father_data_page_object_identifier );

	byte_stream_copy_to_uint16_little_endian(
	 ( (esedb_page_header_t *) data )->available_page_tag,
	 page_header->available_page_tag );

	byte_stream_copy_to_uint32_little_endian(
	 ( (esedb_page_header_t *) data )->page_flags,
	 page_header->flags );

	/* Make sure to read after the page flags
	 */
	if( has_extended_header != 0 )
	{
/* TODO store checksum into page header */
	}
	else
	{
		byte_stream_copy_to_uint32_little_endian(
		 ( (esedb_page_header_t *) data )->xor_checksum,
		 page_header->xor32_checksum );

		if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_NEW_RECORD_FORMAT )
		 && ( ( page_header->flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) != 0 ) )
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (esedb_page_header_t *) data )->ecc_checksum,
			 page_header->ecc32_checksum );
		}
		else
		{
			byte_stream_copy_to_uint32_little_endian(
			 ( (esedb_page_header_t *) data )->page_number,
			 page_header->page_number );
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( has_extended_header != 0 )
		{
/* TODO print checksum from page header */

			byte_stream_copy_to_uint64_little_endian(
			 data,
			 value_64bit );
			libcnotify_printf(
			 "%s: checksum\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );
		}
		else
		{
			libcnotify_printf(
			 "%s: XOR checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
			 function,
			 page_header->xor32_checksum );

			if( ( io_handle->format_revision >= LIBESEDB_FORMAT_REVISION_NEW_RECORD_FORMAT )
			 && ( ( page_header->flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) != 0 ) )
			{
				libcnotify_printf(
				 "%s: ECC checksum\t\t\t\t\t: 0x%08" PRIx32 "\n",
				 function,
				 page_header->ecc32_checksum );
			}
			else
			{
				libcnotify_printf(
				 "%s: page number\t\t\t\t\t: %" PRIu32 "\n",
				 function,
				 page_header->page_number );
			}
		}
		libcnotify_printf(
		 "%s: database modification time:\n",
		 function );
		libcnotify_print_data(
		 ( (esedb_page_header_t *) data )->database_modification_time,
		 8,
		 0 );

		libcnotify_printf(
		 "%s: previous page number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 page_header->previous_page_number );

		libcnotify_printf(
		 "%s: next page number\t\t\t\t: %" PRIu32 "\n",
		 function,
		 page_header->next_page_number );

		libcnotify_printf(
		 "%s: father data page (FDP) object identifier\t: %" PRIu32 "\n",
		 function,
		 page_header->father_data_page_object_identifier );

		libcnotify_printf(
		 "%s: available data size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 available_data_size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (esedb_page_header_t *) data )->available_uncommitted_data_size,
		 value_16bit );
		libcnotify_printf(
		 "%s: available uncommitted data size\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (esedb_page_header_t *) data )->available_data_offset,
		 value_16bit );
		libcnotify_printf(
		 "%s: available data offset\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: available page tag\t\t\t\t: %" PRIu32 "\n",
		 function,
		 page_header->available_page_tag );

		libcnotify_printf(
		 "%s: page flags\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 page_header->flags );
		libesedb_debug_print_page_flags(
		 page_header->flags );
		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	data_offset = sizeof( esedb_page_header_t );

	if( has_extended_header != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: extended page header:\n",
			 function );
			libcnotify_print_data(
			 &( data[ data_offset ] ),
			 40,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) &( data[ data_offset ] ) )->checksum1,
			 value_64bit );
			libcnotify_printf(
			 "%s: checksum1\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) &( data[ data_offset ] ) )->checksum2,
			 value_64bit );
			libcnotify_printf(
			 "%s: checksum2\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) &( data[ data_offset ] ) )->checksum3,
			 value_64bit );
			libcnotify_printf(
			 "%s: checksum3\t\t\t\t\t: 0x%08" PRIx64 "\n",
			 function,
			 value_64bit );

			byte_stream_copy_to_uint64_little_endian(
			 ( (esedb_extended_page_header_t *) &( data[ data_offset ] ) )->page_number,
			 value_64bit );
			libcnotify_printf(
			 "%s: page number\t\t\t\t\t: %" PRIu64 "\n",
			 function,
			 value_64bit );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		data_offset += sizeof( esedb_extended_page_header_t );
	}
	page_header->data_size = data_offset;

	return( 1 );
}

/* Retrieves the previous page number
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_header_get_previous_page_number(
     libesedb_page_header_t *page_header,
     uint32_t *previous_page_number,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_header_get_previous_page_number";

	if( page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page header.",
		 function );

		return( -1 );
	}
	if( previous_page_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid previous page number.",
		 function );

		return( -1 );
	}
	*previous_page_number = page_header->previous_page_number;

	return( 1 );
}

/* Retrieves the next page number
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_header_get_next_page_number(
     libesedb_page_header_t *page_header,
     uint32_t *next_page_number,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_header_get_next_page_number";

	if( page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page header.",
		 function );

		return( -1 );
	}
	if( next_page_number == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid next page number.",
		 function );

		return( -1 );
	}
	*next_page_number = page_header->next_page_number;

	return( 1 );
}

/* Retrieves the father data page object identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_header_get_father_data_page_object_identifier(
     libesedb_page_header_t *page_header,
     uint32_t *father_data_page_object_identifier,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_header_get_father_data_page_object_identifier";

	if( page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page header.",
		 function );

		return( -1 );
	}
	if( father_data_page_object_identifier == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid father data page object identifier.",
		 function );

		return( -1 );
	}
	*father_data_page_object_identifier = page_header->father_data_page_object_identifier;

	return( 1 );
}

/* Retrieves the flags
 * Returns 1 if successful or -1 on error
 */
int libesedb_page_header_get_flags(
     libesedb_page_header_t *page_header,
     uint32_t *flags,
     libcerror_error_t **error )
{
	static char *function = "libesedb_page_header_get_flags";

	if( page_header == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page header.",
		 function );

		return( -1 );
	}
	if( flags == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid flags.",
		 function );

		return( -1 );
	}
	*flags = page_header->flags;

	return( 1 );
}

