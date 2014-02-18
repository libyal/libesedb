/*
 * The python header wrapper
 *
 * Copyright (c) 2009-2014, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYESEDB_PYTHON_H )
#define _PYESEDB_PYTHON_H

#include <common.h>

/* Fix defines in pyconfig.h
 */
#undef _POSIX_C_SOURCE
#undef _XOPEN_SOURCE

/* Fix defines in pyport.h
 */
#undef HAVE_FSTAT
#undef HAVE_STAT
#undef HAVE_SSIZE_T
#undef HAVE_INT32_T
#undef HAVE_UINT32_T
#undef HAVE_INT64_T
#undef HAVE_UINT64_T

#include <Python.h>
#include <structmember.h>

#endif

