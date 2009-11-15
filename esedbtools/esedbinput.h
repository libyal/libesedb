/*
 * Common input functions for the esedbtools
 *
 * Copyright (c) 2008-2009, Joachim Metz <forensics@hoffmannbv.nl>,
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

#if !defined( _ESEDBINPUT_H )
#define _ESEDBINPUT_H

#include <common.h>
#include <types.h>

#include <liberror.h>

#include <libsystem.h>

#if defined( __cplusplus )
extern "C" {
#endif

int esedbinput_determine_ascii_codepage(
     const libsystem_character_t *argument,
     int *ascii_codepage,
     liberror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif

