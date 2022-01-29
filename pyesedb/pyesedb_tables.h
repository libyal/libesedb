/*
 * Python object definition of the sequence and iterator object of tables
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

#if !defined( _PYESEDB_TABLES_H )
#define _PYESEDB_TABLES_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_tables pyesedb_tables_t;

struct pyesedb_tables
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The parent object
	 */
	PyObject *parent_object;

	/* The get item by index callback function
	 */
	PyObject* (*get_item_by_index)(
	             PyObject *parent_object,
	             int index );

	/* The current index
	 */
	int current_index;

	/* The number of items
	 */
	int number_of_items;
};

extern PyTypeObject pyesedb_tables_type_object;

PyObject *pyesedb_tables_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyesedb_tables_init(
     pyesedb_tables_t *sequence_object );

void pyesedb_tables_free(
      pyesedb_tables_t *sequence_object );

Py_ssize_t pyesedb_tables_len(
            pyesedb_tables_t *sequence_object );

PyObject *pyesedb_tables_getitem(
           pyesedb_tables_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyesedb_tables_iter(
           pyesedb_tables_t *sequence_object );

PyObject *pyesedb_tables_iternext(
           pyesedb_tables_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_TABLES_H ) */

