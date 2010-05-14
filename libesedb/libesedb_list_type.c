/*
 * List type functions
 *
 * Copyright (c) 2010, Joachim Metz <jbmetz@users.sourceforge.net>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_list_type.h"

/* Creates a list element
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_element_initialize(
     libesedb_list_element_t **list_element,
     liberror_error_t **error )
{
	static char *function = "libesedb_list_element_initialize";

	if( list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( *list_element == NULL )
	{
		*list_element = (libesedb_list_element_t *) memory_allocate(
		                                             sizeof( libesedb_list_element_t ) );

		if( *list_element == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create list element.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *list_element,
		     0,
		     sizeof( libesedb_list_element_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear list element.",
			 function );

			memory_free(
			 *list_element );

			*list_element = NULL;

			return( -1 );
		}
	}
	return( 1 );
}

/* Frees a list element
 * Uses the value_free_function to free the element value
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_element_free(
     libesedb_list_element_t **list_element,
     int (*value_free_function)(
            intptr_t *value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	static char *function = "libesedb_list_element_free";
	int result            = 1;

	if( list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid list element.",
		 function );

		return( -1 );
	}
	if( *list_element != NULL )
	{
		if( ( ( *list_element )->previous != NULL )
		 || ( ( *list_element )->next != NULL ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
			 "%s: list element part of a list.",
			 function );

			return( -1 );
		}
		if( ( value_free_function != NULL )
		 && ( value_free_function(
		       ( *list_element )->value,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value in element.",
			 function );

			result = -1;
		}
		memory_free(
		 *list_element );

		*list_element = NULL;
	}
	return( result );
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
		*list = (libesedb_list_t *) memory_allocate(
		                             sizeof( libesedb_list_t ) );

		if( *list == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create list.",
			 function );

			return( -1 );
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

			memory_free(
			 *list );

			*list = NULL;

			return( -1 );
		}
	}
	return( 1 );
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
	int number_of_elements                = 0;
	int iterator                          = 0;
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

		for( iterator = 0;
		     iterator < number_of_elements;
		     iterator++ )
		{
			list_element = list->first;

			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			list->first = list_element->next;

			if( list->last == list_element )
			{
				list->last = list_element->next;
			}
			list->number_of_elements -= 1;

			if( list_element->next != NULL )
			{
				list_element->next->previous = NULL;
			}
			list_element->next = NULL;

			if( libesedb_list_element_free(
			     &( list_element ),
			     value_free_function,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free element: %d.",
				 function,
				 iterator + 1 );

				result = -1;
			}
		}
	}
	return( result );
}

/* Clones the existing list and its elements
 * This function can return a partially cloned list on error
 * Returns 1 if successful or -1 on error
 */
int libesedb_list_clone(
     libesedb_list_t **destination,
     libesedb_list_t *source,
     int (*value_clone_function)(
            intptr_t **destination,
            intptr_t *source,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_list_element_t *source_list_element = NULL;
	intptr_t *destination_value                  = NULL;
	static char *function                        = "libesedb_list_clone";
	int iterator                                 = 0;

	if( destination == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid destination list.",
		 function );

		return( -1 );
	}
	if( *destination != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid destination list already set.",
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
	if( source == NULL )
	{
		*destination = NULL;
	}
	else
	{
		*destination = (libesedb_list_t *) memory_allocate(
		                                    sizeof( libesedb_list_t ) );

		if( *destination == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create list.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     *destination,
		     0,
		     sizeof( libesedb_list_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear list.",
			 function );

			memory_free(
			 *destination );

			*destination = NULL;

			return( -1 );
		}
		source_list_element = source->first;

		for( iterator = 0;
		     iterator < source->number_of_elements;
		     iterator++ )
		{
			if( source_list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in source list element: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			destination_value = NULL;

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
				 iterator + 1 );

				return( -1 );
			}
			if( libesedb_list_append_value(
			     *destination,
			     destination_value,
			     error ) != 1 )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append value of list element: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			source_list_element = source_list_element->next;
		}
	}
	return( 1 );
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
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libesedb_list_get_element(
     libesedb_list_t *list,
     int element_index,
     libesedb_list_element_t **element,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_get_element";
	int iterator                          = 0;

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
		 LIBERROR_ARGUMENT_ERROR_VALUE_OUT_OF_RANGE,
		 "%s: invalid element index out of range.",
		 function );

		return( -1 );
	}
	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( element_index < ( list->number_of_elements / 2 ) )
	{
		list_element = list->first;

		for( iterator = 0;
		     iterator < element_index;
		     iterator++ )
		{
			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			list_element = list_element->next;
		}
	}
	else
	{
		list_element = list->last;

		for( iterator = ( list->number_of_elements - 1 );
		     iterator > element_index;
		     iterator-- )
		{
			if( list_element == NULL )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
				 "%s: corruption detected in element: %d.",
				 function,
				 iterator + 1 );

				return( -1 );
			}
			list_element = list_element->previous;
		}
	}
	*element = list_element;

	if( list_element == NULL )
	{
		return( 0 );
	}
	return( 1 );
}

/* Retrieves a specific value from the list
 * Returns 1 if successful, 0 if not available or -1 on error
 */
int libesedb_list_get_value(
     libesedb_list_t *list,
     int element_index,
     intptr_t **value,
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_get_value";
	int result                            = 0;

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
	result = libesedb_list_get_element(
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
		 "%s: unable to retrieve list element for index: %d.",
		 function,
		 element_index );

		return( -1 );
	}
	else if( result != 0 )
	{
		*value = list_element->value;
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
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( list->first != NULL )
	{
		list->first->previous = element;
		element->next         = list->first;
	}
	if( list->last == NULL )
	{
		list->last = element;
	}
	list->first               = element;
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
	libesedb_list_element_t *element = NULL;
	static char *function            = "libesedb_list_prepend_value";

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
	element = (libesedb_list_element_t *) memory_allocate(
	                                       sizeof( libesedb_list_element_t ) );

	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create element.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     element,
	     0,
	     sizeof( libesedb_list_element_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear element.",
		 function );

		memory_free(
		 element );

		return( -1 );
	}
	element->value = value;

	if( libesedb_list_prepend_element(
	     list,
	     element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to prepend element to list.",
		 function );

		memory_free(
		 element );

		return( -1 );
	}
	return( 1 );
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
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( list->first == NULL )
	{
		list->first = element;
	}
	if( list->last != NULL )
	{
		list->last->next  = element;
		element->previous = list->last;
	}
	list->last                = element;
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
	libesedb_list_element_t *element = NULL;
	static char *function            = "libesedb_list_append_value";

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
	element = (libesedb_list_element_t *) memory_allocate(
	                                       sizeof( libesedb_list_element_t ) );

	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create element.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     element,
	     0,
	     sizeof( libesedb_list_element_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear element.",
		 function );

		memory_free(
		 element );

		return( -1 );
	}
	element->value = value;

	if( libesedb_list_append_element(
	     list,
	     element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append element to list.",
		 function );

		memory_free(
		 element );

		return( -1 );
	}
	return( 1 );
}

/* Inserts a list element into the list
 * Uses the value_compare_function to determine the order of the list elements
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libesedb_list_insert_element(
     libesedb_list_t *list,
     libesedb_list_element_t *element,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_list_element_t *list_element = NULL;
	static char *function                 = "libesedb_list_insert_element";
	int result                            = 0;
	int iterator                          = 0;

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
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( ( element->previous != NULL )
	 || ( element->next != NULL ) )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: element already part of a list.",
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
	if( list->number_of_elements == 0 )
	{
		if( list->first != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - first already set.",
			 function );

			return( -1 );
		}
		if( list->last != NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - last already set.",
			 function );

			return( -1 );
		}
		list->first = element;
		list->last  = element;
	}
	else
	{
		if( list->first == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing first.",
			 function );

			return( -1 );
		}
		if( list->last == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: corruption detected - missing last.",
			 function );

			return( -1 );
		}
		list_element = list->first;

		for( iterator = 0;
		     iterator < list->number_of_elements;
		     iterator++ )
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
				 iterator + 1 );

				return( -1 );
			}
			else if( result == LIBESEDB_LIST_COMPARE_EQUAL )
			{
				return( 0 );
			}
			else if( result == LIBESEDB_LIST_COMPARE_LESS )
			{
				element->previous = list_element->previous;
				element->next     = list_element;

				if( list_element == list->first )
				{
					list->first = element;
				}
				else if( list_element->previous == NULL )
				{
					liberror_error_set(
					 error,
					 LIBERROR_ERROR_DOMAIN_RUNTIME,
					 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
					 "%s: corruption detected - missing previous in list element: %d.",
					 function,
					 iterator + 1 );

					return( -1 );
				}
				else
				{
					list_element->previous->next = element;
				}
				list_element->previous = element;

				break;
			}
			list_element = list_element->next;
		}
		if( result >= 1 )
		{
			element->previous = list->last;
			list->last->next  = element;
			list->last        = element;
		}
	}
	list->number_of_elements += 1;

	return( 1 );
}

/* Inserts a value to the list
 * Creates a new list element
 * Uses the value_compare_function to determine the order of the list elements
 * Returns 1 if successful, 0 if the list element already exists or -1 on error
 */
int libesedb_list_insert_value(
     libesedb_list_t *list,
     intptr_t *value,
     int (*value_compare_function)(
            intptr_t *first_value,
            intptr_t *second_value,
            liberror_error_t **error ),
     liberror_error_t **error )
{
	libesedb_list_element_t *element = NULL;
	static char *function            = "libesedb_list_insert_value";
	int result                       = 0;

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
	element = (libesedb_list_element_t *) memory_allocate(
	                                       sizeof( libesedb_list_element_t ) );

	if( element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create element.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     element,
	     0,
	     sizeof( libesedb_list_element_t ) ) == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_MEMORY,
		 LIBERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear element.",
		 function );

		memory_free(
		 element );

		return( -1 );
	}
	element->value = value;

	result = libesedb_list_insert_element(
	          list,
	          element,
	          value_compare_function,
	          error );

	if( result != 1 )
	{
		memory_free(
		 element );
	}
	if( result == -1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to insert element to list.",
		 function );

		return( -1 );
	}
	return( result );
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
		 "%s: invalid element.",
		 function );

		return( -1 );
	}
	if( element == list->first )
	{
		list->first = element->next;
	}
	if( element == list->last )
	{
		list->last = element->previous;
	}
	if( element->next != NULL )
	{
		element->next->previous = element->previous;
	}
	if( element->previous != NULL )
	{
		element->previous->next = element->next;
	}
	element->next             = NULL;
	element->previous         = NULL;
	list->number_of_elements -= 1;

	return( 1 );
}

