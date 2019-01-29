/*************************************************************************************
 * MIT License                                                                       *
 *                                                                                   *
 * Copyright (C) 2016 Charly Lamothe, Stéphane Arcellier                             *
 *                                                                                   *
 * This file is part of LCompiler.                                                   *
 *                                                                                   *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy    *
 *   of this software and associated documentation files (the "Software"), to deal   *
 *   in the Software without restriction, including without limitation the rights    *
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       *
 *   copies of the Software, and to permit persons to whom the Software is           *
 *   furnished to do so, subject to the following conditions:                        *
 *                                                                                   *
 *   The above copyright notice and this permission notice shall be included in all  *
 *   copies or substantial portions of the Software.                                 *
 *                                                                                   *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
 *   SOFTWARE.                                                                       *
 *************************************************************************************/

#ifndef L_ERROR_H
#define L_ERROR_H

#include "error.h"

#include <stdio.h>

error *l_error_create_undeclared_variable(char *func_name, char *file_name, int line_number, char *undeclared_var_name);

/* error: implicit declaration of function 'func' */
error *l_error_create_undeclared_function(char *func_name, char *file_name, int line_number, char *undeclared_func_name);

/* error: too many arguments to function 'func' */
error *l_error_create_too_many_args(char *func_name, char *file_name, int line_number, char *called_function_name);

/* error: too few arguments to function 'func' */
error *l_error_create_too_few_args(char *func_name, char *file_name, int line_number, char *called_function_name);

/* error: excepted ';' before 'return' */
error *l_error_create_excepted_before(char *func_name, char *file_name, int line_number, char *excepted_identifier, char *before_identifier);

/* error: excepted '=' or '<' before '5' */
error *l_error_create_excepted_twice_before(char *func_name, char *file_name, int line_number, char *excepted_identifier1, char *excepted_identifier2, char *before_identifier);

/* error: excepted expression before ')' */
error *l_error_create_excepted_expression_before(char *func_name, char *file_name, int line_number, char *before_identifier);

/* errors: excepted '=' before expression */
error *l_error_create_excepted_before_expression(char *func_name, char *file_name, int line_number, char *excepted_identifier);

/* error: excepted '=', ',', ';' or expression before '+' token */
error *l_error_create_excepted_assignment_or_expression_before(char *func_name, char *file_name, int line_number, char *before_identifier);

/* error: excepted declaration or statement at end of input */
error *l_error_create_excepted_at_eof(char *func_name, char *file_name, int line_number);

/* error: wrong assignment to array 'var' */
error *l_error_create_wrong_array_assignment(char *func_name, char *file_name, int line_number, char *variable_name);

/* error: wrong assignment to integer 'var' */
error *l_error_create_wrong_integer_assignment(char *func_name, char *file_name, int line_number, char *variable_name);

/* warning: variable '$a' already declared in global scope */
error *l_error_create_warning_varible_global_scope(char *func_name, char *file_name, int line_number, char *declared_variable_name);

/* error: undefined reference to 'main' */
error *l_error_create_undefined_main(char *file_name, int line_number);

/* error: redefinition of 'func' */
error *l_error_create_func_redefinition(char *file_name, int line_number, char *redefine_func_name);

/* error: array cannot be declared in local scope */
error *l_error_create_wrong_array_declaration(char *func_name, char *file_name, int line_number);

/* error: variable 'a' already declared */
error *l_error_create_redeclared_variable(char *func_name, char *file_name, int line_number, char *redeclared_variable_name);

/* error: unknown type name 'float' */
error *l_error_create_unknown_type_name(char *func_name, char *file_name, int line_number, char *type_name);

/* error: excepted while keyword and a valid expression before 'do' token */
error *l_error_create_excepted_while(char *func_name, char *file_name, int line_number, char *before_identifier);

/* error: excepted correct statement before '' token */
error *l_error_create_excpted_correct_statement(char *func_name, char *file_name, int line_number, char *before_identifier);

char *l_error_to_string(error *e);

void l_error_print(error *e, FILE *out);

#endif
