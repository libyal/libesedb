/*
 * Codepage functions
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

#if !defined( _LIBESEDB_CODEPAGE_H )
#define _LIBESEDB_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_VERBOSE_OUTPUT )

typedef struct libesedb_codepage libesedb_codepage_t;

struct libesedb_codepage
{
	/* The codepage
	 */
	uint32_t codepage;

	/* The identifier
	 */
	const char *identifier;

	/* The description
	 */
	const char *description;
};
const char *libesedb_codepage_get_identifier(
             uint32_t codepage );

const char *libesedb_codepage_get_description(
             uint32_t codepage );

#endif

#if defined( __cplusplus )
}
#endif

#endif

