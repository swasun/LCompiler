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

#include "../headers/l_source_file.h"
#include "../headers/alloc.h"
#include "../headers/check_parameter.h"
#include "../headers/utils.h"
#include "../headers/stacktrace.h"

l_source_file *l_source_file_create(const char *path_name) {
    l_source_file *file;

    CHECK_PARAMETER_OR_RETURN(path_name)

    if (!is_file_exists(path_name)) {
        PUSH_STACK(FILE_NOT_FOUND)
        return NULL;
    }

    SAFE_ALLOC(file, l_source_file, 1)

    file->path_name = string_create_from((char *)path_name);
    file->name = get_file_name_from_path((char *)path_name);
    file->fd = fopen(path_name, "r");

    return file;
}

void l_source_file_destroy(l_source_file *file) {
    if (file) {
        SAFE_FREE(file->path_name)
        SAFE_FREE(file->name)
        SAFE_FCLOSE(file->fd)
        SAFE_FREE(file)
    }
}
