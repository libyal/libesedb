/*
 * Database functions
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

#if !defined( _LIBESEDB_DATABASE_H )
#define _LIBESEDB_DATABASE_H

#include <common.h>
#include <types.h>

#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_database libesedb_database_t;

struct libesedb_database
{
	/* TODO implement something useful */
	int dummy;
};

int libesedb_database_initialize(
     libesedb_database_t **database,
     libcerror_error_t **error );

int libesedb_database_free(
     libesedb_database_t **database,
     libcerror_error_t **error );

int libesedb_database_read(
     libesedb_database_t *database,
     libbfio_handle_t *file_io_handle,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_DATABASE_H ) */

