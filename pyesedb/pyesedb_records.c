/*
 * Python object definition of the sequence and iterator object of records
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

#include "pyesedb_libcerror.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"
#include "pyesedb_record.h"
#include "pyesedb_records.h"

PySequenceMethods pyesedb_records_sequence_methods = {
	/* sq_length */
	(lenfunc) pyesedb_records_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pyesedb_records_getitem,
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

PyTypeObject pyesedb_records_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pyesedb.records",
	/* tp_basicsize */
	sizeof( pyesedb_records_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pyesedb_records_free,
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
	&pyesedb_records_sequence_methods,
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
	"pyesedb sequence and iterator object of records",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pyesedb_records_iter,
	/* tp_iternext */
	(iternextfunc) pyesedb_records_iternext,
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
	(initproc) pyesedb_records_init,
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

/* Creates a new records sequence and iterator object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pyesedb_records_new(
           PyObject *parent_object,
           PyObject* (*get_item_by_index)(
                        PyObject *parent_object,
                        int index ),
           int number_of_items )
{
	pyesedb_records_t *sequence_object = NULL;
	static char *function              = "pyesedb_records_new";

	if( parent_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid parent object.",
		 function );

		return( NULL );
	}
	if( get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get item by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the records values are initialized
	 */
	sequence_object = PyObject_New(
	                   struct pyesedb_records,
	                   &pyesedb_records_type_object );

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		goto on_error;
	}
	sequence_object->parent_object     = parent_object;
	sequence_object->get_item_by_index = get_item_by_index;
	sequence_object->current_index     = 0;
	sequence_object->number_of_items   = number_of_items;

	Py_IncRef(
	 (PyObject *) sequence_object->parent_object );

	return( (PyObject *) sequence_object );

on_error:
	if( sequence_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) sequence_object );
	}
	return( NULL );
}

/* Initializes a records sequence and iterator object
 * Returns 0 if successful or -1 on error
 */
int pyesedb_records_init(
     pyesedb_records_t *sequence_object )
{
	static char *function = "pyesedb_records_init";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( -1 );
	}
	/* Make sure the records values are initialized
	 */
	sequence_object->parent_object     = NULL;
	sequence_object->get_item_by_index = NULL;
	sequence_object->current_index     = 0;
	sequence_object->number_of_items   = 0;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of records not supported.",
	 function );

	return( -1 );
}

/* Frees a records sequence object
 */
void pyesedb_records_free(
      pyesedb_records_t *sequence_object )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pyesedb_records_free";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           sequence_object );

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
	if( sequence_object->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) sequence_object->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) sequence_object );
}

/* The records len() function
 */
Py_ssize_t pyesedb_records_len(
            pyesedb_records_t *sequence_object )
{
	static char *function = "pyesedb_records_len";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) sequence_object->number_of_items );
}

/* The records getitem() function
 */
PyObject *pyesedb_records_getitem(
           pyesedb_records_t *sequence_object,
           Py_ssize_t item_index )
{
	PyObject *record_object = NULL;
	static char *function   = "pyesedb_records_getitem";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( NULL );
	}
	if( sequence_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( sequence_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) sequence_object->number_of_items ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	record_object = sequence_object->get_item_by_index(
	                 sequence_object->parent_object,
	                 (int) item_index );

	return( record_object );
}

/* The records iter() function
 */
PyObject *pyesedb_records_iter(
           pyesedb_records_t *sequence_object )
{
	static char *function = "pyesedb_records_iter";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) sequence_object );

	return( (PyObject *) sequence_object );
}

/* The records iternext() function
 */
PyObject *pyesedb_records_iternext(
           pyesedb_records_t *sequence_object )
{
	PyObject *record_object = NULL;
	static char *function   = "pyesedb_records_iternext";

	if( sequence_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object.",
		 function );

		return( NULL );
	}
	if( sequence_object->get_item_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object - missing get item by index function.",
		 function );

		return( NULL );
	}
	if( sequence_object->current_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object - invalid current index.",
		 function );

		return( NULL );
	}
	if( sequence_object->number_of_items < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid sequence object - invalid number of items.",
		 function );

		return( NULL );
	}
	if( sequence_object->current_index >= sequence_object->number_of_items )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	record_object = sequence_object->get_item_by_index(
	                 sequence_object->parent_object,
	                 sequence_object->current_index );

	if( record_object != NULL )
	{
		sequence_object->current_index++;
	}
	return( record_object );
}

