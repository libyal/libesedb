/*
 * Database functions
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

#if !defined( _LIBESEDB_DATABASE_H )
#define _LIBESEDB_DATABASE_H

#include <common.h>
#include <types.h>

#include "libesedb_block_tree.h"
#include "libesedb_io_handle.h"
#include "libesedb_libbfio.h"
#include "libesedb_libcerror.h"
#include "libesedb_libfcache.h"
#include "libesedb_libfdata.h"
#include "libesedb_page_tree.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_database libesedb_database_t;

struct libesedb_database
{
	/* The page tree
	 */
	libesedb_page_tree_t *page_tree;

	/* The page block tree
	 */
	libesedb_block_tree_t *page_block_tree;
};

int libesedb_database_initialize(
     libesedb_database_t **database,
     libesedb_io_handle_t *io_handle,
     libfdata_vector_t *pages_vector,
     libfcache_cache_t *pages_cache,
     libcerror_error_t **error );

int libesedb_database_free(
     libesedb_database_t **database,
     libcerror_error_t **error );

int libesedb_database_read_values_from_leaf_page(
     libesedb_database_t *database,
     libesedb_page_t *page,
     int *current_leaf_value_index,
     libcerror_error_t **error );

int libesedb_database_read_file_io_handle(
     libesedb_database_t *database,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_DATABASE_H ) */

