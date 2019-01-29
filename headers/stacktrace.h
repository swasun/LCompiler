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

#ifndef STRACKTRACE_H
#define STRACKTRACE_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "error.h"
#include "internal_error.h"
#include "bool.h"
#include "thread_storage.h"
#include "stacktrace_struct.h"

void stacktrace_create(stacktrace **stack);

void stacktrace_destroy(stacktrace *stack);

void push_to_stacktrace(stacktrace *stack, error *e);

char *stacktrace_to_string(stacktrace *stack);

void stacktrace_print();

void stacktrace_print_this(stacktrace *stack);

void stacktrace_print_fd(FILE *fd);

void stacktrace_print_fd_this(stacktrace *stack, FILE *fd);

char *stacktrace_get_cause();

char *stacktrace_get_cause_this(stacktrace *stack);

bool stacktrace_is_filled_this(stacktrace *stack);

bool stacktrace_is_filled();

#define PUSH_STACK(code) \
    char *description; \
    description = internal_error_get_description(code); \
    push_to_stacktrace(thread_storage_get_stacktrace(), error_create(description, (char *)__func__, __FILE__, __LINE__)); \
    free((void*)description); \

#define PUSH_STACK_ERRNO() \
    char *description; \
    if (errno == 0) { \
        description = internal_error_get_description(UNKNOWN_ERROR); \
        push_to_stacktrace(thread_storage_get_stacktrace(), error_create(description, (char *)__func__, __FILE__, __LINE__)); \
        free((void*)description); \
    } else { \
        push_to_stacktrace(thread_storage_get_stacktrace(), error_create(strerror(errno), (char *)__func__, __FILE__, __LINE__)); \
    } \

#define PUSH_STACK_MSG(msg) \
    push_to_stacktrace(thread_storage_get_stacktrace(), error_create(msg, (char *)__func__, __FILE__, __LINE__)); \

#endif
