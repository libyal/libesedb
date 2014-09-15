/*
 * Long value functions
 *
 * Copyright (C) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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
#include "libesedb_data_segment.h"
#include "libesedb_definitions.h"
#include "libesedb_key.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_long_value.h"

/* Creates a long value
 * Make sure the value long_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_initialize(
     libesedb_long_value_t **long_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_catalog_definition_t *column_catalog_definition,
     libfdata_vector_t *long_values_pages_vector,
     libfcache_cache_t *long_values_pages_cache,
     libfdata_btree_t *long_values_tree,
     libfcache_cache_t *long_values_cache,
     uint8_t *long_value_key,
     size_t long_value_key_size,
     libcerror_error_t **error )
{
	uint8_t long_value_segment_key[ 8 ];

	libesedb_data_definition_t *data_definition         = NULL;
	libesedb_internal_long_value_t *internal_long_value = NULL;
	libesedb_key_t *key                                 = NULL;
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
	if( libfdata_list_initialize(
	     &( internal_long_value->data_segments_list ),
	     NULL,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfcache_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libesedb_data_segment_read_element_data,
	     NULL,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data segments list.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_long_value->data_segments_cache ),
	     LIBESEDB_MAXIMUM_CACHE_ENTRIES_LONG_VALUES_DATA,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data segments cache.",
		 function );

		goto on_error;
	}
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
	if( libesedb_key_set_data(
	     key,
	     long_value_key,
	     long_value_key_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set long value key data in key.",
		 function );

		goto on_error;
	}
	key->type = LIBESEDB_KEY_TYPE_LONG_VALUE;

	if( libfdata_btree_get_leaf_value_by_key(
	     long_values_tree,
	     (intptr_t *) file_io_handle,
	     long_values_cache,
	     (intptr_t *) key,
	     (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libesedb_key_compare,
	     (intptr_t **) &data_definition,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve leaf value by key.",
		 function );

		goto on_error;
	}
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
	if( libesedb_data_definition_read_long_value(
	     data_definition,
	     file_io_handle,
	     long_values_pages_vector,
	     long_values_pages_cache,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data definition long value.",
		 function );

		goto on_error;
	}
	/* Reverse the reversed long value key
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
		if( libesedb_key_set_data(
		     key,
		     long_value_segment_key,
		     8,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set long value segment key data in key.",
			 function );

			goto on_error;
		}
		key->type = LIBESEDB_KEY_TYPE_LONG_VALUE_SEGMENT;

		result = libfdata_btree_get_leaf_value_by_key(
			  long_values_tree,
			  (intptr_t *) file_io_handle,
			  long_values_cache,
		          (intptr_t *) key,
		          (int (*)(intptr_t *, intptr_t *, libcerror_error_t **)) &libesedb_key_compare,
		          (intptr_t **) &data_definition,
			  0,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve leaf value by key.",
			 function );

			goto on_error;
		}
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
		if( result != 0 )
		{
			if( libesedb_data_definition_read_long_value_segment(
			     data_definition,
			     file_io_handle,
			     io_handle,
			     long_values_pages_vector,
			     long_values_pages_cache,
			     long_value_segment_offset,
			     internal_long_value->data_segments_list,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read data definition long value segment.",
				 function );

				goto on_error;
			}
			long_value_segment_offset += data_definition->data_size;
		}
	}
	while( result == 1 );

	internal_long_value->file_io_handle            = file_io_handle;
	internal_long_value->column_catalog_definition = column_catalog_definition;

	*long_value = (libesedb_long_value_t *) internal_long_value;

	return( 1 );

on_error:
	if( key != NULL )
	{
		libesedb_key_free(
		 &key,
		 NULL );
	}
	if( internal_long_value != NULL )
	{
		if( internal_long_value->data_segments_cache != NULL )
		{
			libfcache_cache_free(
			 &( internal_long_value->data_segments_cache ),
			 NULL );
		}
		if( internal_long_value->data_segments_list != NULL )
		{
			libfdata_list_free(
			 &( internal_long_value->data_segments_list ),
			 NULL );
		}
		memory_free(
		 internal_long_value );
	}
	return( -1 );
}

/* Frees a long value
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

		/* The file_io_handle reference is freed elsewhere
		 */
		if( libfdata_list_free(
		     &( internal_long_value->data_segments_list ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data segments list.",
			 function );

			result = -1;
		}
		if( libfcache_cache_free(
		     &( internal_long_value->data_segments_cache ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data segments cache.",
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

	if( libfdata_list_get_number_of_elements(
	     internal_long_value->data_segments_list,
	     number_of_segments,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of elements from data segments list.",
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
	libesedb_data_segment_t *data_segment               = NULL;
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

	if( segment_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data.",
		 function );

		return( -1 );
	}
	if( segment_data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid segment data size.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_element_value_by_index(
	     internal_long_value->data_segments_list,
	     (intptr_t *) internal_long_value->file_io_handle,
	     internal_long_value->data_segments_cache,
	     data_segment_index,
	     (intptr_t **) &data_segment,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data segment: %d.",
		 function,
		 data_segment_index );

		return( -1 );
	}
	if( data_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing data segment: %d.",
		 function,
		 data_segment_index );

		return( -1 );
	}
	*segment_data      = data_segment->data;
	*segment_data_size = data_segment->data_size;

	return( 1 );
}

