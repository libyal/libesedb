/*
 * Python object definition of the columns sequence and iterator
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
#include "pyesedb_columns.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_table.h"

PySequenceMethods pyesedb_columns_sequence_methods = {
	/* sq_length */
	(lenfunc) pyesedb_columns_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyesedb_columns_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pyesedb_columns_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb._columns",
	/* tp_basicsize */
	sizeof( pyesedb_columns_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_columns_free,
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
	&pyesedb_columns_sequence_methods,
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
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pyesedb columns sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyesedb_columns_iter,
	/* tp_iternext */
	(iternextfunc) pyesedb_columns_iternext,
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
	(initproc) pyesedb_columns_init,
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

/* Creates a new columns object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_columns_new(
           pyesedb_table_t *table_object,
           PyObject* (*get_column_by_index)(
                        pyesedb_table_t *table_object,
                        int column_entry ),
           int number_of_columns )
{
	pyesedb_columns_t *pyesedb_columns = NULL;
	static char *function              = "pyesedb_columns_new";

	if( table_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid table object.",
		 function );

		return( NULL );
	}
	if( get_column_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get column by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the columns values are initialized
	 */
	pyesedb_columns = PyObject_New(
	                  struct pyesedb_columns,
	                  &pyesedb_columns_type_object );

	if( pyesedb_columns == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize columns.",
		 function );

		goto on_error;
	}
	if( pyesedb_columns_init(
	     pyesedb_columns ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize columns.",
		 function );

		goto on_error;
	}
	pyesedb_columns->table_object        = table_object;
	pyesedb_columns->get_column_by_index = get_column_by_index;
	pyesedb_columns->number_of_columns   = number_of_columns;

	Py_IncRef(
	 (PyObject *) pyesedb_columns->table_object );

	return( (PyObject *) pyesedb_columns );

on_error:
	if( pyesedb_columns != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_columns );
	}
	return( NULL );
}

/* Intializes a columns object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_columns_init(
     pyesedb_columns_t *pyesedb_columns )
{
	static char *function = "pyesedb_columns_init";

	if( pyesedb_columns == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns.",
		 function );

		return( -1 );
	}
	/* Make sure the columns values are initialized
	 */
	pyesedb_columns->table_object        = NULL;
	pyesedb_columns->get_column_by_index = NULL;
	pyesedb_columns->column_entry        = 0;
	pyesedb_columns->number_of_columns   = 0;

	return( 0 );
}

/* Frees a columns object
 */
void pyesedb_columns_free(
      pyesedb_columns_t *pyesedb_columns )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyesedb_columns_free";

	if( pyesedb_columns == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_columns );

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
	if( pyesedb_columns->table_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_columns->table_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_columns );
}

/* The columns len() function
 */
Py_ssize_t pyesedb_columns_len(
            pyesedb_columns_t *pyesedb_columns )
{
	static char *function = "pyesedb_columns_len";

	if( pyesedb_columns == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyesedb_columns->number_of_columns );
}

/* The columns getitem() function
 */
PyObject *pyesedb_columns_getitem(
           pyesedb_columns_t *pyesedb_columns,
           Py_ssize_t item_index )
{
	PyObject *column_object = NULL;
	static char *function   = "pyesedb_columns_getitem";

	if( pyesedb_columns == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns.",
		 function );

		return( NULL );
	}
	if( pyesedb_columns->get_column_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns - missing get column by index function.",
		 function );

		return( NULL );
	}
	if( pyesedb_columns->number_of_columns < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns - invalid number of columns.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyesedb_columns->number_of_columns ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index column out of bounds.",
		 function );

		return( NULL );
	}
	column_object = pyesedb_columns->get_column_by_index(
	                 pyesedb_columns->table_object,
	                 (int) item_index );

	return( column_object );
}

/* The columns iter() function
 */
PyObject *pyesedb_columns_iter(
           pyesedb_columns_t *pyesedb_columns )
{
	static char *function = "pyesedb_columns_iter";

	if( pyesedb_columns == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyesedb_columns );

	return( (PyObject *) pyesedb_columns );
}

/* The columns iternext() function
 */
PyObject *pyesedb_columns_iternext(
           pyesedb_columns_t *pyesedb_columns )
{
	PyObject *column_object = NULL;
	static char *function   = "pyesedb_columns_iternext";

	if( pyesedb_columns == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns.",
		 function );

		return( NULL );
	}
	if( pyesedb_columns->get_column_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns - missing get column by index function.",
		 function );

		return( NULL );
	}
	if( pyesedb_columns->column_entry < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns - invalid column entry.",
		 function );

		return( NULL );
	}
	if( pyesedb_columns->number_of_columns < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid columns - invalid number of columns.",
		 function );

		return( NULL );
	}
	if( pyesedb_columns->column_entry >= pyesedb_columns->number_of_columns )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	column_object = pyesedb_columns->get_column_by_index(
	                 pyesedb_columns->table_object,
	                 pyesedb_columns->column_entry );

	if( column_object != NULL )
	{
		pyesedb_columns->column_entry++;
	}
	return( column_object );
}

