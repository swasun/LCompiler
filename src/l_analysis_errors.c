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
