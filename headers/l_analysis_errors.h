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
