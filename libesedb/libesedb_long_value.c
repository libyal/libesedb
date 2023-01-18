/*
 * Long value functions
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

#include "libesedb_compression.h"
#include "libesedb_data_segment.h"
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_libfvalue.h"
#include "libesedb_long_value.h"
#include "libesedb_record_value.h"

/* Creates a long value
 * Make sure the value long_value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_initialize(
     libesedb_long_value_t **long_value,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libesedb_catalog_definition_t *column_catalog_definition,
     libfdata_list_t *data_segments_list,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_initialize";

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
	if( data_segments_list == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data segments list.",
		 function );

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
	internal_long_value->file_io_handle            = file_io_handle;
	internal_long_value->io_handle                 = io_handle;
	internal_long_value->column_catalog_definition = column_catalog_definition;
	internal_long_value->data_segments_list        = data_segments_list;

	*long_value = (libesedb_long_value_t *) internal_long_value;

	return( 1 );

on_error:
	if( internal_long_value != NULL )
	{
		if( internal_long_value->data_segments_cache != NULL )
		{
			libfcache_cache_free(
			 &( internal_long_value->data_segments_cache ),
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

		/* The file_io_handle and io_handle references are freed elsewhere
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
		if( internal_long_value->record_value != NULL )
		{
			if( libfvalue_value_free(
			     &( internal_long_value->record_value ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free record value.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_long_value );
	}
	return( result );
}

/* Retrieve the data size
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_data_size(
     libesedb_long_value_t *long_value,
     size64_t *data_size,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_get_data_size";

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

	if( libfdata_list_get_size(
	     internal_long_value->data_segments_list,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size from data segments list.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieve the data
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_data(
     libesedb_long_value_t *long_value,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libesedb_data_segment_t *data_segment               = NULL;
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_get_data";
	size64_t data_segments_size                         = 0;
	size_t data_offset                                  = 0;
	int data_segment_index                              = 0;
	int number_of_data_segments                         = 0;

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
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_size(
	     internal_long_value->data_segments_list,
	     &data_segments_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve size from data segments list.",
		 function );

		return( -1 );
	}
	if( (size64_t) data_size < data_segments_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data size value too small.",
		 function );

		return( -1 );
	}
	if( libfdata_list_get_number_of_elements(
	     internal_long_value->data_segments_list,
	     &number_of_data_segments,
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
	for( data_segment_index = 0;
	     data_segment_index < number_of_data_segments;
	     data_segment_index++ )
	{
		if( libfdata_list_get_element_value_by_index(
		     internal_long_value->data_segments_list,
		     (intptr_t *) internal_long_value->file_io_handle,
		     (libfdata_cache_t *) internal_long_value->data_segments_cache,
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
		if( memory_copy(
		     &( data[ data_offset ] ),
		     data_segment->data,
		     data_segment->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			return( -1 );
		}
		data_offset += data_segment->data_size;
	}
	return( 1 );
}

/* Retrieve the record value
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_record_value(
     libesedb_internal_long_value_t *internal_long_value,
     libfvalue_value_t **record_value,
     libcerror_error_t **error )
{
	libesedb_data_segment_t *data_segment = NULL;
	uint8_t *compressed_data              = NULL;
	uint8_t *data                         = NULL;
	static char *function                 = "libesedb_long_value_get_record_value";
	size64_t data_size                    = 0;
	size_t compressed_data_size           = 0;
	size_t data_offset                    = 0;
	uint32_t column_type                  = 0;
	uint8_t record_value_type             = 0;
	int data_segment_index                = 0;
	int encoding                          = 0;
	int long_value_codepage               = 0;
	int number_of_data_segments           = 0;

	if( internal_long_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	if( internal_long_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid long value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( record_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record value.",
		 function );

		return( -1 );
	}
	if( internal_long_value->record_value == NULL )
	{
		if( libfdata_list_get_size(
		     internal_long_value->data_segments_list,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve size from data segments list.",
			 function );

			goto on_error;
		}
		if( ( data_size == 0 )
		 || ( data_size > (size64_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		data = (uint8_t *) memory_allocate(
		                    (size_t) data_size );

		if( data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( libfdata_list_get_number_of_elements(
		     internal_long_value->data_segments_list,
		     &number_of_data_segments,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of elements from data segments list.",
			 function );

			goto on_error;
		}
		for( data_segment_index = 0;
		     data_segment_index < number_of_data_segments;
		     data_segment_index++ )
		{
			if( libfdata_list_get_element_value_by_index(
			     internal_long_value->data_segments_list,
			     (intptr_t *) internal_long_value->file_io_handle,
			     (libfdata_cache_t *) internal_long_value->data_segments_cache,
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

				goto on_error;
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

				goto on_error;
			}
			if( memory_copy(
			     &( data[ data_offset ] ),
			     data_segment->data,
			     data_segment->data_size ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy data.",
				 function );

				goto on_error;
			}
			data_offset += data_segment->data_size;
		}
		if( libesedb_catalog_definition_get_column_type(
		     internal_long_value->column_catalog_definition,
		     &column_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve catalog definition column type.",
			 function );

			goto on_error;
		}
		switch( column_type )
		{
			case LIBESEDB_COLUMN_TYPE_BINARY_DATA:
			case LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA:
				record_value_type = LIBFVALUE_VALUE_TYPE_BINARY_DATA;
				break;

			case LIBESEDB_COLUMN_TYPE_TEXT:
			case LIBESEDB_COLUMN_TYPE_LARGE_TEXT:
				record_value_type = LIBFVALUE_VALUE_TYPE_STRING_BYTE_STREAM;
				break;

			default:
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported column type: %" PRIu32 ".",
				 function,
				 column_type );

				goto on_error;
		}
		if( ( column_type == LIBESEDB_COLUMN_TYPE_TEXT )
		 || ( column_type == LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
		{
			long_value_codepage = (int) internal_long_value->column_catalog_definition->codepage;

			/* If the codepage is not set use the default codepage
			 */
			if( long_value_codepage == 0 )
			{
				long_value_codepage = internal_long_value->io_handle->ascii_codepage;
			}
			if( ( data_size > 1 )
			 && ( data[ 0 ] == 0x18 ) )
			{
				compressed_data      = data;
				compressed_data_size = data_size;
				data                 = NULL;
				data_size            = 0;

				if( libesedb_compression_lzxpress_decompress_get_size(
				     compressed_data,
				     compressed_data_size,
				     (size_t *) &data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable retrieve uncompressed data size.",
					 function );

					goto on_error;
				}
				if( ( data_size == 0 )
				 || ( data_size > MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
					 "%s: invalid uncompressed data size value out of bounds.",
					 function );

					goto on_error;
				}
				data = (uint8_t *) memory_allocate(
				                    sizeof( uint8_t ) * data_size );

				if( data == NULL )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create uncompressed data.",
					 function );

					goto on_error;
				}
				if( libesedb_compression_lzxpress_decompress(
				     compressed_data,
				     compressed_data_size,
				     data,
				     data_size,
				     error ) != 1 )
				{
					libcerror_error_set(
					 error,
					 LIBCERROR_ERROR_DOMAIN_COMPRESSION,
					 LIBCERROR_COMPRESSION_ERROR_DECOMPRESS_FAILED,
					 "%s: unable decompressed data.",
					 function );

					goto on_error;
				}
				memory_free(
				 compressed_data );

				compressed_data = NULL;

#if defined( HAVE_DEBUG_OUTPUT )
				if( libcnotify_verbose != 0 )
				{
					libcnotify_printf(
					 "%s: uncompressed data:\n",
					 function );
					libcnotify_print_data(
					 data,
					 data_size,
					 0 );
				}
#endif
			}
			encoding = long_value_codepage;
		}
		else
		{
			encoding = LIBFVALUE_ENDIAN_LITTLE;
		}
/* TODO create value */
		if( libfvalue_value_type_initialize(
		     &( internal_long_value->record_value ),
		     record_value_type,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create record value.",
			 function );

			goto on_error;
		}
		if( libfvalue_value_set_data(
		     internal_long_value->record_value,
		     data,
		     (size_t) data_size,
		     encoding,
		     LIBFVALUE_VALUE_DATA_FLAG_MANAGED | LIBFVALUE_VALUE_DATA_FLAG_CLONE_BY_REFERENCE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set data in record value.",
			 function );

			goto on_error;
		}
		/* The record value takes over management of data
		 */
		data = NULL;
	}
	*record_value = internal_long_value->record_value;

	return( 1 );

on_error:
	if( internal_long_value->record_value != NULL )
	{
		libfvalue_value_free(
		 &( internal_long_value->record_value ),
		 NULL );
	}
	if( compressed_data != NULL )
	{
		memory_free(
		 compressed_data );
	}
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	return( -1 );
}

/* Retrieve the number of data segments
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_number_of_data_segments(
     libesedb_long_value_t *long_value,
     int *number_of_data_segments,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_get_number_of_data_segments";

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
	     number_of_data_segments,
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

/* Retrieve the data segment size
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_data_segment_size(
     libesedb_long_value_t *long_value,
     int data_segment_index,
     size_t *data_size,
     libcerror_error_t **error )
{
	libesedb_data_segment_t *data_segment               = NULL;
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_get_data_segment_size";

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

	if( libfdata_list_get_element_value_by_index(
	     internal_long_value->data_segments_list,
	     (intptr_t *) internal_long_value->file_io_handle,
	     (libfdata_cache_t *) internal_long_value->data_segments_cache,
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
	if( libesedb_data_segment_get_data_size(
	     data_segment,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data segment: %d data size.",
		 function,
		 data_segment_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieve the data segment
 * Returns 1 if successful or -1 on error
 */
int libesedb_long_value_get_data_segment(
     libesedb_long_value_t *long_value,
     int data_segment_index,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	libesedb_data_segment_t *data_segment               = NULL;
	libesedb_internal_long_value_t *internal_long_value = NULL;
	static char *function                               = "libesedb_long_value_get_data_segment";

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

	if( libfdata_list_get_element_value_by_index(
	     internal_long_value->data_segments_list,
	     (intptr_t *) internal_long_value->file_io_handle,
	     (libfdata_cache_t *) internal_long_value->data_segments_cache,
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
	if( libesedb_data_segment_get_data(
	     data_segment,
	     data,
	     data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data segment: %d data.",
		 function,
		 data_segment_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the size of the data as an UTF-8 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_long_value_get_utf8_string_size(
     libesedb_long_value_t *long_value,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	libfvalue_value_t *record_value                     = NULL;
	static char *function                               = "libesedb_long_value_get_utf8_string_size";
	uint32_t column_type                                = 0;
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
	internal_long_value = (libesedb_internal_long_value_t *) long_value;

	if( libesedb_catalog_definition_get_column_type(
	     internal_long_value->column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_long_value_get_record_value(
	     internal_long_value,
	     &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record value.",
		 function );

		return( -1 );
	}
	result = libesedb_record_value_get_utf8_string_size(
	          record_value,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string size from record value.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the data as an UTF-8 encoded string
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_long_value_get_utf8_string(
     libesedb_long_value_t *long_value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	libfvalue_value_t *record_value                     = NULL;
	static char *function                               = "libesedb_long_value_get_utf8_string";
	uint32_t column_type                                = 0;
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
	internal_long_value = (libesedb_internal_long_value_t *) long_value;

	if( libesedb_catalog_definition_get_column_type(
	     internal_long_value->column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_long_value_get_record_value(
	     internal_long_value,
	     &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record value.",
		 function );

		return( -1 );
	}
	result = libesedb_record_value_get_utf8_string(
	          record_value,
	          utf8_string,
	          utf8_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string from record value.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the size of the data as an UTF-16 encoded string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_long_value_get_utf16_string_size(
     libesedb_long_value_t *long_value,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	libfvalue_value_t *record_value                     = NULL;
	static char *function                               = "libesedb_long_value_get_utf16_string_size";
	uint32_t column_type                                = 0;
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
	internal_long_value = (libesedb_internal_long_value_t *) long_value;

	if( libesedb_catalog_definition_get_column_type(
	     internal_long_value->column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_long_value_get_record_value(
	     internal_long_value,
	     &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record value.",
		 function );

		return( -1 );
	}
	result = libesedb_record_value_get_utf16_string_size(
	          record_value,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string size from record value.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Retrieves the data as an UTF-16 encoded string
 * The function uses the codepage in the column definition if necessary
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_long_value_get_utf16_string(
     libesedb_long_value_t *long_value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libesedb_internal_long_value_t *internal_long_value = NULL;
	libfvalue_value_t *record_value                     = NULL;
	static char *function                               = "libesedb_long_value_get_utf16_string";
	uint32_t column_type                                = 0;
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
	internal_long_value = (libesedb_internal_long_value_t *) long_value;

	if( libesedb_catalog_definition_get_column_type(
	     internal_long_value->column_catalog_definition,
	     &column_type,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve catalog definition column type.",
		 function );

		return( -1 );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported column type: %" PRIu32 ".",
		 function,
		 column_type );

		return( -1 );
	}
	if( libesedb_long_value_get_record_value(
	     internal_long_value,
	     &record_value,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve record value.",
		 function );

		return( -1 );
	}
	result = libesedb_record_value_get_utf16_string(
	          record_value,
	          utf16_string,
	          utf16_string_size,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string from record value.",
		 function );

		return( -1 );
	}
	return( result );
}

