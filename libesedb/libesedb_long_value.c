/*
 * Long value functions
 *
 * Copyright (c) 2009-2012, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_long_value.h"
#include "libesedb_values_tree.h"
#include "libesedb_values_tree_value.h"

/* Creates a long value
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_initialize(
     libesedb_long_value_t **long_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_catalog_definition_t *column_catalog_definition,
     libfdata_vector_t *long_values_pages_vector,
     libfcache_cache_t *long_values_pages_cache,
     libfdata_tree_t *long_values_tree,
     libfcache_cache_t *long_values_cache,
     uint8_t *long_value_key,
     size_t long_value_key_size,
     uint8_t flags,
     libcerror_error_t **error )
{
	uint8_t long_value_segment_key[ 8 ];

	libesedb_internal_long_value_t *internal_long_value = NULL;
	libesedb_values_tree_value_t *values_tree_value     = NULL;
	static char *function                               = "libesedb_long_value_initialize";
	uint32_t long_value_segment_offset                  = 0;
	int result                                          = 0;

	if( long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	if( *long_value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid long value value already set.",
		 function );

		return( -1 );
	}
	if( column_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid column catalog definition.",
		 function );

		return( -1 );
	}
	if( long_value_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value key.",
		 function );

		return( -1 );
	}
	if( long_value_key_size != 4 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupport long value key size: %" PRIzd ".",
		 function,
		 long_value_key_size );

		return( -1 );
	}
	if( ( flags & ~( LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags: 0x%02" PRIx8 ".",
		 function,
		 flags );

		return( -1 );
	}
	internal_long_value = memory_allocate_structure(
	                       libesedb_internal_long_value_t );

	if( internal_long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create long value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_long_value,
	     0,
	     sizeof( libesedb_internal_long_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear long value.",
		 function );

		memory_free(
		 internal_long_value );

		return( -1 );
	}
	if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) == 0 )
	{
		internal_long_value->file_io_handle = file_io_handle;
	}
	else
	{
		if( libbfio_handle_clone(
		     &( internal_long_value->file_io_handle ),
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy file IO handle.",
			 function );

			goto on_error;
		}
		if( libbfio_handle_set_open_on_demand(
		     internal_long_value->file_io_handle,
		     1,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to set open on demand in file IO handle.",
			 function );

			goto on_error;
		}
	}
	if( libfdata_block_initialize(
	     &( internal_long_value->data_block ),
	     NULL,
	     NULL,
	     NULL,
	     &libfdata_block_read_segment_data,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_long_value->data_cache ),
	     LIBESEDB_MAXIMUM_CACHE_ENTRIES_LONG_VALUES_DATA,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data cache.",
		 function );

		goto on_error;
	}
	if( libesedb_values_tree_get_value_by_key(
	     long_values_tree,
	     internal_long_value->file_io_handle,
	     long_values_cache,
	     long_value_key,
	     long_value_key_size,
	     &values_tree_value,
	     LIBESEDB_PAGE_KEY_FLAG_REVERSED_KEY,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve values tree value.",
		 function );

		goto on_error;
	}
	if( libesedb_values_tree_value_read_long_value(
	     values_tree_value,
	     internal_long_value->file_io_handle,
	     long_values_pages_vector,
	     long_values_pages_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read values tree value long value.",
		 function );

		goto on_error;
	}
	/* Reverse the reversed-key
	 */
	long_value_segment_key[ 0 ] = long_value_key[ 3 ];
	long_value_segment_key[ 1 ] = long_value_key[ 2 ];
	long_value_segment_key[ 2 ] = long_value_key[ 1 ];
	long_value_segment_key[ 3 ] = long_value_key[ 0 ];

	do
	{
		byte_stream_copy_from_uint32_big_endian(
		 &( long_value_segment_key[ 4 ] ),
		 long_value_segment_offset );

		result = libesedb_values_tree_get_value_by_key(
			  long_values_tree,
			  internal_long_value->file_io_handle,
			  long_values_cache,
			  long_value_segment_key,
			  8,
			  &values_tree_value,
			  0,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve values tree value.",
			 function );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libesedb_values_tree_value_read_long_value_segment(
			     values_tree_value,
			     internal_long_value->file_io_handle,
			     io_handle,
			     long_values_pages_vector,
			     long_values_pages_cache,
			     long_value_segment_offset,
			     internal_long_value->data_block,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read values tree value long value.",
				 function );

				goto on_error;
			}
			long_value_segment_offset += values_tree_value->data_size;
		}
	}
	while( result == 1 );

	internal_long_value->column_catalog_definition = column_catalog_definition;
	internal_long_value->flags                     = flags;

	*long_value = (libesedb_long_value_t *) internal_long_value;

	return( 1 );

on_error:
	if( internal_long_value != NULL )
	{
		if( internal_long_value->data_cache != NULL )
		{
			libfcache_cache_free(
			 &( internal_long_value->data_cache ),
			 NULL );
		}
		if( internal_long_value->data_block != NULL )
		{
			libfdata_block_free(
			 &( internal_long_value->data_block ),
			 NULL );
		}
		if( internal_long_value->file_io_handle != NULL )
		{
			if( ( flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
			{
				libbfio_handle_free(
				 &( internal_long_value->file_io_handle ),
				 NULL );
			}
		}
		memory_free(
		 internal_long_value );
	}
	return( -1 );
}

/* Frees the long value
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_free(
     libesedb_long_value_t **long_value,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_free";
	int result                                          = 1;

	if( long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	if( *long_value != NULL )
	{
		internal_long_value = (libesedb_internal_long_value_t *) *long_value;
		*long_value         = NULL;

		if( ( internal_long_value->flags & LIBESEDB_ITEM_FLAG_MANAGED_FILE_IO_HANDLE ) != 0 )
		{
			if( internal_long_value->file_io_handle != NULL )
			{
				if( libbfio_handle_close(
				     internal_long_value->file_io_handle,
				     error ) != 0 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_IO,
					 LIBCERROR_IO_ERROR_CLOSE_FAILED,
					 "%s: unable to close file IO handle.",
					 function );

					result = -1;
				}
				if( libbfio_handle_free(
				     &( internal_long_value->file_io_handle ),
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
					 "%s: unable to free file IO handle.",
					 function );

					result = -1;
				}
			}
		}
		if( libfdata_block_free(
		     &( internal_long_value->data_block ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data block.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( internal_long_value->data_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data cache.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_long_value );
	}
	return( result );
}

/* Retrieve the number of data segments
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_number_of_segments(
     libesedb_long_value_t *long_value,
     int *number_of_segments,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_get_number_of_segments";

	if( long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	internal_long_value = (libesedb_internal_long_value_t *) long_value;

	if( libfdata_block_get_number_of_segments(
	     internal_long_value->data_block,
	     number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of segments.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieve the segment data
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_segment_data(
     libesedb_long_value_t *long_value,
     int data_segment_index,
     uint8_t **segment_data,
     size_t *segment_data_size,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_get_segment_data";

	if( long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	internal_long_value = (libesedb_internal_long_value_t *) long_value;

	if( libfdata_block_get_segment_data(
	     internal_long_value->data_block,
	     internal_long_value->file_io_handle,
	     internal_long_value->data_cache,
	     data_segment_index,
	     segment_data,
	     segment_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve segment data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

