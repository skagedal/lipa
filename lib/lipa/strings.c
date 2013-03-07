/* string.c - part of LIPA
 *
 * Procedures operating on strings
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
 * Created by Simon 1999-02-20
 */

#include <stdio.h>
#include <glib.h>
#include <string.h>
#include "list.h"
#include "basic.h"
#include "strings.h"

static GString *
_append (LObject *args, GString *str)
{
  if (!lipa_null (args))
    {
      LObject *obj = lipa_car (args);
      if (obj->type == L_OBJ_STRING)
	{
	  return g_string_prepend (_append (lipa_cdr (args), str),
				   L_STRING(obj)->str);
	}
      else
	{
	  fprintf (stderr, "append can only take string arguments");
	}
    }
  else
    return str;

  return NULL;
}

static LObject *
lisp_string_append (LObject *args)
{
  LObject *obj = lipa_new_string ();

  L_STRING(obj) = _append (args, L_STRING(obj));
  return obj;
}

static LObject *
lisp_display (LObject *args)
{
  while (!lipa_null(args))
    {
      fputs (L_STRING(lipa_car (args))->str, stdout);
      args = lipa_cdr (args);
    }

  return NULL;
}

static LObject *
lisp_string_length (LObject *args)
{
  LObject *obj = lipa_new_int (0);

  if (!lipa_list_length (args, 1))
    {
      fputs ("string-length wants 1 argument\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)))
    {
      fputs ("string-length wants a string argument\n", stderr);
      return NULL;
    }

  L_INT (obj) = strlen (L_STRING (lipa_car (args))->str);

  return obj;
}

LObject *
lisp_string_equalp (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string=? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!(STRINGP (lipa_car (args)) && STRINGP (lipa_car (lipa_cdr (args)))))
    {
      fputs ("string=? want string arguments\n", stderr);
      return NULL;
    }

  return (strcmp (L_STRING (lipa_car (args))->str, 
		  L_STRING (lipa_car (lipa_cdr (args)))->str) ? 
	  lisp_false : lisp_true);
}

/*
 * Same as previous function, but not case sensitive 
 */

LObject *
lisp_string_equalp_ci (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string-ci=? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!(STRINGP (lipa_car (args)) && STRINGP (lipa_car (lipa_cdr (args)))))
    {
      fputs ("string-ci=? want string arguments\n", stderr);
      return NULL;
    }

  return (strcasecmp (L_STRING (lipa_car (args))->str, 
		  L_STRING (lipa_car (lipa_cdr (args)))->str) ? 
	  lisp_false : lisp_true);
}

/*
 * The following scheme procedures take exactly two arguments, as 
 *   specified in r5rs.  It does say that implementations may generalize
 *   these (and string=? and string-ci=?) to take more than 2 arguments.
 *   I might fix that later
 */

LObject *
lisp_string_smaller_thanp (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string<? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string<? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) < 0) ? 
	  lisp_true : lisp_false);
}

LObject *
lisp_string_greater_thanp (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string>? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string>? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) > 0) ? 
	  lisp_true : lisp_false);
}


LObject *
lisp_string_smaller_than_or_equalp (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string<=? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string<=? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) <= 0) ? 
	  lisp_true : lisp_false);
}

LObject *
lisp_string_greater_than_or_equalp (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string>=? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string>=? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) >= 0) ? 
	  lisp_true : lisp_false);
}

LObject *
lisp_string_smaller_thanp_ci (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string-ci<? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string-ci<? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcasecmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) < 0) ? 
	  lisp_true : lisp_false);
}

LObject *
lisp_string_greater_thanp_ci (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string-ci>? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string-ci>? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcasecmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) > 0) ? 
	  lisp_true : lisp_false);
}

LObject *
lisp_string_smaller_than_or_equalp_ci (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string-ci<=? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string-ci<=? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcasecmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) <= 0) ? 
	  lisp_true : lisp_false);
}

LObject *
lisp_string_greater_than_or_equalp_ci (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("string-ci>=? wants 2 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !STRINGP (lipa_car (lipa_cdr (args))))
    {
      fputs ("string-ci>=? wants string arguments\n", stderr);
      return NULL;
    }

  return ((strcasecmp (L_STRING (lipa_car (args))->str, 
		   L_STRING (lipa_car (lipa_cdr (args)))->str) >= 0) ? 
	  lisp_true : lisp_false);
}

/*
 * I think this one should be r5rs compliant.
 */

LObject *
lisp_substring (LObject *args)
{
  char *str;

  LObject *obj = lipa_new_string ("");
  gint x, y, i;
  
  if (!lipa_list_length (args, 3))
    {
      fputs ("substring wants 3 arguments\n", stderr);
      return NULL;
    }
  if (!STRINGP (lipa_car (args)) || !INTP (lipa_car (lipa_cdr (args))) ||
      !INTP (lipa_car (lipa_cdr (lipa_cdr (args)))))
    {
      fputs ("substring wants 1 string and 2 ints as arguments\n", stderr);
      return NULL;
    }
  
  x = L_INT (lipa_car (lipa_cdr (args)));
  y = L_INT (lipa_car (lipa_cdr (lipa_cdr (args))));
  
  if ((x < 0) || (y > strlen (L_STRING (lipa_car (args))->str)) || (x >= y))
    {
      fputs ("substring: int arguments out of range\n", stderr);
      return NULL;
    }

  str = strdup (L_STRING (lipa_car (args))->str);
  
  for (i = x; i < y; i++)
    {
      g_string_append_c (L_STRING (obj), str[i]);
    }
  
  return obj;
}

void
lipa_register_string_functions (void)
{
  lipa_register_subr ("string-append", lisp_string_append); /* r5rs */
  lipa_register_subr ("display", lisp_display);
  lipa_register_subr ("string-length", lisp_string_length);
  lipa_register_subr ("string=?", lisp_string_equalp);
  lipa_register_subr ("string-ci=?", lisp_string_equalp_ci);
  lipa_register_subr ("string<?", lisp_string_smaller_thanp);
  lipa_register_subr ("string>?", lisp_string_greater_thanp);
  lipa_register_subr ("string<=?", lisp_string_smaller_than_or_equalp);
  lipa_register_subr ("string>=?", lisp_string_greater_than_or_equalp);
  lipa_register_subr ("string-ci<?", lisp_string_smaller_thanp_ci);
  lipa_register_subr ("string-ci>?", lisp_string_greater_thanp_ci);
  lipa_register_subr ("string-ci<=?", lisp_string_smaller_than_or_equalp_ci);
  lipa_register_subr ("string-ci>=?", lisp_string_greater_than_or_equalp_ci);
  lipa_register_subr ("substring", lisp_substring); 
}
