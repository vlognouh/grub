#ifndef _STRING_H
#define _STRING_H

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
 * String operations
 *
 */
#include <grub/misc.h>
#include <stdint.h>

static inline void *
memcpy ( void *dest, const void *src, size_t len ) {
  return grub_memcpy (dest, src, len);
}

static inline void *
memset ( void *dest, int c, size_t len ) {
  return grub_memset (dest, c, len);
}

static inline int 
memcmp ( const void *src1, const void *src2, size_t len ) {
  return grub_memcmp (src1, src2, len);
};

static inline int 
strcmp ( const char *str1, const char *str2 ) {
  return grub_strcmp (str1, str2);
};

static inline grub_size_t 
strlen ( const char *str ) {
  return grub_strlen (str);
};

#endif /* _STRING_H */
