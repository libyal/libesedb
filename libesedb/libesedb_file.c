/*
 * File functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#include <libcstring.h>
#include <liberror.h>
#include <libnotify.h>

#include "libesedb_array_type.h"
#include "libesedb_codepage.h"
#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_io_handle.h"
#include "libesedb_file.h"
#include "libesedb_libbfio.h"
#include "libesedb_page.h"
#include "libesedb_page_tree.h"
#include "libesedb_table.h"
#include "libesedb_table_definition.h"

/* Initialize a file
 * Make sure the value file is pointing to is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_initialize(
     libesedb_file_t **file,
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_initialize";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file == NULL )
	{
		internal_file = (libesedb_internal_file_t *) memory_allocate(
		                                              sizeof( libesedb_internal_file_t ) );

		if( internal_file == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create file.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_file,
		     0,
		     sizeof( libesedb_internal_file_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
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
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create io handle.",
			 function );

			libesedb_page_tree_free(
			 &( internal_file->catalog_page_tree ),
			 NULL );
			memory_free(
			 internal_file );

			return( -1 );
		}
		internal_file->ascii_codepage = LIBESEDB_CODEPAGE_WINDOWS_1252;

		*file = (libesedb_file_t *) internal_file;
	}
	return( 1 );
}

/* Frees an exisisting file
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_free(
     libesedb_file_t **file,
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_free";
	int result                              = 1;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( *file != NULL )
	{
		internal_file = (libesedb_internal_file_t *) *file;

		if( libesedb_page_tree_free(
		     &( internal_file->catalog_page_tree ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free page table.",
			 function );

			result = -1;
		}
		if( libesedb_io_handle_free(
		     &( internal_file->io_handle ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free io handle.",
			 function );

			result = -1;
		}
		if( ( internal_file->file_io_handle_created_in_library != 0 )
		 && ( internal_file->file_io_handle != NULL ) )
		{
			if( libbfio_handle_free(
			     &( internal_file->file_io_handle ),
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free file io handle.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 internal_file );

		*file = NULL;
	}
	return( result );
}

/* Signals the libesedb file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_signal_abort(
     libesedb_file_t *file,
     liberror_error_t **error )
{
	static char *function = "libesedb_file_signal_abort";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	( (libesedb_internal_file_t *) file )->abort = 1;

	return( 1 );
}

/* Opens a Extensible Storage Engine Database file
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open(
     libesedb_file_t *file,
     const char *filename,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle        = NULL;
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_open";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBESEDB_FLAG_READ ) != LIBESEDB_FLAG_READ )
	 && ( ( flags & LIBESEDB_FLAG_WRITE ) != LIBESEDB_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBESEDB_FLAG_WRITE ) == LIBESEDB_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to Extensible Storage Engine Database files currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
#endif
	if( libbfio_file_set_name(
	     file_io_handle,
	     filename,
	     libcstring_narrow_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
	if( libesedb_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %s.",
		 function,
		 filename );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Opens a Extensible Storage Engine Database file
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open_wide(
     libesedb_file_t *file,
     const wchar_t *filename,
     int flags,
     liberror_error_t **error )
{
	libbfio_handle_t *file_io_handle        = NULL;
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_open_wide";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( filename == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid filename.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBESEDB_FLAG_READ ) != LIBESEDB_FLAG_READ )
	 && ( ( flags & LIBESEDB_FLAG_WRITE ) != LIBESEDB_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBESEDB_FLAG_WRITE ) == LIBESEDB_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to Extensible Storage Engine Database files currently not supported.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( libbfio_file_initialize(
	     &file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create file io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libbfio_handle_set_track_offsets_read(
	     file_io_handle,
	     1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set track offsets read in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
#endif
	if( libbfio_file_set_name_wide(
	     file_io_handle,
	     filename,
	     libcstring_wide_string_length(
	      filename ) + 1,
	     error ) != 1 )
	{
                liberror_error_set(
                 error,
                 LIBERROR_ERROR_DOMAIN_RUNTIME,
                 LIBERROR_RUNTIME_ERROR_SET_FAILED,
                 "%s: unable to set filename in file io handle.",
                 function );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

                return( -1 );
	}
	if( libesedb_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     flags,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file: %ls.",
		 function,
		 filename );

		libbfio_handle_free(
		 &file_io_handle,
		 NULL );

		return( -1 );
	}
	internal_file->file_io_handle_created_in_library = 1;

	return( 1 );
}

#endif

/* Opens a Extensible Storage Engine Database file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open_file_io_handle(
     libesedb_file_t *file,
     libbfio_handle_t *file_io_handle,
     int flags,
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_open_file_io_handle";
	int file_io_flags                       = 0;
	int file_io_handle_is_open              = 0;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->file_io_handle != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal file - file io handle already set.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( ( ( flags & LIBESEDB_FLAG_READ ) != LIBESEDB_FLAG_READ )
	 && ( ( flags & LIBESEDB_FLAG_WRITE ) != LIBESEDB_FLAG_WRITE ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported flags.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBESEDB_FLAG_WRITE ) == LIBESEDB_FLAG_WRITE )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access to personal folder files currently not supported.",
		 function );

		return( -1 );
	}
	if( ( flags & LIBESEDB_FLAG_READ ) == LIBESEDB_FLAG_READ )
	{
		file_io_flags = LIBBFIO_FLAG_READ;
	}
	internal_file->file_io_handle = file_io_handle;

	file_io_handle_is_open = libbfio_handle_is_open(
	                          internal_file->file_io_handle,
	                          error );

	if( file_io_handle_is_open == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		return( -1 );
	}
	else if( file_io_handle_is_open == 0 )
	{
		if( libbfio_handle_open(
		     internal_file->file_io_handle,
		     flags,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_OPEN_FAILED,
			 "%s: unable to open file io handle.",
			 function );

			return( -1 );
		}
	}
	if( libesedb_file_open_read(
	     internal_file,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read from file handle.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes a Extensible Storage Engine Database file
 * Returns 0 if successful or -1 on error
 */
int libesedb_file_close(
     libesedb_file_t *file,
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_close";
	int result                              = 0;

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid file - missing file IO handle.",
		 function );

		return( -1 );
	}
	if( internal_file->file_io_handle_created_in_library != 0 )
	{
#if defined( HAVE_DEBUG_OUTPUT )
		if( libnotify_verbose != 0 )
		{
			if( libesedb_debug_print_read_offsets(
			     internal_file->file_io_handle,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print the read offsets.",
				 function );

				result = -1;
			}
		}
#endif
		if( libbfio_handle_close(
		     internal_file->file_io_handle,
		     error ) != 0 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close file io handle.",
			 function );

			return( -1 );
		}
	}
	return( result );
}

/* Opens a Extensible Storage Engine Database file for reading
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_open_read(
     libesedb_internal_file_t *internal_file,
     liberror_error_t **error )
{
	static char *function = "libesedb_file_open_read";

	if( internal_file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal file.",
		 function );

		return( -1 );
	}
	if( internal_file->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing io handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading file header:\n" );
	}
#endif

	if( libesedb_io_handle_read_file_header(
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header.",
		 function );

		return( -1 );
	}
	/* TODO What about page 1 ? present in empty database */
#if defined( HAVE_VERBOSE_OUTPUT )
	if( libnotify_verbose != 0 )
	{
		libnotify_printf(
		 "Reading the catalog page tree:\n" );
	}
#endif

	if( libesedb_page_tree_initialize(
	     &( internal_file->catalog_page_tree ),
	     NULL,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create catalog page tree.",
		 function );

		return( -1 );
	}
	/* TODO handle missing catalog e.g. empty database*/
	if( libesedb_page_tree_read(
	     internal_file->catalog_page_tree,
	     internal_file->io_handle,
	     internal_file->file_io_handle,
	     LIBESEDB_PAGE_NUMBER_CATALOG,
	     LIBESEDB_PAGE_TREE_FLAG_READ_CATALOG_DEFINITION,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_IO,
		 LIBERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read page tree.",
		 function );

		libesedb_page_tree_free(
		 &( internal_file->catalog_page_tree ),
		 NULL );

		return( -1 );
	}
#ifdef TEST
#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t page_number  = 0;
	libesedb_page_t *page = NULL;
	off64_t file_offset   = 0;
	size64_t file_size    = 0;

	if( libbfio_handle_get_size(
	     internal_file->file_io_handle,
	     &file_size,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file size.",
		 function );

		return( -1 );
	}
	file_offset = 2 * internal_file->io_handle->page_size;
	page_number = 1;

	while( file_offset < (off64_t) file_size )
	{
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
		     internal_file->io_handle,
		     internal_file->file_io_handle,
		     page_number,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_IO,
			 LIBERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read page.",
			 function );

			libesedb_page_free(
			 &page,
			 NULL );

			return( -1 );
		}
		file_offset += internal_file->io_handle->page_size;

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
		page_number += 1;
	}
#endif
#endif
	return( 1 );
}

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_ascii_codepage(
     libesedb_file_t *file,
     int *ascii_codepage,
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_ascii_codepage";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( ascii_codepage == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid ASCII codepage.",
		 function );

		return( -1 );
	}
	*ascii_codepage = internal_file->ascii_codepage;

	return( 1 );
}

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_set_ascii_codepage(
     libesedb_file_t *file,
     int ascii_codepage,
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_set_ascii_codepage";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( ( ascii_codepage != LIBESEDB_CODEPAGE_ASCII )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1250 )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1251 )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1252 )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1253 )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1254 )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1256 )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1257 )
	 || ( ascii_codepage != LIBESEDB_CODEPAGE_WINDOWS_1258 ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported ASCII codepage.",
		 function );

		return( -1 );
	}
	internal_file->ascii_codepage = ascii_codepage;

	return( 1 );
}

/* Retrieves the file (current) version
 * Returns 1 if successful or -1 on error
 */
int libesedb_file_get_format_version(
     libesedb_file_t *file,
     uint32_t *format_version,
     uint32_t *format_revision,
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_format_version";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing io handle.",
		 function );

		return( -1 );
	}
	if( format_version == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format version.",
		 function );

		return( -1 );
	}
	if( format_revision == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_creation_format_version";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing io handle.",
		 function );

		return( -1 );
	}
	if( format_version == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid format version.",
		 function );

		return( -1 );
	}
	if( format_revision == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_page_size";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing io handle.",
		 function );

		return( -1 );
	}
	if( page_size == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
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
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file = NULL;
	static char *function                   = "libesedb_file_get_number_of_tables";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->catalog_page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing catalog page tree.",
		 function );

		return( -1 );
	}
	if( internal_file->catalog_page_tree->table_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - invalid catalog page tree - missing table definition list.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_number_of_elements(
	     internal_file->catalog_page_tree->table_definition_list,
	     number_of_tables,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of tables.",
		 function );

		return( -1 );
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
     liberror_error_t **error )
{
	libesedb_internal_file_t *internal_file       = NULL;
	libesedb_table_definition_t *table_definition = NULL;
	static char *function                         = "libesedb_file_get_table";

	if( file == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	internal_file = (libesedb_internal_file_t *) file;

	if( internal_file->catalog_page_tree == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - missing catalog page tree.",
		 function );

		return( -1 );
	}
	if( internal_file->catalog_page_tree->table_definition_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal file - invalid catalog page tree - missing table definition list.",
		 function );

		return( -1 );
	}
	if( table == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid table.",
		 function );

		return( -1 );
	}
	if( libesedb_list_get_value(
	     internal_file->catalog_page_tree->table_definition_list,
	     table_entry,
	     (intptr_t **) &table_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve table definition.",
		 function );

		return( -1 );
	}
	if( libesedb_table_initialize(
	     table,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create table.",
		 function );

		return( -1 );
	}
	if( libesedb_table_attach(
	     (libesedb_internal_table_t *) *table,
	     internal_file,
	     table_definition,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to attach table.",
		 function );

		libesedb_table_free(
		 table,
		 NULL );

		return( -1 );
	}
	return( 1 );
}

