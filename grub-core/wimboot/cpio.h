#ifndef _CPIO_H
#define _CPIO_H

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
 * CPIO archives
 *
 */

#include <stdint.h>
#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>

/** A CPIO archive header
 *
 * All field are hexadecimal ASCII numbers padded with '0' on the
 * left to the full width of the field.
 */
struct newc_head
{
  char magic[6];
  char ino[8];
  char mode[8];
  char uid[8];
  char gid[8];
  char nlink[8];
  char mtime[8];
  char filesize[8];
  char devmajor[8];
  char devminor[8];
  char rdevmajor[8];
  char rdevminor[8];
  char namesize[8];
  char check[8];
} GRUB_PACKED;

struct dir
{
  char *name;
  struct dir *next;
  struct dir *child;
};

static char
hex (grub_uint8_t val)
{
  if (val < 10)
    return '0' + val;
  return 'a' + val - 10;
}

static inline void
set_field (char *var, grub_uint32_t val)
{
  int i;
  char *ptr = var;
  for (i = 28; i >= 0; i -= 4)
    *ptr++ = hex((val >> i) & 0xf);
}

static inline grub_uint8_t *
make_header (grub_uint8_t *ptr,
	     const char *name, grub_size_t len,
	     grub_uint32_t mode,
	     grub_off_t fsize)
{
  struct newc_head *head = (struct newc_head *) ptr;
  grub_uint8_t *optr;
  grub_size_t oh = 0;
  grub_memcpy (head->magic, "070701", 6);
  set_field (head->ino, 0);
  set_field (head->mode, mode);
  set_field (head->uid, 0);
  set_field (head->gid, 0);
  set_field (head->nlink, 1);
  set_field (head->mtime, 0);
  set_field (head->filesize, fsize);
  set_field (head->devmajor, 0);
  set_field (head->devminor, 0);
  set_field (head->rdevmajor, 0);
  set_field (head->rdevminor, 0);
  set_field (head->namesize, len);
  set_field (head->check, 0);
  optr = ptr;
  ptr += sizeof (struct newc_head);
  grub_memcpy (ptr, name, len);
  ptr += len;
  oh = ALIGN_UP_OVERHEAD (ptr - optr, 4);
  grub_memset (ptr, 0, oh);
  ptr += oh;
  return ptr;
}

static inline void
free_dir (struct dir *root)
{
  if (!root)
    return;
  free_dir (root->next);
  free_dir (root->child);
  grub_free (root->name);
  grub_free (root);
}

static inline grub_size_t
insert_dir (const char *name, struct dir **root,
	    grub_uint8_t *ptr)
{
  struct dir *cur, **head = root;
  const char *cb, *ce = name;
  grub_size_t size = 0;
  while (1)
    {
      for (cb = ce; *cb == '/'; cb++);
      for (ce = cb; *ce && *ce != '/'; ce++);
      if (!*ce)
	break;

      for (cur = *root; cur; cur = cur->next)
	if (grub_memcmp (cur->name, cb, ce - cb)
	    && cur->name[ce - cb] == 0)
	  break;
      if (!cur)
	{
	  struct dir *n;
	  n = grub_zalloc (sizeof (*n));
	  if (!n)
	    return 0;
	  n->next = *head;
	  n->name = grub_strndup (cb, ce - cb);
	  if (ptr)
	    {
	      ptr = make_header (ptr, name, ce - name,
				 040777, 0);
	    }
	  size += ALIGN_UP ((ce - (char *) name)
			    + sizeof (struct newc_head), 4);
	  *head = n;
	  cur = n;
	}
      root = &cur->next;
    }
  return size;
}

/** CPIO magic */
#define CPIO_MAGIC "070701"

/** CPIO trailer */
#define CPIO_TRAILER "TRAILER!!!"

extern int cpio_extract ( void *data, size_t len,
			  int ( * file ) ( const char *name, void *data,
					   size_t len ) );

#endif /* _CPIO_H */
