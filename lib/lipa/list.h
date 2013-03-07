/* list.h - part of LIPA
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

#include <glib.h>
#include "basic.h"

LObject *lipa_cons (LObject *car, LObject *cdr);
LObject *lipa_car (LObject *list);
LObject *lipa_cdr (LObject *list);
LObject *lipa_cadr (LObject *list);
LObject *lipa_caddr (LObject *list);
gboolean lipa_null (LObject *list);

gboolean lipa_list_length (LObject *list, gint length);
gboolean lipa_list_at_least (LObject *list, gint length);

void lipa_register_list_functions (void);
