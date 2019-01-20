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

#ifndef ERROR_H
#define ERROR_H

#include "bool.h"

#if defined(_WIN32) || defined(_WIN64)
    #include <windows.h>
#endif

typedef struct {
    char *description;
    char *func_name;
    char *file_name;
    int line_number;
    bool is_main_error;
} error;

#if defined(_WIN32) || defined(_WIN64)
    #define FORMAT_WERROR(error_buffer, code) \
        error_buffer = NULL; \
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
        NULL, \
        code, \
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
        (LPTSTR)&buffer_error, \
        0, \
        NULL); \

    #define GET_LAST_WERROR(error_buffer) FORMAT_WERROR(error_buffer, GetLastError())

    #define GET_LAST_WSA_ERROR(error_buffer) FORMAT_WERROR(error_buffer, WSAGetLastError())
#endif

error *error_create(char *description, char *func_name, char *file_name, int line_number);

void error_destroy(error *e);

bool error_equals(error *e1, error *e2);

#endif
