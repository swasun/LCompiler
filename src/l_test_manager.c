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

#include "../headers/l_test_manager.h"
#include "../headers/alloc.h"
#include "../headers/check_parameter.h"
#include "../headers/stacktrace.h"
#include "../headers/l_analysis.h"
#include "../headers/utils.h"

l_test_ctx *l_test_manager_create_default() {
    return NULL;
}

l_test_ctx *l_test_manager_create_from_file(char *file_name) {
    char **files_name;

    CHECK_PARAMETER_OR_RETURN(file_name)

    SAFE_ALLOC(files_name, char *, 1)
    files_name[0] = file_name;

    return l_test_manager_create_from_files(files_name, 1);
}

l_test_ctx *l_test_manager_create_from_files(char **files_name, int files_number) {
    l_test_ctx *ctx;
    int i;

    CHECK_PARAMETER_OR_RETURN(files_name)
    CHECK_PARAMETER_OR_RETURN(files_number > 0)

    SAFE_ALLOC(ctx, l_test_ctx, 1)

    SAFE_ALLOC(ctx->tests, l_test *, files_number)
    ctx->tests_number = files_number;
    ctx->total_time = 0.0;

    for (i = 0; i < files_number; i++) {
        if (files_name[i] != NULL) {
            ctx->tests[i] = l_test_create(l_source_file_create(files_name[i]));
        }
    }

    SAFE_FREE(files_name)

    return ctx;
}

l_test_ctx *l_test_manager_create_from_dir(char *dir_name) {
    l_test_ctx *ctx;
    char **files_name, **tmp_files_name;
    unsigned short files, tmp_files;
    int i;

    tmp_files = -1;

    tmp_files_name = list_directory(dir_name, &tmp_files, true);
    if (tmp_files == (unsigned short)-1) {
        PUSH_STACK_MSG("Specified directory doesn't contains files")
        return NULL;
    }

    files_name = NULL;
    files = 0;

    for (i = 0; i < tmp_files; i++) {
        if (strcmp(get_file_name_extension(tmp_files_name[i]), "l") == 0) {
            if (files_name) {
                SAFE_REALLOC(files_name, char *, files, 1)
            } else {
                SAFE_ALLOC(files_name, char *, 1)
            }
            files_name[files] = string_create_from(tmp_files_name[i]);
            files++;
        }
        SAFE_FREE(tmp_files_name[i])
    }
    SAFE_FREE(tmp_files_name)

    SAFE_ALLOC(ctx, l_test_ctx, 1)

    SAFE_ALLOC(ctx->tests, l_test *, files)
    ctx->tests_number = files;
    ctx->total_time = 0.0;

    for (i = 0; i < files; i++) {
        if (files_name[i] != NULL) {
            ctx->tests[i] = l_test_create(l_source_file_create(files_name[i]));
            SAFE_FREE(files_name[i])
        }
    }
    SAFE_FREE(files_name)

    return ctx;
}

void l_test_manager_destroy(l_test_ctx *ctx) {
    int i;

    if (ctx) {
        for (i = 0; i < ctx->tests_number; i++) {
            l_test_destroy(ctx->tests[i]);
        }
        SAFE_FREE(ctx->tests)
        SAFE_FREE(ctx)
    }
}

void l_test_manager_dump_lex(l_test_ctx *ctx) {
    int i;

    for (i = 0; i < ctx->tests_number; i++) {
        l_analysis_dump_lex(ctx->tests[i]->ctx);
    }
}

void l_test_manager_dump_synt(l_test_ctx *ctx) {
    int i;

    for (i = 0; i < ctx->tests_number; i++) {
        l_analysis_dump_synt(ctx->tests[i]->ctx);
    }
}

void l_test_manager_dump_asynt(l_test_ctx *ctx) {
    int i;

    for (i = 0; i < ctx->tests_number; i++) {
        l_analysis_dump_asynt(ctx->tests[i]->ctx);
    }
}

void l_test_manager_dump_symb(l_test_ctx *ctx) {
    int i;

    for (i = 0; i < ctx->tests_number; i++) {
        l_analysis_dump_symb(ctx->tests[i]->ctx);
    }
}

bool l_test_manager_process(l_test_ctx *ctx, FILE *out) {
    int i;
    bool passed;

    for (i = 0; i < ctx->tests_number; i++) {
        passed = l_test_execute(ctx->tests[i]);
        l_test_print(ctx->tests[i], out);
        if (!passed && stacktrace_is_filled()) {
            stacktrace_print(out);
        } else {
            ctx->total_time += ctx->tests[i]->total_time;
        }
    }

    fprintf(out, "Tests passed in %fs\n", ctx->total_time);
}
