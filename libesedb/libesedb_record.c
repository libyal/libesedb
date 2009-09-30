/*
 * Record functions
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include <liberror.h>

#include "libesedb_data_definition.h"
#include "libesedb_definitions.h"
#include "libesedb_record.h"
#include "libesedb_table.h"
#include "libesedb_types.h"

/* Creates a record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_initialize(
     libesedb_record_t **record,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                      = "libesedb_record_initialize";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record == NULL )
	{
		internal_record = (libesedb_internal_record_t *) memory_allocate(
		                                                  sizeof( libesedb_internal_record_t ) );

		if( internal_record == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create record.",
			 function );

			return( -1 );
		}
		if( memory_set(
		     internal_record,
		     0,
		     sizeof( libesedb_internal_record_t ) ) == NULL )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_MEMORY,
			 LIBERROR_MEMORY_ERROR_SET_FAILED,
			 "%s: unable to clear record.",
			 function );

			memory_free(
			 internal_record );

			return( -1 );
		}
		if( libesedb_list_element_initialize(
		     &( internal_record->list_element ),
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create list element.",
			 function );

			memory_free(
			 internal_record );

			return( -1 );
		}
		*record = (libesedb_record_t *) internal_record;
	}
	return( 1 );
}

/* Frees record
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_free(
     libesedb_record_t **record,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_free";
	int result                                  = 1;

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	if( *record != NULL )
	{
		internal_record = (libesedb_internal_record_t *) *record;
		*record         = NULL;

		/* The internal_table and data_definition references
		 * are freed elsewhere
		 */
		if( libesedb_record_detach(
		     internal_record,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to detach internal record.",
			 function );

			return( -1 );
		}
		if( ( internal_record->list_element != NULL )
		 && ( libesedb_list_element_free(
		       &( internal_record->list_element ),
		       NULL,
		       error ) != 1 ) )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free list element.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_record );
	}
	return( result );
}

/* Frees an record
 * Return 1 if successful or -1 on error
 */
int libesedb_record_free_no_detach(
     intptr_t *internal_record,
     liberror_error_t **error )
{
	static char *function = "libesedb_record_free_no_detach";
	int result            = 1;

	if( internal_record != NULL )
	{
		if( ( (libesedb_internal_record_t *) internal_record )->internal_table != NULL )
		{
			if( ( ( (libesedb_internal_record_t *) internal_record )->list_element != NULL )
			 && ( libesedb_list_element_free(
			       &( ( (libesedb_internal_record_t *) internal_record )->list_element ),
			       NULL,
			       error ) != 1 ) )
			{
				liberror_error_set(
				 error,
				 LIBERROR_ERROR_DOMAIN_RUNTIME,
				 LIBERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free list element.",
				 function );

				result = -1;
			}
		}
		/* The internal_table and data_definition references
		 * are freed elsewhere
		 */
		memory_free(
		 internal_record );
	}
	return( result );
}

/* Attaches the record to the table
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_attach(
     libesedb_internal_record_t *internal_record,
     libesedb_internal_table_t *internal_table,
     libesedb_data_definition_t *data_definition,
     liberror_error_t **error )
{
	static char *function = "libesedb_record_attach";

	if( internal_record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal record.",
		 function );

		return( -1 );
	}
	if( internal_record->list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing list element.",
		 function );

		return( -1 );
	}
	if( internal_record->internal_table != NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid internal record - already attached to table.",
		 function );

		return( -1 );
	}
	/* Add record to table
	 */
	if( libesedb_list_append_element(
	     internal_table->record_reference_list,
	     internal_record->list_element,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append internal record to table.",
		 function );

		return( -1 );
	}
	internal_record->internal_table  = internal_table;
	internal_record->data_definition = data_definition;

	return( 1 );
}

/* Detaches the record from its table reference
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_detach(
     libesedb_internal_record_t *internal_record,
     liberror_error_t **error )
{
	static char *function = "libesedb_record_detach";

	if( internal_record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid internal record.",
		 function );

		return( -1 );
	}
	if( internal_record->list_element == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing list element.",
		 function );

		return( -1 );
	}
	if( internal_record->internal_table != NULL )
	{
		/* Remove record from table
		 */
		if( libesedb_list_remove_element(
		     internal_record->internal_table->record_reference_list,
		     internal_record->list_element,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_REMOVE_FAILED,
			 "%s: unable to remove internal record from table.",
			 function );

			return( -1 );
		}
		internal_record->internal_table  = NULL;
		internal_record->data_definition = NULL;
	}
	return( 1 );
}

/* Retrieves the record identifier or Father Data Page (FDP) object identifier
 * Returns 1 if successful or -1 on error
 */
int libesedb_record_get_identifier(
     libesedb_record_t *record,
     uint32_t *identifier,
     liberror_error_t **error )
{
	libesedb_internal_record_t *internal_record = NULL;
	static char *function                       = "libesedb_record_get_identifier";

	if( record == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid record.",
		 function );

		return( -1 );
	}
	internal_record = (libesedb_internal_record_t *) record;

	if( internal_record->data_definition == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid internal record - missing data definition.",
		 function );

		return( -1 );
	}
	if( identifier == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid identifier.",
		 function );

		return( -1 );
	}
	*identifier = internal_record->data_definition->identifier;

	return( 1 );
}

