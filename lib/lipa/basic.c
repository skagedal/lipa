/* basic.c - part of LIPA
 *
 * Basic LIPA stuff
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
 * Created by Simon 1999-02-19
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "basic.h"
#include "strings.h"
#include "numbers.h"
#include "chars.h"
#include "list.h"
#include "convert.h"
#include "typepredicates.h"
#include "read.h"
#include "eval.h"
#include "print.h"
#include "load.h"
#include "readfuncs.h"
#include "evalprocs.h"

/***********************************************************************
 * Data
 ***********************************************************************/

/* these should probably be stored in a better way but i don't care */

LObject *lipa_symbols = NULL;

LObject *lisp_nil;
LObject *lisp_true;
LObject *lisp_false;

/***********************************************************************
 * Creating new primitives
 ***********************************************************************/

/* create a new LObject of specified type */

LObject *
lipa_new_object (LObjectType type)
{
  LObject *obj = g_new0 (LObject, 1);

  obj->type = type;

  return obj;
}

/* create a new symbolname */

LObject *
lipa_new_symbolname ()
{
  LObject *obj = g_new0 (LObject, 1);
  
  obj->type = L_OBJ_SYMBOLNAME;
  L_STRING(obj) = g_string_new ("");

  return obj;
}

/* create a new symbol */

LObject *
lipa_new_symbol (gchar *name, LObject *value)
{
  LObject *obj = g_new0 (LObject, 1);

  obj->type = L_OBJ_SYMBOL;
  L_SYMBOL(obj).name = g_string_new (name);
  L_SYMBOL(obj).value = value;

  return obj;
}

/* create a new list */

LObject *
lipa_new_list ()
{
  LObject *obj = g_new0 (LObject, 1);

  obj->type = L_OBJ_LIST;

  /* let car and cdr be initialized to 0 */

  return obj;
}

/* create a new string */

LObject *
lipa_new_string ()
{
  LObject *obj = g_new0 (LObject, 1);

  obj->type = L_OBJ_STRING;
  L_STRING (obj) = g_string_new ("");

  return obj;
}

/* create a new int */

LObject *
lipa_new_int (gint i)
{
  LObject *obj = g_new0 (LObject, 1);

  obj->type = L_OBJ_INT;
  L_INT (obj) = (i);

  return obj;
}

/* create a new float */

LObject *
lipa_new_float (gfloat i)
{
  LObject *obj = g_new0 (LObject, 1);

  obj->type = L_OBJ_FLOAT;
  L_FLOAT (obj) = i;

  return obj;
}

/* create a new char */

LObject *
lipa_new_char (guchar c)
{
  LObject *obj = g_new0 (LObject, 1);
  
  obj->type = L_OBJ_CHAR;
  L_CHAR (obj) = c;

  return obj;
}

/* create a new function */

LObject *
lipa_new_function ()
{
  LObject *obj = g_new0 (LObject, 1);

  obj->type = L_OBJ_FUNCTION;

  L_FUNCTION(obj).functype = L_FUNC_UNSPECIFIED;

  /* let stuff remain initialized to 0 */

  return obj;
}

LObject *
lipa_new_function_lambda (LObject *args, LObject *body)
{
  LObject *fun = lipa_new_function ();
  
  L_FUNCTION (fun).functype = L_FUNC_LAMBDA;
  L_FUNCTION (fun).arguments = args;
  L_FUNCTION (fun).body = body;

  return fun;
}

LObject *
lipa_new_userobject (gpointer data)
{
  LObject *obj = lipa_new_object (L_OBJ_USEROBJECT);

  L_USEROBJECT (obj) = data;

  return obj;
}

/* register a `subr' (an internal C function) */

void
lipa_register_subr (gchar *name, LFunctionFunc *func)
{
  LObject *sym;
  LObject *obj;

  /* set up the _function_ */

  obj = lipa_new_function ();

  L_FUNCTION(obj).functype = L_FUNC_SUBR;
  L_FUNCTION(obj).subr = func;

  /* set up the _symbol_ */

  sym = lipa_symbol (name);

  L_SYMBOL(sym).value = obj;
}

/***********************************************************************
 * Some basic procedures
 ***********************************************************************/

/* show all symbols */

static LObject *
lisp_showsyms (LObject *args)	/* arguments ignored */
{
  fputc ('(', stdout);
  lipa_print_list (lipa_symbols);
  fputs (")\n", stdout);

  return NULL;
}

/* completion generator - for use with readline */

int
lipa_symbol_complete (char *text, int state)
{
  static LObject *foundsyms = NULL;
  LObject *syms;
  int len = strlen (text);

  if (state == 0)
    {
      /* should be freeing foundsyms here but what the heck */
      foundsyms = NULL;

      syms = lipa_symbols;

      while (syms)
	{
	  gchar *str = L_SYMBOL (lipa_car (syms)).name->str;
	  
	  if (strlen (str) >= len)
	    {
	      if (!strncmp (text, str, len))
		{
		  foundsyms = lipa_cons (lipa_car (syms), foundsyms);
		}
	    }

	  syms = lipa_cdr (syms);
	}			
    }

  if (!lipa_null (foundsyms))
    {
      /* it has to be a strdup */
      gchar *str = strdup (L_SYMBOL (lipa_car (foundsyms)).name->str);

      foundsyms = lipa_cdr (foundsyms);

      return (int)str;
    }
  else
    {
      return (int)NULL;
    }
}

/* init lipa
 * returns 0 on success
 * TODO: do we wanna give it argc and argv? */

int
lipa_init (void)
{
  /* `lisp_nil' may be used as "nil" from lisp code, but in C code,
   * use NULL. */

  lisp_nil = lipa_new_symbol ("nil", NULL);
  lipa_register_symbol (lisp_nil);

  lisp_true = lipa_new_object (L_OBJ_TRUE);
  lisp_false = lipa_new_object (L_OBJ_FALSE);

  lipa_register_symbol (lipa_new_symbol ("#t", lisp_true));
  lipa_register_symbol (lipa_new_symbol ("#f", lisp_false));

  /* register all functions */

  lipa_register_list_functions ();
  lipa_register_string_functions ();
  lipa_register_number_functions ();
  lipa_register_char_functions ();
  lipa_register_read_functions ();
  lipa_register_load_functions ();
  lipa_register_type_predicates ();
  lipa_register_convert_functions ();
  lipa_register_eval_functions ();

  lipa_register_subr ("showsyms", lisp_showsyms);

  return 0;
}
