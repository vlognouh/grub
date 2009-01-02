/* font_cmd.c - Font command definition. */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2003,2005,2006,2007,2008,2009  Free Software Foundation, Inc.
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

#include <grub/font.h>
#include <grub/dl.h>
#include <grub/normal.h>
#include <grub/misc.h>

static grub_err_t
loadfont_command (struct grub_arg_list *state __attribute__ ((unused)),
	      int argc,
	      char **args)
{
  if (argc == 0)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, "no font specified");

  while (argc--)
    if (grub_font_load (*args++) != 0)
      return GRUB_ERR_BAD_FONT;

  return GRUB_ERR_NONE;
}

static grub_err_t
lsfonts_command (struct grub_arg_list *state __attribute__ ((unused)),
                 int argc __attribute__ ((unused)),
                 char **args __attribute__ ((unused)))
{
  struct grub_font_node *node;

  grub_printf ("Loaded fonts:\n");
  for (node = grub_font_list; node; node = node->next)
    {
      grub_font_t font = node->value;
      grub_printf ("%s\n", grub_font_get_name (font));
    }

  return GRUB_ERR_NONE;
}

GRUB_MOD_INIT(font_manager)
{
  grub_font_loader_init ();

  grub_register_command ("loadfont", loadfont_command, GRUB_COMMAND_FLAG_BOTH,
			 "loadfont FILE...",
			 "Specify one or more font files to load.", 0);

  grub_register_command ("lsfonts", lsfonts_command, GRUB_COMMAND_FLAG_BOTH,
			 "lsfonts",
			 "List the loaded fonts.", 0);
}

GRUB_MOD_FINI(font_manager)
{
  /* TODO: Determine way to free allocated resources.  
     Warning: possible pointer references could be in use.  */

  grub_unregister_command ("loadfont");
}

