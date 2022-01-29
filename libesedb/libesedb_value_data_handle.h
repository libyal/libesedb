/*
 * Value data handle functions
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

#if !defined( _LIBESEDB_VALUE_DATA_HANDLE_H )
#define _LIBESEDB_VALUE_DATA_HANDLE_H

#include <common.h>
#include <types.h>

#include "libesedb_libcdata.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfvalue.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libesedb_value_data_handle_read_value_entries(
     libfvalue_data_handle_t *data_handle,
     const uint8_t *data,
     size_t data_size,
     int encoding,
     uint32_t data_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_VALUE_DATA_HANDLE_H ) */

