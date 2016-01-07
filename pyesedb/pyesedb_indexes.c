/*
 * Python object definition of the indexes sequence and iterator
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

#include "pyesedb_libcerror.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_index.h"
#include "pyesedb_indexes.h"
#include "pyesedb_table.h"

PySequenceMethods pyesedb_indexes_sequence_methods = {
	/* sq_length */
	(lenfunc) pyesedb_indexes_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyesedb_indexes_getitem,
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

PyTypeObject pyesedb_indexes_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb._indexes",
	/* tp_basicsize */
	sizeof( pyesedb_indexes_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_indexes_free,
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
	&pyesedb_indexes_sequence_methods,
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
	"internal pyesedb indexes sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyesedb_indexes_iter,
	/* tp_iternext */
	(iternextfunc) pyesedb_indexes_iternext,
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
	(initproc) pyesedb_indexes_init,
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

/* Creates a new indexes object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_indexes_new(
           pyesedb_table_t *table_object,
           PyObject* (*get_index_by_index)(
                        pyesedb_table_t *table_object,
                        int index_entry ),
           int number_of_indexes )
{
	pyesedb_indexes_t *pyesedb_indexes = NULL;
	static char *function              = "pyesedb_indexes_new";

	if( table_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid table object.",
		 function );

		return( NULL );
	}
	if( get_index_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get index by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the indexes values are initialized
	 */
	pyesedb_indexes = PyObject_New(
	                   struct pyesedb_indexes,
	                   &pyesedb_indexes_type_object );

	if( pyesedb_indexes == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize indexes.",
		 function );

		goto on_error;
	}
	if( pyesedb_indexes_init(
	     pyesedb_indexes ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize indexes.",
		 function );

		goto on_error;
	}
	pyesedb_indexes->table_object       = table_object;
	pyesedb_indexes->get_index_by_index = get_index_by_index;
	pyesedb_indexes->number_of_indexes  = number_of_indexes;

	Py_IncRef(
	 (PyObject *) pyesedb_indexes->table_object );

	return( (PyObject *) pyesedb_indexes );

on_error:
	if( pyesedb_indexes != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_indexes );
	}
	return( NULL );
}

/* Intializes a indexes object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_indexes_init(
     pyesedb_indexes_t *pyesedb_indexes )
{
	static char *function = "pyesedb_indexes_init";

	if( pyesedb_indexes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes.",
		 function );

		return( -1 );
	}
	/* Make sure the indexes values are initialized
	 */
	pyesedb_indexes->table_object       = NULL;
	pyesedb_indexes->get_index_by_index = NULL;
	pyesedb_indexes->index_entry        = 0;
	pyesedb_indexes->number_of_indexes  = 0;

	return( 0 );
}

/* Frees a indexes object
 */
void pyesedb_indexes_free(
      pyesedb_indexes_t *pyesedb_indexes )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyesedb_indexes_free";

	if( pyesedb_indexes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pyesedb_indexes );

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
	if( pyesedb_indexes->table_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pyesedb_indexes->table_object );
	}
	ob_type->tp_free(
	 (PyObject*) pyesedb_indexes );
}

/* The indexes len() function
 */
Py_ssize_t pyesedb_indexes_len(
            pyesedb_indexes_t *pyesedb_indexes )
{
	static char *function = "pyesedb_indexes_len";

	if( pyesedb_indexes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pyesedb_indexes->number_of_indexes );
}

/* The indexes getitem() function
 */
PyObject *pyesedb_indexes_getitem(
           pyesedb_indexes_t *pyesedb_indexes,
           Py_ssize_t item_index )
{
	PyObject *index_object = NULL;
	static char *function  = "pyesedb_indexes_getitem";

	if( pyesedb_indexes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes.",
		 function );

		return( NULL );
	}
	if( pyesedb_indexes->get_index_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes - missing get index by index function.",
		 function );

		return( NULL );
	}
	if( pyesedb_indexes->number_of_indexes < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes - invalid number of indexes.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pyesedb_indexes->number_of_indexes ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	index_object = pyesedb_indexes->get_index_by_index(
	                pyesedb_indexes->table_object,
	                (int) item_index );

	return( index_object );
}

/* The indexes iter() function
 */
PyObject *pyesedb_indexes_iter(
           pyesedb_indexes_t *pyesedb_indexes )
{
	static char *function = "pyesedb_indexes_iter";

	if( pyesedb_indexes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pyesedb_indexes );

	return( (PyObject *) pyesedb_indexes );
}

/* The indexes iternext() function
 */
PyObject *pyesedb_indexes_iternext(
           pyesedb_indexes_t *pyesedb_indexes )
{
	PyObject *index_object = NULL;
	static char *function  = "pyesedb_indexes_iternext";

	if( pyesedb_indexes == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes.",
		 function );

		return( NULL );
	}
	if( pyesedb_indexes->get_index_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes - missing get index by index function.",
		 function );

		return( NULL );
	}
	if( pyesedb_indexes->index_entry < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes - invalid index entry.",
		 function );

		return( NULL );
	}
	if( pyesedb_indexes->number_of_indexes < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid indexes - invalid number of indexes.",
		 function );

		return( NULL );
	}
	if( pyesedb_indexes->index_entry >= pyesedb_indexes->number_of_indexes )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	index_object = pyesedb_indexes->get_index_by_index(
	                pyesedb_indexes->table_object,
	                pyesedb_indexes->index_entry );

	if( index_object != NULL )
	{
		pyesedb_indexes->index_entry++;
	}
	return( index_object );
}

