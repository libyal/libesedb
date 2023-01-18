/*
 * Python object wrapper of libesedb_index_t
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
#include "pyesedb_index.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_record.h"
#include "pyesedb_records.h"
#include "pyesedb_unused.h"

PyMethodDef pyesedb_index_object_methods[] = {

	{ "get_identifier",
	  (PyCFunction) pyesedb_index_get_identifier,
	  METH_NOARGS,
	  "get_identifier() -> Integer\n"
	  "\n"
	  "Retrieves the identifier." },

	{ "get_name",
	  (PyCFunction) pyesedb_index_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_number_of_records",
	  (PyCFunction) pyesedb_index_get_number_of_records,
	  METH_NOARGS,
	  "get_number_of_records() -> Integer\n"
	  "\n"
	  "Retrieves the number of records." },

	{ "get_record",
	  (PyCFunction) pyesedb_index_get_record,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_record(record_entry) -> Object\n"
	  "\n"
	  "Retrieves the record specified by the entry." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyesedb_index_object_get_set_definitions[] = {

	{ "identifier",
	  (getter) pyesedb_index_get_identifier,
	  (setter) 0,
	  "The identifier.",
	  NULL },

	{ "name",
	  (getter) pyesedb_index_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "number_of_records",
	  (getter) pyesedb_index_get_number_of_records,
	  (setter) 0,
	  "The number of records.",
	  NULL },

	{ "records",
	  (getter) pyesedb_index_get_records,
	  (setter) 0,
	  "The records.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyesedb_index_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb.index",
	/* tp_basicsize */
	sizeof( pyesedb_index_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_index_free,
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
	"pyesedb index object (wraps libesedb_index_t)",
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
	pyesedb_index_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyesedb_index_object_get_set_definitions,
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
	(initproc) pyesedb_index_init,
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

/* Creates a new index object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_index_new(
           libesedb_index_t *index,
           PyObject *parent_object )
{
	pyesedb_index_t *pyesedb_index = NULL;
	static char *function          = "pyesedb_index_new";

	if( index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pyesedb_index = PyObject_New(
	                 struct pyesedb_index,
	                 &pyesedb_index_type_object );

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize index.",
		 function );

		goto on_error;
	}
	pyesedb_index->index         = index;
	pyesedb_index->parent_object = parent_object;

	if( pyesedb_index->parent_object != NULL )
	{
		Py_IncRef(
		 pyesedb_index->parent_object );
	}
	return( (PyObject *) pyesedb_index );

on_error:
	if( pyesedb_index != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_index );
	}
	return( NULL );
}

/* Initializes an index object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_index_init(
     pyesedb_index_t *pyesedb_index )
{
	static char *function = "pyesedb_index_init";

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return( -1 );
	}
	/* Make sure libesedb index is set to NULL
	 */
	pyesedb_index->index = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of index not supported.",
	 function );

	return( -1 );
}

/* Frees an index object
 */
void pyesedb_index_free(
      pyesedb_index_t *pyesedb_index )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyesedb_index_free";
	int result                  = 0;

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_index );

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
	if( pyesedb_index->index != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libesedb_index_free(
		          &( pyesedb_index->index ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libesedb index.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pyesedb_index->parent_object != NULL )
	{
		Py_DecRef(
		 pyesedb_index->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_index );
}

/* Retrieves the identifier
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_index_get_identifier(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyesedb_index_get_identifier";
	uint32_t value_32bit     = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_index_get_identifier(
	          pyesedb_index->index,
	          &value_32bit,
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
	integer_object = PyLong_FromUnsignedLong(
	                  (unsigned long) value_32bit );

	return( integer_object );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_index_get_name(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	const char *errors       = NULL;
	static char *function    = "pyesedb_index_get_name";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_index_get_utf8_name_size(
	          pyesedb_index->index,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of name as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( utf8_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	utf8_string = (char *) PyMem_Malloc(
	                        sizeof( char ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_index_get_utf8_name(
	          pyesedb_index->index,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string.
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 errors );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

/* Retrieves the number of records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_index_get_number_of_records(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pyesedb_index_get_number_of_records";
	int number_of_records    = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_index_get_number_of_records(
	          pyesedb_index->index,
	          &number_of_records,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of records.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_records );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_records );
#endif
	return( integer_object );
}

/* Retrieves a specific record by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_index_get_record_by_index(
           PyObject *pyesedb_index,
           int record_index )
{
	PyObject *record_object   = NULL;
	libcerror_error_t *error  = NULL;
	libesedb_record_t *record = NULL;
	static char *function     = "pyesedb_index_get_record_by_index";
	int result                = 0;

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_index_get_record(
	          ( (pyesedb_index_t *) pyesedb_index )->index,
	          record_index,
	          &record,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve record: %d.",
		 function,
		 record_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	record_object = pyesedb_record_new(
	                 record,
	                 pyesedb_index );

	if( record_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create record object.",
		 function );

		goto on_error;
	}
	return( record_object );

on_error:
	if( record != NULL )
	{
		libesedb_record_free(
		 &record,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific record
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_index_get_record(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *record_object     = NULL;
	static char *keyword_list[] = { "record_index", NULL };
	int record_index            = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &record_index ) == 0 )
	{
		return( NULL );
	}
	record_object = pyesedb_index_get_record_by_index(
	                 (PyObject *) pyesedb_index,
	                 record_index );

	return( record_object );
}

/* Retrieves a sequence and iterator object for the records
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_index_get_records(
           pyesedb_index_t *pyesedb_index,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pyesedb_index_get_records";
	int number_of_records     = 0;
	int result                = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid index.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_index_get_number_of_records(
	          pyesedb_index->index,
	          &number_of_records,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of records.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pyesedb_records_new(
	                   (PyObject *) pyesedb_index,
	                   &pyesedb_index_get_record_by_index,
	                   number_of_records );

	if( sequence_object == NULL )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

