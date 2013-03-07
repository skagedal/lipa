/* chars.c - part of LIPA
 *
 * Procedures operating on characters
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
 * Created by Linus 1999-03-10
 */

#include <stdio.h>
#include <glib.h>
#include <math.h>
#include <ctype.h>
#include "basic.h"
#include "chars.h"
#include "list.h"

LObject *
lisp_char_equalp (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char=? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char=? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (L_CHAR (lipa_car (args)) != (L_CHAR (lipa_cadr (args))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_greaterp (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char>? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char>? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (L_CHAR (lipa_car (args)) <= (L_CHAR (lipa_cadr (args))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_smallerp (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char<? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char<? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (L_CHAR (lipa_car (args)) >= (L_CHAR (lipa_cadr (args))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_greater_or_equalp (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char>=? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char>=? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (L_CHAR (lipa_car (args)) < (L_CHAR (lipa_cadr (args))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_smaller_or_equalp (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char<=? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char<=? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (L_CHAR (lipa_car (args)) > (L_CHAR (lipa_cadr (args))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_equalp_ci (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char-ci=? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char-ci=? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (tolower (L_CHAR (lipa_car (args))) 
	       != (tolower (L_CHAR (lipa_cadr (args)))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_greaterp_ci (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char-ci>? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char-ci>? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (tolower (L_CHAR (lipa_car (args))) <= 
	       (tolower (L_CHAR (lipa_cadr (args)))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_smallerp_ci (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char-ci<? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char-ci<? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (tolower (L_CHAR (lipa_car (args))) >= 
	       (tolower (L_CHAR (lipa_cadr (args)))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_greater_or_equalp_ci (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char-ci>=? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char-ci>=? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (tolower (L_CHAR (lipa_car (args))) <
	       (tolower (L_CHAR (lipa_cadr (args)))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_smaller_or_equalp_ci (LObject *args)
{
  if (!lipa_list_at_least (args, 2) || !CHARP (lipa_car (args)))
    {
      fputs ("char-ci<=? wants at least two arguments\n", stderr);
      return NULL;
    }
  
  while (lipa_cdr (args))
    {
      if (!CHARP (lipa_cadr (args)))
	{
	  fputs ("char-ci<=? wants char arguments!\n", stderr);
	  return NULL;
	}
      else if (tolower (L_CHAR (lipa_car (args))) >
	       (tolower (L_CHAR (lipa_cadr (args)))))
	{
	  return lisp_false;
	}
      else
	args = (lipa_cdr (args));
    }

  return lisp_true;
}

LObject *
lisp_char_alphabeticp (LObject *args)
{
  if (!lipa_list_length (args, 1) || !CHARP (lipa_car (args)))
    {
      fputs ("char-alphabetic? wants 1 character argument\n", stderr);
      return NULL;
    }
  
  return (isalpha (L_CHAR (lipa_car (args))) ? lisp_true : lisp_false);
}

LObject *
lisp_char_numericp (LObject *args)
{
  if (!lipa_list_length (args, 1) || !CHARP (lipa_car (args)))
    {
      fputs ("char-numeric? wants 1 character argument\n", stderr);
      return NULL;
    }
  
  return (isdigit (L_CHAR (lipa_car (args))) ? lisp_true : lisp_false);
}

LObject *
lisp_char_whitespacep (LObject *args)
{
  if (!lipa_list_length (args, 1) || !CHARP (lipa_car (args)))
    {
      fputs ("char-whitespace? wants 1 character argument\n", stderr);
      return NULL;
    }
  
  return (isspace (L_CHAR (lipa_car (args))) ? lisp_true : lisp_false);
}

LObject *
lisp_char_upperp (LObject *args)
{
  if (!lipa_list_length (args, 1) || !CHARP (lipa_car (args)))
    {
      fputs ("char-upper? wants 1 character argument\n", stderr);
      return NULL;
    }
  
  return (isupper (L_CHAR (lipa_car (args))) ? lisp_true : lisp_false);
}

LObject *
lisp_char_lowerp (LObject *args)
{
  if (!lipa_list_length (args, 1) || !CHARP (lipa_car (args)))
    {
      fputs ("char-lower? wants 1 character argument\n", stderr);
      return NULL;
    }
  
  return (islower (L_CHAR (lipa_car (args))) ? lisp_true : lisp_false);
}

void
lipa_register_char_functions (void)
{
  lipa_register_subr ("char=?", lisp_char_equalp);
  lipa_register_subr ("char<?", lisp_char_smallerp);
  lipa_register_subr ("char>?", lisp_char_greaterp);
  lipa_register_subr ("char<=?", lisp_char_smaller_or_equalp);
  lipa_register_subr ("char>=?", lisp_char_greater_or_equalp);
  lipa_register_subr ("char-ci=?", lisp_char_equalp_ci);
  lipa_register_subr ("char-ci<?", lisp_char_smallerp_ci);
  lipa_register_subr ("char-ci>?", lisp_char_greaterp_ci);
  lipa_register_subr ("char-ci<=?", lisp_char_smaller_or_equalp_ci);
  lipa_register_subr ("char-ci>=?", lisp_char_greater_or_equalp_ci);
  lipa_register_subr ("char-alphabetic?", lisp_char_alphabeticp);
  lipa_register_subr ("char-numeric?", lisp_char_numericp);
  lipa_register_subr ("char-whitespace?", lisp_char_whitespacep);
  lipa_register_subr ("char-upper?", lisp_char_upperp);
  lipa_register_subr ("char-lower?", lisp_char_lowerp);
}
