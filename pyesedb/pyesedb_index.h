/*
 * Python object wrapper of libesedb_index_t
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

#if !defined( _PYESEDB_INDEX_H )
#define _PYESEDB_INDEX_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_index pyesedb_index_t;

struct pyesedb_index
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libesedb index
	 */
	libesedb_index_t *index;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyesedb_index_object_methods[];
extern PyTypeObject pyesedb_index_type_object;

PyObject *pyesedb_index_new(
           libesedb_index_t *index,
           PyObject *parent_object );

int pyesedb_index_init(
     pyesedb_index_t *pyesedb_index );

void pyesedb_index_free(
      pyesedb_index_t *pyesedb_index );

PyObject *pyesedb_index_get_identifier(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments );

PyObject *pyesedb_index_get_name(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments );

PyObject *pyesedb_index_get_number_of_records(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments );

PyObject *pyesedb_index_get_record_by_index(
           PyObject *pyesedb_index,
           int record_index );

PyObject *pyesedb_index_get_record(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_index_get_records(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_INDEX_H ) */

