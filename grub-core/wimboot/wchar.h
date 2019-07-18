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

typedef void mbstate_t;

/**
 * Convert wide character to multibyte sequence
 *
 * @v buf        Buffer
 * @v wc        Wide character
 * @v ps        Shift state
 * @ret len        Number of characters written
 *
 * This is a stub implementation, sufficient to handle basic ASCII
 * characters.
 */
static inline size_t wcrtomb ( char *buf, char16_t wc,
                   mbstate_t *ps __attribute__ (( unused )) ) {
    *buf = wc;
    return 1;
}

static inline char16_t* c8_to_c16 ( const char *str)
{
    int len = (strlen (str) + 1) * sizeof (char16_t);
    char16_t *ret = NULL;
    char16_t *p16;
    char *p8 = (char *) str;
    ret = p16 = malloc (len);
    if (!ret)
        return NULL;
    while (*p8)
        *(p16++) = *(p8++);
    *p16 = 0;
    return ret;
}    

static inline size_t mbstowcs ( char16_t *dst, const char *src,
                   size_t size ) {
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

extern int wwcscasecmp ( const char16_t *str1, const char16_t *str2 );
extern size_t wwcslen ( const char16_t *str );
extern char16_t * wcschr ( const char16_t *str, char16_t c );

#endif /* _WCHAR_H */
