/*
 * Data segment functions
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
#include <memory.h>
#include <types.h>

#include "libesedb_data_segment.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfdata.h"
#include "libesedb_unused.h"

/* Creates a data segment
 * Make sure the value data_segment is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_segment_initialize(
     libesedb_data_segment_t **data_segment,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_data_segment_initialize";

	if( data_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data segment.",
		 function );

		return( -1 );
	}
	if( *data_segment != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data segment value already set.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum allocation size.",
		 function );

		return( -1 );
	}
	*data_segment = memory_allocate_structure(
	                 libesedb_data_segment_t );

	if( *data_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data segment.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_segment,
	     0,
	     sizeof( libesedb_data_segment_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data segment.",
		 function );

		memory_free(
		 *data_segment );

		*data_segment = NULL;

		return( -1 );
	}
	if( data_size > 0 )
	{
		( *data_segment )->data = (uint8_t *) memory_allocate(
		                                       sizeof( uint8_t ) * data_size );

		if( ( *data_segment )->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		( *data_segment )->data_size = data_size;
	}
	return( 1 );

on_error:
	if( *data_segment != NULL )
	{
		memory_free(
		 *data_segment );

		*data_segment = NULL;
	}
	return( -1 );
}

/* Frees a data segment
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_segment_free(
     libesedb_data_segment_t **data_segment,
     libcerror_error_t **error )
{
	static char *function = "libesedb_data_segment_free";

	if( data_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data segment.",
		 function );

		return( -1 );
	}
	if( *data_segment != NULL )
	{
		if( ( *data_segment )->data != NULL )
		{
			memory_free(
			 ( *data_segment )->data );
		}
		memory_free(
		 *data_segment );

		*data_segment = NULL;
	}
	return( 1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_segment_get_data_size(
     libesedb_data_segment_t *data_segment,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_data_segment_get_data_size";

	if( data_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data segment.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = data_segment->data_size;

	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_segment_get_data(
     libesedb_data_segment_t *data_segment,
     uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function = "libesedb_data_segment_get_data";

	if( data_segment == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data segment.",
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
	if( data_size < data_segment->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: data size value too small.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     data,
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
	return( 1 );
}

/* Reads a data segment
 * Callback for the data segments list
 * Returns 1 if successful or -1 on error
 */
int libesedb_data_segment_read_element_data(
     intptr_t *data_handle LIBESEDB_ATTRIBUTE_UNUSED,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *element,
     libfdata_cache_t *cache,
     int element_file_index LIBESEDB_ATTRIBUTE_UNUSED,
     off64_t element_offset,
     size64_t element_size,
     uint32_t element_flags LIBESEDB_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBESEDB_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libesedb_data_segment_t *data_segment = NULL;
	static char *function                 = "libesedb_data_segment_read_element_data";
	ssize_t read_count                    = 0;

	LIBESEDB_UNREFERENCED_PARAMETER( data_handle )
	LIBESEDB_UNREFERENCED_PARAMETER( element_file_index )
	LIBESEDB_UNREFERENCED_PARAMETER( element_flags )
	LIBESEDB_UNREFERENCED_PARAMETER( read_flags )

	if( element_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element size value out of bounds.",
		 function );

		return( -1 );
	}
	if( libesedb_data_segment_initialize(
	     &data_segment,
	     (size_t) element_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data segment.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading data segment at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 element_offset,
		 element_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
		      file_io_handle,
		      data_segment->data,
		      data_segment->data_size,
		      element_offset,
		      error );

	if( read_count == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data segment at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 element_offset,
		 element_offset );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) data_segment,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libesedb_data_segment_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data segment as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_segment != NULL )
	{
		libesedb_data_segment_free(
		 &data_segment,
		 NULL );
	}
	return( -1 );
}

