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

#if !defined( _PYESEDB_FILE_H )
#define _PYESEDB_FILE_H

#include <common.h>
#include <types.h>

#include "pyesedb_libbfio.h"
#include "pyesedb_libesedb.h"
#include "pyesedb_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pyesedb_file pyesedb_file_t;

struct pyesedb_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libesedb file
	 */
	libesedb_file_t *file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pyesedb_file_object_methods[];
extern PyTypeObject pyesedb_file_type_object;

PyObject *pyesedb_file_new(
           void );

PyObject *pyesedb_file_new_open(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_file_new_open_file_object(
           PyObject *self,
           PyObject *arguments,
           PyObject *keywords );

int pyesedb_file_init(
     pyesedb_file_t *pyesedb_file );

void pyesedb_file_free(
      pyesedb_file_t *pyesedb_file );

PyObject *pyesedb_file_signal_abort(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_open(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_file_open_file_object(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_file_close(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_get_type(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_get_format_version(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_get_creation_format_version(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_get_page_size(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_get_number_of_tables(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_get_table_by_index(
           PyObject *pyesedb_file,
           int table_index );

PyObject *pyesedb_file_get_table(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pyesedb_file_get_tables(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments );

PyObject *pyesedb_file_get_table_by_name(
           pyesedb_file_t *pyesedb_file,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYESEDB_FILE_H ) */

