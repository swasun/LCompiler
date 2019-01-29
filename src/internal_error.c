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

#include "../headers/internal_error.h"

#include <stdlib.h>
#include <string.h>

char *internal_error_get_description(internal_error_type type) {
    size_t size;
    char *description;

    size = 50;
    description = (char *)malloc(size * sizeof(char));

    switch (type) {
        case SUCCESS:
            strncpy(description, "No error detected", size);
        break;

        case FILE_NOT_FOUND:
            strncpy(description, "File not found", size);
        break;

        case NO_SUCH_MEMORY:
            strncpy(description, "No such memory available", size);
        break;

        case INVALID_PARAMETER:
            strncpy(description, "Specified parameter is invalid", size);
        break;

        case UNKNOWN_ERROR:
            strncpy(description, "Unknown error", size);
        break;

        default:
            strncpy(description, "Unknown error type", size);
    }

    description = (char *)realloc(description, (strlen(description) + 1) * sizeof(char));

    return description;
}

char *internal_error_to_string(error *e) {
    size_t size;
    char *error_buffer, *line_number_buffer, *prefix;

    error_buffer = NULL;
    line_number_buffer = NULL;
    prefix = NULL;

    line_number_buffer = (char *)malloc(10 * sizeof(char));
    sprintf(line_number_buffer, "%d", e->line_number);

    size = strlen("   at  (:)");
    size += strlen(e->func_name);
    size += strlen(e->file_name);
    size += strlen(line_number_buffer);
    free((void*)line_number_buffer);

    if (e->is_main_error) {
        size += strlen("Caused by: \n");
        size += strlen(e->description);
        prefix = (char *)malloc((strlen("Caused by: \n") + strlen(e->description) + 1) * sizeof(char));
        sprintf(prefix, "Caused by: %s\n", e->description);
    } else {
        prefix = (char *)malloc(sizeof(char));
        strcpy(prefix, "");
    }

    error_buffer = (char *)malloc((size + 1) * sizeof(char));
    sprintf(
        error_buffer, 
        "%s   at %s (%s:%d)",
        prefix,
        e->func_name,
        e->file_name, 
        e->line_number
    );

    free((void*)prefix);

    return error_buffer;
}

void internal_error_print(error *e, FILE *out) {
    char *error_buffer;

    error_buffer = internal_error_to_string(e);
    
    fprintf(out, "%s\n", error_buffer);

    free((void*)error_buffer);
}
