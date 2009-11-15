/*
 * Memory functions
 *
 * Copyright (c) 2006-2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations.
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

#if !defined( _MEMORY_H )
#define _MEMORY_H

#include "common.h"

#if defined( MEMWATCH )
#include "memwatch.h"
#endif

#if defined( HAVE_GLIB_H )
#include <glib.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_STRING_H ) || defined( WINAPI )
#include <string.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* Memory allocation
 */
#if defined( HAVE_GLIB_H )
#define memory_allocate( size ) \
	g_malloc( (gsize) size )

#elif defined( HAVE_MALLOC ) || defined( WINAPI )
#define memory_allocate( size ) \
	malloc( size )
#endif

/* Memory reallocation
 */
#if defined( HAVE_GLIB_H )
#define memory_reallocate( buffer, size ) \
	g_realloc( (gpointer) buffer, (gsize) size )

#elif defined( HAVE_REALLOC ) || defined( WINAPI )
#define memory_reallocate( buffer, size ) \
	realloc( (void *) buffer, size )
#endif

/* Memory free
 */
#if defined( HAVE_GLIB_H )
#define memory_free( buffer ) \
	g_free( (gpointer) buffer )

#elif defined( HAVE_FREE ) || defined( WINAPI )
#define memory_free( buffer ) \
	free( (void *) buffer )
#endif

/* Memory compare
 */
#if defined( HAVE_MEMCMP ) || defined( WINAPI )
#define memory_compare( buffer1, buffer2, size ) \
	memcmp( (const void *) buffer1, (const void *) buffer2, size )
#endif

/* Memory copy
 */
#if defined( HAVE_MEMCPY ) || defined( WINAPI )
#define memory_copy( destination, source, count ) \
	memcpy( (void *) destination, (void *) source, count )
#endif

/* Memory set
 */
#if defined( HAVE_MEMSET ) || defined( WINAPI )
#define memory_set( buffer, value, count ) \
	memset( (void *) buffer, (int) value, count )
#endif

#if defined( __cplusplus )
}
#endif

#endif

