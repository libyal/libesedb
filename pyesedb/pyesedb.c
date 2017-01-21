/*
 * Python bindings module for libesedb (pyesedb)
 *
 * Copyright (C) 2009-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pyesedb.h"
#include "pyesedb_column.h"
#include "pyesedb_column_types.h"
#include "pyesedb_columns.h"
#include "pyesedb_error.h"
#include "pyesedb_file.h"
#include "pyesedb_file_object_io_handle.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_long_value.h"
#include "pyesedb_multi_value.h"
#include "pyesedb_python.h"
#include "pyesedb_record.h"
#include "pyesedb_records.h"
#include "pyesedb_table.h"
#include "pyesedb_tables.h"
#include "pyesedb_unused.h"
#include "pyesedb_value_flags.h"

#if !defined( LIBESEDB_HAVE_BFIO )
LIBESEDB_EXTERN \
int libesedb_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libesedb_error_t **error );
#endif

/* The pyesedb module methods
 */
PyMethodDef pyesedb_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pyesedb_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_file_signature",
	  (PyCFunction) pyesedb_check_file_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a file has a Personal Folder Format (ESEDB) signature." },

	{ "check_file_signature_file_object",
	  (PyCFunction) pyesedb_check_file_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature_file_object(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a file has a Personal Folder Format (ESEDB) signature using a file-like object." },

	{ "open",
	  (PyCFunction) pyesedb_file_new_open,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a file." },

	{ "open_file_object",
	  (PyCFunction) pyesedb_file_new_open_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a file using a file-like object." },

	/* Sentinel */
	{ NULL,
	  NULL,
	  0,
	  NULL}
};

/* Retrieves the pyesedb/libesedb version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_get_version(
           PyObject *self PYESEDB_ATTRIBUTE_UNUSED,
           PyObject *arguments PYESEDB_ATTRIBUTE_UNUSED )
{
	const char *errors           = NULL;
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYESEDB_UNREFERENCED_PARAMETER( self )
	PYESEDB_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libesedb_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         errors ) );
}

/* Checks if the file has a Personal Folder File (ESEDB) signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_check_file_signature(
           PyObject *self PYESEDB_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object      = NULL;
	libcerror_error_t *error     = NULL;
	static char *function        = "pyesedb_check_file_signature";
	static char *keyword_list[]  = { "filename", NULL };
	const char *filename_narrow  = NULL;
	int result                   = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	PYESEDB_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &string_object ) == 0 )
	{
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

		result = libesedb_check_file_signature_wide(
		          filename_wide,
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

		result = libesedb_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );
#endif
		if( result == -1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
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

		result = libesedb_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pyesedb_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
			 function );

			libcerror_error_free(
			 &error );

			return( NULL );
		}
		if( result != 0 )
		{
			Py_IncRef(
			 (PyObject *) Py_True );

			return( Py_True );
		}
		Py_IncRef(
		 (PyObject *) Py_False );

		return( Py_False );
	}
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if the file has a Personal Folder File (ESEDB) file signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_check_file_signature_file_object(
           PyObject *self PYESEDB_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error         = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	PyObject *file_object            = NULL;
	static char *function            = "pyesedb_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYESEDB_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "|O",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pyesedb_file_object_initialize(
	     &file_io_handle,
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

	result = libesedb_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check file signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pyesedb_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

#if PY_MAJOR_VERSION >= 3

/* The pyesedb module definition
 */
PyModuleDef pyesedb_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pyesedb",
	/* m_doc */
	"Python libesedb module (pyesedb).",
	/* m_size */
	-1,
	/* m_methods */
	pyesedb_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pyesedb module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pyesedb(
                void )
#else
PyMODINIT_FUNC initpyesedb(
                void )
#endif
{
	PyObject *module                       = NULL;
	PyTypeObject *column_type_object       = NULL;
	PyTypeObject *column_types_type_object = NULL;
	PyTypeObject *columns_type_object      = NULL;
	PyTypeObject *file_type_object         = NULL;
	PyTypeObject *long_value_type_object   = NULL;
	PyTypeObject *multi_value_type_object  = NULL;
	PyTypeObject *record_type_object       = NULL;
	PyTypeObject *records_type_object      = NULL;
	PyTypeObject *table_type_object        = NULL;
	PyTypeObject *tables_type_object       = NULL;
	PyTypeObject *value_flags_type_object  = NULL;
	PyGILState_STATE gil_state             = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libesedb_notify_set_stream(
	 stderr,
	 NULL );
	libesedb_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pyesedb_module_definition );
#else
	module = Py_InitModule3(
	          "pyesedb",
	          pyesedb_module_methods,
	          "Python libesedb module (pyesedb)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
	PyEval_InitThreads();

	gil_state = PyGILState_Ensure();

	/* Setup the file type object
	 */
	pyesedb_file_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_file_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_file_type_object );

	file_type_object = &pyesedb_file_type_object;

	PyModule_AddObject(
	 module,
	 "file",
	 (PyObject *) file_type_object );

	/* Setup the tables type object
	 */
	pyesedb_tables_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_tables_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_tables_type_object );

	tables_type_object = &pyesedb_tables_type_object;

	PyModule_AddObject(
	 module,
	 "_tables",
	 (PyObject *) tables_type_object );

	/* Setup the table type object
	 */
	pyesedb_table_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_table_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_table_type_object );

	table_type_object = &pyesedb_table_type_object;

	PyModule_AddObject(
	 module,
	 "table",
	 (PyObject *) table_type_object );

	/* Setup the columns type object
	 */
	pyesedb_columns_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_columns_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_columns_type_object );

	columns_type_object = &pyesedb_columns_type_object;

	PyModule_AddObject(
	 module,
	 "_columns",
	 (PyObject *) columns_type_object );

	/* Setup the column type object
	 */
	pyesedb_column_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_column_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_column_type_object );

	column_type_object = &pyesedb_column_type_object;

	PyModule_AddObject(
	 module,
	 "column",
	 (PyObject *) column_type_object );

	/* Setup the records type object
	 */
	pyesedb_records_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_records_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_records_type_object );

	records_type_object = &pyesedb_records_type_object;

	PyModule_AddObject(
	 module,
	 "_records",
	 (PyObject *) records_type_object );

	/* Setup the record type object
	 */
	pyesedb_record_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_record_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_record_type_object );

	record_type_object = &pyesedb_record_type_object;

	PyModule_AddObject(
	 module,
	 "record",
	 (PyObject *) record_type_object );

	/* Setup the long value type object
	 */
	pyesedb_long_value_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_long_value_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_long_value_type_object );

	long_value_type_object = &pyesedb_long_value_type_object;

	PyModule_AddObject(
	 module,
	 "long_value",
	 (PyObject *) long_value_type_object );

	/* Setup the multi value type object
	 */
	pyesedb_multi_value_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pyesedb_multi_value_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_multi_value_type_object );

	multi_value_type_object = &pyesedb_multi_value_type_object;

	PyModule_AddObject(
	 module,
	 "multi_value",
	 (PyObject *) multi_value_type_object );

	/* Setup the column types type object
	 */
	pyesedb_column_types_type_object.tp_new = PyType_GenericNew;

	if( pyesedb_column_types_init_type(
	     &pyesedb_column_types_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyesedb_column_types_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_column_types_type_object );

	column_types_type_object = &pyesedb_column_types_type_object;

	PyModule_AddObject(
	 module,
	 "column_types",
	 (PyObject *) column_types_type_object );

	/* Setup the value flags type object
	 */
	pyesedb_value_flags_type_object.tp_new = PyType_GenericNew;

	if( pyesedb_value_flags_init_type(
	     &pyesedb_value_flags_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pyesedb_value_flags_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pyesedb_value_flags_type_object );

	value_flags_type_object = &pyesedb_value_flags_type_object;

	PyModule_AddObject(
	 module,
	 "value_flags",
	 (PyObject *) value_flags_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

