/*
 * Value data handle functions
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
#include <memory.h>
#include <types.h>

#include "libesedb_definitions.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfvalue.h"
#include "libesedb_value_data_handle.h"
#include "libesedb_unused.h"

/* Retrieves the number of value entries
 * Return if successful or -1 on error
 */
int libesedb_value_data_handle_read_value_entries(
     libfvalue_data_handle_t *data_handle,
     const uint8_t *data,
     size_t data_size,
     int encoding LIBESEDB_ATTRIBUTE_UNUSED,
     uint32_t data_flags,
     libcerror_error_t **error )
{
	static char *function                  = "libesedb_value_data_handle_read_value_entries";
	size_t data_offset                     = 0;
	uint16_t number_of_value_entry_offsets = 0;
	uint16_t value_16bit                   = 0;
	uint16_t value_entry_offset            = 0;
	uint16_t value_entry_offset_index      = 0;
	uint16_t value_entry_size              = 0;
	int value_entry_index                  = 0;

	LIBESEDB_UNREFERENCED_PARAMETER( encoding )

	if( data_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data handle.",
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
	if( data_size > (size_t) UINT16_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: multi value data:\n",
			 function );
			libcnotify_print_data(
			 data,
			 data_size,
			 0 );
		}
#endif
		/* The first 2 bytes contain the offset to the first value
		 * there is an offset for every value
		 * therefore first offset / 2 = the number of value entries
		 */
		byte_stream_copy_to_uint16_little_endian(
		 &( data[ data_offset ] ),
		 value_16bit );

		data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: multi value offset: %03" PRIu16 "\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
			 function,
			 value_entry_offset_index,
			 value_16bit,
			 value_16bit & 0x7fff );
		}
#endif
		value_entry_offset = value_16bit & 0x7fff;

		number_of_value_entry_offsets = value_entry_offset / 2;

		if( number_of_value_entry_offsets == 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value entries.",
			 function );

			return( -1 );
		}
		for( value_entry_offset_index = 1;
		     value_entry_offset_index < number_of_value_entry_offsets;
		     value_entry_offset_index++ )
		{
			byte_stream_copy_to_uint16_little_endian(
			 &( data[ data_offset ] ),
			 value_16bit );

			data_offset += 2;

#if defined( HAVE_DEBUG_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: multi value offset: %03" PRIu16 "\t: 0x%04" PRIx16 " (%" PRIu16 ")\n",
				 function,
				 value_entry_offset_index,
				 value_16bit,
				 value_16bit & 0x7fff );
			}
#endif
			value_16bit &= 0x7fff;

			if( value_16bit < value_entry_offset )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid value offset: %" PRIu16 " value is smaller than previous.",
				 function,
				 value_entry_offset_index );

				return( -1 );
			}
			value_entry_size = value_16bit - value_entry_offset;

			if( libfvalue_data_handle_append_value_entry(
			     data_handle,
			     &value_entry_index,
			     (size_t) value_entry_offset,
			     (size_t) value_entry_size,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable append value entry: %" PRIu16 ".",
				 function,
				 value_entry_offset_index - 1 );

				return( -1 );
			}
			value_entry_offset = value_16bit;
		}
		value_entry_size = (uint16_t) data_size - value_entry_offset;

		if( libfvalue_data_handle_append_value_entry(
		     data_handle,
		     &value_entry_index,
		     (size_t) value_entry_offset,
		     (size_t) value_entry_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable append value entry: %" PRIu16 ".",
			 function,
			 value_entry_offset_index - 1 );

			return( -1 );
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "\n" );
		}
#endif
	}
	return( 1 );
}

