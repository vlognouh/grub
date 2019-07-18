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
 * String functions
 *
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "ctype.h"
#include "wctype.h"
#include "wchar.h"

/**
 * Compare two wide-character strings, case-insensitively
 *
 * @v str1        First string
 * @v str2        Second string
 * @ret diff        Difference
 */
int wwcscasecmp ( const char16_t *str1, const char16_t *str2 ) {
    int c1;
    int c2;

    do {
        c1 = towupper ( *(str1++) );
        c2 = towupper ( *(str2++) );
    } while ( ( c1 != 0 ) && ( c1 == c2 ) );

    return ( c1 - c2 );
}

/**
 * Get length of wide-character string
 *
 * @v str        String
 * @ret len        Length (in characters)
 */
size_t wwcslen ( const char16_t *str ) {
    size_t len = 0;

    while ( *(str++) )
        len++;
    return len;
}

/**
 * Find character in wide-character string
 *
 * @v str        String
 * @v c            Wide character
 * @ret first        First occurrence of wide character in string, or NULL
 */
char16_t * wcschr ( const char16_t *str, char16_t c ) {

    for ( ; *str ; str++ ) {
        if ( *str == c )
            return ( ( char16_t * )str );
    }
    return NULL;
}

size_t mbstowcs ( char16_t *dst, const char *src, size_t size ) {
    if ( !dst || !src )
        return 0;
    char *p;
    char16_t *q;
    size_t i;
    p = (char *) src;
    q = dst;
    for ( i=1; i<=size; i++ ) {
        *q++ = *p++;
        if ( *p == '\0' )
            break;
    }
    *q = 0;
    return i;
}
