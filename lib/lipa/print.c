/* print.c - part of LIPA
 *
 * Print functions
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
 * Created by Simon 1999-03-04 - split off of lispparser.c
 */

#include <stdio.h>
#include <glib.h>
#include <string.h>
#include "basic.h"
#include "list.h"
#include "print.h"

/***********************************************************************
 * Printing
 ***********************************************************************/

void
lipa_print_list (LObject *lst)
{
  if (!lipa_null (lst))
    {
      lipa_print (lipa_car (lst));
      if (!lipa_null (lipa_cdr (lst)))
	{
	  fputc (' ', stdout);
	  lipa_print_list (lipa_cdr (lst));
	}
    }
}

void
lipa_print (LObject *obj)
{
  gchar *str; /* Baaaaaaaaaaaaaaaaad Linus! */ /* why? */
  gint c = 0;
  if (obj)
    {
      switch (obj->type)
	{
	case L_OBJ_LIST:
	  fputc ('(', stdout);
	  lipa_print_list (obj);
	  fputc (')', stdout);
	  break;

	case L_OBJ_SYMBOLNAME:
	  fputs (L_SYMBOLNAME(obj)->str, stdout);
	  break;

	case L_OBJ_STRING:
	  printf ("\"");
	  str = strdup(L_STRING(obj)->str);

	  while (str && *str)
	    {
	      c = *str;
	      str++;
	      switch (c)
		{
		case '\a':
		  printf ("\\a");
		  break;
		case '\b':
		  printf ("\\b");
		  break;
		case '\f':
		  printf ("\\f");
		  break;
		case '\n':
		  printf ("\\n");
		  break;
		case '\r':
		  printf ("\\r");
		  break;
		case '\t':
		  printf ("\\t");
		  break;
		case '\v':
		  printf ("\\v");
		  break;
		case '\\':
		  printf ("\\\\");
		  break;
		case '\?':
		  printf ("\\?");
		  break;
		case '\'':
		  printf ("\\'");
		  break;
		case '\"':
		  printf ("\\\"");
		  break;
		default:
		  printf ("%c", c);
		}
	    }
	  printf ("\"");
	  break;

	case L_OBJ_INT:
	  printf ("%d", L_INT(obj));
	  break;

	case L_OBJ_FLOAT:
	  printf ("%f", L_FLOAT(obj));
	  break;

	case L_OBJ_CHAR:
	  printf ("#\\%c", L_CHAR(obj));
	  break;

	case L_OBJ_SYMBOL:
	  fputs (L_SYMBOL(obj).name->str, stdout);
	  break;

	case L_OBJ_TRUE:
	  fputs ("<#t>", stdout);
	  break;

	case L_OBJ_FALSE:
	  fputs ("<#f>", stdout);
	  break;

	case L_OBJ_FUNCTION:
	  fputs ("<function>", stdout);
	  break;

	case L_OBJ_USEROBJECT:
	  fputs ("<userobject>", stdout);
	  break;
	  
	}
    }
  else
    fputs ("<nil>", stdout);	/* or "nil" ? */
}
