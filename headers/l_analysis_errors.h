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

#ifndef L_ANALYSIS_ERRORS_H
#define L_ANALYSIS_ERRORS_H

#include "error.h"
#include "l_tokens_definitions.h"
#include "l_error.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    error **errors;
    int errors_number;
} l_analysis_errors;

l_analysis_errors *l_analysis_errors_create();

void l_analysis_errors_destroy(l_analysis_errors *ae);

bool l_analysis_errors_append(l_analysis_errors **ae, error *e);

bool l_analysis_errors_contains(l_analysis_errors *ae, error *e);

void l_analysis_errors_print(l_analysis_errors *ae, FILE *out);

#define ERROR_EXCEPTED(ctx, excepted) \
    if (ctx->current_token->unity == END) { \
        l_analysis_errors_append( \
            &ctx->ae, \
            l_error_create_excepted_at_eof( \
                ctx->current_function_name, \
                ctx->source_file->path_name, \
                ctx->current_line \
            ) \
        ); \
    } else { \
        l_analysis_errors_append( \
            &ctx->ae, \
            l_error_create_excepted_before( \
                ctx->current_function_name, \
                ctx->source_file->path_name, \
                ctx->current_line, \
                l_token_get_name_from(excepted), \
                ctx->current_token->word_name \
            ) \
        ); \
    } \

#define ERROR_EXCEPTED_BEFORE_EXPRESSION(ctx, excepted) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_excepted_before_expression( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            excepted \
        ) \
    ); \

#define ERROR_EXCEPTED_TWICE(ctx, excepted1, excepted2) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_excepted_twice_before( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            l_token_get_name_from(excepted1), \
            l_token_get_name_from(excepted2), \
            ctx->current_token->word_name \
        ) \
    ); \

#define ERROR_EXCEPTED_EXPRESSION(ctx) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_excepted_expression_before( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            ctx->current_token->word_name \
        ) \
    ); \

#define ERROR_EXCEPTED_ASSIGNMENT_OR_EXPRESSION_BEFORE(ctx, before) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_excepted_assignment_or_expression_before( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            before \
        ) \
    ); \

#define ERROR_UNDECLARED_VARIABLE(ctx, var_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_undeclared_variable( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            var_name \
        ) \
    ); \

#define ERROR_UNDECLARED_FUNCTION(ctx, func_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_undeclared_function( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            func_name \
        ) \
    ); \

#define ERROR_WRONG_ARRAY_DECLARATION(ctx) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_wrong_array_declaration( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line \
        ) \
    ); \

#define ERROR_UNDEFINED_MAIN(ctx) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_undefined_main( \
            ctx->source_file->path_name, \
            ctx->current_line \
        ) \
    ); \

#define ERROR_TOO_MANY_ARGS(ctx, func_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_too_many_args( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            func_name \
        ) \
    ); \

#define ERROR_TOO_FEW_ARGS(ctx, func_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_too_few_args( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            func_name \
        ) \
    ); \

#define ERROR_REDECLARED_VARIABLE(ctx, var_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_redeclared_variable( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            var_name \
        ) \
    ); \

#define WARNING_VARIABLE_GLOBAL_SCOPE(ctx, var_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_warning_varible_global_scope( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            var_name \
        ) \
    ); \

#define ERROR_FUNC_REDEFINITION(ctx, func_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_func_redefinition( \
            ctx->source_file->path_name, \
            ctx->current_line, \
            func_name \
        ) \
    ); \

#define ERROR_WRONG_ARRAY_ASSIGNMENT(ctx, var_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_wrong_array_assignment( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            var_name \
        ) \
    ); \

#define ERROR_WRONG_INTEGER_ASSIGNMENT(ctx, var_name) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_wrong_integer_assignment( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            var_name \
        ) \
    ); \

#define ERROR_WHILE_KEYWORD(ctx, before_identifier) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_excepted_while( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            before_identifier \
        ) \
    ); \

#define ERROR_UNCORRECT_STATEMENT(ctx, before_identifier) \
    l_analysis_errors_append( \
        &ctx->ae, \
        l_error_create_excpted_correct_statement( \
            ctx->current_function_name, \
            ctx->source_file->path_name, \
            ctx->current_line, \
            before_identifier \
        ) \
    ); \

#endif
