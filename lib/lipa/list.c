/* list.c - part of LIPA
 *
 * List functions
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
 * Created by Simon 1999-02-21
 */

#include <stdio.h>
#include <glib.h>
#include "basic.h"
#include "list.h"

/***********************************************************************
 * List primitives - use them everywhere
 *
 * `lipa_car' and `lipa_cdr' are "safe", car (NULL) returns NULL
 ***********************************************************************/

/* `lipa_cons' - for use i C */

LObject *
lipa_cons (LObject *car, LObject *cdr)
{
  LObject *obj = lipa_new_list ();

  L_LIST (obj).car = car;
  L_LIST (obj).cdr = cdr;

  return obj;
}

/* `lisp_cons' - for use in Lisp code */

LObject *
lisp_cons (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("you... you IDIOT!!!!  cons should have TWO ARGUMENTS!!!\n",
	     stderr);
      return NULL;
    }

  return lipa_cons (lipa_car (args), lipa_car (lipa_cdr (args)));
}

/* `lipa_car' - for use in C */

LObject *
lipa_car (LObject *list)
{
  if (list)
    return (L_LIST(list).car);
  else
    return NULL;
}

/* `lisp_car' */

LObject *
lisp_car (LObject *list)
{
  if (!lipa_list_length (list, 1) || !LISTP (lipa_car (list)))
    {
      fputs ("error! car wants one list argument.\n", stderr);
      return NULL;
    }

  return lipa_car (lipa_car (list));
}

/* `lipa_cdr' - for use in C*/

LObject *
lipa_cdr (LObject *list)
{
  if (list)
    return (L_LIST(list).cdr);
  else
    return NULL;
}

/* `lisp_cdr' */

LObject *
lisp_cdr (LObject *list)
{
  LObject *ret;

  if (!lipa_list_length (list, 1) || !LISTP (lipa_car (list)))
    {
      fputs ("error! cdr wants one argument.\n", stderr);
      return NULL;
    }
  
  ret = lipa_cdr (lipa_car (list));
  if (ret)
    return ret;
  else
    return NULL;
}

LObject *
lipa_cadr (LObject *list)
{
  return lipa_car (lipa_cdr (list));
}

LObject *
lipa_caddr (LObject *list)
{
  return lipa_car (lipa_cdr (lipa_cdr (list)));
}

/* `lipa_null' is for use from C code */

gboolean
lipa_null (LObject *list)
{
  return (!list);
}

/* `lisp_null' is for use from Lisp code, not from C code */

static LObject *
lisp_null (LObject *list)
{
  if (!lipa_list_length (list, 1) || !LISTP (lipa_car (list)))
    {
      fputs ("error! null? wants one list argument.\n", stderr);
      return NULL;
    }

  return (lipa_null (lipa_car (list)) ? lisp_true : lisp_false);
}

/* checks if LIST has exactly LENGTH members */

gboolean
lipa_list_length (LObject *list, gint length)
{
  gint i;

  /* too few? */
  for (i=0; i<length; i++)
    {
      if (!list)
	return FALSE;
      list = lipa_cdr (list);
    }
  
  /* list should by now be NULL.  otherwise it's too long. */

  return lipa_null (list);
}

/* checks if LIST has at least LENGTH members */

gboolean
lipa_list_at_least (LObject *list, gint length)
{
  gint i;

  for (i=0; i<length; i++)
    {
      if (!list)
	return FALSE;
      list = lipa_cdr(list);
    }

  return TRUE;
}

void
lipa_register_list_functions (void)
{
  lipa_register_subr ("cons", lisp_cons);
  lipa_register_subr ("car", lisp_car);
  lipa_register_subr ("cdr", lisp_cdr);
  lipa_register_subr ("null?", lisp_null);
}
