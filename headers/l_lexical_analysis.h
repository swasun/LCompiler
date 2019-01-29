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
