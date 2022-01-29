/*
 * Codepage functions
 *
 * Copyright (C) 2009-2022, Joachim Metz <joachim.metz@gmail.com>
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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libesedb_codepage.h"

#if defined( HAVE_DEBUG_OUTPUT )

libesedb_codepage_t libesedb_codepages[ ] = {
	{ 874,		"windows-874",		"Thai" },
	{ 932,		"iso-2022-jp",		"Japanese (Shift-JIS)" },
	{ 936,		"gb2312",		"Chinese Simplified" },
	{ 949,		"ks_c_5601-1987",	"Korean" },
	{ 950,		"big5",			"Chinese Traditional" },
	{ 1200,		"unicode",		"16-bit Unicode Transformation Format (UTF-16) or ASCII" },
	{ 1250,		"windows-1250",		"Central European" },
	{ 1251,		"windows-1251",		"Cyrillic" },
	{ 1252,		"windows-1252",		"Western European" },
	{ 1253,		"windows-1253",		"Greek" },
	{ 1254,		"windows-1254",		"Turkish" },
	{ 1255,		"windows-1255",		"Hebrew" },
	{ 1256,		"windows-1256",		"Arabic" },
	{ 1257,		"windows-1257",		"Baltic" },
	{ 1258,		"windows-1258",		"Vietnamese" },
	{ 20127,	"us-ascii",		"7-bit American Standard Code for Information Interchange (ASCII)" },
	{ 20866,	"koi8-r",		"Cyrillic" },
	{ 21866,	"koi8-u",		"Cyrillic" },
	{ 28591,	"iso-8859-1",		"Western European" },
	{ 28592,	"iso-8859-2",		"Central European" },
	{ 28593,	"iso-8859-3",		"Latin 3 " },
	{ 28594,	"iso-8859-4",		"Baltic" },
	{ 28595,	"iso-8859-5",		"Cyrillic" },
	{ 28596,	"iso-8859-6",		"Arabic" },
	{ 28597,	"iso-8859-7",		"Greek" },
	{ 28598,	"iso-8859-8-i",		"Hebrew" },
	{ 28599,	"iso-8859-7",		"Turkish" },
	{ 28605,	"iso-8859-15",		"Latin 9 " },
	{ 50220,	"iso-2022-jp",		"Japanese (JIS)" },
	{ 50221,	"csISO2022JP",		"Japanese (JIS-Allow 1 byte Kana)" },
	{ 51932,	"euc-jp",		"Japanese" },
	{ 51949,	"euc-kr",		"Korean" },
	{ 52936,	"hz-gb-2312",		"Chinese Simplified" },
	{ 65000,	"utf-7",		"7-bit Unicode Transformation Format (UTF-7)" },
	{ 65001,	"utf-8",		"8-bit Unicode Transformation Format (UTF-8)" },

	{ (uint32_t) -1, "_UNKNOWN_", "Unknown" } };

/* Retrieves a string containing the codepage identifier
 */
const char *libesedb_codepage_get_identifier(
             uint32_t codepage )
{
	int iterator = 0;

	while( ( libesedb_codepages[ iterator ] ).codepage != (uint32_t) -1 )
	{
		if( ( libesedb_codepages[ iterator ] ).codepage == codepage )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libesedb_codepages[ iterator ] ).identifier );
}

/* Retrieves a string containing the codepage description
 */
const char *libesedb_codepage_get_description(
             uint32_t codepage )
{
	int iterator = 0;

	while( ( libesedb_codepages[ iterator ] ).codepage != (uint32_t) -1 )
	{
		if( ( libesedb_codepages[ iterator ] ).codepage == codepage )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libesedb_codepages[ iterator ] ).description );
}

#endif

