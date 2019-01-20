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
