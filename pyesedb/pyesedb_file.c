/*
 * Python object wrapper of libesedb_file_t
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyesedb_error.h"
#include "pyesedb_file.h"
#include "pyesedb_file_object_io_handle.h"
#include "pyesedb_integer.h"
#include "pyesedb_libbfio.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libclocale.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_table.h"
#include "pyesedb_tables.h"
#include "pyesedb_unused.h"

#if !defined( LIBESEDB_HAVE_BFIO )

LIBESEDB_EXTERN \
int libesedb_file_open_file_io_handle(
     libesedb_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libesedb_error_t **error );

#endif /* !defined( LIBESEDB_HAVE_BFIO ) */

PyMethodDef pyesedb_file_object_methods[] = {

	{ "signal_abort",
	  (PyCFunction) pyesedb_file_signal_abort,
	  METH_NOARGS,
	  "signal_abort() -> None\n"
	  "\n"
	  "Signals the file to abort the current activity." },

	/* Functions to access the file */

	{ "open",
	  (PyCFunction) pyesedb_file_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> None\n"
	  "\n"
	  "Opens a file." },

	{ "open_file_object",
	  (PyCFunction) pyesedb_file_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> None\n"
	  "\n"
	  "Opens a file using a file-like object." },

	{ "close",
	  (PyCFunction) pyesedb_file_close,
	  METH_NOARGS,
	  "close() -> None\n"
	  "\n"
	  "Closes a file." },

	/* Functions to access the file values */

	{ "get_type",
	  (PyCFunction) pyesedb_file_get_type,
	  METH_NOARGS,
	  "get_type() -> Integer\n"
	  "\n"
	  "Retrieves the type." },

	{ "get_format_version",
	  (PyCFunction) pyesedb_file_get_format_version,
	  METH_NOARGS,
	  "get_format_version() -> Integer\n"
	  "\n"
	  "Retrieves the format version." },

	{ "get_creation_format_version",
	  (PyCFunction) pyesedb_file_get_creation_format_version,
	  METH_NOARGS,
	  "get_creation_format_version() -> Integer\n"
	  "\n"
	  "Retrieves the creation format version." },

	{ "get_page_size",
	  (PyCFunction) pyesedb_file_get_page_size,
	  METH_NOARGS,
	  "get_page_size() -> Integer\n"
	  "\n"
	  "Retrieves the page size." },

	/* Functions to access the tables */

	{ "get_number_of_tables",
	  (PyCFunction) pyesedb_file_get_number_of_tables,
	  METH_NOARGS,
	  "get_number_of_tables() -> Integer\n"
	  "\n"
	  "Retrieves the number of tables" },

	{ "get_table",
	  (PyCFunction) pyesedb_file_get_table,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_table(table_entry) -> Object or None\n"
	  "\n"
	  "Retrieves a specific table" },

	{ "get_table_by_name",
	  (PyCFunction) pyesedb_file_get_table_by_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_table_by_name(name) -> Object or None\n"
	  "\n"
	  "Retrieves a table specified by the name." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pyesedb_file_object_get_set_definitions[] = {

	{ "type",
	  (getter) pyesedb_file_get_type,
	  (setter) 0,
	  "The type.",
	  NULL },

	{ "format_version",
	  (getter) pyesedb_file_get_format_version,
	  (setter) 0,
	  "The format version.",
	  NULL },

	{ "creation_format_version",
	  (getter) pyesedb_file_get_creation_format_version,
	  (setter) 0,
	  "The creation format version.",
	  NULL },

	{ "page_size",
	  (getter) pyesedb_file_get_page_size,
	  (setter) 0,
	  "The page size.",
	  NULL },

	{ "number_of_tables",
	  (getter) pyesedb_file_get_number_of_tables,
	  (setter) 0,
	  "The number of tables",
	  NULL },

	{ "tables",
	  (getter) pyesedb_file_get_tables,
	  (setter) 0,
	  "The tables",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pyesedb_file_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb.file",
	/* tp_basicsize */
	sizeof( pyesedb_file_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_file_free,
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
	"pyesedb file object (wraps libesedb_file_t)",
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
	pyesedb_file_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pyesedb_file_object_get_set_definitions,
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
	(initproc) pyesedb_file_init,
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

/* Initializes a file object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_file_init(
     pyesedb_file_t *pyesedb_file )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyesedb_file_init";

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	/* Make sure libesedb file is set to NULL
	 */
	pyesedb_file->file           = NULL;
	pyesedb_file->file_io_handle = NULL;

	if( libesedb_file_initialize(
	     &( pyesedb_file->file ),
	     &error ) != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file.",
		 function );

		libcerror_error_free(
		 &error );

		return( -1 );
	}
	return( 0 );
}

/* Frees a file object
 */
void pyesedb_file_free(
      pyesedb_file_t *pyesedb_file )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pyesedb_file_free";
	int result                  = 0;

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_file );

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
	if( pyesedb_file->file_io_handle != NULL )
	{
		if( pyesedb_file_close(
		     pyesedb_file,
		     NULL ) == NULL )
		{
			return;
		}
	}
	if( pyesedb_file->file != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libesedb_file_free(
		          &( pyesedb_file->file ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libesedb file.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_file );
}

/* Signals the file to abort the current activity
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_signal_abort(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyesedb_file_signal_abort";	
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_signal_abort(
	          pyesedb_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to signal abort.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Opens a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_open(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyesedb_file_open";
	static char *keyword_list[]  = { "filename", "mode", NULL };
	const char *filename_narrow  = NULL;
	char *mode                   = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &string_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pyesedb_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
		Py_BEGIN_ALLOW_THREADS

		result = libesedb_file_open_wide(
		          pyesedb_file->file,
	                  filename_wide,
		          LIBESEDB_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pyesedb_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
				   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libesedb_file_open(
		          pyesedb_file->file,
	                  filename_narrow,
		          LIBESEDB_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result != 1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
		  string_object,
		  (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pyesedb_error_fetch_and_raise(
	         PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
				   string_object );
#else
		filename_narrow = PyString_AsString(
				   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libesedb_file_open(
		          pyesedb_file->file,
	                  filename_narrow,
		          LIBESEDB_OPEN_READ,
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to open file.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Opens a file using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_open_file_object(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object       = NULL;
	libcerror_error_t *error    = NULL;
	char *mode                  = NULL;
	static char *keyword_list[] = { "file_object", "mode", NULL };
	static char *function       = "pyesedb_file_open_file_object";
	int result                  = 0;

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|s",
	     keyword_list,
	     &file_object,
	     &mode ) == 0 )
	{
		return( NULL );
	}
	if( ( mode != NULL )
	 && ( mode[ 0 ] != 'r' ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: unsupported mode: %s.",
		 function,
		 mode );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "read" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing read attribute.",
		 function );

		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_HasAttrString(
	          file_object,
	          "seek" );

	if( result != 1 )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: unsupported file object - missing seek attribute.",
		 function );

		return( NULL );
	}
	if( pyesedb_file->file_io_handle != NULL )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: invalid file - file IO handle already set.",
		 function );

		goto on_error;
	}
	if( pyesedb_file_object_initialize(
	     &( pyesedb_file->file_io_handle ),
	     file_object,
	     &error ) != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_open_file_io_handle(
	          pyesedb_file->file,
	          pyesedb_file->file_io_handle,
	          LIBESEDB_OPEN_READ,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to open file.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );

on_error:
	if( pyesedb_file->file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &( pyesedb_file->file_io_handle ),
		 NULL );
	}
	return( NULL );
}

/* Closes a file
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_close(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pyesedb_file_close";
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_close(
	          pyesedb_file->file,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 0 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to close file.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( pyesedb_file->file_io_handle != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libbfio_handle_free(
		          &( pyesedb_file->file_io_handle ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to free libbfio file IO handle.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
	}
	Py_IncRef(
	 Py_None );

	return( Py_None );
}

/* Retrieves the type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_type(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_file_get_type";
	uint32_t type            = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_type(
	          pyesedb_file->file,
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

/* Retrieves the format version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_format_version(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_file_get_format_version";
	uint32_t format_revision = 0;
	uint32_t format_version  = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_format_version(
	          pyesedb_file->file,
	          &format_version,
	          &format_revision,
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
	                  ( (uint64_t) format_version << 32 ) | (uint64_t) format_revision );

	return( integer_object );
}

/* Retrieves the creation format version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_creation_format_version(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_file_get_creation_format_version";
	uint32_t format_revision = 0;
	uint32_t format_version  = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_creation_format_version(
	          pyesedb_file->file,
	          &format_version,
	          &format_revision,
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
	                  ( (uint64_t) format_version << 32 ) | (uint64_t) format_revision );

	return( integer_object );
}

/* Retrieves the page size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_page_size(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_file_get_page_size";
	uint32_t page_size       = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_page_size(
	          pyesedb_file->file,
	          &page_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve page size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pyesedb_integer_unsigned_new_from_64bit(
	                  (uint64_t) page_size );

	return( integer_object );
}

/* Retrieves the number of tables
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_number_of_tables(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pyesedb_file_get_number_of_tables";
	int number_of_tables     = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_number_of_tables(
	          pyesedb_file->file,
	          &number_of_tables,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of tables.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_tables );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_tables );
#endif
	return( integer_object );
}

/* Retrieves a specific table by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_table_by_index(
           PyObject *pyesedb_file,
           int table_entry )
{
	libcerror_error_t *error = NULL;
	libesedb_table_t *table  = NULL;
	PyObject *table_object   = NULL;
	static char *function    = "pyesedb_file_get_table_by_index";
	int result               = 0;

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_table(
	          ( (pyesedb_file_t *) pyesedb_file )->file,
	          table_entry,
	          &table,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve table: %d.",
		 function,
		 table_entry );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	table_object = pyesedb_table_new(
	                table,
	                pyesedb_file );

	if( table_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create table object.",
		 function );

		goto on_error;
	}
	return( table_object );

on_error:
	if( table != NULL )
	{
		libesedb_table_free(
		 &table,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific table
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_table(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *table_object      = NULL;
	static char *keyword_list[] = { "table_entry", NULL };
	int table_entry             = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &table_entry ) == 0 )
	{
		return( NULL );
	}
	table_object = pyesedb_file_get_table_by_index(
	                (PyObject *) pyesedb_file,
	                table_entry );

	return( table_object );
}

/* Retrieves a tables sequence and iterator object for the tables
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_tables(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *tables_object  = NULL;
	static char *function    = "pyesedb_file_get_tables";
	int number_of_tables     = 0;
	int result               = 0;

	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_number_of_tables(
	          pyesedb_file->file,
	          &number_of_tables,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of tables.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	tables_object = pyesedb_tables_new(
	                 (PyObject *) pyesedb_file,
	                 &pyesedb_file_get_table_by_index,
	                 number_of_tables );

	if( tables_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create tables object.",
		 function );

		return( NULL );
	}
	return( tables_object );
}

/* Retrieves the table specified by the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_file_get_table_by_name(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	libesedb_table_t *table     = NULL;
	PyObject *table_object      = NULL;
	char *table_name            = NULL;
	static char *keyword_list[] = { "table_name", NULL };
	static char *function       = "pyesedb_file_get_table_by_name";
	size_t table_name_length    = 0;
	int result                  = 0;

	if( pyesedb_file == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid file.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &table_name ) == 0 )
	{
		goto on_error;
	}
	table_name_length = narrow_string_length(
	                     table_name );

	Py_BEGIN_ALLOW_THREADS

	result = libesedb_file_get_table_by_utf8_name(
	          pyesedb_file->file,
	          (uint8_t *) table_name,
	          table_name_length,
	          &table,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve table.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if the table is present
	 */
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	table_object = pyesedb_table_new(
	                table,
	                (PyObject *) pyesedb_file );

	if( table_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create table object.",
		 function );

		goto on_error;
	}
	return( table_object );

on_error:
	if( table != NULL )
	{
		libesedb_table_free(
		 &table,
		 NULL );
	}
	return( NULL );
}

