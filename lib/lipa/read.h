/* read.h - part of LIPA
 *
 * Reading functions
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

typedef struct LGetc LGetc;

struct LGetc
{
  gpointer data;
  gint (*get) (LGetc *lgetc);
  void (*unget) (LGetc *lgetc, gint c);
  gboolean (*anyleft) (LGetc *lgetc);
};

LObject *lipa_read (guchar *s);

LObject *lipa_read_general (GString *s);
