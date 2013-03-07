/* numbers.c - part of LIPA
 *
 * Procedures operating on numbers
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
#include <math.h>
#include "basic.h"
#include "numbers.h"
#include "list.h"

/* TODO: typecheck... */

static LObject *
lisp_plus (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  LObject *obj_f = lipa_new_float (0.0);

  while (!lipa_null (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("+ wants numerical arguments\n", stderr);
	  return NULL;
	}
      if (FLOATP (lipa_car (args)))
	L_FLOAT (obj_f) += L_FLOAT (lipa_car (args));
      else 
	L_FLOAT (obj_f) += L_INT (lipa_car (args));
      args = lipa_cdr (args);
    }

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  else
    return obj_f;
}

/*
 * FIXME!!  Might have added more typechecks than necessary here.
 */

static LObject *
lisp_minus (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  LObject *obj_f = lipa_new_float (0.0);

  if (!lipa_list_at_least (args, 1))
    {
      fputs ("error!! - must have at least one argument!!\n", stderr);
      return NULL;
    }

  /* With one arg, negate it.  With more than one arg, substract all
     but the first from the first.  */

  if (lipa_list_length (args, 1))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("- wants numerical arguments\n", stderr);
	  return NULL;
	}
      if (FLOATP (lipa_car (args)))
	L_FLOAT (obj_f) -= L_FLOAT (lipa_car (args));
      else
	L_FLOAT (obj_f) -= L_INT (lipa_car (args));
    }
  else
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("- wants numerical arguments\n", stderr);
	  return NULL;
	}
      if (FLOATP (lipa_car (args)))
	L_FLOAT (obj_f) = L_FLOAT (lipa_car (args));
      else
	L_FLOAT (obj_f) = L_INT (lipa_car (args));
    }

  args = lipa_cdr (args);
  while (!lipa_null (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("- wants numerical arguments\n", stderr);
	  return NULL;
	}
      if (FLOATP (lipa_car (args)))
	L_FLOAT (obj_f) -= L_FLOAT (lipa_car (args));
      else
	L_FLOAT (obj_f) -= L_INT (lipa_car (args));
      args = lipa_cdr (args);
    }

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

static LObject *
lisp_multiply (LObject *args)
{
  LObject *obj = lipa_new_int (1);
  LObject *obj_f = lipa_new_float (1.0);

  while (!lipa_null (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("* wants numerical arguments\n", stderr);
	  return NULL;
	}
      if (FLOATP (lipa_car (args)))
	L_FLOAT (obj_f) *= L_FLOAT (lipa_car (args));
      else
	L_FLOAT (obj_f) *= L_INT (lipa_car (args));
      args = lipa_cdr (args);
    }

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

/*
 * FIXME!!  Might have added more typechecks than necessary here.
 */

static LObject *
lisp_divide (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  LObject *obj_f = lipa_new_float (0.0);

  if (!lipa_list_at_least (args, 2))
    {
      fputs ("error!! / must have at least two arguments!!\n", stderr);
      return NULL;
    }

  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("/ wants numerical arguments\n", stderr);
      return NULL;
    }
  
  if (FLOATP (lipa_car (args)))
    L_FLOAT (obj_f) = L_FLOAT (lipa_car (args));
  else
    L_FLOAT (obj_f) = L_INT (lipa_car (args));

  args = lipa_cdr (args);
  while (!lipa_null (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("/ wants numerical arguments\n", stderr);
	  return NULL;
	}
      if (FLOATP (lipa_car (args)))
	{
	  if (L_FLOAT (lipa_car (args)) == 0.0)
	    {
	      fputs ("Bad stuff: division by zero!!\n", stderr);
	      return NULL;
	    }
	  L_FLOAT (obj_f) /= L_FLOAT (lipa_car (args));
	}
      else
	{
	  if (L_INT (lipa_car (args)) == 0)
	    {
	      fputs ("Bad stuff: division by zero!!\n", stderr);
	      return NULL;
	    }
	  L_FLOAT (obj_f) /= L_INT (lipa_car (args));
	}
      args = lipa_cdr (args);
    }

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

static LObject *
lisp_max (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  LObject *obj_f = lipa_new_float (0.0);

  if (!lipa_list_at_least (args, 2))
    {
      fputs ("Error: must have at least 2 arguments!\n", stderr);
      return NULL;
    }

  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("max wants numerical arguments\n", stderr);
      return NULL;
    }

  while (!lipa_null (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("max wants numerical arguments\n", stderr);
	  return NULL;
	}

      if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
	   L_INT (lipa_car (args))) > L_FLOAT (obj_f))
	{
	L_FLOAT (obj_f) = (FLOATP (lipa_car (args)) ? 
			   L_FLOAT (lipa_car (args)) : 
			   L_INT (lipa_car (args)));
	}
      args = lipa_cdr (args);
    }

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

static LObject *
lisp_min (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  LObject *obj_f = lipa_new_float (0.0);

  if (!lipa_list_at_least (args, 2))
    {
      fputs ("Error: must have at least 2 arguments!\n", stderr);
      return NULL;
    }

  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("min wants numerical arguments\n", stderr);
      return NULL;
    }

  if (FLOATP (lipa_car (args)))
    L_FLOAT (obj_f) = L_FLOAT (lipa_car (args));
  else
    L_FLOAT (obj_f) = L_INT (lipa_car (args));

  while (!lipa_null (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("min wants numerical arguments\n", stderr);
	  return NULL;
	}

      if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) 
	   : L_INT (lipa_car (args))) < L_FLOAT (obj_f))
	{
	  L_FLOAT (obj_f) = (FLOATP (lipa_car (args)) ? 
			     L_FLOAT (lipa_car (args)) : 
			     L_INT (lipa_car (args)));
	}
      args = lipa_cdr (args);
    }

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

static LObject *
lisp_abs (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  LObject *obj_f = lipa_new_float (0.0);

  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: abs wants exactly one argument\n", stderr);
      return NULL;
    }

  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("abs wants numerical arguments\n", stderr);
      return NULL;
    }
  if (FLOATP (lipa_car (args)))
    L_FLOAT (obj_f) = L_FLOAT (lipa_car (args));
  else
    L_FLOAT (obj_f) = L_INT (lipa_car (args));

  if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
       L_INT (lipa_car (args))) < 0)
    {
      L_FLOAT (obj_f) = - (FLOATP (lipa_car (args)) ? 
			   L_FLOAT (lipa_car (args)) : 
			   L_INT (lipa_car (args)));
    }
 
  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

static LObject *
lisp_gcd (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  gint x, y, temp;

  if (!lipa_list_length (args, 2))
    {
      fputs ("Error: gcd wants exactly two arguments\n", stderr);
      return NULL;
    }
  if (!(INTP (lipa_car (args)) && INTP (lipa_car (lipa_cdr (args)))))
    {
      fputs ("Error: gcd wants integer arguments\n", stderr);
      return NULL;
    }

  x = L_INT (lipa_car (args));
  y = L_INT (lipa_car (lipa_cdr (args)));

  x = (x > 0 ? x : -x);
  y = (y > 0 ? y : -y);
  
  while (y != 0)
    {
      temp = x % y;
      x = y;
      y = temp;
    }

  L_INT (obj) = x;

  return obj;
}

static LObject *
lisp_floor (LObject *args)
{
  LObject *obj = lipa_new_int (0);

  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: floor wants exactly one argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("Error: floor wants a number as argument\n", stderr);
      return NULL;
    }

  L_INT (obj) = (gint)floor(FLOATP (lipa_car (args)) ? 
			    L_FLOAT (lipa_car (args)) :
			    (gfloat)L_INT (lipa_car (args)));

  return (obj);
}

static LObject *
lisp_ceiling (LObject *args)
{
  LObject *obj = lipa_new_int (0);

  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: ceiling wants exactly one argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("Error: ceiling wants a number as argument\n", stderr);
      return NULL;
    }

  L_INT (obj) = (gint)ceil(FLOATP (lipa_car (args)) ? 
			   L_FLOAT (lipa_car (args)) :
			   (gfloat)L_INT (lipa_car (args)));

  return (obj);
}

static LObject *
lisp_truncate (LObject *args)
{
  LObject *obj = lipa_new_int (0);

  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: truncate wants exactly one argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("Error: truncate wants a number as argument\n", stderr);
      return NULL;
    }

  L_INT (obj) = (gint)(FLOATP (lipa_car (args)) ? 
		       L_FLOAT (lipa_car (args)) :
		       (gfloat)L_INT (lipa_car (args)));

  return (obj);
}


static LObject *
lisp_round (LObject *args)
{
  LObject *obj = lipa_new_int (0);

  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: round wants exactly one argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("Error: round wants a number as argument\n", stderr);
      return NULL;
    }

  L_INT (obj) = (gint)rint(FLOATP (lipa_car (args)) ? 
			   L_FLOAT (lipa_car (args)) :
			   (gfloat)L_INT (lipa_car (args)));

  return (obj);
}  

static LObject *
lisp_modulo (LObject *args)
{
  LObject *obj = NULL;

  if (!lipa_list_length (args, 2))
    {
      fputs ("error!! mod must have exactly two arguments!!\n", stderr);
      return NULL;
    }

  if (!(INTP (lipa_car (args))) || !(INTP (lipa_car (lipa_cdr (args)))))
    {
      fputs ("Error: modulo is an integer operator!\n", stderr);
      return NULL;
    }

  obj = lipa_new_int (L_INT (lipa_car (args)) %
		      L_INT (lipa_car (lipa_cdr (args))));

  return obj;
}

static LObject *
lisp_expt (LObject *args)
{
  LObject *obj = lipa_new_int (0);
  LObject *obj_f = lipa_new_float (0.0);

  if (!lipa_list_length (args, 2))
    {
      fputs ("segmentation fault (core dumped).  No, but expt needs 2 args\n",
             stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)) || !NUMBERP (lipa_cadr (args)))
    {
      fputs ("expt wants numerical arguments!\n", stderr);
      return NULL;
    }

  L_FLOAT (obj_f) = pow (FLOATP (lipa_car (args)) ? 
			 L_FLOAT (lipa_car (args)) :
			 L_INT (lipa_car (args)),
			 FLOATP (lipa_car (lipa_cdr (args))) ?
			 L_FLOAT (lipa_car (lipa_cdr (args))) :
			 L_INT (lipa_car (lipa_cdr (args))));

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}

static LObject *
lisp_sqrt (LObject *args)
{
  LObject *obj_f = lipa_new_float (0.0);
  LObject *obj = lipa_new_int (0);

  if (!lipa_list_length (args, 1))
    {
      fputs ("Error: sqrt wants exactly one argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("Error: sqrt wants a number as argument\n", stderr);
      return NULL;
    }
  if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) :
       L_INT (lipa_car (args))) < 0)
    {
      fputs ("BUG: lipa currently does not support complex numbers\n",
	     stderr);
      return NULL;
    }

  L_FLOAT (obj_f) = sqrt(FLOATP (lipa_car (args)) ? 
			 L_FLOAT (lipa_car (args)) :
			 (gfloat)L_INT (lipa_car (args)));

  if (floor (L_FLOAT (obj_f)) == L_FLOAT (obj_f))
    {
      L_INT (obj) = (gint)(L_FLOAT (obj_f));
      return obj;
    }
  return obj_f;
}  

static LObject *
lisp_less_than (LObject *args)
{
  gboolean boolo = TRUE;

  if (!lipa_list_at_least (args, 2))
    {
      fputs ("error!! < must have at least two arguments!!\n", stderr);
      return NULL;
    }

  while (lipa_cdr (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("< wants numerical arguments!\n", stderr);
	  return NULL;
	}

      if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
	   L_INT (lipa_car (args))) <
	  (FLOATP (lipa_car (lipa_cdr (args))) ? 
	   L_FLOAT (lipa_car (lipa_cdr (args))) :
	   L_INT (lipa_car (lipa_cdr (args)))))
	boolo = TRUE;
      else
	{
	  boolo = FALSE;
	  break;
	}
      args = lipa_cdr (args);
    }
  if (boolo)
    return lisp_true;
  else
    return lisp_false;
}

static LObject *
lisp_less_than_or_equal (LObject *args)
{
  gboolean boolo = TRUE;

  if (!lipa_list_at_least (args, 2))
    {
      fputs ("error!! <= must have at least two arguments!!\n", stderr);
      return NULL;
    }

  while (lipa_cdr (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("<= wants numerical arguments!\n", stderr);
	  return NULL;
	}
      if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
	   L_INT (lipa_car (args))) <=
	  (FLOATP (lipa_car (lipa_cdr (args))) ? 
	   L_FLOAT (lipa_car (lipa_cdr (args))) :
	   L_INT (lipa_car (lipa_cdr (args)))))
	{
	  boolo = TRUE;
	}
      else
	{
	  boolo = FALSE;
	  break;
	}
      args = lipa_cdr (args);
    }
  if (boolo)
    return lisp_true;
  else
    return lisp_false;
}

static LObject *
lisp_more_than_or_equal (LObject *args)
{
  gboolean boolo = TRUE;

  if (!lipa_list_at_least (args, 2))
    {
      fputs ("error!! >= must have at least two arguments!!\n", stderr);
      return NULL;
    }

  while (lipa_cdr (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs (">= wants numerical arguments!\n", stderr);
	  return NULL;
	}

      if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
	   L_INT (lipa_car (args))) >=
	  (FLOATP (lipa_car (lipa_cdr (args))) ? 
	   L_FLOAT (lipa_car (lipa_cdr (args))) :
	   L_INT (lipa_car (lipa_cdr (args)))))
	{
	  boolo = TRUE;
	}
      else
	{
	  boolo = FALSE;
	  break;
	}
      args = lipa_cdr (args);
    }
  if (boolo)
    return lisp_true;
  else
    return lisp_false;
}

static LObject *
lisp_more_than (LObject *args)
{
  gboolean boolo = TRUE;

  if (!lipa_list_at_least (args, 2))
    {
      fputs ("error!! > must have at least two arguments!!\n", stderr);
      return NULL;
    }

  while (lipa_cdr (args))
    {
      if (!NUMBERP (lipa_car (args)))
	{
	  fputs ("> wants numerical arguments!\n", stderr);
	  return NULL;
	}
      if ((FLOATP (lipa_car (args)) ? 
	   L_FLOAT (lipa_car (args)) : L_INT (lipa_car (args))) >
	  (FLOATP (lipa_car (lipa_cdr (args))) ? 
	   L_FLOAT (lipa_car (lipa_cdr (args))) :
	   L_INT (lipa_car (lipa_cdr (args)))))
	{
	  boolo = TRUE;
	}
      else
	{
	  boolo = FALSE;
	  break;
	}
      args = lipa_cdr (args);
    }
  if (boolo)
    return lisp_true;
  else
    return lisp_false;
}

static LObject *
lisp_equal (LObject *args)
{
  if (!lipa_list_length (args, 2))
    {
      fputs ("error!! = must have exactly two arguments!!\n", stderr);
      return NULL;
    }

  if (!NUMBERP (lipa_car (args)) || !NUMBERP (lipa_cadr (args)))
    {
      fputs ("= wants numerical arguments!\n", stderr);
      return NULL;
    }


  if ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
       L_INT (lipa_car (args))) ==
      (FLOATP (lipa_car (lipa_cdr (args))) ? 
       L_FLOAT (lipa_car (lipa_cdr (args))) :
       L_INT (lipa_car (lipa_cdr (args)))))
    return lisp_true;
  else
    return lisp_false;
}

/*
 * A bunch of predicates
 */

static LObject *
lisp_zerop (LObject *args)
{
  LObject *obj = NULL;

  if (!lipa_list_length (args, 1))
    {
      fputs ("zero? wants 1 argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("zero? wants numerical arguments!\n", stderr);
      return NULL;
    }

  obj = lipa_car (args);

  return (L_FLOAT (lipa_car (args)) == 0 ? lisp_true : lisp_false);
}

LObject *
lisp_positivep (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("positive? wants 1 argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("positive? wants numerical arguments!\n", stderr);
      return NULL;
    }
  
  return ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
	   L_INT (lipa_car (args)) 
	  >= 0) ? lisp_true : lisp_false);
}

static LObject *
lisp_negativep (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("negative? wants 1 argument\n", stderr);
      return NULL;
    }
  if (!NUMBERP (lipa_car (args)))
    {
      fputs ("negative? wants numerical arguments!\n", stderr);
      return NULL;
    }

  return ((FLOATP (lipa_car (args)) ? L_FLOAT (lipa_car (args)) : 
	   L_INT (lipa_car (args)) 
	  < 0) ? lisp_true : lisp_false);
}

static LObject *
lisp_oddp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("odd? wants 1 argument\n", stderr);
      return NULL;
    }
  if (!(INTP (lipa_car (args))))
    {
      fputs ("odd? is an integer predicate\n", stderr);
      return NULL;
    }

  return (((L_INT (lipa_car (args)) % 2) != 0) ? lisp_true : lisp_false);
}  

static LObject *
lisp_evenp (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("even? wants 1 argument\n", stderr);
      return NULL;
    }
  if (!INTP (lipa_car (args)))
    {
      fputs ("even? is an integer predicate\n", stderr);
      return NULL;
    }

  return (((L_INT (lipa_car (args)) % 2) == 0) ? lisp_true : lisp_false);
}  

void
lipa_register_number_functions (void)
{
  lipa_register_subr ("+", lisp_plus);
  lipa_register_subr ("-", lisp_minus);
  lipa_register_subr ("*", lisp_multiply);
  lipa_register_subr ("/", lisp_divide);
  lipa_register_subr ("modulo", lisp_modulo);
  lipa_register_subr ("<", lisp_less_than);
  lipa_register_subr (">", lisp_more_than);
  lipa_register_subr (">=", lisp_more_than_or_equal);
  lipa_register_subr ("<=", lisp_less_than_or_equal);
  lipa_register_subr ("=", lisp_equal);
  lipa_register_subr ("expt", lisp_expt);
  lipa_register_subr ("min", lisp_min);
  lipa_register_subr ("max", lisp_max);
  lipa_register_subr ("abs", lisp_abs);
  lipa_register_subr ("gcd", lisp_gcd);
  lipa_register_subr ("floor", lisp_floor);
  lipa_register_subr ("ceiling", lisp_ceiling);
  lipa_register_subr ("truncate", lisp_truncate);
  lipa_register_subr ("round", lisp_round);
  lipa_register_subr ("sqrt", lisp_sqrt);
  lipa_register_subr ("zero?", lisp_zerop);
  lipa_register_subr ("positive?", lisp_positivep);
  lipa_register_subr ("negative?", lisp_negativep);
  lipa_register_subr ("odd?", lisp_oddp);
  lipa_register_subr ("even?", lisp_evenp);
}
