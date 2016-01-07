/*
 * Python object definition of the tables sequence and iterator
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

#if !defined( _PYESEDB_TABLES_H )
#define _PYESEDB_TABLES_H

#include <common.h>
#include <types.h>

#include "pyesedb_file.h"
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

	/* The file object
	 */
	pyesedb_file_t *file_object;

	/* The get table by index callback function
	 */
	PyObject* (*get_table_by_index)(
	             pyesedb_file_t *file_object,
	             int table_entry );

	/* The (current) table entry
	 */
	int table_entry;

	/* The number of tables
	 */
	int number_of_tables;
};

extern PyTypeObject pyesedb_tables_type_object;

PyObject *pyesedb_tables_new(
           pyesedb_file_t *file_object,
           PyObject* (*get_table_by_index)(
                        pyesedb_file_t *file_object,
                        int table_entry ),
           int number_of_tables );

int pyesedb_tables_init(
     pyesedb_tables_t *pyesedb_tables );

void pyesedb_tables_free(
      pyesedb_tables_t *pyesedb_tables );

Py_ssize_t pyesedb_tables_len(
            pyesedb_tables_t *pyesedb_tables );

PyObject *pyesedb_tables_getitem(
           pyesedb_tables_t *pyesedb_tables,
           Py_ssize_t item_index );

PyObject *pyesedb_tables_iter(
           pyesedb_tables_t *pyesedb_tables );

PyObject *pyesedb_tables_iternext(
           pyesedb_tables_t *pyesedb_tables );

#if defined( __cplusplus )
}
#endif

#endif

