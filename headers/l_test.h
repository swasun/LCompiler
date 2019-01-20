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

#ifndef L_TEST_H
#define L_TEST_H

#include "l_source_file.h"
#include "l_analysis_ctx.h"
#include "bool.h"

#include <stdio.h>

typedef struct {

    /* File path */
    l_source_file *source_file;

    /* Context structure of the file compilation */
    l_analysis_ctx *ctx;

    /* At the true specify that the test had succeed */
    bool passed;

    float total_time;

} l_test;

l_test *l_test_create(l_source_file *source_file);

void l_test_destroy(l_test *test);

bool l_test_execute(l_test *test);

void l_test_print(l_test *test, FILE *out);

#endif
