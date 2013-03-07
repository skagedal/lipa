/* evalprocs.c - part of LIPA
 *
 * Evaluation - procedures exported to the Lisp environment
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
 * Created by Simon 1999-03-13
 */

#include <stdio.h>
#include "basic.h"
#include "list.h"
#include "eval.h"
#include "evalprocs.h"

static LObject *
lisp_eval (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("eval just wants one argument\n", stderr);
      return NULL;
    }

  return lipa_eval (lipa_car (args));
}

void
lipa_register_eval_functions (void)
{
  lipa_register_subr ("eval", lisp_eval);
}
