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

#include "../headers/l_error.h"

#include <string.h>
#include <stdlib.h>

error *l_error_create_undeclared_variable(char *func_name, char *file_name, int line_number, char *var_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: '") + strlen(var_name) + strlen("' undeclared (first use in this function)");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: '%s' undeclared (first use in this function)",
        var_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: implicit declaration of function 'func' */
error *l_error_create_undeclared_function(char *func_name, char *file_name, int line_number, char *undeclared_func_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: implicit declaration of function '") + strlen(undeclared_func_name) + strlen("'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: implicit declaration of function '%s'",
        undeclared_func_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: too many arguments to function 'func' */
error *l_error_create_too_many_args(char *func_name, char *file_name, int line_number, char *called_function_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: too many arguments to function '") + strlen(called_function_name) + strlen("'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: too many arguments to function '%s'",
        called_function_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: too few arguments to function 'func' */
error *l_error_create_too_few_args(char *func_name, char *file_name, int line_number, char *called_function_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: too few arguments to function '") + strlen(called_function_name) + strlen("'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: too few arguments to function '%s'",
        called_function_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: excepted ';' before 'return' */
error *l_error_create_excepted_before(char *func_name, char *file_name, int line_number, char *excepted_identifier, char *before_identifier) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted '") + strlen(excepted_identifier) + strlen("' before ") + strlen(before_identifier) + strlen("' token");
    description = (char *)malloc((size + 2) * sizeof(char));
    sprintf(
        description, 
        "error: excepted '%s' before '%s' token",
        excepted_identifier,
        before_identifier
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: excepted '=' or '<' before '5' */
error *l_error_create_excepted_twice_before(char *func_name, char *file_name, int line_number, char *excepted_identifier1, char *excepted_identifier2, char *before_identifier) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted '") + strlen(excepted_identifier1) + strlen("' or '") + strlen(excepted_identifier2) + strlen("' before ") + strlen(before_identifier) + strlen("' token");
    description = (char *)malloc((size + 2) * sizeof(char));
    sprintf(
        description, 
        "error: excepted '%s' or '%s' before '%s' token",
        excepted_identifier1,
        excepted_identifier2,
        before_identifier
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: excepted expression before ')' */
error *l_error_create_excepted_expression_before(char *func_name, char *file_name, int line_number, char *before_identifier) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted expression before ") + strlen(before_identifier) + strlen("' token");
    description = (char *)malloc((size + 2) * sizeof(char));
    sprintf(
        description, 
        "error: excepted expression before '%s' token",
        before_identifier
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;    
}

/* errors: excepted '=' before expression */
error *l_error_create_excepted_before_expression(char *func_name, char *file_name, int line_number, char *excepted_identifier) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted '") + strlen(excepted_identifier) + strlen("' before expression");
    description = (char *)malloc((size + 2) * sizeof(char));
    sprintf(
        description, 
        "error: excepted '%s' before expression",
        excepted_identifier
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;    
}

/* error: excepted '=', ',', ';' or expression before '+' token */
error *l_error_create_excepted_assignment_or_expression_before(char *func_name, char *file_name, int line_number, char *before_identifier) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted '=', ',', ';' or expression before ") + strlen(before_identifier) + strlen("' token");
    description = (char *)malloc((size + 2) * sizeof(char));
    sprintf(
        description, 
        "error: excepted '=', ',', ';' or expression before '%s' token",
        before_identifier
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;        
}

/* error: excepted declaration or statement at end of input */
error *l_error_create_excepted_at_eof(char *func_name, char *file_name, int line_number) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted declaration or statement at end of input");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: excepted declaration or statement at end of input"
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;        
}

/* error: wrong assignment to array 'var' */
error *l_error_create_wrong_array_assignment(char *func_name, char *file_name, int line_number, char *variable_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: wrong assignment to array '") + strlen(variable_name) + strlen("'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: wrong assignment to array '%s'",
        variable_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: wrong assignment to integer 'var' */
error *l_error_create_wrong_integer_assignment(char *func_name, char *file_name, int line_number, char *variable_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: wrong assignment to integer '") + strlen(variable_name) + strlen("'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: wrong assignment to integer '%s'",
        variable_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* warning: variable '$a' already declared in global scope */
error *l_error_create_warning_varible_global_scope(char *func_name, char *file_name, int line_number, char *declared_variable_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("warning: variable '") + strlen(declared_variable_name) + strlen("' already declared in global scope");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "warning: variable '%s' already declared in global scope",
        declared_variable_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: undefined reference to 'main' */
error *l_error_create_undefined_main(char *file_name, int line_number) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: undefined reference to 'main'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: undefined reference to 'main'"
    );

    e = error_create(description, "", file_name, line_number);

    free((void *)description);

    return e;
}

/* error: redefinition of 'func' */
error *l_error_create_func_redefinition(char *file_name, int line_number, char *redefine_func_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: redefinition of '") + strlen(redefine_func_name) + strlen("'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: redefinition of '%s'",
        redefine_func_name
    );

    e = error_create(description, "", file_name, line_number);

    free((void *)description);

    return e;
}

/* error: array cannot be declared in local scope */
error *l_error_create_wrong_array_declaration(char *func_name, char *file_name, int line_number) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: array cannot be declared in local scope");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: array cannot be declared in local scope"
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: variable 'a' already declared */
error *l_error_create_redeclared_variable(char *func_name, char *file_name, int line_number, char *redeclared_variable_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: variable '") + strlen(redeclared_variable_name) + strlen("' already declared");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: variable '%s' already declared",
        redeclared_variable_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: unknown type name 'float' */
error *l_error_create_unknown_type_name(char *func_name, char *file_name, int line_number, char *type_name) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: unknown type name '") + strlen(type_name) + strlen("'");
    description = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        description, 
        "error: unknown type name '%s'",
        type_name
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: excepted while keyword and a valid expression before 'do' token */
error *l_error_create_excepted_while(char *func_name, char *file_name, int line_number, char *before_identifier) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted while keyword before '") + strlen(before_identifier) + strlen("' token");
    description = (char *)malloc((size + 2) * sizeof(char));
    sprintf(
        description, 
        "error: excepted while keyword before '%s' token",
        before_identifier
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

/* error: excepted correct statement before '' token */
error *l_error_create_excpted_correct_statement(char *func_name, char *file_name, int line_number, char *before_identifier) {
    error *e;
    size_t size;
    char *description;

    size = strlen("error: excepted correct statement before '") + strlen(before_identifier) + strlen("' token");
    description = (char *)malloc((size + 2) * sizeof(char));
    sprintf(
        description, 
        "error: excepted correct statement before '%s' token",
        before_identifier
    );

    e = error_create(description, func_name, file_name, line_number);

    free((void *)description);

    return e;
}

char *l_error_to_string(error *e) {
    size_t size, prefix_size;
    char *error_buffer, *line_number_buffer, *prefix;

    error_buffer = NULL;
    line_number_buffer = NULL;
    prefix = NULL;

    line_number_buffer = (char*)malloc(10 * sizeof(char));
    sprintf(line_number_buffer, "%d", e->line_number);

    size = strlen(e->file_name) + strlen(":") + strlen(line_number_buffer) + strlen(": ");
    size += strlen(e->description);

    if (e->is_main_error) {
        prefix_size = strlen(e->file_name) + strlen(": In function '") + strlen(e->func_name) + strlen("':\n");
        size += prefix_size;
        prefix = (char *)malloc((prefix_size + 1) * sizeof(char));
        sprintf(
            prefix,
            "%s: In function '%s':\n",
            e->file_name,
            e->func_name
        );
    } else {
        prefix = (char *)malloc(sizeof(char));
        strcpy(prefix, "");
    }
    free((void *)line_number_buffer);

    error_buffer = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        error_buffer, 
        "%s%s:%d: %s",
        prefix,
        e->file_name,
        e->line_number, 
        e->description
    );

    free((void*)prefix);

    return error_buffer;
}

void l_error_print(error *e, FILE *out) {
    char *error_buffer;

    error_buffer = l_error_to_string(e);
    
    fprintf(out, "%s\n", error_buffer);

    free((void*)error_buffer);
}
