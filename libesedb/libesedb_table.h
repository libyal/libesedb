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
#include "libesedb_file.h"
#include "libesedb_list_type.h"
#include "libesedb_page_tree.h"
#include "libesedb_table_definition.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_table libesedb_internal_table_t;

struct libesedb_internal_table
{
	/* The internal file
	 */
	libesedb_internal_file_t *internal_file;

	/* The table definition
	 */
	libesedb_table_definition_t *table_definition;

	/* The table reference list element
	 */
	libesedb_list_element_t *list_element;

	/* The table page tree
	 */
	libesedb_page_tree_t *page_tree;

	/* The column reference list
	 */
	libesedb_list_t *column_reference_list;

	/* The index reference list
	 */
	libesedb_list_t *index_reference_list;

	/* The record reference list
	 */
	libesedb_list_t *record_reference_list;
};

int libesedb_table_initialize(
     libesedb_table_t **table,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_free(
                     libesedb_table_t **table,
                     liberror_error_t **error );

int libesedb_table_free_no_detach(
     intptr_t *internal_table,
     liberror_error_t **error );

int libesedb_table_attach(
     libesedb_internal_table_t *internal_table,
     libesedb_internal_file_t *internal_file,
     libesedb_table_definition_t *table_definition,
     liberror_error_t **error );

int libesedb_table_detach(
     libesedb_internal_table_t *internal_table,
     liberror_error_t **error );

int libesedb_table_read_page_tree(
     libesedb_internal_table_t *internal_table,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_get_identifier(
                     libesedb_table_t *table,
                     uint32_t *identifier,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_get_utf8_name_size(
                     libesedb_table_t *table,
                     size_t *utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_get_utf8_name(
                     libesedb_table_t *table,
                     uint8_t *utf8_string,
                     size_t utf8_string_size,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_get_amount_of_indexes(
                     libesedb_table_t *table,
                     int *amount_of_indexes,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_get_index(
                     libesedb_table_t *table,
                     int index_entry,
                     libesedb_index_t **index,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_get_amount_of_records(
                     libesedb_table_t *table,
                     int *amount_of_records,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_table_get_record(
                     libesedb_table_t *table,
                     int record_entry,
                     libesedb_record_t **record,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

