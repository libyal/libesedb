/*
 * Page tree values functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#if !defined( _LIBESEDB_PAGE_TREE_VALUES_H )
#define _LIBESEDB_PAGE_TREE_VALUES_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_array_type.h"
#include "libesedb_data_definition.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libesedb_page_tree_values libesedb_page_tree_values_t;

struct libesedb_page_tree_values
{
	/* The common key data
	 */
	uint8_t *common_key;

	/* The common key data size
	 */
	size_t common_key_size;

	/* The key data
	 */
	uint8_t *key;

	/* The key data size
	 */
	size_t key_size;

	/* TODO remove */

	/* The number of value definitions
	 */
	uint32_t number_of_value_definitions;

	/* The value definition array
	 */
	libesedb_array_t *value_definition_array;
};

int libesedb_page_tree_values_initialize(
     libesedb_page_tree_values_t **page_tree_values,
     liberror_error_t **error );

int libesedb_page_tree_values_free(
     intptr_t *page_tree_values,
     liberror_error_t **error );

int libesedb_page_tree_values_set_common_key(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *common_key,
     size_t common_key_size,
     liberror_error_t **error );

int libesedb_page_tree_values_set_key_common(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *common_key,
     size_t common_key_size,
     liberror_error_t **error );

int libesedb_page_tree_values_set_key_local(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *local_key,
     size_t local_key_size,
     liberror_error_t **error );

int libesedb_page_tree_values_get_value_definition(
     libesedb_page_tree_values_t *page_tree_values,
     int value_definition_index,
     libesedb_data_definition_t **data_definition,
     liberror_error_t **error );

int libesedb_page_tree_values_get_value_definition_by_key(
     libesedb_page_tree_values_t *page_tree_values,
     uint8_t *key,
     size_t key_size,
     libesedb_data_definition_t **data_definition,
     uint8_t flags,
     liberror_error_t **error );

int libesedb_page_tree_values_append_value_definition(
     libesedb_page_tree_values_t *page_tree_values,
     libesedb_data_definition_t *value_data_definition,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

