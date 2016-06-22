/*
 * Library for system independent implementation of functionality
 * for programs
 *
 * Copyright (C) 2009-2016, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _ESEDBTOOLS_LIBCSYSTEM_H )
#define _ESEDBTOOLS_LIBCSYSTEM_H

/* Define HAVE_LOCAL_LIBCSYSTEM for local use of libcsystem
 */
#if defined( HAVE_LOCAL_LIBCSYSTEM )

#include <libcsystem_definitions.h>
#include <libcsystem_file_io.h>
#include <libcsystem_getopt.h>
#include <libcsystem_glob.h>
#include <libcsystem_i18n.h>
#include <libcsystem_signal.h>
#include <libcsystem_string.h>
#include <libcsystem_support.h>
#include <libcsystem_unused.h>

#else

/* If libtool DLL support is enabled set LIBCSYSTEM_DLL_IMPORT
 * before including libcsystem.h
 */
#if defined( _WIN32 ) && defined( DLL_IMPORT ) && !defined( HAVE_STATIC_EXECUTABLES )
#define LIBCSYSTEM_DLL_IMPORT
#endif

#include <libcsystem.h>

#endif /* defined( HAVE_LOCAL_LIBCSYSTEM ) */

#endif /* !defined( _ESEDBTOOLS_LIBCSYSTEM_H ) */

