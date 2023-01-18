/*
 * Python object wrapper of libesedb_long_value_t
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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyesedb_error.h"
#include "pyesedb_integer.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_long_value.h"
#include "pyesedb_python.h"
#include "pyesedb_unused.h"

PyMethodDef pyesedb_long_value_object_methods[] = {

	/* Functions to access the data */

	{ "get_data",
	  (PyCFunction) pyesedb_long_value_get_data,
	  METH_NOARGS,
	  "get_data() -> String or None\n"
	  "\n"
	  "Retrieves the data as a binary string." },

	{ "get_data_as_string",
	  (PyCFunction) pyesedb_long_value_get_data_as_string,
	  METH_NOARGS,
	  "get_data_as_string() -> Unicode string or None\n"
	  "\n"
	  "Retrieves the data as a string." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyesedb_long_value_object_get_set_definitions[] = {

	{ "data",
	  (getter) pyesedb_long_value_get_data,
	  (setter) 0,
	  "The data",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyesedb_long_value_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb.long_value",
	/* tp_basicsize */
	sizeof( pyesedb_long_value_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_long_value_free,
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
	"pyesedb long value object (wraps libesedb_long_value_t)",
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
	pyesedb_long_value_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyesedb_long_value_object_get_set_definitions,
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
	(initproc) pyesedb_long_value_init,
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

/* Creates a new long value object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_long_value_new(
           libesedb_long_value_t *long_value,
           PyObject *parent_object )
{
	pyesedb_long_value_t *pyesedb_long_value = NULL;
	static char *function                    = "pyesedb_long_value_new";

	if( long_value == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid long value.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyesedb_long_value = PyObject_New(
	                      struct pyesedb_long_value,
	                      &pyesedb_long_value_type_object );

	if( pyesedb_long_value == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize long value.",
		 function );

		goto on_error;
	}
	pyesedb_long_value->long_value    = long_value;
	pyesedb_long_value->parent_object = parent_object;

	if( pyesedb_long_value->parent_object != NULL )
	{
		Py_IncRef(
		 pyesedb_long_value->parent_object );
	}
	return( (PyObject *) pyesedb_long_value );

on_error:
	if( pyesedb_long_value != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_long_value );
	}
	return( NULL );
}

/* Initializes a long value object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_long_value_init(
     pyesedb_long_value_t *pyesedb_long_value )
{
	static char *function = "pyesedb_long_value_init";

	if( pyesedb_long_value == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid long value.",
		 function );

		return( -1 );
	}
	/* Make sure libesedb long value is set to NULL
	 */
	pyesedb_long_value->long_value = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of long value not supported.",
	 function );

	return( -1 );
}

/* Frees a long value object
 */
void pyesedb_long_value_free(
      pyesedb_long_value_t *pyesedb_long_value )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyesedb_long_value_free";
	int result                  = 0;

	if( pyesedb_long_value == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid long value.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_long_value );

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
	if( pyesedb_long_value->long_value != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libesedb_long_value_free(
		          &( pyesedb_long_value->long_value ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libesedb long value.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyesedb_long_value->parent_object != NULL )
	{
		Py_DecRef(
		 pyesedb_long_value->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_long_value );
}

/* Retrieves the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_long_value_get_data(
           pyesedb_long_value_t *pyesedb_long_value,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *data            = NULL;
	static char *function    = "pyesedb_long_value_get_data";
	size64_t data_size       = 0;
	int result               = 0;

	if( pyesedb_long_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid long value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_long_value_get_data_size(
	          pyesedb_long_value->long_value,
	          &data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve data size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( data_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	if( data_size > (size64_t) SSIZE_MAX )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
	data = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * data_size );

	if( data == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create data.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_long_value_get_data(
		  pyesedb_long_value->long_value,
		  data,
		  data_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve data.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
			 (char *) data,
			 (Py_ssize_t) data_size );
#else
	string_object = PyString_FromStringAndSize(
			 (char *) data,
			 (Py_ssize_t) data_size );
#endif
	PyMem_Free(
	 data );

	return( string_object );

on_error:
	if( data != NULL )
	{
		PyMem_Free(
		 data );
	}
	return( NULL );
}

/* Retrieves the data represented as a string
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_long_value_get_data_as_string(
           pyesedb_long_value_t *pyesedb_long_value,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_string    = NULL;
	const char *errors       = NULL;
	static char *function    = "pyesedb_long_value_get_data_as_string";
	size_t value_string_size = 0;
	int result               = 0;

	if( pyesedb_long_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid long value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_long_value_get_utf8_string_size(
	          pyesedb_long_value->long_value,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve string size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( value_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_string = (uint8_t *) PyMem_Malloc(
	                            sizeof( uint8_t ) * value_string_size );

	if( value_string == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_long_value_get_utf8_string(
		  pyesedb_long_value->long_value,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1,
			 errors );

	PyMem_Free(
	 value_string );

	return( string_object );

on_error:
	if( value_string != NULL )
	{
		PyMem_Free(
		 value_string );
	}
	return( NULL );
}

