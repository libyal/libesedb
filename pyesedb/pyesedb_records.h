/*
 * Python object definition of the sequence and iterator object of records
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

#if !defined( _PYESEDB_RECORDS_H )
#define _PYESEDB_RECORDS_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_records pyesedb_records_t;

struct pyesedb_records
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

extern PyTypeObject pyesedb_records_type_object;

PyObject *pyesedb_records_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items );

int pyesedb_records_init(
     pyesedb_records_t *sequence_object );

void pyesedb_records_free(
      pyesedb_records_t *sequence_object );

Py_ssize_t pyesedb_records_len(
            pyesedb_records_t *sequence_object );

PyObject *pyesedb_records_getitem(
           pyesedb_records_t *sequence_object,
           Py_ssize_t item_index );

PyObject *pyesedb_records_iter(
           pyesedb_records_t *sequence_object );

PyObject *pyesedb_records_iternext(
           pyesedb_records_t *sequence_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_RECORDS_H ) */

