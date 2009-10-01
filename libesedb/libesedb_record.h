/*
 * Record (row) functions
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

#if !defined( _LIBESEDB_RECORD_H )
#define _LIBESEDB_RECORD_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_data_definition.h"
#include "libesedb_extern.h"
#include "libesedb_table.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_internal_record libesedb_internal_record_t;

struct libesedb_internal_record
{
	/* The internal table
	 */
	libesedb_internal_table_t *internal_table;

	/* The data definition
	 */
	libesedb_data_definition_t *data_definition;

	/* The record reference list element
	 */
	libesedb_list_element_t *list_element;
};

int libesedb_record_initialize(
     libesedb_record_t **record,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_free(
                     libesedb_record_t **record,
                     liberror_error_t **error );

int libesedb_record_free_no_detach(
     intptr_t *internal_record,
     liberror_error_t **error );

int libesedb_record_attach(
     libesedb_internal_record_t *internal_record,
     libesedb_internal_table_t *internal_table,
     libesedb_data_definition_t *data_definition,
     liberror_error_t **error );

int libesedb_record_detach(
     libesedb_internal_record_t *internal_record,
     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_amount_of_values(
                     libesedb_record_t *record,
                     int *amount_of_values,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value_identifier(
                     libesedb_table_t *table,
                     int value_entry,
                     uint32_t *value_identifier,
                     liberror_error_t **error );

LIBESEDB_EXTERN int libesedb_record_get_value(
                     libesedb_table_t *table,
                     int value_entry,
                     uint32_t *column_type,
                     uint8_t **value_data,
                     size_t *value_data_size,
                     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

