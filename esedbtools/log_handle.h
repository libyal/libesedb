/*
 * Log handle
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

#if !defined( _LOG_HANDLE_H )
#define _LOG_HANDLE_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "esedbtools_libcerror.h"

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
     libcerror_error_t **error );

int log_handle_free(
     log_handle_t **log_handle,
     libcerror_error_t **error );

int log_handle_open(
     log_handle_t *log_handle,
     const system_character_t *filename,
     libcerror_error_t **error );

int log_handle_close(
     log_handle_t *log_handle,
     libcerror_error_t **error );

void log_handle_printf(
      log_handle_t *log_handle,
      char *format,
      ... );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LOG_HANDLE_H ) */

