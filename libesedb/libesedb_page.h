/*
 * Page block functions
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

#if !defined( _LIBPFF_PAGE_H )
#define _LIBPFF_PAGE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_io_handle.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_page_tags_value libesedb_page_tags_value_t;

struct libesedb_page_tags_value
{
	/* The offset of the value
	 */
	uint16_t offset;

	/* The size of the value
	 */
	uint16_t size;

	/* The flags of the value
	 */
	uint8_t flags;
};

typedef struct libesedb_page libesedb_page_t;

struct libesedb_page
{
	/* The page number
	 */
	uint32_t number;
};

int libesedb_page_tags_value_free(
     intptr_t *page_tags_value,
     liberror_error_t **error );

int libesedb_page_initialize(
     libesedb_page_t **page,
     liberror_error_t **error );

int libesedb_page_free(
     libesedb_page_t **page,
     liberror_error_t **error );

int libesedb_page_read(
     libesedb_page_t *page,
     libesedb_io_handle_t *io_handle,
     off64_t page_offset,
     size_t page_size,
     liberror_error_t **error );

int libesedb_page_read_tags(
     libesedb_array_t **page_tags_array,
     uint16_t amount_of_page_entries,
     uint8_t *page_data,
     size_t page_data_size,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

