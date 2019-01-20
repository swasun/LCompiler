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

#include "../headers/l_analysis_errors.h"
#include "../headers/alloc.h"

l_analysis_errors *l_analysis_errors_create() {
    l_analysis_errors *ae;

    SAFE_ALLOC(ae, l_analysis_errors, 1)

    return ae;
}

void l_analysis_errors_destroy(l_analysis_errors *ae) {
    int i;

    if (ae) {
        if (ae->errors) {
            for (i = 0; i < ae->errors_number; i++) {
                error_destroy(ae->errors[i]);
            }
            SAFE_FREE(ae->errors)
        }
        SAFE_FREE(ae)
    }
}

bool l_analysis_errors_append(l_analysis_errors **ae, error *e) {
    if (l_analysis_errors_contains(*ae, e)) {
        return true;
    }

    if (!(*ae)->errors) {
        SAFE_ALLOC((*ae)->errors, error *, 1)
        (*ae)->errors_number = 0;
    } else {
        SAFE_REALLOC((*ae)->errors, error *, (*ae)->errors_number, 1)
    }

    (*ae)->errors[(*ae)->errors_number] = e;
    (*ae)->errors_number++;

    return true;
}

bool l_analysis_errors_contains(l_analysis_errors *ae, error *e) {
    int i;

    for (i = 0; i < ae->errors_number; i++) {
        if (error_equals(ae->errors[i], e)) {
            return true;
        }
    }

    return false;
}

void l_analysis_errors_print(l_analysis_errors *ae, FILE *out) {
    int i;

    if (ae && ae->errors && out) {
        for (i = 0; i < ae->errors_number; i++) {
            l_error_print(ae->errors[i], out);
            fprintf(out, "\n");
        }
    }
}
