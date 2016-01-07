/*
 * Python object definition of the libesedb column
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

#include "pyesedb_column.h"
#include "pyesedb_error.h"
#include "pyesedb_integer.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libcstring.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_unused.h"

PyMethodDef pyesedb_column_object_methods[] = {

	/* Functions to access the column values */

	{ "get_identifier",
	  (PyCFunction) pyesedb_column_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Integer\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_type",
	  (PyCFunction) pyesedb_column_get_type,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the type." },

	{ "get_name",
	  (PyCFunction) pyesedb_column_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string\n"
	  "\n"
	  "Retrieves the name." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyesedb_column_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyesedb_column_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "type",
	  (getter) pyesedb_column_get_type,
	  (setter) 0,
	  "The type.",
	  NULL },

	{ "name",
	  (getter) pyesedb_column_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyesedb_column_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb.column",
	/* tp_basicsize */
	sizeof( pyesedb_column_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_column_free,
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
	"pyesedb column object (wraps libesedb_column_t)",
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
	pyesedb_column_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyesedb_column_object_get_set_definitions,
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
	(initproc) pyesedb_column_init,
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

/* Creates a new column object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_new(
           libesedb_column_t *column,
           pyesedb_table_t *table_object )
{
	pyesedb_column_t *pyesedb_column = NULL;
	static char *function            = "pyesedb_column_new";

	if( column == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column.",
		 function );

		return( NULL );
	}
	pyesedb_column = PyObject_New(
	                  struct pyesedb_column,
	                  &pyesedb_column_type_object );

	if( pyesedb_column == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize column.",
		 function );

		goto on_error;
	}
	if( pyesedb_column_init(
	     pyesedb_column ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize column.",
		 function );

		goto on_error;
	}
	pyesedb_column->column       = column;
	pyesedb_column->table_object = table_object;

	Py_IncRef(
	 (PyObject *) pyesedb_column->table_object );

	return( (PyObject *) pyesedb_column );

on_error:
	if( pyesedb_column != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_column );
	}
	return( NULL );
}

/* Intializes a column object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_column_init(
     pyesedb_column_t *pyesedb_column )
{
	static char *function = "pyesedb_column_init";

	if( pyesedb_column == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column.",
		 function );

		return( -1 );
	}
	/* Make sure libesedb column is set to NULL
	 */
	pyesedb_column->column = NULL;

	return( 0 );
}

/* Frees a column object
 */
void pyesedb_column_free(
      pyesedb_column_t *pyesedb_column )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyesedb_column_free";

	if( pyesedb_column == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column.",
		 function );

		return;
	}
	if( pyesedb_column->column == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column - missing libesedb column.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_column );

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
	if( libesedb_column_free(
	     &( pyesedb_column->column ),
	     &error ) != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free libesedb column.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pyesedb_column->table_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_column->table_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_column );
}

/* Retrieves the identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_get_identifier(
           pyesedb_column_t *pyesedb_column,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_column_get_identifier";
	uint32_t identifier      = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_column == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_column_get_identifier(
	          pyesedb_column->column,
	          &identifier,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve identifier.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyesedb_integer_unsigned_new_from_64bit(
	                  (uint64_t) identifier );

	return( integer_object );
}

/* Retrieves the type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_get_type(
           pyesedb_column_t *pyesedb_column,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_column_get_type";
	uint32_t type            = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_column == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_column_get_type(
	          pyesedb_column->column,
	          &type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyesedb_integer_unsigned_new_from_64bit(
	                  (uint64_t) type );

	return( integer_object );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_column_get_name(
           pyesedb_column_t *pyesedb_column,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	const char *errors       = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pyesedb_column_get_name";
	size_t name_size         = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_column == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid column.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_column_get_utf8_name_size(
	          pyesedb_column->column,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	name = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * name_size );

	if( name == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_column_get_utf8_name(
		  pyesedb_column->column,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name.",
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
			 (char *) name,
			 (Py_ssize_t) name_size - 1,
			 errors );

	PyMem_Free(
	 name );

	return( string_object );

on_error:
	if( name != NULL )
	{
		PyMem_Free(
		 name );
	}
	return( NULL );
}

