/*
 * Python object definition of the libesedb column types
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

int pyesedb_column_types_init_type(
     PyTypeObject *type_object );

PyObject *pyesedb_column_types_new(
           void );

int pyesedb_column_types_init(
     pyesedb_column_types_t *definitions_object );

void pyesedb_column_types_free(
      pyesedb_column_types_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_COLUMN_TYPES_H ) */

