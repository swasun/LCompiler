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
