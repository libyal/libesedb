/*
 * The page values definition of an Extensible Storage Engine (ESE) Database File (EDB)
 *
 * Copyright (c) 2009, Joachim Metz <forensics@hoffmannbv.nl>,
 * Hoffmann Investigations. All rights reserved.
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

#if !defined( _ESEDB_PAGE_VALUES_H )
#define _ESEDB_PAGE_VALUES_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct esedb_father_data_page_header esedb_father_data_page_header_t;

struct esedb_father_data_page_header
{
	/* The intial amount of pages
	 * Consists of 4 bytes
	 */
	uint8_t initial_amount_of_pages[ 4 ];

	/* The parent father page (FDP) number
	 * Consists of 4 bytes
	 */
	uint8_t parent_father_data_page_number[ 4 ];

	/* Extent space
	 * Consists of 4 bytes
	 */
	uint8_t extent_space[ 4 ];

	/* The space tree page number
	 * Consists of 4 bytes
	 */
	uint8_t space_tree_page_number[ 4 ];
};

typedef struct esedb_space_tree_page_entry esedb_space_tree_page_entry_t;

struct esedb_space_tree_page_entry
{
	/* Size of key entry
	 * Consists of 2 bytes
	 * Contains 4
	 */
	uint8_t key_size[ 2 ];

	/* The last page number
	 * Consists of 4 bytes
	 */
	uint8_t last_page_number[ 4 ];

	/* Amount of pages
	 * Consists of 4 bytes
	 */
	uint8_t amount_of_pages[ 4 ];
};

typedef struct esedb_definition_header esedb_definition_header_t;

struct esedb_definition_header
{
	/* The definition flags
	 * Consists of 2 bytes
	 */
	uint8_t flags[ 2 ];

	/* The definition size
	 * Consists of 2 bytes
	 * Includes the 4 bytes of the header (flags and size)
	 */
	uint8_t size[ 2 ];
};

typedef struct esedb_table_definition esedb_table_definition_t;

struct esedb_table_definition
{
	/* The father data page (FDP) object identifier
	 * Consists of 4 bytes
	 */
	uint8_t father_data_page_object_identifier[ 4 ];

	/* The father data page (FDP) number
	 * Consists of 4 bytes
	 */
	uint8_t father_data_page_number[ 4 ];

	/* The table density percentage
	 * Consists of 4 bytes
	 */
	uint8_t table_density_percentage[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The initial amount of pages
	 * Consists of 4 bytes
	 */
	uint8_t initial_amount_of_pages[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];
};

typedef struct esedb_column_definition esedb_column_definition_t;

struct esedb_column_definition
{
	uint8_t dummy;
};

typedef struct esedb_index_definition esedb_index_definition_t;

struct esedb_index_definition
{
	/* The father data page (FDP) object identifier
	 * Consists of 4 bytes
	 */
	uint8_t father_data_page_object_identifier[ 4 ];

	/* The father data page (FDP) number
	 * Consists of 4 bytes
	 */
	uint8_t father_data_page_number[ 4 ];

	/* The index density percentage
	 * Consists of 4 bytes
	 */
	uint8_t index_density_percentage[ 4 ];

	/* The index group of bits
	 * Consists of 4 bytes
	 */
	uint8_t group_of_bits[ 4 ];

	/* The locale identifier
	 * Consists of 4 bytes
	 */
	uint8_t locale_identifier[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown3;
};

typedef struct esedb_long_value_definition esedb_long_value_definition_t;

struct esedb_long_value_definition
{
	/* The father data page (FDP) object identifier
	 * Consists of 4 bytes
	 */
	uint8_t father_data_page_object_identifier[ 4 ];

	/* The father data page (FDP) number
	 * Consists of 4 bytes
	 */
	uint8_t father_data_page_number[ 4 ];

	/* The initial amount of pages
	 * Consists of 4 bytes
	 */
	uint8_t initial_amount_of_pages[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The extent space
	 * Consists of 4 bytes
	 */
	uint8_t extent_space[ 4 ];

	/* Unknown
	 * Consists of 1 byte
	 */
	uint8_t unknown2;
};

#if defined( __cplusplus )
}
#endif

#endif

