/*
 * List type functions
 *
 * Copyright (c) 2009-2011, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <liberror.h>

#include "libesedb_list_type.h"

/* Creates a list element
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_element_initialize(
     libesedb_list_element_t **element,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_element_initialize";

	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( *element == NULL )
	{
		*element = memory_allocate_structure(
		            libesedb_list_element_t );

		if( *element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create list element.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *element,
		     0,
		     sizeof( libesedb_list_element_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear list element.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *element != NULL )
	{
		memory_free(
		 *element );

		*element = NULL;
	}
	return( -1 );
}

/* Frees a list element
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_element_free(
     libesedb_list_element_t **element,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	static char *function = "libesedb_list_element_free";
	int result            = 1;

	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( *element != NULL )
	{
		if( ( ( *element )->previous_element != NULL )
		 || ( ( *element )->next_element != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: list element part of a list.",
			 function );

			return( -1 );
		}
		if( value_free_function != NULL )
		{
			if( value_free_function(
			     ( *element )->value,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *element );

		*element = NULL;
	}
	return( result );
}

/* Retrieves the value from the list element
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_element_get_value(
     libesedb_list_element_t *element,
     intptr_t **value,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_element_get_value";

	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	*value = element->value;

	return( 1 );
}

/* Sets the value in the list element
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_element_set_value(
     libesedb_list_element_t *element,
     intptr_t *value,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_element_set_value";

	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	element->value = value;

	return( 1 );
}

/* Creates a list
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_initialize(
     libesedb_list_t **list,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_initialize";

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( *list == NULL )
	{
		*list = memory_allocate_structure(
		         libesedb_list_t );

		if( *list == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create list.",
			 function );

			goto on_error;
		}
		if( memory_set(
		     *list,
		     0,
		     sizeof( libesedb_list_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear list.",
			 function );

			goto on_error;
		}
	}
	return( 1 );

on_error:
	if( *list != NULL )
	{
		memory_free(
		 *list );

		*list = NULL;
	}
	return( -1 );
}

/* Frees a list including the elements
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_free(
     libesedb_list_t **list,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	static char *function = "libesedb_list_free";
	int result            = 0;

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( *list != NULL )
	{
		result = libesedb_list_empty(
		          *list,
		          value_free_function,
		          error );

		if( result != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to empty list.",
			 function );
		}
		memory_free(
		 *list );

		*list = NULL;
	}
	return( result );
}

/* Empties a list and frees the elements
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_empty(
     libesedb_list_t *list,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_empty";
	int element_index                     = 0;
	int number_of_elements                = 0;
	int result                            = 1;

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( list->number_of_elements > 0 )
	{
		number_of_elements = list->number_of_elements;

		for( element_index = 0;
		     element_index < number_of_elements;
		     element_index++ )
		{
			list_element = list->first_element;

			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			list->first_element = list_element->next_element;

			if( list->last_element == list_element )
			{
				list->last_element = list_element->next_element;
			}
			list->number_of_elements -= 1;

			if( list_element->next_element != NULL )
			{
				list_element->next_element->previous_element = NULL;
			}
			list_element->next_element = NULL;

			if( libesedb_list_element_free(
			     &list_element,
			     value_free_function,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free element: %d.",
				 function,
				 element_index );

				result = -1;
			}
		}
	}
	return( result );
}

/* Clones the list and its elements
 *
 * The values are cloned using the value_clone_function
 * On error the values are freed using the value_free_function
 *
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_clone(
     libesedb_list_t **destination_list,
     libesedb_list_t *source_list,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_list_element_t *source_list_element = NULL;
	intptr_t *destination_value                  = NULL;
	static char *function                        = "libesedb_list_clone";
	int element_index                            = 0;

	if( destination_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination list.",
		 function );

		return( -1 );
	}
	if( *destination_list != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination list already set.",
		 function );

		return( -1 );
	}
	if( value_free_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value free function.",
		 function );

		return( -1 );
	}
	if( value_clone_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value clone function.",
		 function );

		return( -1 );
	}
	if( source_list == NULL )
	{
		*destination_list = NULL;

		return( 1 );
	}
	if( libesedb_list_initialize(
	     destination_list,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create destination list.",
		 function );

		goto on_error;
	}
	if( *destination_list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing destination list.",
		 function );

		goto on_error;
	}
	source_list_element = source_list->first_element;

	for( element_index = 0;
	     element_index < source_list->number_of_elements;
	     element_index++ )
	{
		if( source_list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected in source list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( value_clone_function(
		     &destination_value,
		     source_list_element->value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to clone value of list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		if( libesedb_list_append_value(
		     *destination_list,
		     destination_value,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append value of list element: %d.",
			 function,
			 element_index );

			goto on_error;
		}
		destination_value = NULL;

		source_list_element = source_list_element->next_element;
	}
	return( 1 );

on_error:
	if( destination_value != NULL )
	{
		value_free_function(
		 destination_value,
		 NULL );
	}
	if( *destination_list != NULL )
	{
		libesedb_list_free(
		 destination_list,
		 value_free_function,
		 error );
	}
	return( -1 );
}

/* Retrieves the number of elements in the list
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_get_number_of_elements(
     libesedb_list_t *list,
     int *number_of_elements,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_get_number_of_elements";

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( number_of_elements == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of elements.",
		 function );

		return( -1 );
	}
	*number_of_elements = list->number_of_elements;

	return( 1 );
}

/* Retrieves a specific element from the list
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_get_element_by_index(
     libesedb_list_t *list,
     int element_index,
     libesedb_list_element_t **element,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_get_element_by_index";
	int element_iterator                  = 0;

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( ( element_index < 0 )
	 || ( element_index >= list->number_of_elements ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element index value out of bounds.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( element_index < ( list->number_of_elements / 2 ) )
	{
		list_element = list->first_element;

		for( element_iterator = 0;
		     element_iterator < element_index;
		     element_iterator++ )
		{
			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 element_iterator );

				return( -1 );
			}
			list_element = list_element->next_element;
		}
	}
	else
	{
		list_element = list->last_element;

		for( element_iterator = ( list->number_of_elements - 1 );
		     element_iterator > element_index;
		     element_iterator-- )
		{
			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 element_iterator );

				return( -1 );
			}
			list_element = list_element->previous_element;
		}
	}
	if( list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: corruption detected - missing list element.",
		 function );

		return( -1 );
	}
	*element = list_element;

	return( 1 );
}

/* Retrieves a specific value from the list
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_get_value_by_index(
     libesedb_list_t *list,
     int element_index,
     intptr_t **value,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_get_value_by_index";
	int result                            = 0;

	result = libesedb_list_get_element_by_index(
	          list,
	          element_index,
	          &list_element,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve list element: %d from list.",
		 function,
		 element_index );

		return( -1 );
	}
	if( libesedb_list_element_get_value(
	     list_element,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value of list element: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	return( 1 );
}

/* Prepend an element to the list
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_prepend_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_prepend_element";

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( list->first_element != NULL )
	{
		list->first_element->previous_element = element;
		element->next_element                 = list->first_element;
	}
	if( list->last_element == NULL )
	{
		list->last_element = element;
	}
	list->first_element       = element;
	list->number_of_elements += 1;

	return( 1 );
}

/* Prepend a value to the list
 * Creates a new list element
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_prepend_value(
     libesedb_list_t *list,
     intptr_t *value,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_prepend_value";

	if( libesedb_list_element_initialize(
	     &list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libesedb_list_prepend_element(
	     list,
	     list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to prepend element to list.",
		 function );

		goto on_error;
	}
	if( libesedb_list_element_set_value(
	     list_element,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set value of list element.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( list_element != NULL )
	{
		libesedb_list_element_free(
		 &list_element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Append an element to the list
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_append_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_append_element";

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( list->first_element == NULL )
	{
		list->first_element = element;
	}
	if( list->last_element != NULL )
	{
		list->last_element->next_element = element;
		element->previous_element        = list->last_element;
	}
	list->last_element        = element;
	list->number_of_elements += 1;

	return( 1 );
}

/* Append a value to the list
 * Creates a new list element
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_append_value(
     libesedb_list_t *list,
     intptr_t *value,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_append_value";

	if( libesedb_list_element_initialize(
	     &list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		goto on_error;
	}
	if( libesedb_list_append_element(
	     list,
	     list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to list.",
		 function );

		goto on_error;
	}
	if( libesedb_list_element_set_value(
	     list_element,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set value of list element.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( list_element != NULL )
	{
		libesedb_list_element_free(
		 &list_element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Inserts a list element into the list
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBESEDB_LIST_COMPARE_LESS,
 * LIBESEDB_LIST_COMPARE_EQUAL, LIBESEDB_LIST_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBESEDB_LIST_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libesedb_list_insert_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_insert_element";
	int element_index                     = 0;
	int result                            = -1;

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( ( element->previous_element != NULL )
	 || ( element->next_element != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: list element already part of a list.",
		 function );

		return( -1 );
	}
	if( value_compare_function == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value compare function.",
		 function );

		return( -1 );
	}
	if( ( insert_flags & ~( LIBESEDB_LIST_INSERT_FLAG_UNIQUE_ENTRIES ) ) != 0 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported insert flags: 0x%02" PRIx8 ".",
		 function,
		 insert_flags );

		return( -1 );
	}
	if( list->number_of_elements == 0 )
	{
		if( list->first_element != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first element already set.",
			 function );

			return( -1 );
		}
		if( list->last_element != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last element already set.",
			 function );

			return( -1 );
		}
		list->first_element = element;
		list->last_element  = element;
	}
	else
	{
		if( list->first_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first.",
			 function );

			return( -1 );
		}
		if( list->last_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last.",
			 function );

			return( -1 );
		}
		list_element = list->first_element;

		for( element_index = 0;
		     element_index < list->number_of_elements;
		     element_index++ )
		{
			result = value_compare_function(
			          element->value,
			          list_element->value,
			          error );

			if( result == -1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to compare list element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			else if( result == LIBESEDB_LIST_COMPARE_EQUAL )
			{
				if( ( insert_flags & LIBESEDB_LIST_INSERT_FLAG_UNIQUE_ENTRIES ) != 0 )
				{
					return( 0 );
				}
			}
			else if( result == LIBESEDB_LIST_COMPARE_LESS )
			{
				break;
			}
			else if( result != LIBESEDB_LIST_COMPARE_GREATER )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
				 LIBERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
				 "%s: unsupported value compare function return value: %d.",
				 function,
				 result );

				return( -1 );
			}
			list_element = list_element->next_element;
		}
		if( result == LIBESEDB_LIST_COMPARE_LESS )
		{
			element->previous_element = list_element->previous_element;
			element->next_element     = list_element;

			if( list_element == list->first_element )
			{
				list->first_element = element;
			}
			else if( list_element->previous_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected - missing previous in list element: %d.",
				 function,
				 element_index );

				return( -1 );
			}
			else
			{
				list_element->previous_element->next_element = element;
			}
			list_element->previous_element = element;
		}
		else
		{
			element->previous_element        = list->last_element;
			list->last_element->next_element = element;
			list->last_element               = element;
		}
	}
	list->number_of_elements += 1;

	return( 1 );
}

/* Inserts a value to the list
 *
 * Creates a new list element
 *
 * Uses the value_compare_function to determine the order of the entries
 * The value_compare_function should return LIBESEDB_LIST_COMPARE_LESS,
 * LIBESEDB_LIST_COMPARE_EQUAL, LIBESEDB_LIST_COMPARE_GREATER if successful or -1 on error
 *
 * Duplicate entries are allowed by default and inserted after the last duplicate value.
 * Only allowing unique entries can be enforced by setting the flag LIBESEDB_LIST_INSERT_FLAG_UNIQUE_ENTRIES
 *
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libesedb_list_insert_value(
     libesedb_list_t *list,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     uint8_t insert_flags,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_insert_value";
	int result                            = 0;

	if( libesedb_list_element_initialize(
	     &list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create list element.",
		 function );

		return( -1 );
	}
	if( libesedb_list_element_set_value(
	     list_element,
	     value,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to set value of list element.",
		 function );

		goto on_error;
	}
	result = libesedb_list_insert_element(
	          list,
	          list_element,
	          value_compare_function,
	          insert_flags,
	          error );

	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert element to list.",
		 function );

		goto on_error;
	}
	else if( result == 0 )
	{
		if( libesedb_list_element_free(
		     &list_element,
		     NULL,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( list_element != NULL )
	{
		libesedb_list_element_free(
		 &list_element,
		 NULL,
		 NULL );
	}
	return( -1 );
}

/* Removes an element from the list
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_remove_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_remove_element";

	if( list == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( element == list->first_element )
	{
		list->first_element = element->next_element;
	}
	if( element == list->last_element )
	{
		list->last_element = element->previous_element;
	}
	if( element->next_element != NULL )
	{
		element->next_element->previous_element = element->previous_element;
	}
	if( element->previous_element != NULL )
	{
		element->previous_element->next_element = element->next_element;
	}
	element->next_element     = NULL;
	element->previous_element = NULL;
	list->number_of_elements -= 1;

	return( 1 );
}

