/*
 * Page value functions
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

#if !defined( _LIBESEDB_PAGE_VALUE_H )
#define _LIBESEDB_PAGE_VALUE_H

#include <common.h>
#include <types.h>

#include "libesedb_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_page_value libesedb_page_value_t;

struct libesedb_page_value
{
	/* The reference to the page value data
	 */
	uint8_t *data;

	/* The page value size
	 */
	uint16_t size;

	/* The offset of the page value
	 */
	uint16_t offset;

	/* The flags of the value
	 */
	uint8_t flags;
};

int libesedb_page_value_initialize(
     libesedb_page_value_t **page_value,
     libcerror_error_t **error );

int libesedb_page_value_free(
     libesedb_page_value_t **page_value,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_PAGE_VALUE_H ) */

