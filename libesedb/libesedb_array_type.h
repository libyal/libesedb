/* 
 * Array type functions
 *
 * Copyright (c) 2009-2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBESEDB_ARRAY_TYPE_H )
#define _LIBESEDB_ARRAY_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* The array comparison definitions
 */
enum LIBESEDB_ARRAY_COMPARE_DEFINITIONS
{
	/* The first value is less than the second value
	 */
        LIBESEDB_ARRAY_COMPARE_LESS,

	/* The first and second values are equal
	 */
        LIBESEDB_ARRAY_COMPARE_EQUAL,

	/* The first value is greater than the second value
	 */
        LIBESEDB_ARRAY_COMPARE_GREATER
};

/* The array insert flag definitions
 */
enum LIBESEDB_ARRAY_INSERT_FLAGS
{
	/* Allow duplicate entries
	 */
	LIBESEDB_ARRAY_INSERT_FLAG_NON_UNIQUE_ENTRIES	= 0x00,

	/* Only allow unique entries, no duplicates
	 */
	LIBESEDB_ARRAY_INSERT_FLAG_UNIQUE_ENTRIES	= 0x01,
};

typedef struct libesedb_array libesedb_array_t;

struct libesedb_array
{
	/* The number of allocated entries
	 */
	int number_of_allocated_entries;

	/* The number of entries
	 */
	int number_of_entries;

	/* The entries
	 */
	intptr_t **entries;
};

int libesedb_array_initialize(
     libesedb_array_t **array,
     int number_of_entries,
     liberror_error_t **error );

int libesedb_array_free(
     libesedb_array_t **array,
     int (*entry_free_function)(
            intptr_t *entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_array_empty(
     libesedb_array_t *array,
     int (*entry_free_function)(
            intptr_t *entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_array_clear(
     libesedb_array_t *array,
     int (*entry_free_function)(
            intptr_t *entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_array_clone(
     libesedb_array_t **destination_array,
     libesedb_array_t *source_array,
     int (*entry_free_function)(
            intptr_t *entry,
            liberror_error_t **error ),
     int (*entry_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_array_resize(
     libesedb_array_t *array,
     int number_of_entries,
     int (*entry_free_function)(
            intptr_t *entry,
            liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_array_get_number_of_entries(
     libesedb_array_t *array,
     int *number_of_entries,
     liberror_error_t **error );

int libesedb_array_get_entry_by_index(
     libesedb_array_t *array,
     int entry_index,
     intptr_t **entry,
     liberror_error_t **error );

int libesedb_array_set_entry_by_index(
     libesedb_array_t *array,
     int entry_index,
     intptr_t *entry,
     liberror_error_t **error );

int libesedb_array_append_entry(
     libesedb_array_t *array,
     int *entry_index,
     intptr_t *entry,
     liberror_error_t **error );

int libesedb_array_insert_entry(
     libesedb_array_t *array,
     int *entry_index,
     intptr_t *entry,
     int (*entry_compare_function)(
            intptr_t *first_entry,
            intptr_t *second_entry,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

