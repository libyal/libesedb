/*
 * Space tree value functions
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

#if !defined( _LIBESEDB_SPACE_TREE_VALUE_H )
#define _LIBESEDB_SPACE_TREE_VALUE_H

#include <common.h>
#include <types.h>

#include "libesedb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_space_tree_value libesedb_space_tree_value_t;

struct libesedb_space_tree_value
{
	/* The number of pages
	 */
	uint32_t number_of_pages;
};

int libesedb_space_tree_value_initialize(
     libesedb_space_tree_value_t **space_tree_value,
     libcerror_error_t **error );

int libesedb_space_tree_value_free(
     libesedb_space_tree_value_t **space_tree_value,
     libcerror_error_t **error );

int libesedb_space_tree_value_read_data(
     libesedb_space_tree_value_t *space_tree_value,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_SPACE_TREE_VALUE_H ) */

