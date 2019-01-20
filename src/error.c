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
