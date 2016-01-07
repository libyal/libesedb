/*
 * Python object definition of the indexes sequence and iterator
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

#if !defined( _PYESEDB_INDEXES_H )
#define _PYESEDB_INDEXES_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_table.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_indexes pyesedb_indexes_t;

struct pyesedb_indexes
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The table object
	 */
	pyesedb_table_t *table_object;

	/* The get index by index callback function
	 */
	PyObject* (*get_index_by_index)(
	             pyesedb_table_t *table_object,
	             int index_entry );

	/* The (current) index entry
	 */
	int index_entry;

	/* The number of indexes
	 */
	int number_of_indexes;
};

extern PyTypeObject pyesedb_indexes_type_object;

PyObject *pyesedb_indexes_new(
           pyesedb_table_t *table_object,
           PyObject* (*get_index_by_index)(
                        pyesedb_table_t *table_object,
                        int index_entry ),
           int number_of_indexes );

int pyesedb_indexes_init(
     pyesedb_indexes_t *pyesedb_indexes );

void pyesedb_indexes_free(
      pyesedb_indexes_t *pyesedb_indexes );

Py_ssize_t pyesedb_indexes_len(
            pyesedb_indexes_t *pyesedb_indexes );

PyObject *pyesedb_indexes_getitem(
           pyesedb_indexes_t *pyesedb_indexes,
           Py_ssize_t item_index );

PyObject *pyesedb_indexes_iter(
           pyesedb_indexes_t *pyesedb_indexes );

PyObject *pyesedb_indexes_iternext(
           pyesedb_indexes_t *pyesedb_indexes );

#if defined( __cplusplus )
}
#endif

#endif

