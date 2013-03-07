/* eval.h - part of LIPA
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

LObject *lipa_symbolname_lookup (gchar *name);
void lipa_register_symbol (LObject *sym);
LObject *lipa_symbol (gchar *name);
LObject *lipa_define (gchar *name, LObject *value);

LObject *lipa_apply_func (LObject *function, LObject *args);

LObject *lipa_eval_list (LObject *list);
LObject *lipa_eval (LObject *obj);



