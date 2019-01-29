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

#ifndef L_TEST_MANAGER_H
#define L_TEST_MANAGER_H

#include "l_test.h"
#include "bool.h"

typedef struct {

	/* Test list to perform */
    l_test **tests;

	/* Number of tests to perform */
    int tests_number;

    float total_time;

} l_test_ctx;

l_test_ctx *l_test_manager_create_default();

l_test_ctx *l_test_manager_create_from_file(char *file_name);

l_test_ctx *l_test_manager_create_from_files(char **files_name, int files_number);

l_test_ctx *l_test_manager_create_from_dir(char *dir_name);

void l_test_manager_destroy(l_test_ctx *ctx);

void l_test_manager_dump_lex(l_test_ctx *ctx);

void l_test_manager_dump_synt(l_test_ctx *ctx);

void l_test_manager_dump_asynt(l_test_ctx *ctx);

void l_test_manager_dump_symb(l_test_ctx *ctx);

bool l_test_manager_process(l_test_ctx *ctx, FILE *out);

#endif
