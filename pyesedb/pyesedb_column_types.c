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
#include "pyesedb_unused.h"

PyGetSetDef pyesedb_column_types_object_get_set_definitions[] = {

	{ "NULL",
	  (getter) pyesedb_column_types_get_null,
	  (setter) 0,
	  "NULL column type (wraps LIBESEDB_COLUMN_TYPE_NULL)",
	  NULL },

	{ "BOOLEAN",
	  (getter) pyesedb_column_types_get_boolean,
	  (setter) 0,
	  "BOOLEAN column type (wraps LIBESEDB_COLUMN_TYPE_BOOLEAN)",
	  NULL },

	{ "INTEGER_8BIT_UNSIGNED",
	  (getter) pyesedb_column_types_get_integer_8bit_unsigned,
	  (setter) 0,
	  "INTEGER_8BIT_UNSIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED)",
	  NULL },

	{ "INTEGER_16BIT_SIGNED",
	  (getter) pyesedb_column_types_get_integer_16bit_signed,
	  (setter) 0,
	  "INTEGER_16BIT_SIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED)",
	  NULL },

	{ "INTEGER_32BIT_SIGNED",
	  (getter) pyesedb_column_types_get_integer_32bit_signed,
	  (setter) 0,
	  "INTEGER_32BIT_SIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED)",
	  NULL },

	{ "CURRENCY",
	  (getter) pyesedb_column_types_get_currency,
	  (setter) 0,
	  "CURRENCY column type (wraps LIBESEDB_COLUMN_TYPE_CURRENCY)",
	  NULL },

	{ "FLOAT_32BIT",
	  (getter) pyesedb_column_types_get_float_32bit,
	  (setter) 0,
	  "FLOAT_32BIT column type (wraps LIBESEDB_COLUMN_TYPE_FLOAT_32BIT)",
	  NULL },

	{ "DOUBLE_64BIT",
	  (getter) pyesedb_column_types_get_double_64bit,
	  (setter) 0,
	  "DOUBLE_64BIT column type (wraps LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT)",
	  NULL },

	{ "DATE_TIME",
	  (getter) pyesedb_column_types_get_date_time,
	  (setter) 0,
	  "DATE_TIME column type (wraps LIBESEDB_COLUMN_TYPE_DATE_TIME)",
	  NULL },

	{ "BINARY_DATA",
	  (getter) pyesedb_column_types_get_binary_data,
	  (setter) 0,
	  "BINARY_DATA column type (wraps LIBESEDB_COLUMN_TYPE_BINARY_DATA)",
	  NULL },

	{ "TEXT",
	  (getter) pyesedb_column_types_get_text,
	  (setter) 0,
	  "TEXT column type (wraps LIBESEDB_COLUMN_TYPE_TEXT)",
	  NULL },

	{ "LARGE_BINARY_DATA",
	  (getter) pyesedb_column_types_get_large_binary_data,
	  (setter) 0,
	  "LARGE_BINARY_DATA column type (wraps LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA)",
	  NULL },

	{ "LARGE_TEXT",
	  (getter) pyesedb_column_types_get_large_text,
	  (setter) 0,
	  "LARGE_TEXT column type (wraps LIBESEDB_COLUMN_TYPE_LARGE_TEXT)",
	  NULL },

	{ "SUPER_LARGE_VALUE",
	  (getter) pyesedb_column_types_get_super_large_value,
	  (setter) 0,
	  "SUPER_LARGE_VALUE column type (wraps LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE)",
	  NULL },

	{ "INTEGER_32BIT_UNSIGNED",
	  (getter) pyesedb_column_types_get_integer_32bit_unsigned,
	  (setter) 0,
	  "INTEGER_32BIT_UNSIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED)",
	  NULL },

	{ "INTEGER_64BIT_SIGNED",
	  (getter) pyesedb_column_types_get_integer_64bit_signed,
	  (setter) 0,
	  "INTEGER_64BIT_SIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED)",
	  NULL },

	{ "GUID",
	  (getter) pyesedb_column_types_get_guid,
	  (setter) 0,
	  "GUID column type (wraps LIBESEDB_COLUMN_TYPE_GUID)",
	  NULL },

	{ "INTEGER_16BIT_UNSIGNED",
	  (getter) pyesedb_column_types_get_integer_16bit_unsigned,
	  (setter) 0,
	  "INTEGER_16BIT_UNSIGNED column type (wraps LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED)",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
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
	0,
	/* tp_getset */
	pyesedb_column_types_object_get_set_definitions,
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

/* Retrieves the LIBESEDB_COLUMN_TYPE_NULL column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_null(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_NULL ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_BOOLEAN column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_boolean(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_BOOLEAN ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_integer_8bit_unsigned(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_integer_16bit_signed(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_integer_32bit_signed(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_CURRENCY column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_currency(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_CURRENCY ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_FLOAT_32BIT column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_float_32bit(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_FLOAT_32BIT ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_double_64bit(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_DATE_TIME column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_date_time(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_DATE_TIME ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_BINARY_DATA column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_binary_data(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_BINARY_DATA ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_TEXT column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_text(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_TEXT ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_large_binary_data(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_LARGE_TEXT column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_large_text(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_super_large_value(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_integer_32bit_unsigned(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_integer_64bit_signed(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_GUID column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_guid(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_GUID ) );
}

/* Retrieves the LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_get_integer_16bit_unsigned(
           pyesedb_column_types_t *pyesedb_column_types,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	return( PyInt_FromLong(
	         (long) LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED ) );
}

