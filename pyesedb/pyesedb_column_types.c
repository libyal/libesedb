/*
 * Python object definition of the libesedb column types
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyesedb_column_types.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_unused.h"

PyTypeObject pyesedb_column_types_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

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

/* Initializes the type object
 * Returns 1 if successful or -1 on error
 */
int pyesedb_column_types_init_type(
     PyTypeObject *type_object )
{
	PyObject *value_object = NULL;

	if( type_object == NULL )
	{
		return( -1 );
	}
	type_object->tp_dict = PyDict_New();

	if( type_object->tp_dict == NULL )
	{
		return( -1 );
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_NULL );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_NULL );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "NULL",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_BOOLEAN );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_BOOLEAN );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "BOOLEAN",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "INTEGER_8BIT_UNSIGNED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "INTEGER_16BIT_SIGNED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "INTEGER_32BIT_SIGNED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_CURRENCY );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_CURRENCY );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "CURRENCY",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_FLOAT_32BIT );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_FLOAT_32BIT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "FLOAT_32BIT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DOUBLE_64BIT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_DATE_TIME );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_DATE_TIME );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "DATE_TIME",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_BINARY_DATA );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_BINARY_DATA );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "BINARY_DATA",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_TEXT );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_TEXT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "TEXT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_LARGE_BINARY_DATA );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "LARGE_BINARY_DATA",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_LARGE_TEXT );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_LARGE_TEXT );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "LARGE_TEXT",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_SUPER_LARGE_VALUE );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "SUPER_LARGE_VALUE",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "INTEGER_32BIT_UNSIGNED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "INTEGER_64BIT_SIGNED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_GUID );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_GUID );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "GUID",
	     value_object ) != 0 )
	{
		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	value_object = PyLong_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED );
#else
	value_object = PyInt_FromLong(
	                LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED );
#endif
	if( PyDict_SetItemString(
	     type_object->tp_dict,
	     "INTEGER_16BIT_UNSIGNED",
	     value_object ) != 0 )
	{
		goto on_error;
	}
	return( 1 );

on_error:
	if( type_object->tp_dict != NULL )
	{
		Py_DecRef(
		 type_object->tp_dict );

		type_object->tp_dict = NULL;
	}
	return( -1 );
}

/* Creates a new column types object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_types_new(
           void )
{
	pyesedb_column_types_t *definitions_object = NULL;
	static char *function                      = "pyesedb_column_types_new";

	definitions_object = PyObject_New(
	                      struct pyesedb_column_types,
	                      &pyesedb_column_types_type_object );

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create definitions object.",
		 function );

		goto on_error;
	}
	if( pyesedb_column_types_init(
	     definitions_object ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize definitions object.",
		 function );

		goto on_error;
	}
	return( (PyObject *) definitions_object );

on_error:
	if( definitions_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) definitions_object );
	}
	return( NULL );
}

/* Initializes a column types object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_column_types_init(
     pyesedb_column_types_t *definitions_object )
{
	static char *function = "pyesedb_column_types_init";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Frees a column types object
 */
void pyesedb_column_types_free(
      pyesedb_column_types_t *definitions_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyesedb_column_types_free";

	if( definitions_object == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid definitions object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           definitions_object );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	ob_type->tp_free(
	 (PyObject*) definitions_object );
}

