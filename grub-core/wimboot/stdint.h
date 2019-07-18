#ifndef _STDINT_H
#define _STDINT_H

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
 * Standard integer types
 *
 */

#include <grub/types.h>

typedef grub_uint8_t uint8_t;
typedef grub_uint16_t uint16_t;
typedef grub_uint32_t uint32_t;
typedef grub_uint64_t uint64_t;

typedef grub_int8_t int8_t;
typedef grub_int16_t int16_t;
typedef grub_int32_t int32_t;
typedef grub_int64_t int64_t;

typedef grub_addr_t intptr_t;

typedef grub_size_t size_t;
typedef grub_ssize_t ssize_t;

typedef grub_uint16_t char16_t;

#endif /* _STDINT_H */
