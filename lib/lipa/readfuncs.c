/* readfuncs.c - part of LIPA
 *
 * Lisp interface to reading functions
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
 * Created by Simon 1999-02-23
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <glib.h>
#include "list.h"
#include "basic.h"
#include "read.h"
#include "eval.h"

static LObject *
lisp_read (LObject *args)
{
  if (!lipa_list_length (args, 1))
    {
      fputs ("read says: I just want one argument!\n", stderr);
      return NULL;
    }

  return lipa_eval (lipa_read (L_STRING (lipa_car (args))->str));
}

void
lipa_register_read_functions (void)
{
  lipa_register_subr ("read", lisp_read);
}

