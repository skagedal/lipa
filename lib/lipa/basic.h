/* basic.h - part of LIPA
 *
 * Basic LIPA stuff
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
 * Created by simon 1999-02-19
 */

#ifndef _LISPPARSER_H_
#define _LISPPARSER_H_

#include <glib.h>

#define L_SYMBOLNAME(obj)		((*obj).as.SymbolName.s)
#define L_SYMBOL(obj)			((*obj).as.Symbol)
#define L_LIST(obj)			((*obj).as.Cell)
#define L_STRING(obj)			((*obj).as.String.s)
#define L_INT(obj)			((*obj).as.Int.i)
#define L_FLOAT(obj)                    ((*obj).as.Float.i)
#define L_NUMBER(obj)			(INTP(obj)?L_INT(obj):L_FLOAT(obj))
#define L_CHAR(obj)                     ((*obj).as.Char.c)
#define L_FUNCTION(obj)			((*obj).as.Function)
#define L_USEROBJECT(obj)		((*obj).as.Userobject.data)

typedef enum LObjectType	LObjectType;
typedef enum LFunctionType	LFunctionType;
typedef struct LObject		LObject;

typedef LObject* (LFunctionFunc) (LObject *args);

enum LObjectType
{
  L_OBJ_SYMBOLNAME,		/* data is a (GString*) */
  L_OBJ_LIST,			/* data is a (LCell*) */
  L_OBJ_STRING,			/* data is a (GString*) */
  L_OBJ_INT,			/* data is a gint (use GINT_TO_POINTER) */
  L_OBJ_FLOAT,                  /* data is a gfloat */
  L_OBJ_CHAR,                   /* data is a guchar */
  L_OBJ_SYMBOL,			/* data is a (LSymbol*) */
  L_OBJ_FUNCTION,		/* data is a (LFunction*) */
  L_OBJ_USEROBJECT,		/* data is a gpointer */
  L_OBJ_TRUE,			/* the special #t object */
  L_OBJ_FALSE			/* the special #f object */
};

#define TYPEP(obj,tp)		((obj)->type == (tp))
#define SYMBOLNAMEP(obj)	TYPEP((obj), L_OBJ_SYMBOLNAME)
#define LISTP(obj)		(!(obj) || TYPEP((obj), L_OBJ_LIST))
#define STRINGP(obj)		TYPEP((obj), L_OBJ_STRING)
#define INTP(obj)		TYPEP((obj), L_OBJ_INT)
#define FLOATP(obj)             TYPEP((obj), L_OBJ_FLOAT)
#define NUMBERP(obj)		(INTP(obj) || FLOATP(obj))
#define CHARP(obj)              TYPEP((obj), L_OBJ_CHAR)
#define SYMBOLP(obj)		TYPEP((obj), L_OBJ_SYMBOL)
#define FUNCTIONP(obj)		TYPEP((obj), L_OBJ_FUNCTION)
#define USEROBJECTP(obj)	TYPEP((obj), L_OBJ_USEROBJECT)
#define BOOLEANP(obj)  (TYPEP((obj), L_OBJ_TRUE) || TYPEP((obj), L_OBJ_FALSE))

enum LFunctionType
{
  L_FUNC_UNSPECIFIED,
  L_FUNC_SUBR,			/* internal C function */
  L_FUNC_LAMBDA			/* Lisp function */
};

struct LObject
{
  LObjectType type;
  union
  {
    struct 
    {
      GString *s;
    } SymbolName;
    struct 
    {
      GString *name;
      LObject *value;
    } Symbol;
    struct 
    {
      LObject *car;
      LObject *cdr;
    } Cell;
    struct
    {
      GString *s;
    } String;
    struct
    {
      gint i;
    } Int;
    struct
    {
      gfloat i;
    } Float;
    struct
    {
      guchar c;
    } Char;
    struct
    {
      LFunctionType functype;
      LFunctionFunc *subr;		/* If functype == L_FUNC_SUBR */
      LObject *arguments;		/* If functype == L_FUNC_LAMBDA */
      LObject *body;		/* If functype == L_FUNC_LAMBDA */
    } Function;
    struct
    {
      gpointer data;
    } Userobject;
  } as;
};

int lipa_init (void);
LObject *lipa_new_object (LObjectType type);
LObject *lipa_new_symbolname ();
LObject *lipa_new_symbol (gchar *name, LObject *value);
LObject *lipa_new_list ();
LObject *lipa_new_string ();
LObject *lipa_new_int (gint);
LObject *lipa_new_float (gfloat);
LObject *lipa_new_char (guchar);
LObject *lipa_new_function ();
LObject *lipa_new_function_lambda (LObject *args, LObject *body);
LObject *lipa_new_userobject (gpointer userdata);

void lipa_register_subr (gchar *name, LFunctionFunc *func);
int lipa_symbol_complete (char *text, int state);

extern LObject *lipa_symbols;

extern LObject *lisp_nil;
extern LObject *lisp_true;
extern LObject *lisp_false;

#endif /* _LISPPARSER_H_ */
