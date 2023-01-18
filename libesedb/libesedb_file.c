/*
 * File functions
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
#include <narrow_string.h>
#include <types.h>
#include <wide_string.h>

#include "libesedb_catalog.h"
#include "libesedb_database.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_i18n.h"
#include "libesedb_io_handle.h"
#include "libesedb_file.h"
#include "libesedb_file_header.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libcnotify.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page.h"
#include "libesedb_table.h"
#include "libesedb_table_definition.h"

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_initialize(
     libesedb_file_t **file,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_initialize";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file value already set.",
		 function );

		return( -1 );
	}
	internal_file = memory_allocate_structure(
	                 libesedb_internal_file_t );

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_file,
	     0,
	     sizeof( libesedb_internal_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear file.",
		 function );

		memory_free(
		 internal_file );

		return( -1 );
	}
	if( libesedb_io_handle_initialize(
	     &( internal_file->io_handle ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( libesedb_i18n_initialize(
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initalize internationalization (i18n).",
		 function );

		goto on_error;
	}
	*file = (libesedb_file_t *) internal_file;

	return( 1 );

on_error:
	if( internal_file != NULL )
	{
		if( internal_file->io_handle != NULL )
		{
			libesedb_io_handle_free(
			 &( internal_file->io_handle ),
			 NULL );
		}
		memory_free(
		 internal_file );
	}
	return( -1 );
}

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_free(
     libesedb_file_t **file,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_free";
	int result                              = 1;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libesedb_internal_file_t *) *file;

		if( internal_file->file_io_handle != NULL )
		{
			if( libesedb_file_close(
			     *file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close file.",
				 function );

				result = -1;
			}
		}
		*file = NULL;

		if( libesedb_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free IO handle.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_file );
	}
	return( result );
}

/* Signals a file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_signal_abort(
     libesedb_file_t *file,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_signal_abort";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->abort = 1;

	return( 1 );
}

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open(
     libesedb_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle        = NULL;
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_open";
	size_t filename_length                  = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBESEDB_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBESEDB_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBESEDB_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = narrow_string_length(
	                   filename );

	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libesedb_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open_wide(
     libesedb_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error )
{
	libbfio_handle_t *file_io_handle        = NULL;
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_open_wide";
	size_t filename_length                  = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( filename == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBESEDB_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBESEDB_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBESEDB_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file IO handle.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file IO handle.",
                 function );

		goto on_error;
	}
#endif
	filename_length = wide_string_length(
	                   filename );

	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     filename_length + 1,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file IO handle.",
                 function );

		goto on_error;
	}
	if( libesedb_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     access_flags,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		goto on_error;
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( -1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open_file_io_handle(
     libesedb_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_open_file_io_handle";
	int bfio_access_flags                   = 0;
	int file_io_handle_is_open              = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( ( ( access_flags & LIBESEDB_ACCESS_FLAG_READ ) == 0 )
	 && ( ( access_flags & LIBESEDB_ACCESS_FLAG_WRITE ) == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported access flags.",
		 function );

		return( -1 );
	}
	if( ( access_flags & LIBESEDB_ACCESS_FLAG_WRITE ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( ( access_flags & LIBESEDB_ACCESS_FLAG_READ ) != 0 )
	{
		bfio_access_flags = LIBBFIO_ACCESS_FLAG_READ;
	}
	file_io_handle_is_open = libbfio_handle_is_open(
	                          file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     file_io_handle,
		     bfio_access_flags,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file IO handle.",
			 function );

			goto on_error;
		}
		internal_file->file_io_handle_opened_in_library = 1;
	}
	if( libesedb_file_open_read(
	     internal_file,
	     file_io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		goto on_error;
	}
	internal_file->file_io_handle = file_io_handle;

	return( 1 );

on_error:
	if( ( file_io_handle_is_open == 0 )
	 && ( internal_file->file_io_handle_opened_in_library != 0 ) )
	{
		libbfio_handle_close(
		 file_io_handle,
		 error );

		internal_file->file_io_handle_opened_in_library = 0;
	}
	return( -1 );
}

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
int libesedb_file_close(
     libesedb_file_t *file,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_close";
	int result                              = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( internal_file->file_io_handle_created_in_library != 0 )
		{
			if( libesedb_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
	}
#endif
	if( internal_file->file_io_handle_opened_in_library != 0 )
	{
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
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
		internal_file->file_io_handle_opened_in_library = 0;
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
		if( libbfio_handle_free(
		     &( internal_file->file_io_handle ),
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
		internal_file->file_io_handle_created_in_library = 0;
	}
	internal_file->file_io_handle = NULL;

	if( libesedb_io_handle_clear(
	     internal_file->io_handle,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		result = -1;
	}
	if( libfdata_vector_free(
	     &( internal_file->pages_vector ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free pages vector.",
		 function );

		result = -1;
	}
	if( libfcache_cache_free(
	     &( internal_file->pages_cache ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free pages cache.",
		 function );

		result = -1;
	}
	if( libesedb_database_free(
	     &( internal_file->database ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free database.",
		 function );

		result = -1;
	}
	if( libesedb_catalog_free(
	     &( internal_file->catalog ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free catalog.",
		 function );

		result = -1;
	}
	if( libesedb_catalog_free(
	     &( internal_file->backup_catalog ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free backup catalog.",
		 function );

		result = -1;
	}
	return( result );
}

/* Opens a file for reading
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open_read(
     libesedb_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	libesedb_file_header_t *file_header = NULL;
	static char *function               = "libesedb_file_open_read";
	size64_t file_size                  = 0;
	off64_t file_offset                 = 0;
	int result                          = 0;
	int segment_index                   = 0;

	if( internal_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->pages_vector != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - pages vector already set.",
		 function );

		return( -1 );
	}
	if( internal_file->pages_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - pages cache already set.",
		 function );

		return( -1 );
	}
	if( internal_file->database != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - database already set.",
		 function );

		return( -1 );
	}
	if( internal_file->catalog != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - catalog already set.",
		 function );

		return( -1 );
	}
	if( internal_file->backup_catalog != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid file - backup catalog already set.",
		 function );

		return( -1 );
	}
	internal_file->io_handle->abort = 0;

	if( libbfio_handle_get_size(
	     file_io_handle,
	     &file_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading file header:\n" );
	}
#endif
	if( libesedb_file_header_initialize(
	     &file_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file header.",
		 function );

		goto on_error;
	}
	if( libesedb_file_header_read_file_io_handle(
	     file_header,
	     file_io_handle,
	     file_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		goto on_error;
	}
	internal_file->io_handle->file_type                = file_header->file_type;
	internal_file->io_handle->format_version           = file_header->format_version;
	internal_file->io_handle->format_revision          = file_header->format_revision;
	internal_file->io_handle->page_size                = file_header->page_size;
	internal_file->io_handle->creation_format_version  = file_header->creation_format_version;
	internal_file->io_handle->creation_format_revision = file_header->creation_format_revision;

	if( libesedb_file_header_free(
	     &file_header,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file header.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "Reading backup file header:\n" );
	}
#endif
	if( internal_file->io_handle->page_size != 0 )
	{
		if( libesedb_file_header_initialize(
		     &file_header,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create backup file header.",
			 function );

			goto on_error;
		}
		result = libesedb_file_header_read_file_io_handle(
		          file_header,
		          file_io_handle,
		          (off64_t) internal_file->io_handle->page_size,
		          error );

		if( result != 1 )
		{
			file_offset = 0x0800;

			while( file_offset <= 0x8000 )
			{
#if defined( HAVE_DEBUG_OUTPUT )
				if( ( libcnotify_verbose != 0 )
				 && ( error != NULL )
				 && ( *error != NULL ) )
				{
					libcnotify_print_error_backtrace(
					 *error );
				}
#endif
				libcerror_error_free(
				 error );

				result = libesedb_file_header_read_file_io_handle(
				          file_header,
				          file_io_handle,
				          (off64_t) internal_file->io_handle->page_size,
				          error );

				if( result == 1 )
				{
					break;
				}
				file_offset <<= 1;
			}
		}
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read backup file header.",
			 function );

			goto on_error;
		}
		if( internal_file->io_handle->format_version == 0 )
		{
			internal_file->io_handle->format_version = file_header->format_version;
		}
		else if( internal_file->io_handle->format_version != file_header->format_version )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in format version: 0x%" PRIx32 " and backup: 0x%" PRIx32 "\n",
				 function,
				 internal_file->io_handle->format_version,
				 file_header->format_version );
			}
#endif
		}
		if( internal_file->io_handle->format_revision == 0 )
		{
			internal_file->io_handle->format_revision = file_header->format_revision;
		}
		else if( internal_file->io_handle->format_revision != file_header->format_revision )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in format revision: 0x%" PRIx32 " and backup: 0x%" PRIx32 "\n",
				 function,
				 internal_file->io_handle->format_revision,
				 file_header->format_revision );
			}
#endif
		}
		if( internal_file->io_handle->page_size == 0 )
		{
			internal_file->io_handle->page_size = file_header->page_size;
		}
		else if( internal_file->io_handle->page_size != file_header->page_size )
		{
#if defined( HAVE_VERBOSE_OUTPUT )
			if( libcnotify_verbose != 0 )
			{
				libcnotify_printf(
				 "%s: mismatch in page size: 0x%04" PRIx32 " and backup: 0x%04" PRIx32 "\n",
				 function,
				 internal_file->io_handle->page_size,
				 file_header->page_size );
			}
#endif
			/* The offset of the backup (database) file header
			 * is a good indication of the actual page size
			 */
			internal_file->io_handle->page_size = (uint32_t) file_offset;
		}
		if( libesedb_file_header_free(
		     &file_header,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free backup file header.",
			 function );

			goto on_error;
		}
	}
	if( internal_file->io_handle->format_version != 0x620 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported format version: 0x%04" PRIx32 ".",
		 function,
		 internal_file->io_handle->format_version );

		goto on_error;
	}
	if( internal_file->io_handle->page_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid page size.",
		 function );

		goto on_error;
	}
	if( internal_file->io_handle->format_version == 0x620 )
	{
		if( internal_file->io_handle->format_revision < 0x11 )
		{
			if( ( internal_file->io_handle->page_size != 0x1000 )
			 && ( internal_file->io_handle->page_size != 0x2000 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page size: %" PRIu32 " (0x%04" PRIx32 ") for format version: 0x%" PRIx32 " revision: 0x%" PRIx32 ".",
				 function,
				 internal_file->io_handle->page_size,
				 internal_file->io_handle->page_size,
				 internal_file->io_handle->format_version,
				 internal_file->io_handle->format_revision );

				goto on_error;
			}
		}
		else
		{
			if( ( internal_file->io_handle->page_size != 0x0800 )
			 && ( internal_file->io_handle->page_size != 0x1000 )
			 && ( internal_file->io_handle->page_size != 0x2000 )
			 && ( internal_file->io_handle->page_size != 0x4000 )
			 && ( internal_file->io_handle->page_size != 0x8000 ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported page size: %" PRIu32 " (0x%04" PRIx32 ") for format version: 0x%" PRIx32 " revision: 0x%" PRIx32 ".",
				 function,
				 internal_file->io_handle->page_size,
				 internal_file->io_handle->page_size,
				 internal_file->io_handle->format_version,
				 internal_file->io_handle->format_revision );

				goto on_error;
			}
		}
	}
	if( libesedb_io_handle_set_pages_data_range(
	     internal_file->io_handle,
	     file_size,
	     error ) != 1 )
	{
                libcerror_error_set(
                 error,
                 LIBCERROR_ERROR_DOMAIN_RUNTIME,
                 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set pages data range in IO handle.",
                 function );

		goto on_error;
	}
/* TODO add clone function ? */
	if( libfdata_vector_initialize(
	     &( internal_file->pages_vector ),
	     (size64_t) internal_file->io_handle->page_size,
	     (intptr_t *) internal_file->io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_vector_t *, libfdata_cache_t *, int, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libesedb_io_handle_read_page,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create pages vector.",
		 function );

		goto on_error;
	}
	if( libfdata_vector_append_segment(
	     internal_file->pages_vector,
	     &segment_index,
	     0,
	     internal_file->io_handle->pages_data_offset,
	     internal_file->io_handle->pages_data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to pages vector.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( internal_file->pages_cache ),
	     LIBESEDB_MAXIMUM_CACHE_ENTRIES_PAGES,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create pages cache.",
		 function );

		goto on_error;
	}
	if( internal_file->io_handle->file_type == LIBESEDB_FILE_TYPE_DATABASE )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading the database:\n" );
		}
#endif
		if( libesedb_database_initialize(
		     &( internal_file->database ),
		     internal_file->io_handle,
		     internal_file->pages_vector,
		     internal_file->pages_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create database.",
			 function );

			goto on_error;
		}
		if( libesedb_database_read_file_io_handle(
		     internal_file->database,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read database.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading the catalog:\n" );
		}
#endif
		if( libesedb_catalog_initialize(
		     &( internal_file->catalog ),
		     internal_file->io_handle,
		     LIBESEDB_PAGE_NUMBER_CATALOG,
		     internal_file->pages_vector,
		     internal_file->pages_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create catalog.",
			 function );

			goto on_error;
		}
		if( libesedb_catalog_read_file_io_handle(
		     internal_file->catalog,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read catalog.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "Reading the backup catalog:\n" );
		}
#endif
		if( libesedb_catalog_initialize(
		     &( internal_file->backup_catalog ),
		     internal_file->io_handle,
		     LIBESEDB_PAGE_NUMBER_CATALOG_BACKUP,
		     internal_file->pages_vector,
		     internal_file->pages_cache,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create backup catalog.",
			 function );

			goto on_error;
		}
		if( libesedb_catalog_read_file_io_handle(
		     internal_file->backup_catalog,
		     file_io_handle,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read backup catalog.",
			 function );

			goto on_error;
		}
/* TODO compare contents of catalogs ? */
	}
	internal_file->io_handle->abort = 0;

	return( 1 );

on_error:
	if( internal_file->backup_catalog != NULL )
	{
		libesedb_catalog_free(
		 &( internal_file->backup_catalog ),
		 NULL );
	}
	if( internal_file->catalog != NULL )
	{
		libesedb_catalog_free(
		 &( internal_file->catalog ),
		 NULL );
	}
	if( internal_file->database != NULL )
	{
		libesedb_database_free(
		 &( internal_file->database ),
		 NULL );
	}
	if( internal_file->pages_cache != NULL )
	{
		libfcache_cache_free(
		 &( internal_file->pages_cache ),
		 NULL );
	}
	if( internal_file->pages_vector != NULL )
	{
		libfdata_vector_free(
		 &( internal_file->pages_vector ),
		 NULL );
	}
	if( file_header != NULL )
	{
		libesedb_file_header_free(
		 &file_header,
		 NULL );
	}
	internal_file->io_handle->abort = 0;

	return( -1 );
}

/* Retrieves the file type
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_type(
     libesedb_file_t *file,
     uint32_t *type,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_type";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid type.",
		 function );

		return( -1 );
	}
	*type = internal_file->io_handle->file_type;

	return( 1 );
}

/* Retrieves the file (current) version
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_format_version(
     libesedb_file_t *file,
     uint32_t *format_version,
     uint32_t *format_revision,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_format_version";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( format_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format version.",
		 function );

		return( -1 );
	}
	if( format_revision == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format revision.",
		 function );

		return( -1 );
	}
	*format_version  = internal_file->io_handle->format_version;
	*format_revision = internal_file->io_handle->format_revision;

	return( 1 );
}

/* Retrieves the file creation format version
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_creation_format_version(
     libesedb_file_t *file,
     uint32_t *format_version,
     uint32_t *format_revision,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_creation_format_version";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( format_version == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format version.",
		 function );

		return( -1 );
	}
	if( format_revision == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format revision.",
		 function );

		return( -1 );
	}
	*format_version  = internal_file->io_handle->creation_format_version;
	*format_revision = internal_file->io_handle->creation_format_revision;

	return( 1 );
}

/* Retrieves the file page size
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_page_size(
     libesedb_file_t *file,
     uint32_t *page_size,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_page_size";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing IO handle.",
		 function );

		return( -1 );
	}
	if( page_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid page size.",
		 function );

		return( -1 );
	}
	*page_size = internal_file->io_handle->page_size;

	return( 1 );
}

/* Retrieves the number of tables
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_number_of_tables(
     libesedb_file_t *file,
     int *number_of_tables,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_number_of_tables";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->catalog == NULL )
	{
		if( number_of_tables == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of tables.",
			 function );

			return( -1 );
		}
		*number_of_tables = 0;
	}
	else
	{
		if( libesedb_catalog_get_number_of_table_definitions(
		     internal_file->catalog,
		     number_of_tables,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of tables.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves a specific table
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_table(
     libesedb_file_t *file,
     int table_entry,
     libesedb_table_t **table,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file                = NULL;
	libesedb_table_definition_t *table_definition          = NULL;
	libesedb_table_definition_t *template_table_definition = NULL;
	static char *function                                  = "libesedb_file_get_table";

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table value already set.",
		 function );

		return( -1 );
	}
	if( libesedb_catalog_get_table_definition_by_index(
	     internal_file->catalog,
	     table_entry,
	     &table_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table definition: %d.",
		 function,
		 table_entry );

		return( -1 );
	}
	if( table_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing table definition.",
		 function );

		return( -1 );
	}
	if( table_definition->table_catalog_definition == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid table definition - missing table catalog definition.",
		 function );

		return( -1 );
	}
	if( table_definition->table_catalog_definition->template_name != NULL )
	{
		if( libesedb_catalog_get_table_definition_by_utf8_name(
		     internal_file->catalog,
		     table_definition->table_catalog_definition->template_name,
		     table_definition->table_catalog_definition->template_name_size,
		     &template_table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve template table definition.",
			 function );

			return( -1 );
		}
	}
	if( libesedb_table_initialize(
	     table,
	     internal_file->file_io_handle,
	     internal_file->io_handle,
	     table_definition,
	     template_table_definition,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the table for the UTF-8 encoded name
 * Returns 1 if successful, 0 if no table could be found or -1 on error
 */
int libesedb_file_get_table_by_utf8_name(
     libesedb_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libesedb_table_t **table,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file                = NULL;
	libesedb_table_definition_t *table_definition          = NULL;
	libesedb_table_definition_t *template_table_definition = NULL;
	static char *function                                  = "libesedb_file_get_table_by_utf8_name";
	int result                                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table value already set.",
		 function );

		return( -1 );
	}
	result = libesedb_catalog_get_table_definition_by_utf8_name(
	          internal_file->catalog,
	          utf8_string,
	          utf8_string_length,
	          &table_definition,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table definition.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition.",
			 function );

			return( -1 );
		}
		if( table_definition->table_catalog_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition - missing table catalog definition.",
			 function );

			return( -1 );
		}
		if( table_definition->table_catalog_definition->template_name != NULL )
		{
			if( libesedb_catalog_get_table_definition_by_name(
			     internal_file->catalog,
			     table_definition->table_catalog_definition->template_name,
			     table_definition->table_catalog_definition->template_name_size,
			     &template_table_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve template table definition.",
				 function );

				return( -1 );
			}
		}
		if( libesedb_table_initialize(
		     table,
		     internal_file->file_io_handle,
		     internal_file->io_handle,
		     table_definition,
		     template_table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Retrieves the table for the UTF-16 encoded name
 * Returns 1 if successful, 0 if no table could be found or -1 on error
 */
int libesedb_file_get_table_by_utf16_name(
     libesedb_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libesedb_table_t **table,
     libcerror_error_t **error )
{
	libesedb_internal_file_t *internal_file                = NULL;
	libesedb_table_definition_t *table_definition          = NULL;
	libesedb_table_definition_t *template_table_definition = NULL;
	static char *function                                  = "libesedb_file_get_table_by_utf16_name";
	int result                                             = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( table == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( *table != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid table value already set.",
		 function );

		return( -1 );
	}
	result = libesedb_catalog_get_table_definition_by_utf16_name(
	          internal_file->catalog,
	          utf16_string,
	          utf16_string_length,
	          &table_definition,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table definition.",
		 function );

		return( -1 );
	}
	else if( result != 0 )
	{
		if( table_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing table definition.",
			 function );

			return( -1 );
		}
		if( table_definition->table_catalog_definition == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid table definition - missing table catalog definition.",
			 function );

			return( -1 );
		}
		if( table_definition->table_catalog_definition->template_name != NULL )
		{
			if( libesedb_catalog_get_table_definition_by_name(
			     internal_file->catalog,
			     table_definition->table_catalog_definition->template_name,
			     table_definition->table_catalog_definition->template_name_size,
			     &template_table_definition,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve template table definition.",
				 function );

				return( -1 );
			}
		}
		if( libesedb_table_initialize(
		     table,
		     internal_file->file_io_handle,
		     internal_file->io_handle,
		     table_definition,
		     template_table_definition,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create table.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

