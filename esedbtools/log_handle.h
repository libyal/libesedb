/* 
 * Log handle
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
 * Copyright (C) 2009-2010, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LOG_HANDLE_H )
#define _LOG_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include <libcstring.h>
#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct log_handle log_handle_t;

struct log_handle
{
	/* The log stream
	 */
	FILE *log_stream;
};

int log_handle_initialize(
     log_handle_t **log_handle,
     liberror_error_t **error );

int log_handle_free(
     log_handle_t **log_handle,
     liberror_error_t **error );

int log_handle_open(
     log_handle_t *log_handle,
     const libcstring_system_character_t *filename,
     liberror_error_t **error );

int log_handle_close(
     log_handle_t *log_handle,
     liberror_error_t **error );

void log_handle_printf(
      log_handle_t *log_handle,
      char *format,
      ... );

#if defined( __cplusplus )
}
#endif

#endif

