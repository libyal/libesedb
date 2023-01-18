/*
 * Internationalization (i18n) functions
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

#if !defined( _LIBESEDB_I18N_H )
#define _LIBESEDB_I18N_H

#include <common.h>
#include <types.h>

#if defined( HAVE_LIBINTL_H ) && defined( ENABLE_NLS )
#include <libintl.h>
#endif

#include "libesedb_libcerror.h"

/* TODO
#include <gettext.h>

#define	_( string ) \
	dgettext( "libesedb", string )
*/
#define	_( string ) \
	string

#if defined( __cplusplus )
extern "C" {
#endif

int libesedb_i18n_initialize(
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBESEDB_I18N_H ) */

