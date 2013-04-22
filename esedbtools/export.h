/*
 * Export functions
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _EXPORT_H )
#define _EXPORT_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#include "esedbtools_libcstring.h"

#if defined( __cplusplus )
extern "C" {
#endif

void export_binary_data(
      const uint8_t *data,
      size_t data_size,
      FILE *stream );

void export_narrow_text(
      const char *string,
      size_t string_size,
      FILE *stream );

void export_text(
      const libcstring_system_character_t *string,
      size_t string_size,
      FILE *stream );

#if defined( __cplusplus )
}
#endif

#endif

