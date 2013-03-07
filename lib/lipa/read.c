/* read.c - part of LIPA
 *
 * Reading functions
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
 * Created by Simon 1999-02-23 (split off of lispparser.[ch])
 */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include "basic.h"
#include "list.h"
#include "read.h"


/***********************************************************************
 * Reading
 *
 * `l_read' parses the symbol S and returns the created LObject 
 *
 * `_l_read*' is a bunch of locally used functions
 ***********************************************************************/

static void
ignore_whitespace (GString *s)
{
  while (s->str && s->str[0] > 0 && isspace (s->str[0]))
    memmove (s->str, s->str + 1, strlen (s->str));
}

static LObject *
_l_read_list (GString *s)
{
  gint c;

  while (strlen (s->str) > 0)
    {
      c = s->str[0];
      if (c == ')')
	{
	  memmove (s->str, s->str + 1, strlen (s->str));
	  return NULL;
	} 
      if (!isspace (c))
	{
	  LObject *read_car, *read_cdr;

	  read_car = lipa_read_general (s);
	  ignore_whitespace (s);
	  read_cdr = _l_read_list (s);
	  return lipa_cons (read_car, read_cdr);
	}
      else
	memmove (s->str, s->str + 1, strlen (s->str));
    }
  return NULL;	/* should really be error */
};

static void
_l_read_symbolname (GString *s, GString *str)
{
  while (s->str[0] > 0 && !isspace (s->str[0]) && (s->str[0] != ')'))
    {
      g_string_append_c (str, s->str[0]);
      memmove (s->str, s->str + 1, strlen (s->str));
    }
}

/* return TRUE on success */

static gboolean
_l_read_string (GString *s, GString *str)
{
  gint c = 0 ;

  while ((c = s->str[0]) > 0 && (c != '\"'))
    {
      memmove (s->str, s->str + 1, strlen (s->str));
      if (c == '\\')
	{
	  c = s->str[0];
	  memmove (s->str, s->str + 1, strlen (s->str));
	  switch (c)
	    {
	    case 't':
	      g_string_append_c (str, '\t');
	      break;
	    case 'n':
	      g_string_append_c (str, '\n');
	      break;
	    case 'a':
	      g_string_append_c (str, '\a');
	      break;
	    case 'b':
	      g_string_append_c (str, '\b');
	      break;
	    case 'f':
	      g_string_append_c (str, '\f');
	      break;
	    case 'r':
	      g_string_append_c (str, '\r');
	      break;
	    case 'v':
	      g_string_append_c (str, '\v');
	      break;
	    case '\\':
	      g_string_append_c (str, '\\');
	      break;
	    case '\?':
	      g_string_append_c (str, '\?');
	      break;
	    case '\'':
	      g_string_append_c (str, '\\');
	      break;
	    case '\"':
	      g_string_append_c (str, '\"');
	      break;
	    default:
	      g_string_append_c (str, '\\');
	      g_string_append_c (str, c);
	    }
	}
      else
	g_string_append_c (str, c);
    }

  if (c == '\"')
    {
      memmove (s->str, s->str + 1, strlen (s->str));

      return TRUE;
    }
  else
    {
      fputs ("error!! string not terminated!!\n", stderr);
      return FALSE;
    }
}

static LObject *
_l_read_number (GString *s)
{
  gint i = 0;
  gint c;
  gfloat dec = 0;
  gfloat incr = 0.1;

  while (isdigit (c = s->str[0]) || c == '.')
    {
      memmove (s->str, s->str + 1, strlen (s->str));
      if (c == '.')
	break;
      i = i*10 + (c) - '0';
    }

  if (c == '.')
    {
      while (isdigit (c = s->str[0]))
	{
	  memmove (s->str, s->str + 1, strlen (s->str));
	  dec = dec + (c - '0') * incr;
	  incr *= 0.1;
	}
    }

  if (c > 0 && !isspace (c) && c != ')')
    {
      fputs ("error!! badly formed number!!\n", stderr);

      return FALSE;
    }

  if (dec != 0)
    {
      return lipa_new_float ((gfloat)i + dec);
    }
  else
    {
      return lipa_new_int (i);
    }
}

static LObject *
_l_read_char (GString *s)
{
  guchar c;

  c = s->str[0];
  memmove (s->str, s->str + 1, strlen (s->str));

  return lipa_new_char (c);
}

static LObject *
_l_quote_read (GString *s)
{
  LObject *symname = lipa_new_symbolname ();
  LObject *next;

  g_string_append (L_SYMBOLNAME (symname), "quote");

  next = lipa_cons (lipa_read_general (s), NULL);

  return lipa_cons (symname, next);
}

static void
_l_read_comment (GString *s)
{
  gint c = 0;

  while (strlen (s->str) > 0 && (c = s->str[0]) != '\n')
    memmove (s->str, s->str + 1, strlen (s->str));
}

LObject *
lipa_read_general (GString *s)
{
  LObject *obj = NULL;

  ignore_whitespace (s);

  if (strlen (s->str) > 0)
    {
      gint c = s->str[0];

      if (c == '(')		/* It's a list! */
	{
	  memmove (s->str, s->str + 1, strlen (s->str));
	  return _l_read_list (s);
	}
      else if (c == '\"')	/* It's a string! */
	{
	  obj = lipa_new_string ();
	  memmove (s->str, s->str + 1, strlen (s->str));
	  _l_read_string (s, L_STRING (obj));
	  return obj;
	}
      else if (isdigit (c))	/* It's a number! */
	{
	  return _l_read_number (s);
	}
      else if (c == '#')	/* It's a char, or a symbol that
				 * starts with #! */
	{
	  if (s->str[1] == '\\')
	    {
	      memmove (s->str, s->str + 1, strlen (s->str));
	      memmove (s->str, s->str + 1, strlen (s->str));
	      return _l_read_char (s);
	    }
	  else  /* if (s->str[1] == 't' || s->str[1] == 'f') */
	    {
	      obj = lipa_new_symbolname ();
	      _l_read_symbolname (s, L_SYMBOLNAME (obj));
	      return obj;
	    }
	}
      else if (c == '\'')     /* It's the syntatic sugar for "quote"! */
	{
	  memmove (s->str, s->str + 1, strlen (s->str));
	  return _l_quote_read (s);
	}
      else if (c == ';')	/* It's a comment! */
	{
	  _l_read_comment (s);
	  return lipa_read_general (s);
	}
      else			/* It's a symbol! */
	{
	  obj = lipa_new_symbolname ();
	  _l_read_symbolname (s, L_SYMBOLNAME (obj));
	  return obj;
	}
    }
  else
    return NULL;
}

/***********************************************************************
 * A LGetc that reads from a string 
 ***********************************************************************/

gint
string_getc (LGetc *lgetc)
{
  guchar **s = lgetc->data;
  gint ret = 0;

  if (**s)
    {
      ret = **s;
      (*s)++;
    }
  else
    ret = -1;

  return ret;
}

void
string_ungetc (LGetc *lgetc, gint c)
{
  guchar **s = lgetc->data;
  
  (*s)--;
    
}

gboolean
string_anyleft (LGetc *lgetc)
{
  guchar **s = lgetc->data;

  return !!(**s);
}

/* read a null-terminated string */

LObject *
lipa_read (guchar *s)
{
  LGetc lgetc;
  GString *in = g_string_new ("");

  lgetc.get = string_getc;
  lgetc.unget = string_ungetc;
  lgetc.anyleft = string_anyleft;
  lgetc.data = &s;


  while (lgetc.anyleft (&lgetc))
    {
      g_string_append_c (in, lgetc.get (&lgetc));
    }

  return lipa_read_general (in);
}
