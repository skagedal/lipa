/* lipa.c - part of LIPA
 *
 * This is the LIPA REPL
 *
 * Copyright (C) 1999 Simon Kågedal and Linus Åkerlund
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License in the file COPYING for more details.
 *
 * Created by Simon 1999-03-07
 */

#include <stdio.h>
#include <glib.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <lipa/lipa.h>

static gboolean quit_flag = FALSE;

/* quit LIPA */

static LObject *
repl_bye (LObject *args)		/* arguments ignored */
{
  quit_flag = TRUE;
  return NULL;
}

/* print usage information and exit
 */

static void
usage_exit (int ret_code)
{
  printf ("Usage: %s [OPTION]...\n\n"
	  "  -n, --no-init\t\t\t\tDon't read ~/.liparc\n"
	  "  -s FILENAME, --script=FILENAME\trun FILENAME and exit\n"
	  "  -h, --help\t\t\t\tdisplay this help and exit\n"
	  "  -v, --version\t\t\t\tdisplay version information and exit\n",
	  g_get_prgname ());

  exit (ret_code);
}

int
main (int argc, char *argv [])
{
  int c, option_index = 0;
  char *line = NULL;

  /* for the 's' option */
  gchar *script_filename = NULL;

  /* for the 'n' option */
  gboolean load_init_file = TRUE;

  GString *config_filename = NULL;
  
  struct option opts [] =
  {
    { "script", required_argument, NULL, 's' },
    { "no-init", no_argument, NULL, 'n' },
    { "help", no_argument, NULL, 'h' },
    { "version", no_argument, NULL, 'v' },
    { 0, 0, 0, 0 }
  };

  g_set_prgname (argv [0]);

  /* parse options */

  while (1)
    {
      c = getopt_long (argc, argv, "s:nhv", opts, &option_index);

      if (c == -1)
	break;

      switch (c)
	{
	case 's':
	  script_filename = g_strdup (optarg);
	  break;

	case 'n':
	  load_init_file = FALSE;
	  break;

	case 'h':
	  usage_exit (0);

	case 'v':
	  fputs ("lipa " VERSION "\n", stdout);
	  exit (0);

	case '?':
	  /* `getopt_long' already printed an error message. */
	  fprintf (stderr, "Try `%s --help' for more information.\n",
		   g_get_prgname ());
	  exit (1);

	default:
	  abort ();
	}
    }


  lipa_init ();


  if (load_init_file)
    {
      config_filename = g_string_new (g_get_home_dir ());
      config_filename = g_string_append (config_filename, "/.liparc");

      lipa_load (config_filename->str);

      g_string_free (config_filename, TRUE);
    }

  lipa_register_subr ("bye", repl_bye);


  if (script_filename)
    {
      lipa_load (script_filename);
      exit (0);
    }
  
  /* REPL */

  rl_completion_entry_function = lipa_symbol_complete;

  fputs ("Hello.  Welcome to LIPA.\n", stdout);
  do
    {
      line = readline ("> ");
      if (line && *line)
	{
	  add_history (line);
	  lipa_print (lipa_eval (lipa_read (line)));
	  fputc ('\n', stdout);
	}
    }
  while (line && !quit_flag);

  fputs ("Bye.\n", stdout);

  return 0;
}
