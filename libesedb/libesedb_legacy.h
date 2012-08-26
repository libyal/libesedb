/*
 * Legacy functions
 *
 * Copyright (c) 2010, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_LEGACY_H )
#define _LIBESEDB_LEGACY_H

#include <common.h>
#include <types.h>

#include "libesedb_extern.h"
#include "libesedb_libcerror.h"
#include "libesedb_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBESEDB )

LIBESEDB_EXTERN \
int libesedb_file_get_amount_of_tables(
     libesedb_file_t *file,
     int *amount_of_tables,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_multi_value_get_amount_of_values(
     libesedb_multi_value_t *multi_value,
     int *amount_of_values,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_amount_of_values(
     libesedb_record_t *record,
     int *amount_of_values,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_record_get_value_floating_point(
     libesedb_record_t *record,
     int value_entry,
     double *value_floating_point,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_amount_of_columns(
     libesedb_table_t *table,
     int *amount_of_columns,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_amount_of_indexes(
     libesedb_table_t *table,
     int *amount_of_indexes,
     libcerror_error_t **error );

LIBESEDB_EXTERN \
int libesedb_table_get_amount_of_records(
     libesedb_table_t *table,
     int *amount_of_records,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBESEDB ) */

#if defined( __cplusplus )
}
#endif

#endif

