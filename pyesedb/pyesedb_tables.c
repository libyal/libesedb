/*
 * Python object definition of the tables sequence and iterator
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

#include "pyesedb_file.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_table.h"
#include "pyesedb_tables.h"

PySequenceMethods pyesedb_tables_sequence_methods = {
	/* sq_length */
	(lenfunc) pyesedb_tables_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyesedb_tables_getitem,
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

PyTypeObject pyesedb_tables_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb._tables",
	/* tp_basicsize */
	sizeof( pyesedb_tables_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_tables_free,
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
	&pyesedb_tables_sequence_methods,
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
	"internal pyesedb tables sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyesedb_tables_iter,
	/* tp_iternext */
	(iternextfunc) pyesedb_tables_iternext,
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
	(initproc) pyesedb_tables_init,
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

/* Creates a new tables object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_tables_new(
           pyesedb_file_t *file_object,
           PyObject* (*get_table_by_index)(
                        pyesedb_file_t *file_object,
                        int table_entry ),
           int number_of_tables )
{
	pyesedb_tables_t *pyesedb_tables = NULL;
	static char *function            = "pyesedb_tables_new";

	if( file_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid file object.",
		 function );

		return( NULL );
	}
	if( get_table_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get table by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the tables values are initialized
	 */
	pyesedb_tables = PyObject_New(
	                  struct pyesedb_tables,
	                  &pyesedb_tables_type_object );

	if( pyesedb_tables == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize tables.",
		 function );

		goto on_error;
	}
	if( pyesedb_tables_init(
	     pyesedb_tables ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize tables.",
		 function );

		goto on_error;
	}
	pyesedb_tables->file_object        = file_object;
	pyesedb_tables->get_table_by_index = get_table_by_index;
	pyesedb_tables->number_of_tables   = number_of_tables;

	Py_IncRef(
	 (PyObject *) pyesedb_tables->file_object );

	return( (PyObject *) pyesedb_tables );

on_error:
	if( pyesedb_tables != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_tables );
	}
	return( NULL );
}

/* Intializes a tables object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_tables_init(
     pyesedb_tables_t *pyesedb_tables )
{
	static char *function = "pyesedb_tables_init";

	if( pyesedb_tables == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables.",
		 function );

		return( -1 );
	}
	/* Make sure the tables values are initialized
	 */
	pyesedb_tables->file_object        = NULL;
	pyesedb_tables->get_table_by_index = NULL;
	pyesedb_tables->table_entry        = 0;
	pyesedb_tables->number_of_tables   = 0;

	return( 0 );
}

/* Frees a tables object
 */
void pyesedb_tables_free(
      pyesedb_tables_t *pyesedb_tables )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyesedb_tables_free";

	if( pyesedb_tables == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_tables );

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
	if( pyesedb_tables->file_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_tables->file_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_tables );
}

/* The tables len() function
 */
Py_ssize_t pyesedb_tables_len(
            pyesedb_tables_t *pyesedb_tables )
{
	static char *function = "pyesedb_tables_len";

	if( pyesedb_tables == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyesedb_tables->number_of_tables );
}

/* The tables getitem() function
 */
PyObject *pyesedb_tables_getitem(
           pyesedb_tables_t *pyesedb_tables,
           Py_ssize_t item_index )
{
	PyObject *table_object = NULL;
	static char *function   = "pyesedb_tables_getitem";

	if( pyesedb_tables == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables.",
		 function );

		return( NULL );
	}
	if( pyesedb_tables->get_table_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables - missing get table by index function.",
		 function );

		return( NULL );
	}
	if( pyesedb_tables->number_of_tables < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables - invalid number of tables.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyesedb_tables->number_of_tables ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	table_object = pyesedb_tables->get_table_by_index(
	                pyesedb_tables->file_object,
	                (int) item_index );

	return( table_object );
}

/* The tables iter() function
 */
PyObject *pyesedb_tables_iter(
           pyesedb_tables_t *pyesedb_tables )
{
	static char *function = "pyesedb_tables_iter";

	if( pyesedb_tables == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyesedb_tables );

	return( (PyObject *) pyesedb_tables );
}

/* The tables iternext() function
 */
PyObject *pyesedb_tables_iternext(
           pyesedb_tables_t *pyesedb_tables )
{
	PyObject *table_object = NULL;
	static char *function  = "pyesedb_tables_iternext";

	if( pyesedb_tables == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables.",
		 function );

		return( NULL );
	}
	if( pyesedb_tables->get_table_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables - missing get table by index function.",
		 function );

		return( NULL );
	}
	if( pyesedb_tables->table_entry < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables - invalid table entry.",
		 function );

		return( NULL );
	}
	if( pyesedb_tables->number_of_tables < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid tables - invalid number of tables.",
		 function );

		return( NULL );
	}
	if( pyesedb_tables->table_entry >= pyesedb_tables->number_of_tables )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	table_object = pyesedb_tables->get_table_by_index(
	                pyesedb_tables->file_object,
	                pyesedb_tables->table_entry );

	if( table_object != NULL )
	{
		pyesedb_tables->table_entry++;
	}
	return( table_object );
}

