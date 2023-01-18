/*
 * Python object wrapper of libesedb_table_t
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

#if !defined( _PYESEDB_TABLE_H )
#define _PYESEDB_TABLE_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_table pyesedb_table_t;

struct pyesedb_table
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libesedb table
	 */
	libesedb_table_t *table;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyesedb_table_object_methods[];
extern PyTypeObject pyesedb_table_type_object;

PyObject *pyesedb_table_new(
           libesedb_table_t *table,
           PyObject *parent_object );

int pyesedb_table_init(
     pyesedb_table_t *pyesedb_table );

void pyesedb_table_free(
      pyesedb_table_t *pyesedb_table );

PyObject *pyesedb_table_get_identifier(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments );

PyObject *pyesedb_table_get_name(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments );

PyObject *pyesedb_table_get_template_name(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments );

PyObject *pyesedb_table_get_number_of_columns(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments );

PyObject *pyesedb_table_get_column_by_index(
           PyObject *pyesedb_table,
           int column_index );

PyObject *pyesedb_table_get_column(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_table_get_columns(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments );

PyObject *pyesedb_table_get_number_of_records(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments );

PyObject *pyesedb_table_get_record_by_index(
           PyObject *pyesedb_table,
           int record_index );

PyObject *pyesedb_table_get_record(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_table_get_records(
           pyesedb_table_t *pyesedb_table,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_TABLE_H ) */

