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

#ifndef L_LEXICAL_ANALYSIS_H
#define L_LEXICAL_ANALYSIS_H

#include <stdio.h>
#include <stddef.h>

#include "l_token.h"
#include "bool.h"
#include "l_analysis_ctx.h"

bool l_lexical_analysis_init(l_analysis_ctx **ctx);

void l_lexical_analysis_uninit(l_analysis_ctx *ctx);

/* Set the maximum size of a variable name */
void l_lexical_analysis_set_variable_max_size(l_analysis_ctx *ctx, size_t size);

/* Set the maximum size of the function name */
void l_lexical_analysis_set_function_max_size(l_analysis_ctx *ctx, size_t size);

/**
 * A each call, returned the next token, until the end token.
 * Must be called in a while() loop, until the ending file state isn't EOF_STATE.
 */
l_token *l_lexical_analysis_next_token(l_analysis_ctx *ctx);

#endif
