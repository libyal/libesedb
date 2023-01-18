/*
 * Python object wrapper of libesedb_multi_value_t
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

#if !defined( _PYESEDB_MULTI_VALUE_H )
#define _PYESEDB_MULTI_VALUE_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_multi_value pyesedb_multi_value_t;

struct pyesedb_multi_value
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libesedb multi value
	 */
	libesedb_multi_value_t *multi_value;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pyesedb_multi_value_object_methods[];
extern PyTypeObject pyesedb_multi_value_type_object;

PyObject *pyesedb_multi_value_new(
           libesedb_multi_value_t *multi_value,
           PyObject *parent_object );

int pyesedb_multi_value_init(
     pyesedb_multi_value_t *pyesedb_multi_value );

void pyesedb_multi_value_free(
      pyesedb_multi_value_t *pyesedb_multi_value );

PyObject *pyesedb_multi_value_get_number_of_values(
           pyesedb_multi_value_t *pyesedb_multi_value,
           PyObject *arguments );

PyObject *pyesedb_multi_value_get_value_data(
           pyesedb_multi_value_t *pyesedb_multi_value,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_multi_value_get_value_data_as_floating_point(
           pyesedb_multi_value_t *pyesedb_multi_value,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_multi_value_get_value_data_as_integer(
           pyesedb_multi_value_t *pyesedb_multi_value,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_multi_value_get_value_data_as_string(
           pyesedb_multi_value_t *pyesedb_multi_value,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_MULTI_VALUE_H ) */

