/*
 * Support functions
 *
 * Copyright (c) 2008-2012, Joachim Metz <joachim.metz@gmail.com>
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

#include "libcpath_definitions.h"
#include "libcpath_libcerror.h"
#include "libcpath_libcstring.h"
#include "libcpath_support.h"

#if !defined( HAVE_LOCAL_LIBCPATH )

/* Returns the library version as a string
 */
const char *libcpath_get_version(
             void )
{
	return( (const char *) LIBCPATH_VERSION_STRING );
}

#endif /* !defined( HAVE_LOCAL_LIBCPATH ) */

