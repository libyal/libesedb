/*
 * Memory allocation functions for testing
 *
 * Copyright (C) 2009-2026, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _ESEDB_TEST_MEMORY_H )
#define _ESEDB_TEST_MEMORY_H

#include <common.h>

#if defined( __cplusplus )
extern "C" {
#endif

#if defined( HAVE_MEMORY_TESTS )

/* Memory tests only support x86 or x86-64 architectures */
#if defined( __x86__ ) || defined( __x86_64__ )

/* Memory tests cannot be used in combination with DLLs or CygWin */
#if !defined( LIBESEDB_DLL_IMPORT ) && !defined( __CYGWIN__ )

/* Memory tests require dlsym */
#if defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ )

#define HAVE_ESEDB_TEST_MEMORY	1

#endif /* defined( HAVE_GNU_DL_DLSYM ) && defined( __GNUC__ ) */
#endif /* !defined( LIBESEDB_DLL_IMPORT ) && !defined( __CYGWIN__ ) */
#endif /* defined( __x86__ ) || defined( __x86_64__ ) */
#endif /* defined( HAVE_MEMORY_TESTS ) */

#if defined( HAVE_ESEDB_TEST_MEMORY )

extern int esedb_test_malloc_attempts_before_fail;

extern int esedb_test_memcpy_attempts_before_fail;

extern int esedb_test_memset_attempts_before_fail;

extern int esedb_test_realloc_attempts_before_fail;

#endif /* defined( HAVE_ESEDB_TEST_MEMORY ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _ESEDB_TEST_MEMORY_H ) */

