/*
 * List type functions
 *
 * Copyright (C) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _LIBESEDB_LIST_TYPE_H )
#define _LIBESEDB_LIST_TYPE_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#if defined( __cplusplus )
extern "C" {
#endif

enum LIBESEDB_LIST_COMPARE_DEFINITIONS
{
	LIBESEDB_LIST_COMPARE_LESS,
	LIBESEDB_LIST_COMPARE_EQUAL,
	LIBESEDB_LIST_COMPARE_GREATER
};

typedef struct libesedb_list_element libesedb_list_element_t;

struct libesedb_list_element
{
	/* The previous element
	 */
	libesedb_list_element_t *previous;

	/* The next element
	 */
	libesedb_list_element_t *next;

	/* The list element value
	 */
	intptr_t *value;
};

typedef struct libesedb_list libesedb_list_t;

struct libesedb_list
{
	/* The amount of elements
	 */
	int amount_of_elements;

	/* The first list element
	 */
	libesedb_list_element_t *first;

	/* The last list element
	 */
	libesedb_list_element_t *last;
};

int libesedb_list_element_initialize(
     libesedb_list_element_t **list_element,
     liberror_error_t **error );

int libesedb_list_element_free(
     libesedb_list_element_t **list_element,
     int (*value_free_function)( intptr_t *value, liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_list_initialize(
     libesedb_list_t **list,
     liberror_error_t **error );

int libesedb_list_free(
     libesedb_list_t **list,
     int (*value_free_function)( intptr_t *value, liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_list_empty(
     libesedb_list_t *list,
     int (*value_free_function)( intptr_t *value, liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_list_clone(
     libesedb_list_t **destination,
     libesedb_list_t *source,
     int (*value_clone_function)( intptr_t **destination, intptr_t *source, liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_list_get_amount_of_elements(
     libesedb_list_t *list,
     int *amount_of_elements,
     liberror_error_t **error );

int libesedb_list_get_element(
     libesedb_list_t *list,
     int element_index,
     libesedb_list_element_t **element,
     liberror_error_t **error );

int libesedb_list_get_value(
     libesedb_list_t *list,
     int element_index,
     intptr_t **value,
     liberror_error_t **error );

int libesedb_list_prepend_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     liberror_error_t **error );

int libesedb_list_prepend_value(
     libesedb_list_t *list,
     intptr_t *value,
     liberror_error_t **error );

int libesedb_list_append_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     liberror_error_t **error );

int libesedb_list_append_value(
     libesedb_list_t *list,
     intptr_t *value,
     liberror_error_t **error );

int libesedb_list_insert_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     int (*value_compare_function)( intptr_t *first, intptr_t *second, liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_list_insert_value(
     libesedb_list_t *list,
     intptr_t *value,
     int (*value_compare_function)( intptr_t *first, intptr_t *second, liberror_error_t **error ),
     liberror_error_t **error );

int libesedb_list_remove_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

