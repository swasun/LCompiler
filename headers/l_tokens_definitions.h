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

#ifndef L_TOKENS_DEFINITIONS_H
#define L_TOKENS_DEFINITIONS_H

#include "l_token.h"

/* Punctuation */
l_token *open_parenthesis_token;
l_token *close_parenthesis_token;
l_token *open_brace_token;
l_token *close_brace_token;
l_token *semicolon_token;
l_token *open_bracket_token;
l_token *close_bracket_token;
l_token *comma_token;

/* Operators */
l_token *equal_token;
l_token *plus_token;
l_token *sub_token;
l_token *star_token;
l_token *slash_token;
l_token *inferior_token;
l_token *superior_token;
l_token *not_token;
l_token *and_token;
l_token *or_token;

/* Control instructions (if, else, then, etc.) */
l_token *if_token;
l_token *then_token;
l_token *else_token;
l_token *while_token;
l_token *do_token;
l_token *return_token;

/* Variable types (integer) */
l_token *integer_token;

/* Known functions (read, write, main) */
l_token *read_func_token;
l_token *write_func_token;
l_token *main_func_token;

/* End of file token, returned when we reach END unit */
l_token *end_of_file_token;

/* Variable name token, that can start with a prefix (here $ symbol) */
l_token *variable_token;

/* File name token */
l_token *function_token;

/* Number token */
l_token *number_token;

/* Epsilon token */
l_token *epsilon_token;

/* Comment character (here #) */
char comment_token;

void l_tokens_init();

void l_tokens_destroy();

char *l_token_get_name_from(int unity);

#endif
