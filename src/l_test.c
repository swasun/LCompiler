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
