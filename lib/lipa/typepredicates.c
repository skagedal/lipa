/* typepredicates.c - part of LIPA
 *
 * Type predicates for use in Lisp 
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
 * Created by Simon 1999-02-26
 */

#include <stdio.h>
#include <glib.h>
#include <math.h>
#include "basic.h"
#include "list.h"
#include "typepredicates.h"


LObject *
lisp_symbolnamep (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return SYMBOLNAMEP(lipa_car (args)) ? lisp_true : lisp_false;
}

LObject *
lisp_listp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return LISTP(lipa_car (args)) ? lisp_true : lisp_false;
}

LObject *
lisp_stringp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return STRINGP(lipa_car (args)) ? lisp_true : lisp_false;
}

LObject *
lisp_intp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return INTP(lipa_car (args)) ? lisp_true : lisp_false;
}

LObject *
lisp_floatp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("Error 0xfe83a83f: internal device register busy!\n", stderr);
      return NULL;
    }
  
  return FLOATP(lipa_car (args)) ? lisp_true : lisp_false;
}

LObject *
lisp_numberp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: wrong number of arguments or other error\n", stderr);
      return NULL;
    }

  return (FLOATP (lipa_car (args)) || INTP (lipa_car (args))) ? 
    lisp_true : lisp_false;
}

LObject *
lisp_charp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("char? wants better arguments than that\n", stderr);
      return NULL;
    }

  return CHARP(lipa_car (args)) ? lisp_true : lisp_false;
}

/* boolean? is also in r5rs */

LObject *
lisp_booleanp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: argument error (as if you didn't know)", stderr);
      return NULL;
    }

  return (BOOLEANP (lipa_car (args)) ? lisp_true : lisp_false);
}

LObject *
lisp_symbolp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return SYMBOLP(lipa_car (args)) ? lisp_true : lisp_false;
}

LObject *
lisp_functionp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return FUNCTIONP(lipa_car (args)) ? lisp_true : lisp_false;
}

LObject *
lisp_subrp (LObject *args)
{
  LObject *obj = lipa_car (args);

  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return (FUNCTIONP(obj) && (L_FUNCTION (obj).functype == L_FUNC_SUBR))
    ? lisp_true : lisp_false;
}

LObject *
lisp_lambdap (LObject *args)
{
  LObject *obj = lipa_car (args);

  if (!lipa_list_length (args, 1))
    {
      fputs ("bah, some stupid error message\n", stderr);
      return NULL;
    }

  return (FUNCTIONP(obj) && (L_FUNCTION (obj).functype == L_FUNC_LAMBDA))
    ? lisp_true : lisp_false;
}

void
lipa_register_type_predicates (void)
{
  lipa_register_subr ("symbolname?", lisp_symbolnamep);
  lipa_register_subr ("list?", lisp_listp);
  lipa_register_subr ("string?", lisp_stringp);
  lipa_register_subr ("integer?", lisp_intp);    /* r5rs */
  lipa_register_subr ("real?", lisp_floatp);     /* r5rs */
  lipa_register_subr ("number?", lisp_numberp);
  lipa_register_subr ("char?", lisp_charp);
  lipa_register_subr ("boolean?", lisp_booleanp);
  lipa_register_subr ("symbol?", lisp_symbolp);
  lipa_register_subr ("procedure?", lisp_functionp); /* r5rs */
  lipa_register_subr ("subr?", lisp_subrp);
  lipa_register_subr ("lambda?", lisp_lambdap);
}
