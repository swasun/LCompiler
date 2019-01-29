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

#ifndef L_ANALYSIS_H
#define L_ANALYSIS_H

#include "l_analysis_ctx.h"
#include "l_source_file.h"

bool l_analysis_create_from_path(l_analysis_ctx **ctx, const char *source_file_path_name);

bool l_analysis_create(l_analysis_ctx **ctx, l_source_file *source_file);

void l_analysis_destroy(l_analysis_ctx *ctx);

bool l_analysis_dump_lex(l_analysis_ctx *ctx);

bool l_analysis_dump_synt(l_analysis_ctx *ctx);

bool l_analysis_dump_asynt(l_analysis_ctx *ctx);

bool l_analysis_dump_symb(l_analysis_ctx *ctx);

bool l_analysis_process(l_analysis_ctx *ctx);

void l_analysis_print_errors(l_analysis_ctx *ctx, FILE *out);

int l_analysis_get_errors_number(l_analysis_ctx *ctx);

#endif
