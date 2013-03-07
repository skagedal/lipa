/* load.c - part of LIPA
 *
 * Loading functions
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
 */

#include <stdio.h>
#include <glib.h>
#include <errno.h>
#include "basic.h"
#include "list.h"
#include "read.h"
#include "eval.h"
#include "load.h"

/***********************************************************************
 * A LGetc that reads from a stream
 ***********************************************************************/

static gint
stream_getc (LGetc *lgetc)
{
  FILE *file = lgetc->data;

  return fgetc (file);
}

static void
stream_ungetc (LGetc *lgetc, gint c)
{
  FILE *file = lgetc->data;

  ungetc (c, file);
}

static gint
stream_anyleft (LGetc *lgetc)
{
  FILE *file = lgetc->data;

  return (!feof (file));
}

/***********************************************************************
 * Loading
 ***********************************************************************/

/*
 * This one's kludgy, but it works.  Even comments work. :-)
 */

LObject *
lipa_load (gchar *filename)
{
  FILE *file = fopen (filename, "r");
  GString *s = g_string_new ("");
  gint c;
  gint paren = 0;
  gint quote = 0;
  gint backslash = 0;
  gint comment = 0;
  
  if (file)
    {
      LGetc lgetc;
      LObject *obj = NULL;

      lgetc.get = stream_getc;
      lgetc.unget = stream_ungetc;
      lgetc.anyleft = stream_anyleft;
      lgetc.data = file;

      while (lgetc.anyleft (&lgetc))
	{
	  g_string_append_c (s, c = lgetc.get (&lgetc));
	  if (c == '(')
	    {
	      backslash = 0;
	      if (!quote && !comment)
		paren++;
	    }
	  else if (c == ')')
	    {
	      backslash = 0;
	      if (!quote && !comment)
		{
		  paren--;
		  if (!paren)
		    {
		      /* fprintf (stderr, "s: %s\n", s->str); */
		      obj = lipa_eval (lipa_read_general (s));
		      g_string_assign (s, "");
		    }
		}
	    }
	  else if (c == ';')
	    {
	      comment = 1;
	      backslash = 0;
	    }
	  else if (c == '\n')
	    {
	      comment = 0;
	      backslash = 0;
	    }
	  else if (c == '\\' && !comment)
	    backslash = 1;
	  else if (c == '\"' && !comment)
	    {
	      if (!backslash)
		{
		  if (!quote)
		    quote = 1;
		  else
		    quote = 0;
		}
	      backslash = 0;
	    }
	}

      fclose (file);

      return obj;
    }
  else
    {
      fprintf (stderr, "lipa: %s: %s\n", filename, g_strerror (errno));
      return NULL;
    }
}

/* loads a file of lipa code - reads and evaluates it */

static LObject *
lisp_load (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("load says: I just want one argument!\n", stderr);
      return NULL;
    }

  return lipa_eval (lipa_load (L_STRING (lipa_car (args))->str));
}

void
lipa_register_load_functions (void)
{
  lipa_register_subr ("load", lisp_load);
}
