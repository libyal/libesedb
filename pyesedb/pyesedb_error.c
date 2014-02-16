/*
 * Integer functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#include <stdarg.h>
#elif defined( HAVE_VARARGS_H )
#include <varargs.h>
#else
#error Missing headers stdarg.h and varargs.h
#endif

#include "pyesedb_error.h"
#include "pyesedb_libcerror.h"
#include "pyesedb_libcstring.h"
#include "pyesedb_python.h"

#if defined( HAVE_STDARG_H ) || defined( WINAPI )
#define VARARGS( function, error, exception_object, type, argument ) \
	function( error, exception_object, type argument, ... )
#define VASTART( argument_list, type, name ) \
	va_start( argument_list, name )
#define VAEND( argument_list ) \
	va_end( argument_list )

#elif defined( HAVE_VARARGS_H )
#define VARARGS( function, error, exception_object, type, argument ) \
	function( error, exception_object, va_alist ) va_dcl
#define VASTART( argument_list, type, name ) \
	{ type name; va_start( argument_list ); name = va_arg( argument_list, type )
#define VAEND( argument_list ) \
	va_end( argument_list ); }

#endif

/* Raises an error
 */
void VARARGS(
      pyesedb_error_raise,
      libcerror_error_t *error,
      PyObject *exception_object,
      const char *,
      format_string )
{
	va_list argument_list;

	char error_string[ PYESEDB_ERROR_STRING_SIZE ];

	static char *function       = "pyesedb_error_raise";
	size_t error_string_index   = 0;
	size_t format_string_length = 0;

       	if( format_string == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing format string.",
		 function );

		return;
	}
	if( error != NULL )
	{
		format_string_length = libcstring_narrow_string_length(
		                        format_string );

		if( format_string_length < ( PYESEDB_ERROR_STRING_SIZE - 2 ) )
		{
			if( memory_copy(
			     error_string,
			     format_string,
			     format_string_length ) != NULL )
			{
				error_string_index = format_string_length;

				error_string[ error_string_index++ ] = (char) ' ';

				if( libcerror_error_backtrace_sprint(
				     error,
				     &( error_string[ error_string_index ] ),
				     PYESEDB_ERROR_STRING_SIZE - error_string_index ) != -1 )
				{
					while( error_string_index < PYESEDB_ERROR_STRING_SIZE )
					{
						if( error_string[ error_string_index ] == 0 )
						{
							break;
						}
						if( error_string[ error_string_index ] == '\n' )
						{
							error_string[ error_string_index ] = ' ';
						}
						error_string_index++;
					}
					if( error_string_index >= PYESEDB_ERROR_STRING_SIZE )
					{
						error_string[ PYESEDB_ERROR_STRING_SIZE - 1 ] = 0;
					}
					VASTART(
					 argument_list,
					 const char *,
					 format_string );

					PyErr_Format(
					 exception_object,
					 error_string,
					 function,
					 argument_list );

					VAEND(
					 argument_list );

					return;
				}
			}
		}
	}
	VASTART(
	 argument_list,
	 const char *,
	 format_string );

	PyErr_Format(
	 exception_object,
	 format_string,
	 function );

	VAEND(
	 argument_list );

	return;
}

#undef VARARGS
#undef VASTART
#undef VAEND

