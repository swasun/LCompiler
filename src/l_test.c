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

#include "../headers/l_test.h"
#include "../headers/alloc.h"
#include "../headers/check_parameter.h"
#include "../headers/l_analysis.h"
#include "../headers/utils.h"

#include <time.h>

l_test *l_test_create(l_source_file *source_file) {
    l_test *test;

    CHECK_PARAMETER_OR_RETURN(source_file)
    CHECK_PARAMETER_OR_RETURN(source_file->name)
    CHECK_PARAMETER_OR_RETURN(source_file->path_name)

    SAFE_ALLOC(test, l_test, 1)
    test->source_file = source_file;
    l_analysis_create(&test->ctx, source_file);
    test->passed = false;

    return test;
}

void l_test_destroy(l_test *test) {
    if (test) {
        l_analysis_destroy(test->ctx);
        SAFE_FREE(test)
    }
}

bool l_test_execute(l_test *test) {
    clock_t begin, end;

    CHECK_PARAMETER_OR_RETURN(test)

    begin = clock();
    if (l_analysis_process(test->ctx)) {
        test->passed = true;
    }
    end = clock();
    test->total_time = ((float)(end - begin) / CLOCKS_PER_SEC);    

    test->passed = test->ctx->ae->errors_number == 0;

    return test->passed;
}

void l_test_print(l_test *test, FILE *out) {
    if (test) {
        if (test->passed) {
            fprintf(out, "[PASSED] - '%s' in %fs\n\n", test->source_file->path_name, test->total_time);
        } else {
            fprintf(out, "[FAILED] - '%s'\n", test->source_file->path_name);
            if (l_analysis_get_errors_number(test->ctx) > 0) {
                l_analysis_errors_print(test->ctx->ae, out);
            }
            fprintf(out, "\n");
        }
    }
}
