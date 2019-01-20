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
