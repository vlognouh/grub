/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2019  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/file.h>
#include <grub/err.h>
#include <grub/device.h>
#include <grub/disk.h>
#include <grub/misc.h>
#include <grub/types.h>
#include <grub/term.h>
#include <grub/dl.h>
#include <grub/env.h>
#include <grub/extcmd.h>
#include <grub/mm.h>
#include <grub/efi/efi.h>
#include <grub/efi/api.h>
#include "wimboot.h"
#include "cpio.h"
#include "efifile.h"
#include "efiblock.h"
#include "efiboot.h"
#include "stdio.h"

GRUB_MOD_LICENSE ("GPLv3+");

  /* wimboot @:boot.wim:/test.wim @:bootx64.efi:/test.efi @:bcd:/bcd
   * params:
   *         --gui     -g
   *         --rawbcd  -b
   *         --rawwim  -w
   *         --pause   -p
   *         --index   -i  --index=n
   */

static const struct grub_arg_option options_wimboot[] = {
  {"gui", 'g', 0, N_("Display graphical boot messages."), 0, 0},
  {"rawbcd", 'b', 0, N_("Disable rewriting .exe to .efi in the BCD file."), 0, 0},
  {"rawwim", 'w', 0, N_("Disable patching the wim file."), 0, 0},
  {"index", 'i', 0, N_("Use WIM image index n."), N_("n"), ARG_TYPE_INT},
  {"pause", 'p', 0, N_("Show info and wait for keypress."), 0, 0},
  {"inject", 'j', 0, N_("Set inject dir."), N_("PATH"), ARG_TYPE_STRING},
  {"mem", 'm', 0, N_("Copy files to RAM."), 0, 0},
  {0, 0, 0, 0, 0, 0}
};

enum options_wimboot
{
  WIMBOOT_GUI,
  WIMBOOT_RAWBCD,
  WIMBOOT_RAWWIM,
  WIMBOOT_INDEX,
  WIMBOOT_PAUSE,
  WIMBOOT_INJECT,
  WIMBOOT_MEM
};

/** Use raw (unpatched) BCD files */
int cmdline_rawbcd;

/** Use raw (unpatched) WIM files */
int cmdline_rawwim;

/** Allow graphical output from bootmgr/bootmgfw */
int cmdline_gui;

/** Pause before booting OS */
int cmdline_pause;

/** WIM boot index */
unsigned int cmdline_index;

/** nfiles **/
unsigned int wimboot_nfiles;

/** inject dir */
char cmdline_inject_dir[256] = WIM_INJECT_DIR;

grub_efi_handle_t vdisk = NULL;
grub_efi_handle_t vpartition = NULL;

static void
grub_wimboot_close (struct grub_wimboot_context *wimboot_ctx)
{
  int i;
  if (!wimboot_ctx->components)
    return;
  for (i = 0; i < wimboot_ctx->nfiles; i++)
    {
      grub_free (wimboot_ctx->components[i].file_name);
      grub_file_close (wimboot_ctx->components[i].file);
    }
  grub_free (wimboot_ctx->components);
  wimboot_ctx->components = 0;
}

static grub_err_t
grub_wimboot_init (int argc, char *argv[],
          struct grub_wimboot_context *wimboot_ctx)
{
  int i;
  int newc = 0;
  struct dir *root = 0;

  wimboot_ctx->nfiles = 0;
  wimboot_ctx->components = 0;

  wimboot_ctx->components = grub_zalloc (argc
                    * sizeof (wimboot_ctx->components[0]));
  if (!wimboot_ctx->components)
    return grub_errno;

  wimboot_ctx->size = 0;

  for (i = 0; i < argc; i++)
    {
      const char *fname = argv[i];
      
      wimboot_ctx->size = ALIGN_UP (wimboot_ctx->size, 4);

      if (grub_memcmp (argv[i], "@:", 2) == 0)
    {
      const char *ptr, *eptr;
      ptr = argv[i] + 2;
      while (*ptr == '/')
        ptr++;
      eptr = grub_strchr (ptr, ':');
      if (eptr)
        {
          wimboot_ctx->components[i].file_name = grub_strndup (ptr, eptr - ptr);
          if (!wimboot_ctx->components[i].file_name)
        {
          grub_wimboot_close (wimboot_ctx);
          return grub_errno;
        }
          wimboot_ctx->size
		+= ALIGN_UP (sizeof (struct newc_head)
			    + grub_strlen (wimboot_ctx->components[i].file_name),
			     4);
	      wimboot_ctx->size += insert_dir (wimboot_ctx->components[i].file_name,
					      &root, 0);
	      newc = 1;
          fname = eptr + 1;
        }
    }
      else if (newc)
	{
	  wimboot_ctx->size += ALIGN_UP (sizeof (struct newc_head)
					+ sizeof ("TRAILER!!!") - 1, 4);
	  free_dir (root);
	  root = 0;
	  newc = 0;
	}
      wimboot_ctx->components[i].file = grub_file_open (fname,
                               GRUB_FILE_TYPE_LINUX_INITRD
                               | GRUB_FILE_TYPE_NO_DECOMPRESS);
      if (!wimboot_ctx->components[i].file)
    {
      grub_wimboot_close (wimboot_ctx);
      return grub_errno;
    }
      wimboot_ctx->nfiles++;
      wimboot_ctx->components[i].size
    = grub_file_size (wimboot_ctx->components[i].file);
      wimboot_ctx->size += wimboot_ctx->components[i].size;
      grub_printf ("file %d: %s path: %s\n", wimboot_ctx->nfiles, wimboot_ctx->components[i].file_name, fname);
    }
    
  if (newc)
    {
      wimboot_ctx->size = ALIGN_UP (wimboot_ctx->size, 4);
      wimboot_ctx->size += ALIGN_UP (sizeof (struct newc_head)
				    + sizeof ("TRAILER!!!") - 1, 4);
      free_dir (root);
      root = 0;
    }
  
  return GRUB_ERR_NONE;
}

static grub_size_t
grub_get_wimboot_size (struct grub_wimboot_context *wimboot_ctx)
{
  return wimboot_ctx->size;
}

static grub_err_t
grub_wimboot_load (struct grub_wimboot_context *wimboot_ctx,
		  char *argv[], void *target)
{
  grub_uint8_t *ptr = target;
  int i;
  int newc = 0;
  struct dir *root = 0;
  grub_ssize_t cursize = 0;

  for (i = 0; i < wimboot_ctx->nfiles; i++)
    {
      grub_memset (ptr, 0, ALIGN_UP_OVERHEAD (cursize, 4));
      ptr += ALIGN_UP_OVERHEAD (cursize, 4);

      if (wimboot_ctx->components[i].file_name)
	{
	  ptr += insert_dir (wimboot_ctx->components[i].file_name,
			     &root, ptr);
	  ptr = make_header (ptr, wimboot_ctx->components[i].file_name,
			     grub_strlen (wimboot_ctx->components[i].file_name),
			     0100777,
			     wimboot_ctx->components[i].size);
	  newc = 1;
	}
      else if (newc)
	{
	  ptr = make_header (ptr, "TRAILER!!!", sizeof ("TRAILER!!!") - 1,
			     0, 0);
	  free_dir (root);
	  root = 0;
	  newc = 0;
	}

      cursize = wimboot_ctx->components[i].size;
      if (grub_file_read (wimboot_ctx->components[i].file, ptr, cursize)
	  != cursize)
	{
	  if (!grub_errno)
	    grub_error (GRUB_ERR_FILE_READ_ERROR, N_("premature end of file %s"),
			argv[i]);
	  grub_wimboot_close (wimboot_ctx);
	  return grub_errno;
	}
      ptr += cursize;
    }
  if (newc)
    {
      grub_memset (ptr, 0, ALIGN_UP_OVERHEAD (cursize, 4));
      ptr += ALIGN_UP_OVERHEAD (cursize, 4);
      ptr = make_header (ptr, "TRAILER!!!", sizeof ("TRAILER!!!") - 1, 0, 0);
    }
  free_dir (root);
  root = 0;
  return GRUB_ERR_NONE;
}

static grub_err_t
grub_cmd_wimboot (grub_extcmd_context_t ctxt,
              int argc, char *argv[])
{
  struct grub_arg_list *state = ctxt->state;
  struct grub_wimboot_context wimboot_ctx = { 0, 0, 0};
  grub_size_t size = 0;
  void *wimboot_mem = NULL;
  
  if (argc == 0)
    {
      grub_error (GRUB_ERR_BAD_ARGUMENT, N_("filename expected"));
      goto fail;
    }
  
  if (grub_wimboot_init (argc, argv, &wimboot_ctx))
    goto fail;

  if (state[WIMBOOT_GUI].set)
    cmdline_gui = 1;
  else
    cmdline_gui = 0;
  if (state[WIMBOOT_RAWBCD].set)
    cmdline_rawbcd = 1;
  else
    cmdline_rawbcd = 0;
  if (state[WIMBOOT_RAWWIM].set)
    cmdline_rawwim = 1;
  else
    cmdline_rawwim = 0;
  if (state[WIMBOOT_PAUSE].set)
    cmdline_pause = 1;
  else
    cmdline_pause = 0;
  if (state[WIMBOOT_INDEX].set)
    cmdline_index = grub_strtoul (state[WIMBOOT_INDEX].arg, NULL, 10);
  else
    cmdline_index = 0;
  if (state[WIMBOOT_INJECT].set)
    grub_strcpy (cmdline_inject_dir, state[WIMBOOT_INJECT].arg);

  wimboot_nfiles = wimboot_ctx.nfiles;
  DBG ( "Extract files\n" );
  if (state[WIMBOOT_MEM].set)
  {
    size = grub_get_wimboot_size (&wimboot_ctx);
    wimboot_mem = grub_malloc (size);
    if (!wimboot_mem)
      goto fail;
    if (grub_wimboot_load (&wimboot_ctx, argv, wimboot_mem))
      goto fail;
    cpio_extract ( wimboot_mem, size, efi_add_file );
  }
  else
    grub_extract ( &wimboot_ctx );
  DBG ( "Install virtual disk\n" );
  efi_install ( &vdisk, &vpartition );
  DBG ( "Invoke boot manager\n" );
  efi_boot ( bootmgfw, bootmgfw_path, vpartition );

fail:
  grub_wimboot_close (&wimboot_ctx);
  if (wimboot_mem)
    grub_free (wimboot_mem);
  return grub_errno;
}

static grub_extcmd_t cmd;

GRUB_MOD_INIT(wimboot)
{
    cmd = grub_register_extcmd ("wimboot", grub_cmd_wimboot, 0,
    N_("[--rawbcd] [--index=n] [--pause] @:NAME:PATH"),
    N_("Windows Imaging Format bootloader"),
    options_wimboot);
}

GRUB_MOD_FINI(wimboot)
{
    grub_unregister_extcmd (cmd);
}
