/*
 * Index functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBESEDB_INDEX_H )
#define _LIBESEDB_INDEX_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_catalog_definition.h"
#include "libesedb_extern.h"
#include "libesedb_file.h"
#include "libesedb_page_tree.h"
#include "libesedb_table.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_index libesedb_internal_index_t;

struct libesedb_internal_index
{
	/* The internal file
	 */
	libesedb_internal_file_t *internal_file;

	/* The internal table
	 */
	libesedb_internal_table_t *internal_table;

	/* The catalog definition
	 */
	libesedb_catalog_definition_t *catalog_definition;

	/* The index page tree
	 */
	libesedb_page_tree_t *index_page_tree;
};

int libesedb_index_initialize(
     libesedb_index_t **index,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_index_free(
                     libesedb_index_t **index,
                     liberror_error_t **error );

int libesedb_index_attach(
     libesedb_internal_index_t *internal_index,
     libesedb_internal_table_t *internal_table,
     libesedb_internal_file_t *internal_file,
     libesedb_catalog_definition_t *catalog_definition,
     liberror_error_t **error );

int libesedb_index_detach(
     libesedb_internal_index_t *internal_index,
     liberror_error_t **error );

int libesedb_index_read_page_tree(
     libesedb_internal_index_t *internal_index,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_index_get_identifier(
                     libesedb_index_t *index,
                     uint32_t *identifier,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_index_get_utf8_name_size(
                     libesedb_index_t *index,
                     size_t *utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_index_get_utf8_name(
                     libesedb_index_t *index,
                     uint8_t *utf8_string,
                     size_t utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_index_test(
                     libesedb_index_t *index,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

