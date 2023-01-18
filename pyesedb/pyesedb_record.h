/*
 * Python object wrapper of libesedb_record_t
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

#if !defined( _PYESEDB_RECORD_H )
#define _PYESEDB_RECORD_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_record pyesedb_record_t;

struct pyesedb_record
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libesedb record
	 */
	libesedb_record_t *record;

	/* The parent (table or index) object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyesedb_record_object_methods[];
extern PyTypeObject pyesedb_record_type_object;

PyObject *pyesedb_record_new(
           libesedb_record_t *record,
           PyObject *parent_object );

int pyesedb_record_init(
     pyesedb_record_t *pyesedb_record );

void pyesedb_record_free(
      pyesedb_record_t *pyesedb_record );

PyObject *pyesedb_record_get_number_of_values(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments );

PyObject *pyesedb_record_get_column_type(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_column_name(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_value_data_flags(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_is_long_value(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_is_multi_value(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_value_data(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_value_data_as_boolean(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_value_data_as_floating_point(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_value_data_as_integer(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_value_data_as_string(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_record_get_value_data_as_long_value(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_RECORD_H ) */

