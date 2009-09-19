/*
 * Table functions
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _LIBESEDB_TABLE_H )
#define _LIBESEDB_TABLE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_extern.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_table libesedb_internal_table_t;

struct libesedb_internal_table
{
	/* Dummy
	 */
	int dummy;
};

int libesedb_table_initialize(
     libesedb_table_t **table,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_free(
                     libesedb_table_t **table,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

