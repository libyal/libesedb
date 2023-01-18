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
#include "pyesedb_record.h"
#include "pyesedb_unused.h"

PyMethodDef pyesedb_record_object_methods[] = {

	/* Functions to access the record values */

	{ "get_number_of_values",
	  (PyCFunction) pyesedb_record_get_number_of_values,
	  METH_NOARGS,
	  "get_number_of_values() -> Integer\n"
	  "\n"
	  "Retrieves the number of values" },

	{ "get_column_type",
	  (PyCFunction) pyesedb_record_get_column_type,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_column_type(value_entry) -> Integer\n"
	  "\n"
	  "Retrieves the column type." },

	{ "get_column_name",
	  (PyCFunction) pyesedb_record_get_column_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_column_name(value_entry) -> Unicode string\n"
	  "\n"
	  "Retrieves the column name." },

	{ "get_value_data_flags",
	  (PyCFunction) pyesedb_record_get_value_data_flags,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_data(value_entry) -> Integer\n"
	  "\n"
	  "Retrieves the value data flags." },

	{ "is_long_value",
	  (PyCFunction) pyesedb_record_is_long_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "is_long_value(value_entry) -> Boolean\n"
	  "\n"
	  "Determines if the value is a long value." },

	{ "is_multi_value",
	  (PyCFunction) pyesedb_record_is_multi_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "is_multi_value(value_entry) -> Boolean\n"
	  "\n"
	  "Determines if the value is a multi value." },

	{ "get_value_data",
	  (PyCFunction) pyesedb_record_get_value_data,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_data(value_entry) -> String or None\n"
	  "\n"
	  "Retrieves the value data as a binary string." },

	{ "get_value_data_as_boolean",
	  (PyCFunction) pyesedb_record_get_value_data_as_boolean,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_data_as_boolean(value_entry) -> Boolean or None\n"
	  "\n"
	  "Retrieves the value data as a boolean." },

	{ "get_value_data_as_floating_point",
	  (PyCFunction) pyesedb_record_get_value_data_as_floating_point,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_data_as_floating_point(value_entry) -> Float or None\n"
	  "\n"
	  "Retrieves the value data as a floating point." },

	{ "get_value_data_as_integer",
	  (PyCFunction) pyesedb_record_get_value_data_as_integer,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_data_as_integer(value_entry) -> Integer or None\n"
	  "\n"
	  "Retrieves the value data as an integer." },

	{ "get_value_data_as_string",
	  (PyCFunction) pyesedb_record_get_value_data_as_string,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_data_as_string(value_entry) -> Unicode string or None\n"
	  "\n"
	  "Retrieves the value data as a string." },

	{ "get_value_data_as_long_value",
	  (PyCFunction) pyesedb_record_get_value_data_as_long_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_data_as_long_value(value_entry) -> pyesedb.long_value or None\n"
	  "\n"
	  "Retrieves the value data as a long value." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyesedb_record_object_get_set_definitions[] = {

	{ "number_of_values",
	  (getter) pyesedb_record_get_number_of_values,
	  (setter) 0,
	  "The number of values",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyesedb_record_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb.record",
	/* tp_basicsize */
	sizeof( pyesedb_record_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_record_free,
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
	"pyesedb record object (wraps libesedb_record_t)",
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
	pyesedb_record_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyesedb_record_object_get_set_definitions,
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
	(initproc) pyesedb_record_init,
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

/* Creates a new record object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_new(
           libesedb_record_t *record,
           PyObject *parent_object )
{
	pyesedb_record_t *pyesedb_record = NULL;
	static char *function            = "pyesedb_record_new";

	if( record == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyesedb_record = PyObject_New(
	                  struct pyesedb_record,
	                  &pyesedb_record_type_object );

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize record.",
		 function );

		goto on_error;
	}
	pyesedb_record->record        = record;
	pyesedb_record->parent_object = parent_object;

	if( pyesedb_record->parent_object != NULL )
	{
		Py_IncRef(
		 pyesedb_record->parent_object );
	}
	return( (PyObject *) pyesedb_record );

on_error:
	if( pyesedb_record != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_record );
	}
	return( NULL );
}

/* Initializes a record object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_record_init(
     pyesedb_record_t *pyesedb_record )
{
	static char *function = "pyesedb_record_init";

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	/* Make sure libesedb record is set to NULL
	 */
	pyesedb_record->record = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of record not supported.",
	 function );

	return( -1 );
}

/* Frees a record object
 */
void pyesedb_record_free(
      pyesedb_record_t *pyesedb_record )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyesedb_record_free";
	int result                  = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid record.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_record );

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
	if( pyesedb_record->record != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libesedb_record_free(
		          &( pyesedb_record->record ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libesedb record.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyesedb_record->parent_object != NULL )
	{
		Py_DecRef(
		 pyesedb_record->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_record );
}

/* Retrieves the number of values
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_number_of_values(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_record_get_number_of_values";
	int number_of_values     = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_number_of_values(
	          pyesedb_record->record,
	          &number_of_values,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of values.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_values );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_values );
#endif
	return( integer_object );
}

/* Retrieves the column type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_column_type(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	static char *function       = "pyesedb_record_get_column_type";
	static char *keyword_list[] = { "value_entry", NULL };
	uint32_t column_type        = 0;
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_column_type(
	          pyesedb_record->record,
	          value_entry,
	          &column_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve column type: %d.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyesedb_integer_unsigned_new_from_64bit(
	                  (uint64_t) column_type );

	return( integer_object );
}

/* Retrieves the column name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_column_name(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	uint8_t *column_name        = NULL;
	const char *errors          = NULL;
	static char *function       = "pyesedb_record_get_column_name";
	static char *keyword_list[] = { "value_entry", NULL };
	size_t column_name_size     = 0;
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_utf8_column_name_size(
	          pyesedb_record->record,
	          value_entry,
	          &column_name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve column name: %d size.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( column_name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	column_name = (uint8_t *) PyMem_Malloc(
	                           sizeof( uint8_t ) * column_name_size );

	if( column_name == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create column name: %d.",
		 function,
		 value_entry );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_utf8_column_name(
		  pyesedb_record->record,
		  value_entry,
		  column_name,
		  column_name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d data.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) column_name,
			 (Py_ssize_t) column_name_size - 1,
			 errors );

	PyMem_Free(
	 column_name );

	return( string_object );

on_error:
	if( column_name != NULL )
	{
		PyMem_Free(
		 column_name );
	}
	return( NULL );
}

/* Retrieves the value data flags
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_value_data_flags(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	static char *function       = "pyesedb_record_get_value_data_flags";
	static char *keyword_list[] = { "value_entry", NULL };
	uint8_t value_data_flags    = 0;
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_value_data_flags(
	          pyesedb_record->record,
	          value_entry,
	          &value_data_flags,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d data flags.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyesedb_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_data_flags );

	return( integer_object );
}

/* Determines if a specific entry is a long value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_is_long_value(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyesedb_record_is_long_value";
	static char *keyword_list[] = { "value_entry", NULL };
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_is_long_value(
	          pyesedb_record->record,
	          value_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if value: %d is a long value.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Determines if a specific entry is a multi value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_is_multi_value(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyesedb_record_is_multi_value";
	static char *keyword_list[] = { "value_entry", NULL };
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_is_multi_value(
	          pyesedb_record->record,
	          value_entry,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if value: %d is a multi value.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Retrieves the value data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_value_data(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	uint8_t *value_data         = NULL;
	static char *function       = "pyesedb_record_get_value_data";
	static char *keyword_list[] = { "value_entry", NULL };
	size_t value_data_size      = 0;
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_value_data_size(
	          pyesedb_record->record,
	          value_entry,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d data size.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( value_data_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_data = (uint8_t *) PyMem_Malloc(
	                          sizeof( uint8_t ) * value_data_size );

	if( value_data == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to create value: %d data.",
		 function,
		 value_entry );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_value_data(
		  pyesedb_record->record,
		  value_entry,
		  value_data,
		  value_data_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d data.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
			 (char *) value_data,
			 (Py_ssize_t) value_data_size );
#else
	string_object = PyString_FromStringAndSize(
			 (char *) value_data,
			 (Py_ssize_t) value_data_size );
#endif
	PyMem_Free(
	 value_data );

	return( string_object );

on_error:
	if( value_data != NULL )
	{
		PyMem_Free(
		 value_data );
	}
	return( NULL );
}

/* Retrieves the value data represented as a boolean
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_value_data_as_boolean(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	static char *function       = "pyesedb_record_get_value_data_as_boolean";
	static char *keyword_list[] = { "value_entry", NULL };
	uint32_t column_type        = 0;
	uint8_t value_8bit          = 0;
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_column_type(
	          pyesedb_record->record,
	          value_entry,
	          &column_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve column: %d type.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( column_type )
	{
		case LIBESEDB_COLUMN_TYPE_BOOLEAN:
			Py_BEGIN_ALLOW_THREADS

			result = libesedb_record_get_value_boolean(
				  pyesedb_record->record,
				  value_entry,
				  &value_8bit,
				  &error );

			Py_END_ALLOW_THREADS

			break;

		default:
			PyErr_Format(
			 PyExc_IOError,
			 "%s: value: %d is not a boolean type.",
			 function,
			 value_entry );

			return( NULL );
	}
	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve boolean value: %d.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	if( value_8bit != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Retrieves the value data represented as a floating point
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_value_data_as_floating_point(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error        = NULL;
	PyObject *floating_point_object = NULL;
	static char *function           = "pyesedb_record_get_value_data_as_floating_point";
	static char *keyword_list[]     = { "value_entry", NULL };
	double value_64bit              = 0.0;
	float value_32bit               = 0.0;
	uint32_t column_type            = 0;
	int result                      = 0;
	int value_entry                 = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_column_type(
	          pyesedb_record->record,
	          value_entry,
	          &column_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve column: %d type.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( column_type )
	{
		case LIBESEDB_COLUMN_TYPE_FLOAT_32BIT:
			Py_BEGIN_ALLOW_THREADS

			result = libesedb_record_get_value_floating_point_32bit(
				  pyesedb_record->record,
				  value_entry,
				  &value_32bit,
				  &error );

			Py_END_ALLOW_THREADS

			value_64bit = (double) value_32bit;

			break;

		case LIBESEDB_COLUMN_TYPE_DOUBLE_64BIT:
			Py_BEGIN_ALLOW_THREADS

			result = libesedb_record_get_value_floating_point_64bit(
				  pyesedb_record->record,
				  value_entry,
				  &value_64bit,
				  &error );

			Py_END_ALLOW_THREADS

			break;

		default:
			PyErr_Format(
			 PyExc_IOError,
			 "%s: value: %d is not a floating point type.",
			 function,
			 value_entry );

			return( NULL );
	}
	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve floating point value: %d.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	floating_point_object = PyFloat_FromDouble(
	                         value_64bit );

	return( floating_point_object );
}

/* Retrieves the value data represented as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_value_data_as_integer(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *integer_object    = NULL;
	static char *function       = "pyesedb_record_get_value_data_as_integer";
	static char *keyword_list[] = { "value_entry", NULL };
	uint64_t value_64bit        = 0;
	int64_t integer_value       = 0;
	uint32_t column_type        = 0;
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
	uint8_t value_is_signed     = 0;
	uint8_t value_8bit          = 0;
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_column_type(
	          pyesedb_record->record,
	          value_entry,
	          &column_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve column: %d type.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( column_type )
	{
		case LIBESEDB_COLUMN_TYPE_INTEGER_8BIT_UNSIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libesedb_record_get_value_8bit(
				  pyesedb_record->record,
				  value_entry,
				  &value_8bit,
				  &error );

			Py_END_ALLOW_THREADS

			/* Interpret the 8-bit value as unsigned
			 */
			integer_value = (uint8_t) value_8bit;

			break;

		case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED:
		case LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_UNSIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libesedb_record_get_value_16bit(
				  pyesedb_record->record,
				  value_entry,
				  &value_16bit,
				  &error );

			Py_END_ALLOW_THREADS

			if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_16BIT_SIGNED )
			{
				/* Interpret the 16-bit value as signed
				 */
				integer_value   = (int16_t) value_16bit;
				value_is_signed = 1;
			}
			else
			{
				/* Interpret the 16-bit value as unsigned
				 */
				integer_value = (uint16_t) value_16bit;
			}
			break;

		case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED:
		case LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_UNSIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libesedb_record_get_value_32bit(
				  pyesedb_record->record,
				  value_entry,
				  &value_32bit,
				  &error );

			Py_END_ALLOW_THREADS

			if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_32BIT_SIGNED )
			{
				/* Interpret the 32-bit value as signed
				 */
				integer_value   = (int32_t) value_32bit;
				value_is_signed = 1;
			}
			else
			{
				/* Interpret the 32-bit value as unsigned
				 */
				integer_value = (uint32_t) value_32bit;
			}
			break;

		case LIBESEDB_COLUMN_TYPE_CURRENCY:
		case LIBESEDB_COLUMN_TYPE_DATE_TIME:
		case LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED:
			Py_BEGIN_ALLOW_THREADS

			result = libesedb_record_get_value_64bit(
				  pyesedb_record->record,
				  value_entry,
				  &value_64bit,
				  &error );

			Py_END_ALLOW_THREADS

			if( column_type == LIBESEDB_COLUMN_TYPE_INTEGER_64BIT_SIGNED )
			{
				/* Interpret the 64-bit value as signed
				 */
				integer_value   = (int64_t) value_64bit;
				value_is_signed = 1;
			}
			else
			{
				/* Interpret the 64-bit value as unsigned
				 */
				integer_value = (uint64_t) value_64bit;
			}
			break;

		default:
			PyErr_Format(
			 PyExc_IOError,
			 "%s: value: %d is not an integer type.",
			 function,
			 value_entry );

			return( NULL );
	}
	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve integer value: %d.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	if( value_is_signed != 0 )
	{
		integer_object = pyesedb_integer_signed_new_from_64bit(
		                  integer_value );
	}
	else
	{
		integer_object = pyesedb_integer_unsigned_new_from_64bit(
		                  integer_value );
	}
	return( integer_object );
}

/* Retrieves the value data represented as a string
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_value_data_as_string(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	PyObject *string_object     = NULL;
	uint8_t *value_string       = NULL;
	const char *errors          = NULL;
	static char *keyword_list[] = { "value_entry", NULL };
	static char *function       = "pyesedb_record_get_value_data_as_string";
	size_t value_string_size    = 0;
	uint32_t column_type        = 0;
	int result                  = 0;
	int value_entry             = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_column_type(
	          pyesedb_record->record,
	          value_entry,
	          &column_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve column: %d type.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( ( column_type != LIBESEDB_COLUMN_TYPE_TEXT )
	 && ( column_type != LIBESEDB_COLUMN_TYPE_LARGE_TEXT ) )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: value: %d is not a string type.",
		 function,
		 value_entry );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_value_utf8_string_size(
	          pyesedb_record->record,
	          value_entry,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d string size.",
		 function,
		 value_entry );

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
		 "%s: unable to create value: %d string.",
		 function,
		 value_entry );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_value_utf8_string(
		  pyesedb_record->record,
		  value_entry,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d string.",
		 function,
		 value_entry );

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

/* Retrieves the value data represented as a long value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_record_get_value_data_as_long_value(
           pyesedb_record_t *pyesedb_record,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error          = NULL;
	libesedb_long_value_t *long_value = NULL;
	PyObject *long_value_object       = NULL;
	static char *keyword_list[]       = { "value_entry", NULL };
	static char *function             = "pyesedb_record_get_value_data_as_long_value";
	int result                        = 0;
	int value_entry                   = 0;

	if( pyesedb_record == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid record.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_entry ) == 0 )
	{
		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_record_get_long_value(
	          pyesedb_record->record,
	          value_entry,
	          &long_value,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve long value: %d.",
		 function,
		 value_entry );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	long_value_object = pyesedb_long_value_new(
	                     long_value,
	                     (PyObject *) pyesedb_record );

	if( long_value_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create long value object.",
		 function );

		goto on_error;
	}
	return( long_value_object );

on_error:
	if( long_value != NULL )
	{
		libesedb_long_value_free(
		 &long_value,
		 NULL );
	}
	return( NULL );
}

