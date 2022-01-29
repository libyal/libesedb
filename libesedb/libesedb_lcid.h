/*
 * LCID functions
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

#if !defined( _LIBESEDB_LCID_H )
#define _LIBESEDB_LCID_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_DEBUG_OUTPUT )

typedef struct libesedb_lcid_language_tag libesedb_lcid_language_tag_t;

struct libesedb_lcid_language_tag
{
	/* The LCID language tag value
	 */
	uint16_t lcid_language_tag;

	/* The identifier
	 */
	const char *identifier;

	/* The description
	 */
	const char *description;
};

const char *libesedb_lcid_language_tag_get_identifier(
             uint16_t lcid_language_tag );

const char *libesedb_lcid_language_tag_get_description(
             uint16_t lcid_language_tag );

#endif

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_LCID_H ) */

