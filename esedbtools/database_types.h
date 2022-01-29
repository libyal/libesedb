/*
 * Database types
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

#if !defined( _DATABASE_TYPES_H )
#define _DATABASE_TYPES_H

#include <common.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "esedbtools_libcdata.h"
#include "esedbtools_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

/* TODO add file history database support */

enum DATABASE_TYPES
{
	DATABASE_TYPE_UNKNOWN = 0,
	DATABASE_TYPE_EXCHANGE,
	DATABASE_TYPE_SRUM,
	DATABASE_TYPE_WEBCACHE,
	DATABASE_TYPE_WINDOWS_SEARCH,
	DATABASE_TYPE_WINDOWS_SECURITY,
	NUMBER_OF_DATABASE_TYPES
};

extern const system_character_t *database_type_descriptions[ ];

typedef struct database_type_descriptor database_type_descriptor_t;

struct database_type_descriptor
{
	/* The database type
	 */
	int database_type;

	/* Known table names
	 */
	const system_character_t *known_table_names[ 32 ];
};

int database_type_determine(
     int *database_type,
     libcdata_array_t *table_names,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _DATABASE_TYPES_H ) */

