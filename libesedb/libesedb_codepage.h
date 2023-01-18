/*
 * Codepage functions
 *
 * Copyright (C) 2009-2023, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _LIBESEDB_INTERNAL_CODEPAGE_H )
#define _LIBESEDB_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBESEDB for local use of libesedb
 * The definitions in <libesedb/codepage.h> are copied here
 * for local use of libesedb
 */
#if !defined( HAVE_LOCAL_LIBESEDB )

#include <libesedb/codepage.h>

#else

/* The codepage definitions
 */
enum LIBESEDB_CODEPAGES
{
	LIBESEDB_CODEPAGE_ASCII		= 20127,

	LIBESEDB_CODEPAGE_ISO_8859_1	= 28591,
	LIBESEDB_CODEPAGE_ISO_8859_2	= 28592,
	LIBESEDB_CODEPAGE_ISO_8859_3	= 28593,
	LIBESEDB_CODEPAGE_ISO_8859_4	= 28594,
	LIBESEDB_CODEPAGE_ISO_8859_5	= 28595,
	LIBESEDB_CODEPAGE_ISO_8859_6	= 28596,
	LIBESEDB_CODEPAGE_ISO_8859_7	= 28597,
	LIBESEDB_CODEPAGE_ISO_8859_8	= 28598,
	LIBESEDB_CODEPAGE_ISO_8859_9	= 28599,
	LIBESEDB_CODEPAGE_ISO_8859_10	= 28600,
	LIBESEDB_CODEPAGE_ISO_8859_11	= 28601,
	LIBESEDB_CODEPAGE_ISO_8859_13	= 28603,
	LIBESEDB_CODEPAGE_ISO_8859_14	= 28604,
	LIBESEDB_CODEPAGE_ISO_8859_15	= 28605,
	LIBESEDB_CODEPAGE_ISO_8859_16	= 28606,

	LIBESEDB_CODEPAGE_KOI8_R	= 20866,
	LIBESEDB_CODEPAGE_KOI8_U	= 21866,

	LIBESEDB_CODEPAGE_WINDOWS_874	= 874,
	LIBESEDB_CODEPAGE_WINDOWS_932	= 932,
	LIBESEDB_CODEPAGE_WINDOWS_936	= 936,
	LIBESEDB_CODEPAGE_WINDOWS_949	= 949,
	LIBESEDB_CODEPAGE_WINDOWS_950	= 950,
	LIBESEDB_CODEPAGE_WINDOWS_1250	= 1250,
	LIBESEDB_CODEPAGE_WINDOWS_1251	= 1251,
	LIBESEDB_CODEPAGE_WINDOWS_1252	= 1252,
	LIBESEDB_CODEPAGE_WINDOWS_1253	= 1253,
	LIBESEDB_CODEPAGE_WINDOWS_1254	= 1254,
	LIBESEDB_CODEPAGE_WINDOWS_1255	= 1255,
	LIBESEDB_CODEPAGE_WINDOWS_1256	= 1256,
	LIBESEDB_CODEPAGE_WINDOWS_1257	= 1257,
	LIBESEDB_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBESEDB ) */

#if defined( HAVE_DEBUG_OUTPUT )

typedef struct libesedb_codepage libesedb_codepage_t;

struct libesedb_codepage
{
	/* The codepage
	 */
	uint32_t codepage;

	/* The identifier
	 */
	const char *identifier;

	/* The description
	 */
	const char *description;
};
const char *libesedb_codepage_get_identifier(
             uint32_t codepage );

const char *libesedb_codepage_get_description(
             uint32_t codepage );

#endif /* defined( HAVE_DEBUG_OUTPUT ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_INTERNAL_CODEPAGE_H ) */

