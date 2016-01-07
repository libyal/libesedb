/*
 * Python object definition of the columns sequence and iterator
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYESEDB_COLUMNS_H )
#define _PYESEDB_COLUMNS_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_columns pyesedb_columns_t;

struct pyesedb_columns
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The table object
	 */
	pyesedb_table_t *table_object;

	/* The get column by index callback function
	 */
	PyObject* (*get_column_by_index)(
	             pyesedb_table_t *table_object,
	             int column_entry );

	/* The (current) column entry
	 */
	int column_entry;

	/* The number of columns
	 */
	int number_of_columns;
};

extern PyTypeObject pyesedb_columns_type_object;

PyObject *pyesedb_columns_new(
           pyesedb_table_t *table_object,
           PyObject* (*get_column_by_index)(
                        pyesedb_table_t *table_object,
                        int column_entry ),
           int number_of_columns );

int pyesedb_columns_init(
     pyesedb_columns_t *pyesedb_columns );

void pyesedb_columns_free(
      pyesedb_columns_t *pyesedb_columns );

Py_ssize_t pyesedb_columns_len(
            pyesedb_columns_t *pyesedb_columns );

PyObject *pyesedb_columns_getitem(
           pyesedb_columns_t *pyesedb_columns,
           Py_ssize_t item_index );

PyObject *pyesedb_columns_iter(
           pyesedb_columns_t *pyesedb_columns );

PyObject *pyesedb_columns_iternext(
           pyesedb_columns_t *pyesedb_columns );

#if defined( __cplusplus )
}
#endif

#endif

