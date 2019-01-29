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
