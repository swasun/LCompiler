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

#include "../headers/l_analysis.h"
#include "../headers/stacktrace.h"
#include "../headers/error.h"
#include "../headers/alloc.h"
#include "../headers/l_tokens_definitions.h"
#include "../headers/check_parameter.h"
#include "../headers/l_lexical_analysis.h"
#include "../headers/l_parser.h"
#include "../headers/utils.h"

#include <stdlib.h>

bool l_analysis_create_from_path(l_analysis_ctx **ctx, const char *source_file_path_name) {
    return l_analysis_create(ctx, l_source_file_create(source_file_path_name));
}

bool l_analysis_create(l_analysis_ctx **ctx, l_source_file *source_file) {
    CHECK_PARAMETER_OR_RETURN(source_file)
    CHECK_PARAMETER_OR_RETURN(source_file->path_name)
    CHECK_PARAMETER_OR_RETURN(source_file->name)
    CHECK_PARAMETER_OR_RETURN(source_file->fd)

    SAFE_ALLOC((*ctx), l_analysis_ctx, 1)

    (*ctx)->source_file = source_file;

    (*ctx)->eof_state = false;

    if (!l_lexical_analysis_init(&(*ctx))) {
        PUSH_STACK_MSG("Failed to init lexical l_analysis")
        goto clean_up;
    }

    if (!l_parser_init(&(*ctx))) {
        PUSH_STACK_MSG("Failed to init syntactic l_analysis")
        goto clean_up;
    }

    (*ctx)->ae = l_analysis_errors_create();

    l_tokens_init();
    atexit(l_tokens_destroy);

    return true;

clean_up:
    l_lexical_analysis_uninit((*ctx));
    l_parser_uninit((*ctx));
    SAFE_FREE((*ctx))
    return false;
}

void l_analysis_destroy(l_analysis_ctx *ctx) {
    if (!ctx) {
        return;
    }

    l_source_file_destroy(ctx->source_file);

    l_lexical_analysis_uninit(ctx);

    l_parser_uninit(ctx);

    l_analysis_errors_destroy(ctx->ae);

    SAFE_FREE(ctx)

    l_tokens_destroy();
}

bool l_analysis_dump_lex(l_analysis_ctx *ctx) {
    char *dump_file_name;

    CHECK_PARAMETER_OR_RETURN(ctx->source_file)

    ctx->dump_lex = true;
    dump_file_name = create_dump_file_name(ctx->source_file->path_name, "lex");
    ctx->lex_fd = fopen(dump_file_name, "w+");
    SAFE_FREE(dump_file_name)
    
    return true;
}

bool l_analysis_dump_synt(l_analysis_ctx *ctx) {
    char *dump_file_name;

    CHECK_PARAMETER_OR_RETURN(ctx->source_file)

    ctx->dump_synt = true;
    dump_file_name = create_dump_file_name(ctx->source_file->path_name, "synt");
    ctx->synt_writer = xml_writer_create(dump_file_name);
    SAFE_FREE(dump_file_name)

    return true;
}

bool l_analysis_dump_asynt(l_analysis_ctx *ctx) {
    char *dump_file_name;

    CHECK_PARAMETER_OR_RETURN(ctx->source_file)

    ctx->dump_asynt = true;
    dump_file_name = create_dump_file_name(ctx->source_file->path_name, "asynt");
    ctx->asynt_writer = xml_writer_create(dump_file_name);
    SAFE_FREE(dump_file_name)
    
    return true;
}

bool l_analysis_dump_symb(l_analysis_ctx *ctx) {
    char *dump_file_name;

    CHECK_PARAMETER_OR_RETURN(ctx->source_file)

    ctx->dump_symb = true;
    dump_file_name = create_dump_file_name(ctx->source_file->path_name, "symb");
    ctx->symb_fd = fopen(dump_file_name, "w+");
    SAFE_FREE(dump_file_name)
    
    return true;
}

bool l_analysis_process(l_analysis_ctx *ctx) {
    return l_parser_process(ctx);
}

int l_analysis_get_errors_number(l_analysis_ctx *ctx) {
    return ctx->ae->errors_number;
}

void l_analysis_print_errors(l_analysis_ctx *ctx, FILE *out) {
    l_analysis_errors_print(ctx->ae, out);
}
