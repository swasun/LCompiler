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
