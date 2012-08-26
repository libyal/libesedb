/*
 * Error functions
 *
 * Copyright (c) 2008-2012, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBCPATH_INTERNAL_ERROR_H )
#define _LIBCPATH_INTERNAL_ERROR_H

#include <common.h>
#include <types.h>

#include <stdio.h>

#if !defined( HAVE_LOCAL_LIBCPATH )
#include <libcpath/error.h>
#endif

#include "libcpath_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBCPATH )

LIBCPATH_EXTERN \
void libcpath_error_free(
      libcpath_error_t **error );

LIBCPATH_EXTERN \
int libcpath_error_fprint(
     libcpath_error_t *error,
     FILE *stream );

LIBCPATH_EXTERN \
int libcpath_error_sprint(
     libcpath_error_t *error,
     char *string,
     size_t size );

LIBCPATH_EXTERN \
int libcpath_error_backtrace_fprint(
     libcpath_error_t *error,
     FILE *stream );

LIBCPATH_EXTERN \
int libcpath_error_backtrace_sprint(
     libcpath_error_t *error,
     char *string,
     size_t size );

#endif /* !defined( HAVE_LOCAL_LIBCPATH ) */

#if defined( __cplusplus )
}
#endif

#endif

