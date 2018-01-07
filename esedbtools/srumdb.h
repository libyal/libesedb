/*
 * System Resource Usage Monitor (SRUM) database export functions
 *
 * Copyright (C) 2009-2018, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _SRUMDB_H )
#define _SRUMDB_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "esedbtools_libcerror.h"
#include "esedbtools_libesedb.h"
#include "log_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

int srumdb_export_record_guid(
     libesedb_record_t *record,
     FILE *record_file_stream,
     log_handle_t *log_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _SRUMDB_H ) */

