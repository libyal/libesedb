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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyesedb_column_types.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

struct PyMemberDef pyesedb_column_types_object_members[] = {

	{ "NULL",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_NULL,
	  READONLY,
	  "NULL column type (wraps LIBESEDB_COLUMN_TYPE_NULL)" },

	{ "BOOLEAN",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_BOOLEAN,
	  READONLY,
	  "BOOLEAN column type (wraps LIBESEDB_COLUMN_TYPE_BOOLEAN)" },

	{ "INTEGER_8BIT_UNSIGNED",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED,
	  READONLY,
	  "INTEGER_8BIT_UNSIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED)" },

	{ "INTEGER_16BIT_SIGNED",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED,
	  READONLY,
	  "INTEGER_16BIT_SIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED)" },

	{ "INTEGER_32BIT_SIGNED",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED,
	  READONLY,
	  "INTEGER_32BIT_SIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED)" },

	{ "CURRENCY",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_CURRENCY,
	  READONLY,
	  "CURRENCY column type (wraps LIBESEDB_COLUMN_TYPE_CURRENCY)" },

	{ "FLOAT_32BIT",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_FLOAT_32BIT,
	  READONLY,
	  "FLOAT_32BIT column type (wraps LIBESEDB_COLUMN_TYPE_FLOAT_32BIT)" },

	{ "DOUBLE_64BIT",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT,
	  READONLY,
	  "DOUBLE_64BIT column type (wraps LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT)" },

	{ "DATE_TIME",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_DATE_TIME,
	  READONLY,
	  "DATE_TIME column type (wraps LIBESEDB_COLUMN_TYPE_DATE_TIME)" },

	{ "BINARY_DATA",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_BINARY_DATA,
	  READONLY,
	  "BINARY_DATA column type (wraps LIBESEDB_COLUMN_TYPE_BINARY_DATA)" },

	{ "TEXT",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_TEXT,
	  READONLY,
	  "TEXT column type (wraps LIBESEDB_COLUMN_TYPE_TEXT)" },

	{ "LARGE_BINARY_DATA",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA,
	  READONLY,
	  "LARGE_BINARY_DATA column type (wraps LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA)" },

	{ "LARGE_TEXT",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_LARGE_TEXT,
	  READONLY,
	  "LARGE_TEXT column type (wraps LIBESEDB_COLUMN_TYPE_LARGE_TEXT)" },

	{ "SUPER_LARGE_VALUE",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE,
	  READONLY,
	  "SUPER_LARGE_VALUE column type (wraps LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE)" },

	{ "INTEGER_32BIT_UNSIGNED",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED,
	  READONLY,
	  "INTEGER_32BIT_UNSIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED)" },

	{ "INTEGER_64BIT_SIGNED",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED,
	  READONLY,
	  "INTEGER_64BIT_SIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED)" },

	{ "GUID",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_GUID,
	  READONLY,
	  "GUID column type (wraps LIBESEDB_COLUMN_TYPE_GUID)" },

	{ "INTEGER_16BIT_UNSIGNED",
	  T_BYTE,
	  offsetof( pyesedb_column_types_t, values ) + LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED,
	  READONLY,
	  "INTEGER_16BIT_UNSIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED)" },

	/* Sentinel */
	{ NULL, T_BYTE, 0, READONLY, NULL }
};

PyTypeObject pyesedb_column_types_type_object = {
	PyObject_HEAD_INIT( NULL )

	/* ob_size */
	0,
	/* tp_name */
	"pyesedb.column_types",
	/* tp_basicsize */
	sizeof( pyesedb_column_types_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_column_types_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	0,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
        /* tp_flags */
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pyesedb column types object (wraps LIBESEDB_COLUMN_TYPES)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	0,
	/* tp_members */
	pyesedb_column_types_object_members,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pyesedb_column_types_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new column types object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_new(
           void )
{
	pyesedb_column_types_t *pyesedb_column_types = NULL;
	static char *function                        = "pyesedb_column_types_new";

	pyesedb_column_types = PyObject_New(
	                        struct pyesedb_column_types,
	                        &pyesedb_column_types_type_object );

	if( pyesedb_column_types == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize column types.",
		 function );

		goto on_error;
	}
	if( pyesedb_column_types_init(
	     pyesedb_column_types ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize column types.",
		 function );

		goto on_error;
	}
	return( (PyObject *) pyesedb_column_types );

on_error:
	if( pyesedb_column_types != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_column_types );
	}
	return( NULL );
}

/* Intializes a column types object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_column_types_init(
     pyesedb_column_types_t *pyesedb_column_types )
{
	static char *function = "pyesedb_column_types_init";

	if( pyesedb_column_types == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column types.",
		 function );

		return( -1 );
	}
	pyesedb_column_types->values[ 0 ]  = (char) LIBESEDB_COLUMN_TYPE_NULL;
	pyesedb_column_types->values[ 1 ]  = (char) LIBESEDB_COLUMN_TYPE_BOOLEAN;
	pyesedb_column_types->values[ 2 ]  = (char) LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED;
	pyesedb_column_types->values[ 3 ]  = (char) LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED;
	pyesedb_column_types->values[ 4 ]  = (char) LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED;
	pyesedb_column_types->values[ 5 ]  = (char) LIBESEDB_COLUMN_TYPE_CURRENCY;
	pyesedb_column_types->values[ 6 ]  = (char) LIBESEDB_COLUMN_TYPE_FLOAT_32BIT;
	pyesedb_column_types->values[ 7 ]  = (char) LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT;
	pyesedb_column_types->values[ 8 ]  = (char) LIBESEDB_COLUMN_TYPE_DATE_TIME;
	pyesedb_column_types->values[ 9 ]  = (char) LIBESEDB_COLUMN_TYPE_BINARY_DATA;
	pyesedb_column_types->values[ 10 ] = (char) LIBESEDB_COLUMN_TYPE_TEXT;
	pyesedb_column_types->values[ 11 ] = (char) LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA;
	pyesedb_column_types->values[ 12 ] = (char) LIBESEDB_COLUMN_TYPE_LARGE_TEXT;
	pyesedb_column_types->values[ 13 ] = (char) LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE;
	pyesedb_column_types->values[ 14 ] = (char) LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED;
	pyesedb_column_types->values[ 15 ] = (char) LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED;
	pyesedb_column_types->values[ 16 ] = (char) LIBESEDB_COLUMN_TYPE_GUID;
	pyesedb_column_types->values[ 17 ] = (char) LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED;

	return( 0 );
}

/* Frees a column types object
 */
void pyesedb_column_types_free(
      pyesedb_column_types_t *pyesedb_column_types )
{
	static char *function = "pyesedb_column_types_free";

	if( pyesedb_column_types == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column types.",
		 function );

		return;
	}
	if( pyesedb_column_types->ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column types - missing ob_type.",
		 function );

		return;
	}
	if( pyesedb_column_types->ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column types - invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	pyesedb_column_types->ob_type->tp_free(
	 (PyObject*) pyesedb_column_types );
}

