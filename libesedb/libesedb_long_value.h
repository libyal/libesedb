/*
 * Long value functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBESEDB_INTERNAL_LONG_VALUE_H )
#define _LIBESEDB_INTERNAL_LONG_VALUE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_extern.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_long_value libesedb_internal_long_value_t;

struct libesedb_internal_long_value
{
	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	size_t data_size;
};

int libesedb_long_value_initialize(
     libesedb_long_value_t **long_value,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_long_value_free(
                     libesedb_long_value_t **long_value,
                     liberror_error_t **error );

int libesedb_long_value_set_data(
     libesedb_internal_long_value_t *internal_long_value,
     uint8_t *data,
     size_t data_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

