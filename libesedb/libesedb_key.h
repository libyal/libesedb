/*
 * Key functions
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_KEY_H )
#define _LIBESEDB_KEY_H

#include <common.h>
#include <types.h>

#include "libesedb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_key libesedb_key_t;

struct libesedb_key
{
	/* The type
	 */
	uint8_t type;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libesedb_key_initialize(
     libesedb_key_t **key,
     libcerror_error_t **error );

int libesedb_key_free(
     libesedb_key_t **key,
     libcerror_error_t **error );

int libesedb_key_set_data(
     libesedb_key_t *key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libesedb_key_append_data(
     libesedb_key_t *key,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

int libesedb_key_compare(
     libesedb_key_t *first_key,
     libesedb_key_t *second_key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_KEY_H ) */

