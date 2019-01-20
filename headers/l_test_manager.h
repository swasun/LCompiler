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
