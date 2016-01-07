/*
 * Python object definition of the records sequence and iterator
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

	/* The parent (table or index) object
	 */
	PyObject *parent_object;

	/* The get record by index callback function
	 */
	PyObject* (*get_record_by_index)(
	             PyObject *parent_object,
	             int record_entry );

	/* The (current) record entry
	 */
	int record_entry;

	/* The number of records
	 */
	int number_of_records;
};

extern PyTypeObject pyesedb_records_type_object;

PyObject *pyesedb_records_new(
           PyObject *parent_object,
           PyObject* (*get_record_by_index)(
                        PyObject *parent_object,
                        int record_entry ),
           int number_of_records );

int pyesedb_records_init(
     pyesedb_records_t *pyesedb_records );

void pyesedb_records_free(
      pyesedb_records_t *pyesedb_records );

Py_ssize_t pyesedb_records_len(
            pyesedb_records_t *pyesedb_records );

PyObject *pyesedb_records_getitem(
           pyesedb_records_t *pyesedb_records,
           Py_ssize_t item_index );

PyObject *pyesedb_records_iter(
           pyesedb_records_t *pyesedb_records );

PyObject *pyesedb_records_iternext(
           pyesedb_records_t *pyesedb_records );

#if defined( __cplusplus )
}
#endif

#endif

