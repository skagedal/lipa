/* eval.c - part of LIPA
 *
 * Evaluation
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
#include <string.h>
#include "basic.h"
#include "list.h"
#include "eval.h"
#include "print.h"

/* `symbolname_lookup' - find a symbol of a specific name */

LObject *
lipa_symbolname_lookup (gchar *name)
{
  LObject *obj;
  LObject *sym = lipa_symbols;

  while (!lipa_null (sym))
    {
      obj = lipa_car (sym);
      if (!strcmp (name, L_SYMBOL(obj).name->str)) 
	{
	  return obj;
	}
      sym = lipa_cdr (sym);
    }

  return NULL;
}

/* `register_symbol' - simply add a symbol on the top of the list,
 * shadowing eventual other symbols of the same name */

void
lipa_register_symbol (LObject *sym)
{
  lipa_symbols = lipa_cons (sym, lipa_symbols);
}

/* `unregister_symbol' - remove the specified symbol */

static LObject *
lipa_unregister_symbol_rec (LObject *sym, LObject *syms)
{
  if (lipa_null (syms))
    return NULL;
  else if (sym == lipa_car (syms))
    return lipa_cdr (syms);
  else
    return lipa_cons (lipa_car (syms),
		      lipa_unregister_symbol_rec (sym, lipa_cdr (syms)));
}

void
lipa_unregister_symbol (LObject *sym)
{
  lipa_symbols = lipa_unregister_symbol_rec (sym, lipa_symbols);
}

/* returns the symbol with the name NAME, if it doesn't exists it
   creates it */

LObject *
lipa_symbol (gchar *name)
{
  LObject *obj;

  obj = lipa_symbolname_lookup (name);

  if (!obj)
    {
      /* didn't exist; need to create */

      obj = lipa_new_symbol (name, NULL);
      lipa_register_symbol (obj);
    }
  
  return obj;
}

/* defining a symbol */

LObject *
lipa_define (gchar *name, LObject *value)
{
  LObject *obj;

  obj = lipa_symbol (name);

  L_SYMBOL(obj).value = value;

  return obj;
}

/***********************************************************************
 * Special forms
 ***********************************************************************/

static LObject *
special_form_define (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("error!! wrong number of arguments to define. ", stderr);
      return NULL;
    }
  else
    {
      LObject *todefine = lipa_car (args);
      LObject *defineto = lipa_car (lipa_cdr (args));

      if (todefine->type == L_OBJ_SYMBOLNAME)
	{
	  return lipa_define (L_STRING (todefine)->str,
			      lipa_eval (defineto));
	}
      else if (todefine->type == L_OBJ_LIST)
	{
	  LObject *symname = lipa_car (todefine);
	  LObject *args = lipa_cdr (todefine);

	  LObject *fun = lipa_new_function_lambda (args, defineto);

	  return lipa_define (L_STRING (symname)->str,
			 fun);
	}
      else
	{
	  fputs ("first argument to define should be a symbolname\n",
		 stderr);
	  return NULL;
	}
    }
}

static LObject *
special_form_if (LObject *args)
{
  LObject *test;
  
  if (!lipa_list_at_least (args, 2))
    {
      fputs ("error! if should have at least two arguments.\n", 
	     stderr);
      return NULL;
    }

  test = lipa_eval (lipa_car (args));

  if (!test)
    {
      fputs ("Symbol not defined!\n", stderr);
      return NULL;
    }
  
  if (test->type != L_OBJ_FALSE)
    {
      return lipa_eval (lipa_car (lipa_cdr (args)));
    }
  else
    {
      LObject *ret = NULL;
      LObject *cur = lipa_cdr (lipa_cdr (args));

      while (!lipa_null (cur))
	{
	  ret = lipa_eval (lipa_car (cur));
	  cur = lipa_cdr (cur);
	}
      return ret;
    }
}

static LObject *
special_form_lambda (LObject *args)
{
  LObject *fun;

  if (!lipa_list_length (args, 2))
    {
      fputs ("error! lambda should have two arguments, arguments and body\n",
	     stderr);
      return NULL;
    }

  /* arg 1 should be a list of L_OBJ_SYMBOLNAME, the arguments
   * arg 2 should be a body
   * but why bother checking? why would the user not do the right
   * thing? :) */

  /* At least lambda doesn't dump core... */
    
  return lipa_new_function_lambda (lipa_car (args), 
				   lipa_car (lipa_cdr (args)));

  return fun;
}

static LObject *
special_form_quote (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("quote should have one argument\n", stderr);
      return NULL;
    }

  return lipa_car (args);
}

static LObject *
special_form_and (LObject *args)
{
  LObject *obj = lisp_true;

  while (!lipa_null (args))
    {
      obj = lipa_eval (lipa_car (args));

      if (!obj)
	{
	  fputs ("Symbol not defined!\n", stderr);
	  return NULL;
	}
      
      if (obj->type == L_OBJ_FALSE)
	return lisp_false;

      args = lipa_cdr (args);
    }

  return obj;
}

static LObject *
special_form_or (LObject *args)
{
  LObject *obj = NULL;

  while (!lipa_null (args))
    {
      obj = lipa_eval (lipa_car (args));

      if (!obj)
	{
	  fputs ("Symbol not defined!\n", stderr);
	  return NULL;
	}

      if (obj->type != L_OBJ_FALSE)
	return obj;

      args = lipa_cdr (args);
    }

  return lisp_false;
}

static LObject *
special_form_begin (LObject *args)
{
  LObject *cur;
  LObject *ret = NULL;

  for (cur = args; !lipa_null(cur); cur = lipa_cdr (cur))
    {
      ret = lipa_eval (lipa_car (cur));
    }

  return ret;
}

static LObject *
eval_special_form (LObject *obj, LObject *args, gboolean *found)
{
  *found = TRUE;
  if (obj->type == L_OBJ_SYMBOLNAME)
    {
      gchar *name = L_STRING(obj)->str;
      if (!strcmp (name, "define"))
	{
	  return special_form_define (args);
	}
      else if (!strcmp (name, "if"))
	{
	  return special_form_if (args);
	}
      else if (!strcmp (name, "lambda"))
	{
	  return special_form_lambda (args);
	}
      else if (!strcmp (name, "quote"))
	{
	  return special_form_quote (args);
	}
      else if (!strcmp (name, "and"))
	{
	  return special_form_and (args);
	}
      else if (!strcmp (name, "or"))
	{
	  return special_form_or (args);
	}
      else if (!strcmp (name, "begin"))
	{
	  return special_form_begin (args);
	}
    }

  *found = FALSE;
  return NULL;
}

/***********************************************************************
 * Functions
 ***********************************************************************/

/* apply a L_OBJ_FUNCTION to args */

LObject *
lipa_apply_func (LObject *function, LObject *args)
{
#if 0
  LObject *args2 = args;
#endif

  /* this is no good - you gotta be able to do (null? '())...  but
   * there sure are problems with the nil handling.  currently, nil
   * and void are the same thing, NULL...  which is bad.  maybe nil
   * should be a special type just like L_TRUE and L_FALSE...  I
   * dunno..  - Simon */
  
#if 0  
  while (args2)
    {
      if (!(lipa_car (args2)))
	{
	  fprintf (stderr, "Nailed your ass!!!  No coredump here.\n");
	  return NULL;
	}
      else if (!lipa_cdr (args2))
	break;
      else
	args2 = lipa_cdr (args2);
    }
#endif

  if (L_FUNCTION (function).functype == L_FUNC_SUBR && 
      L_FUNCTION (function).subr)
    return ((L_FUNCTION (function).subr) (args));
  else if (L_FUNCTION (function).functype == L_FUNC_LAMBDA)
    {
      LObject *fargs = L_FUNCTION (function).arguments;
      LObject *pushed_symbols = NULL;
      LObject *body_ret = NULL;
      
      /* push arguments on symbols list */
      while (!lipa_null (fargs))
	{
	  gchar *name;
	  LObject *sym;

	  if (lipa_null (args))
	    {
	      fputs ("too few arguments! lamer!\n", stderr);
	      return NULL;
	    }

	  name = L_STRING (lipa_car (fargs))->str;
	  sym = lipa_new_symbol (name, lipa_car (args));
	  
	  pushed_symbols = lipa_cons (sym, pushed_symbols);
	  lipa_register_symbol (sym);

	  args = lipa_cdr (args);
	  fargs = lipa_cdr (fargs);
	}

      /* evaluate body */

      body_ret = lipa_eval (L_FUNCTION (function).body);

      /* remove arguments from symbols list */

      while (!lipa_null (pushed_symbols))
	{
	  lipa_unregister_symbol (lipa_car (pushed_symbols));
	  pushed_symbols = lipa_cdr (pushed_symbols);
	}

      return body_ret;
    }
  else
    {
      fputs ("This is a function from outer space.\n", stderr);
      return NULL;
    }

  return NULL;
}

LObject *
lipa_eval_list (LObject *list)
{
  if (!lipa_null (list))
    {
      return lipa_cons (lipa_eval (lipa_car (list)),
			lipa_eval_list (lipa_cdr (list)));
    }
  else
    return NULL;
}

static LObject *
eval_function (LObject *obj, LObject *args, gboolean *found)
{
  *found = TRUE;

  if (obj->type == L_OBJ_SYMBOLNAME)
    {
      LObject *sym = lipa_symbolname_lookup (L_SYMBOLNAME(obj)->str);
      if (sym)
	{
	  LObject *fun;

	  if (sym->type != L_OBJ_SYMBOL)
	    {
	      fputs ("this should like...  NOT happen.\n", stderr);
	      return NULL;
	    }

	  fun = L_SYMBOL(sym).value;

	  if (fun && fun->type == L_OBJ_FUNCTION)
	    {
	      return lipa_apply_func (fun, lipa_eval_list (args));
	    }
	}
    }
  
  *found = FALSE;
  return NULL;
}

LObject *
lipa_eval (LObject *obj)
{
  gboolean found;
  LObject *tmp;

  if (!obj)
    return NULL;

  switch (obj->type)
    {
    case L_OBJ_LIST:

      tmp = eval_special_form (lipa_car (obj), lipa_cdr (obj), &found);

      if (found)
	{
	  return tmp;
	}
      else
	{
	  tmp = eval_function (lipa_car (obj), lipa_cdr (obj), &found);
	  if (found)
	    {
	      return tmp;
	    }
	  else
	    {
	      fputs ("can't be evaluated as a function: ", stdout);
	      lipa_print (obj);
	      fputc ('\n', stdout);
	    }
	}

      break;
      
    case L_OBJ_FUNCTION:
      fputs ("This should not be happning?!?!!?!?!\n", stderr);
      return NULL;
      break;

    case L_OBJ_SYMBOLNAME:
      return (lipa_eval(lipa_symbolname_lookup (L_SYMBOLNAME(obj)->str)));
      break;

    case L_OBJ_SYMBOL:
      return L_SYMBOL(obj).value;
      break;

      /* these evaluate to themselves */
    case L_OBJ_STRING:
    case L_OBJ_INT:
    case L_OBJ_FLOAT:
    case L_OBJ_CHAR:
    case L_OBJ_TRUE:
    case L_OBJ_FALSE:
    case L_OBJ_USEROBJECT:
      return obj;
      break;
    }

  return NULL;
}
