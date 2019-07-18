#ifndef _WCHAR_H
#define _WCHAR_H

/*
 * Copyright (C) 2012 Michael Brown <mbrown@fensystems.co.uk>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/**
 * @file
 *
 * Wide characters
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

extern int wwcscasecmp ( const char16_t *str1, const char16_t *str2 );
extern size_t wwcslen ( const char16_t *str );
extern char16_t * wcschr ( const char16_t *str, char16_t c );
extern size_t mbstowcs ( char16_t *dst, const char *src, size_t size );

#endif /* _WCHAR_H */
