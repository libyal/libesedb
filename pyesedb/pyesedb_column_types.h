/*
 * Python object definition of the libesedb column types
 *
 * Copyright (C) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYESEDB_COLUMN_TYPES_H )
#define _PYESEDB_COLUMN_TYPES_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_column_types pyesedb_column_types_t;

struct pyesedb_column_types
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyesedb_column_types_type_object;

PyObject *pyesedb_column_types_new(
           void );

int pyesedb_column_types_init(
     pyesedb_column_types_t *pyesedb_column_types );

void pyesedb_column_types_free(
      pyesedb_column_types_t *pyesedb_column_types );

PyObject *pyesedb_column_types_get_null(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_boolean(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_integer_8bit_unsigned(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_integer_16bit_signed(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_integer_32bit_signed(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_currency(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_float_32bit(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_double_64bit(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_date_time(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_binary_data(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_text(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_large_binary_data(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_large_text(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_super_large_value(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_integer_32bit_unsigned(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_integer_64bit_signed(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_guid(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

PyObject *pyesedb_column_types_get_integer_16bit_unsigned(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif

