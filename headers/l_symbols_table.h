/*******************************************************************************
 * Copyright (C) 2019 Charly Lamothe, Stéphane Arcellier                      *
 *                                                                             *
 * This file is part of LCompiler.                                             *
 *                                                                             *
 *   Licensed under the Apache License, Version 2.0 (the "License");           *
 *   you may not use this file except in compliance with the License.          *
 *   You may obtain a copy of the License at                                   *
 *                                                                             *
 *   http://www.apache.org/licenses/LICENSE-2.0                                *
 *                                                                             *
 *   Unless required by applicable law or agreed to in writing, software       *
 *   distributed under the License is distributed on an "AS IS" BASIS,         *
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
 *   See the License for the specific language governing permissions and       *
 *   limitations under the License.                                            *
 *******************************************************************************/

#ifndef L_SYMBOLS_TABLE_H
#define L_SYMBOLS_TABLE_H

#include <stdio.h>

/* Maximum number of indentifiers (global + local in one function) */
#define MAX_IDENTIF 1000

/* Scope */
typedef enum {
    L_GLOBAL_SCOPE,
    L_LOCAL_SCOPE,
    L_ARGUMENT_SCOPE
} l_scope;

/* Types */
typedef enum {
    L_INTEGER_IDENTIFIER,
    L_TABLE_IDENTIFIER,
    L_FUNCTION_IDENTIFIER
} l_identifier_type;

/* Record describing an input of the symbol table */
typedef struct {
    char *name; /* Identifier name */
    l_scope current_scope; /* Possible values: GLOBAL_VARIABLE_SCOPE, LOCAL_VARIABLE_SCOPE, ARGUMENT_SCOPE */
    l_identifier_type type; /* Possible values: INTEGER_IDENTIFIER, TABLE_IDENTIFIER et FUNCTION_IDENTIFIER */
    int address; /* Shift from $fp or .data byte number */
    int complement; /* size of an array or argument number of a function */
} l_identifier;

typedef struct {
    l_identifier **identifiers;
    int max_identifiers;
    int current_identifier;
} l_symbols_table;

typedef struct {
    l_symbols_table *global_table;
    l_symbols_table *local_table;
    l_scope current_scope;
    int current_local_address;
    int current_argument_address;
} l_symbols_table_stream;

l_symbols_table_stream *l_symbols_table_stream_create();

/**
 * Function that switch global table to local table. Must be called when
 * we begin to browse through a function declaration, just before the argument
 * list.
 */
void l_symbols_table_function_begin(l_symbols_table_stream *stream);

/**
 * Function that switch local table to global table. Must be called when
 * we finish to browse through a function declaration, just after the
 * instruction block which contains the content of the function.
 */
void l_symbols_table_function_end(l_symbols_table_stream *stream);

/**
  * Add a new identificator to the current symbol table.
  * @param name Name of the new identifier (variable or function)
  * @param current_scope Constant among L_GLOBAL_SCOPE, L_ARGUMENT_SCOPE,
  *               L_LOCAL_SCOPE
  * @param type Constant among INTEGER_IDENTIFIER, TABLE_IDENTIFIER, FUNCTION_IDENTIFIER
  * @param address Offset byte number compared to the memory base area of
  *                the variables ($fp for locals/arguments, .data for
  *                globals).
  * @param complement Number of parameters of a function or number of cases
  *                   of an array. Undefined (0) when type=INTEGER_IDENTIFIER.
  */
void l_symbols_table_identifier_add(l_symbols_table_stream *stream, char *name, l_scope current_scope, l_identifier_type type, int address, int complement);

int l_symbols_table_search_local(l_symbols_table_stream *stream, char *name);

int l_symbols_table_search_global(l_symbols_table_stream *stream, char *name);

/**
 * Auxiliary function that allows to print the current content of the symbol
 * tables. Cet display must be conditioned on a boolean variable which control
 * if we want or not to display the symbol table in function of the options
 * specified to the compiler.
 */
void l_symbols_table_print(l_symbols_table_stream *stream, FILE *out);

void l_symbols_table_stream_destroy(l_symbols_table_stream *stream);

#endif
