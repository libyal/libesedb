/*
 * Python object definition of the libesedb value flags
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

#if !defined( _PYESEDB_VALUE_FLAGS_H )
#define _PYESEDB_VALUE_FLAGS_H

#include <common.h>
#include <types.h>

#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_value_flags pyesedb_value_flags_t;

struct pyesedb_value_flags
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pyesedb_value_flags_type_object;

int pyesedb_value_flags_init_type(
     PyTypeObject *type_object );

PyObject *pyesedb_value_flags_new(
           void );

int pyesedb_value_flags_init(
     pyesedb_value_flags_t *definitions_object );

void pyesedb_value_flags_free(
      pyesedb_value_flags_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_VALUE_FLAGS_H ) */

