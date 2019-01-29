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

#include "../headers/error.h"

#include <string.h>
#include <stdlib.h>

error *error_create(char *description, char *func_name, char *file_name, int line_number) {
    error *e;
    int length;
    char *final_file_name;
    char slash;

    #ifdef __linux__
        slash = '/';
    #elif _WIN32
        slash = '\\';
    #else
        #error "OS not supported"
    #endif

    e = (error *)malloc(sizeof(error));

    length = strlen(description) + 1;
    e->description = (char *)malloc(length * sizeof(char));
    strcpy(e->description, description);

    if (func_name) {
        length = strlen(func_name) + 1;
        e->func_name = (char *)malloc(length * sizeof(char));
        strcpy(e->func_name, func_name);
    } else {
        length = 1;
        e->func_name = (char *)malloc(length * sizeof(char));
        strcpy(e->func_name, "");
    }

    if (strrchr(file_name, slash)) {
        final_file_name = strrchr(file_name, slash) + 1;
    } else {
        final_file_name = file_name;
    }

    length = strlen(final_file_name) + 1;
    e->file_name = (char *)malloc(length * sizeof(char));
    strcpy(e->file_name, final_file_name);

    e->line_number = line_number;
    e->is_main_error = false;

    return e;
}

void error_destroy(error *e) {
    if (e) {
        if (e->description) {
            free((void *)e->description);
        }

        if (e->func_name) {
            free((void *)e->func_name);
        }

        if (e->file_name) {
            free((void *)e->file_name);
        }

        free((void *)e);
        e = NULL;
    }
}

bool error_equals(error *e1, error *e2) {
    return e1 && e2 && ((e1 == e2) || (
        strcmp(e1->description, e2->description) == 0 &&
        strcmp(e1->func_name, e2->func_name) == 0 &&
        strcmp(e1->file_name, e2->file_name) == 0 &&
        e1->line_number == e2->line_number
    ));
}
