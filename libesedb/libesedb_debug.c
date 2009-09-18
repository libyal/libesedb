/*
 * Debug functions
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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
#include <libnotify.h>

#include "libesedb_debug.h"
#include "libesedb_definitions.h"
#include "libesedb_libbfio.h"
#include "libesedb_string.h"

#if defined( HAVE_DEBUG_OUTPUT )

/* Prints the database state
 */
void libesedb_debug_print_database_state(
      uint32_t database_state )
{
	switch( database_state )
	{
		case 1:
			libnotify_verbose_printf(
			 "Just created (JET_dbstateJustCreated)" );
			break;

		case 2:
			libnotify_verbose_printf(
			 "Dirty Shutdown (JET_dbstateDirtyShutdown)" );
			break;

		case 3:
			libnotify_verbose_printf(
			 "Clean Shutdown (JET_dbstateCleanShutdown)" );
			break;

		case 4:
			libnotify_verbose_printf(
			 "Being converted (JET_dbstateBeingConverted)" );
			break;

		case 5:
			libnotify_verbose_printf(
			 "Force Detach (JET_dbstateForceDetach)" );
			break;

		default:
			libnotify_verbose_printf(
			 "Unknown (%" PRIu32 ")",
			 database_state );
			break;
	}
}

/* Prints the page flags
 */
void libesedb_debug_print_page_flags(
      uint32_t page_flags )
{
	libnotify_verbose_printf(
	 "0x%08" PRIx32 "\n",
	 page_flags );

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_ROOT ) == LIBESEDB_PAGE_FLAG_IS_ROOT )
	{
		libnotify_verbose_printf(
		 "\tIs root\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LEAF ) == LIBESEDB_PAGE_FLAG_IS_LEAF )
	{
		libnotify_verbose_printf(
		 "\tIs leaf\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_PARENT ) == LIBESEDB_PAGE_FLAG_IS_PARENT )
	{
		libnotify_verbose_printf(
		 "\tIs parent\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_EMPTY ) == LIBESEDB_PAGE_FLAG_IS_EMPTY )
	{
		libnotify_verbose_printf(
		 "\tIs empty\n" );
	}

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_SPACE_TREE ) == LIBESEDB_PAGE_FLAG_IS_SPACE_TREE )
	{
		libnotify_verbose_printf(
		 "\tIs space tree\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_INDEX ) == LIBESEDB_PAGE_FLAG_IS_INDEX )
	{
		libnotify_verbose_printf(
		 "\tIs index\n" );
	}
	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_LONG_VALUE ) == LIBESEDB_PAGE_FLAG_IS_LONG_VALUE )
	{
		libnotify_verbose_printf(
		 "\tIs long value\n" );
	}

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_PRIMARY ) == LIBESEDB_PAGE_FLAG_IS_PRIMARY )
	{
		libnotify_verbose_printf(
		 "\tIs primary\n" );
	}

	if( ( page_flags & LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT ) == LIBESEDB_PAGE_FLAG_IS_NEW_RECORD_FORMAT )
	{
		libnotify_verbose_printf(
		 "\tIs new record format\n" );
	}
}

/* Prints the column type
 */
void libesedb_debug_print_column_type(
      uint32_t column_type )
{
	switch( column_type )
	{
		case 0:
			libnotify_verbose_printf(
			 "(JET_coltypNil)" );
			break;

		case 1:
			libnotify_verbose_printf(
			 "(JET_coltypBit)" );
			break;

		case 2:
			libnotify_verbose_printf(
			 "(JET_coltypUnsignedByte)" );
			break;

		case 3:
			libnotify_verbose_printf(
			 "(JET_coltypShort)" );
			break;

		case 4:
			libnotify_verbose_printf(
			 "(JET_coltypLong)" );
			break;

		case 5:
			libnotify_verbose_printf(
			 "(JET_coltypCurrency)" );
			break;

		case 6:
			libnotify_verbose_printf(
			 "(JET_coltypIEEESingle)" );
			break;

		case 7:
			libnotify_verbose_printf(
			 "(JET_coltypIEEEDouble)" );
			break;

		case 8:
			libnotify_verbose_printf(
			 "(JET_coltypDateTime)" );
			break;

		case 9:
			libnotify_verbose_printf(
			 "(JET_coltypBinary)" );
			break;

		case 10:
			libnotify_verbose_printf(
			 "(JET_coltypText)" );
			break;

		case 11:
			libnotify_verbose_printf(
			 "(JET_coltypLongBinary)" );
			break;

		case 12:
			libnotify_verbose_printf(
			 "(JET_coltypLongText)" );
			break;

		case 13:
			libnotify_verbose_printf(
			 "(JET_coltypSLV)" );
			break;

		case 14:
			libnotify_verbose_printf(
			 "(JET_coltypUnsignedLong)" );
			break;

		case 15:
			libnotify_verbose_printf(
			 "(JET_coltypLongLong)" );
			break;

		case 16:
			libnotify_verbose_printf(
			 "(JET_coltypGUID)" );
			break;

		case 17:
			libnotify_verbose_printf(
			 "(JET_coltypUnsignedShort)" );
			break;

		default:
			libnotify_verbose_printf(
			 "(Unknown)" );
			break;
	}
}

/* Prints the column type
 */
void libesedb_debug_print_page_value_definition_type(
      uint16_t page_value_definition_type )
{
	switch( page_value_definition_type )
	{
		case 1:
			libnotify_verbose_printf(
			 "(Table)" );
			break;

		case 2:
			libnotify_verbose_printf(
			 "(Column)" );
			break;

		case 3:
			libnotify_verbose_printf(
			 "(Index)" );
			break;

		case 4:
			libnotify_verbose_printf(
			 "(Long Value)" );
			break;

		default:
			libnotify_verbose_printf(
			 "(Unknown)" );
			break;
	}
}

/* Prints the table group of bits
 */
void libesedb_debug_print_table_group_of_bits(
      uint32_t table_group_of_bits )
{
	libnotify_verbose_printf(
	 "0x%08" PRIx32 "\n",
	 table_group_of_bits );

	if( ( table_group_of_bits & 0x00000001 ) == 0x00000001 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitTableCreateFixedDDL)\n" );
	}
	if( ( table_group_of_bits & 0x00000002 ) == 0x00000002 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitTableCreateTemplateTable)\n" );
	}
	if( ( table_group_of_bits & 0x00000004 ) == 0x00000004 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitTableCreateNoFixedVarColumnsInDerivedTables)\n" );
	}
}

/* Prints the column group of bits
 */
void libesedb_debug_print_column_group_of_bits(
      uint32_t column_group_of_bits )
{
	libnotify_verbose_printf(
	 "0x%08" PRIx32 "\n",
	 column_group_of_bits );

	if( ( column_group_of_bits & 0x00000001 ) == 0x00000001 )
	{
		libnotify_verbose_printf(
		 "\tIs fixed size (JET_bitColumnFixed)\n" );
	}
	if( ( column_group_of_bits & 0x00000002 ) == 0x00000002 )
	{
		libnotify_verbose_printf(
		 "\tIs tagged (JET_bitColumnTagged)\n" );
	}
	if( ( column_group_of_bits & 0x00000004 ) == 0x00000004 )
	{
		libnotify_verbose_printf(
		 "\tNot empty (JET_bitColumnNotNULL)\n" );
	}
	if( ( column_group_of_bits & 0x00000008 ) == 0x00000008 )
	{
		libnotify_verbose_printf(
		 "\tIs version column (JET_bitColumnVersion)\n" );
	}
	if( ( column_group_of_bits & 0x00000010 ) == 0x00000010 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnAutoincrement)\n" );
	}
	if( ( column_group_of_bits & 0x00000020 ) == 0x00000020 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnUpdatable)\n" );
	}
	if( ( column_group_of_bits & 0x00000040 ) == 0x00000040 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnTTKey)\n" );
	}
	if( ( column_group_of_bits & 0x00000080 ) == 0x00000080 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnTTDescending)\n" );
	}

	if( ( column_group_of_bits & 0x00000400 ) == 0x00000400 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnMultiValued)\n" );
	}
	if( ( column_group_of_bits & 0x00000800 ) == 0x00000800 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnEscrowUpdate)\n" );
	}
	if( ( column_group_of_bits & 0x00001000 ) == 0x00001000 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnUnversioned)\n" );
	}
	if( ( column_group_of_bits & 0x00002000 ) == 0x00002000 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnDeleteOnZero or JET_bitColumnMaybeNull)\n" );
	}
	if( ( column_group_of_bits & 0x00004000 ) == 0x00004000 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnFinalize)\n" );
	}
	if( ( column_group_of_bits & 0x00008000 ) == 0x00008000 )
	{
		libnotify_verbose_printf(
		 "\t(JET_bitColumnUserDefinedDefault)\n" );
	}
}

/* Prints a log structure 
 * Returns 1 if successful or -1 on error
 */
int libesedb_debug_print_log_time(
     uint8_t *log_time,
     size_t log_time_size,
     const char *description,
     const char *indentation,
     liberror_error_t **error )
{
	static char *function = "libesedb_debug_print_log_time";

	if( log_time == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid log time.",
		 function );

		return( -1 );
	}
	if( log_time_size < 8 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: log time too small.",
		 function );

		return( -1 );
	}
	if( log_time_size > (size_t) SSIZE_MAX )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid log time size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( description == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid description.",
		 function );

		return( -1 );
	}
	if( indentation == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid indentation.",
		 function );

		return( -1 );
	}
	libnotify_verbose_printf(
	 "%s: %s%s: %04d-%02d-%02d %02d:%02d:%02d (0x%02x 0x%02x)\n",
	 function,
	 description,
	 indentation,
	 1900 + log_time[ 5 ],
	 log_time[ 4 ],
	 log_time[ 3 ],
	 log_time[ 2 ],
	 log_time[ 1 ],
	 log_time[ 0 ],
	 log_time[ 6 ],
	 log_time[ 7 ] );

	return( 1 );
}

/* Prints the read offsets
 * Returns 1 if successful or -1 on error
 */
int libesedb_debug_print_read_offsets(
     libbfio_handle_t *file_io_handle,
     liberror_error_t **error )
{
	static char *function = "libesedb_debug_print_read_offsets";
	off64_t offset        = 0;
	size64_t size         = 0;
	int amount_of_offsets = 0;
	int offset_iterator   = 0;

	if( file_io_handle == NULL )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file io handle.",
		 function );

		return( -1 );
	}
	if( libbfio_handle_get_amount_of_offsets_read(
	     file_io_handle,
	     &amount_of_offsets,
	     error ) != 1 )
	{
		liberror_error_set(
		 error,
		 LIBERROR_ERROR_DOMAIN_RUNTIME,
		 LIBERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve amount of offsets read.",
		 function );

		return( -1 );
	}
	libnotify_verbose_printf(
	 "Offsets read:\n" );

	for( offset_iterator = 0;
	     offset_iterator < amount_of_offsets;
	     offset_iterator++ )
	{
		if( libbfio_handle_get_offset_read(
		     file_io_handle,
		     offset_iterator,
		     &offset,
		     &size,
		     error ) != 1 )
		{
			liberror_error_set(
			 error,
			 LIBERROR_ERROR_DOMAIN_RUNTIME,
			 LIBERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve offset: %d.",
			 function,
			 offset_iterator );

			return( -1 );
		}
		libnotify_verbose_printf(
		 "%08" PRIu64 " ( 0x%08" PRIx64 " ) - %08" PRIu64 " ( 0x%08" PRIx64 " ) size: %" PRIu64 "\n",
		 offset,
		 offset,
		 offset + size,
		 offset + size,
		 size );
	}
	libnotify_verbose_printf(
	 "\n" );

	return( 1 );
}

#endif

