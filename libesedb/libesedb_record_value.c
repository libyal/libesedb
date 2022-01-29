/*
 * Record value functions
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
#include <types.h>

#include "libesedb_compression.h"
#include "libesedb_definitions.h"
#include "libesedb_record_value.h"

/* Retrieves the size of an UTF-8 string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_value_get_utf8_string_size(
     libfvalue_value_t *record_value,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *entry_data    = NULL;
	static char *function  = "libesedb_record_value_get_utf8_string_size";
	size_t entry_data_size = 0;
	uint32_t data_flags    = 0;
	int encoding           = 0;
	int result             = 0;

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
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if record value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data_flags(
		     record_value,
		     &data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data flags.",
			 function );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data flags: 0x%02" PRIx32 ".",
			 function,
			 data_flags );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_COMPRESSED ) != 0 )
		{
			if( libfvalue_value_get_entry_data(
			     record_value,
			     0,
			     &entry_data,
			     &entry_data_size,
			     &encoding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record value entry data.",
				 function );

				return( -1 );
			}
			result = libesedb_compression_get_utf8_string_size(
			          entry_data,
			          entry_data_size,
			          utf8_string_size,
			          error );
		}
		else
		{
			result = libfvalue_value_get_utf8_string_size(
			          record_value,
			          0,
			          utf8_string_size,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-8 encoded string
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_value_get_utf8_string(
     libfvalue_value_t *record_value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	uint8_t *entry_data    = NULL;
	static char *function  = "libesedb_record_value_get_utf8_string";
	size_t entry_data_size = 0;
	uint32_t data_flags    = 0;
	int encoding           = 0;
	int result             = 0;

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
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if record value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data_flags(
		     record_value,
		     &data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data flags.",
			 function );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data flags: 0x%02" PRIx32 ".",
			 function,
			 data_flags );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_COMPRESSED ) != 0 )
		{
			if( libfvalue_value_get_entry_data(
			     record_value,
			     0,
			     &entry_data,
			     &entry_data_size,
			     &encoding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record value entry data.",
				 function );

				return( -1 );
			}
			result = libesedb_compression_copy_to_utf8_string(
			          entry_data,
			          entry_data_size,
			          utf8_string,
			          utf8_string_size,
			          error );
		}
		else
		{
			result = libfvalue_value_copy_to_utf8_string(
			          record_value,
			          0,
			          utf8_string,
			          utf8_string_size,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-8 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the size of an UTF-16 string
 * The returned size includes the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_value_get_utf16_string_size(
     libfvalue_value_t *record_value,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *entry_data    = NULL;
	static char *function  = "libesedb_record_value_get_utf16_string_size";
	size_t entry_data_size = 0;
	uint32_t data_flags    = 0;
	int encoding           = 0;
	int result             = 0;

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
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if record value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data_flags(
		     record_value,
		     &data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data flags.",
			 function );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data flags: 0x%02" PRIx32 ".",
			 function,
			 data_flags );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_COMPRESSED ) != 0 )
		{
			if( libfvalue_value_get_entry_data(
			     record_value,
			     0,
			     &entry_data,
			     &entry_data_size,
			     &encoding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record value entry data.",
				 function );

				return( -1 );
			}
			result = libesedb_compression_get_utf16_string_size(
			          entry_data,
			          entry_data_size,
			          utf16_string_size,
			          error );
		}
		else
		{
			result = libfvalue_value_get_utf16_string_size(
			          record_value,
			          0,
			          utf16_string_size,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the UTF-16 encoded string
 * The size should include the end of string character
 * Returns 1 if successful, 0 if value is NULL or -1 on error
 */
int libesedb_record_value_get_utf16_string(
     libfvalue_value_t *record_value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	uint8_t *entry_data    = NULL;
	static char *function  = "libesedb_record_value_get_utf16_string";
	size_t entry_data_size = 0;
	uint32_t data_flags    = 0;
	int encoding           = 0;
	int result             = 0;

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
	result = libfvalue_value_has_data(
	          record_value,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if record value has data.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( libfvalue_value_get_data_flags(
		     record_value,
		     &data_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data flags.",
			 function );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_MULTI_VALUE ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported data flags: 0x%02" PRIx32 ".",
			 function,
			 data_flags );

			return( -1 );
		}
		if( ( data_flags & LIBESEDB_VALUE_FLAG_COMPRESSED ) != 0 )
		{
			if( libfvalue_value_get_entry_data(
			     record_value,
			     0,
			     &entry_data,
			     &entry_data_size,
			     &encoding,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve record value entry data.",
				 function );

				return( -1 );
			}
			result = libesedb_compression_copy_to_utf16_string(
			          entry_data,
			          entry_data_size,
			          utf16_string,
			          utf16_string_size,
			          error );
		}
		else
		{
			result = libfvalue_value_copy_to_utf16_string(
			          record_value,
			          0,
			          utf16_string,
			          utf16_string_size,
			          error );
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
			 "%s: unable to copy value to UTF-16 string.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

