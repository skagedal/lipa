/* convert.c - part of LIPA
 *
 * Functions for converting between types
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
 * Created by Linus 1999-03-05
 */

#include <stdio.h>
#include <glib.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "list.h"
#include "basic.h"
#include "convert.h"

static LObject *
lisp_string_to_number (LObject *args)
{
  LObject *obj_f = lipa_new_float (0.0);
  LObject *obj = lipa_new_int (0);

  if (!lipa_list_length (args, 1) || !STRINGP (lipa_car (args)))
    {
      fputs ("string->number wants one string argument\n", stderr);
      return NULL;
    }

  L_FLOAT (obj_f) = atof (L_STRING (lipa_car (args))->str); 

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

LObject *
lisp_number_to_string (LObject *args)
{
  LObject *obj;

  if (!lipa_list_length (args, 1) 
      || (!INTP (lipa_car (args)) && !FLOATP (lipa_car (args))))
    {
      fputs ("number->string wants one numerical argument\n", stderr);
      return NULL;
    }

  obj = lipa_new_string ("");

  /* 64 as maximum number of characters was arbitrarily chosen.  Not good. */
  if (INTP (lipa_car (args)))
    g_snprintf (L_STRING(obj)->str, 64, "%d", L_INT (lipa_car (args)));
  else
    g_snprintf (L_STRING(obj)->str, 64, "%f", L_FLOAT (lipa_car (args)));

  return obj;
}

LObject *
lisp_symbol_to_string (LObject *args)
{
  LObject *obj;

  if (!lipa_list_length (args, 1) || (!SYMBOLNAMEP (lipa_car (args))))
    {
      fputs ("symbol->string wants a symbol argument\n", stderr);
      return NULL;
    }

  obj = lipa_new_string ("");

  L_STRING(obj)->str = L_SYMBOLNAME (lipa_car (args))->str;

  return obj;
}

LObject *
lisp_string_to_symbol (LObject *args)
{
  LObject *obj;

  if (!lipa_list_length (args, 1) || !STRINGP (lipa_car (args)))
    {
      fputs ("string->symbol wants a string argument\n", stderr);
      return NULL;
    }
  
  obj = lipa_new_symbolname ();

  L_SYMBOLNAME(obj)->str = L_STRING (lipa_car (args))->str;

  return obj;
}

LObject *
lisp_char_to_integer (LObject *args)
{
  LObject *obj;

  if (!lipa_list_length (args, 1) || !CHARP (lipa_car (args)))
    {
      fputs ("char->integer wants 1 character argument\n", stderr);
      return NULL;
    }

  obj = lipa_new_int ((gint) (L_CHAR (lipa_car (args))));

  return obj;
}

LObject *
lisp_integer_to_char (LObject *args)
{
  LObject *obj;

  if (!lipa_list_length (args, 1) || !INTP (lipa_car (args)))
    {
      fputs ("integer->char wants 1 integer argument\n", stderr);
      return NULL;
    }

  obj = lipa_new_char ((guchar) (L_INT (lipa_car (args))));

  return obj;
}

LObject *
lisp_string_to_list (LObject *args)
{
  LObject *obj;
  gint i;

  if (!lipa_list_length (args, 1) || !STRINGP (lipa_car (args)))
    {
      fputs ("string->list wants 1 string argument\n", stderr);
      return NULL;
    }
  
  obj = lipa_new_list ();

  obj = lipa_cons (lipa_new_char (L_STRING (lipa_car (args))->str[strlen
		   (L_STRING (lipa_car (args))->str) - 1]), NULL);

  for (i = strlen (L_STRING (lipa_car (args))->str) - 2; i >=0;  i--)
    {
      obj = lipa_cons (lipa_new_char (L_STRING (lipa_car (args))->str[i]),
		       obj);
    }

  return obj;
}

LObject *
lisp_list_to_string (LObject *args)
{
  LObject *lst;
  LObject *obj;

  if (!lipa_list_length (args, 1) || !LISTP (lipa_car (args)))
    {
      fputs ("list->string wants a list as argument\n", stderr);
      return NULL;
    }

  obj = lipa_new_string ();

  lst = (lipa_car (args));

  if (!CHARP (lipa_car (lst)))
    {
      fputs ("list->string wants a list of characters!\n", stderr);
      return NULL;
    }

  L_STRING (obj) = g_string_append_c (L_STRING (obj), 
				      L_CHAR (lipa_car (lst)));

  while (L_LIST (lst).cdr)
    {
      if (!CHARP (lipa_car (lipa_cdr (lst))))
	{
	  fputs ("list->string wants a list of characters!\n", stderr);
	  return NULL;
	}
      g_string_append_c (L_STRING (obj), 
			  L_CHAR (lipa_car (lipa_cdr (lst))));
      L_LIST (lst) = L_LIST (lipa_cdr (lst));
    }

  return obj;
}

void
lipa_register_convert_functions (void)
{
  lipa_register_subr ("string->number", lisp_string_to_number); 
  lipa_register_subr ("number->string", lisp_number_to_string);
  lipa_register_subr ("symbol->string", lisp_symbol_to_string);
  lipa_register_subr ("string->symbol", lisp_string_to_symbol);
  lipa_register_subr ("char->integer", lisp_char_to_integer);
  lipa_register_subr ("integer->char", lisp_integer_to_char);
  lipa_register_subr ("string->list", lisp_string_to_list);
  lipa_register_subr ("list->string", lisp_list_to_string);
}
