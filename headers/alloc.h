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

#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stacktrace.h"

/**
 * Safely alocate a variable.
 * Initialize the variable to NULL,
 * allocate the variable at size and cast it with
 * the specified type.
 * Initialize all the elements to 0.
 * Check if the variable is correctly allocated.
 */
#define SAFE_ALLOC(var, type, size) \
    var = NULL; \
    var = (type*)malloc(size * sizeof(type)); \
    memset(var, 0, size * sizeof(type)); \
    CHECK_ALLOC(var) \

#define SAFE_ALLOC_OR_GOTO(var, type, size, label) \
    var = NULL; \
    var = (type*)malloc(size * sizeof(type)); \
    memset(var, 0, size * sizeof(type)); \
    CHECK_ALLOC_OR_GOTO(var, label) \

/**
 * Safely reallocate a varaible.
 * If the size needs to be increase, we add additional size in
 * in the parameter more_size.
 * If the size needs to be decrease, the parameter more_size must be
 * equal to 0.
 * Initialize additional elements to 0.
 * Check if the variable is correctly reallocated.
 */
#define SAFE_REALLOC(var, type, old_size, more_size) \
    var = (type*)realloc(var, (old_size + more_size + 1) * sizeof(type)); \
    memset(var + old_size, 0, (more_size + 1) * sizeof(type)); \
    CHECK_ALLOC(var) \

/**
 * Check if the variable is correctly allocated.
 * Check if the errno variable is equal to ENOMEM, and an
 * error message to the stacktrace.
 * Some OS doesn't update errno. If it's the case, we also check
 * if the variable is NULL.
 * If not, we add an error message to the stacktrace.
 */
#define CHECK_ALLOC(var) \
    if (errno == ENOMEM) { \
        PUSH_STACK_ERRNO() \
        SAFE_FREE(var) \
        return 0; \
    } else if (!var) { \
        PUSH_STACK_MSG("No such memory to allocate") \
        SAFE_FREE(var) \
        return 0; \
    } \

#define CHECK_ALLOC_OR_GOTO(var, label) \
    if (errno == ENOMEM) { \
        PUSH_STACK_ERRNO() \
        SAFE_FREE(var) \
        goto label; \
    } else if (!var) { \
        PUSH_STACK_MSG("No such memory to allocate") \
        SAFE_FREE(var) \
        goto label; \
    } \

/**
 * Correctly deallocate a variable.
 * Check if the variable isn't NULL.
 * If not, deallocate the variable, then set the
 * ptr to NULL.
 */
#define SAFE_FREE(var) \
    if (var) { \
        free((void*)var); \
        var = NULL; \
    } \

/**
 * Correctly close a file descriptor.
 * Check if the descriptor of the file isn't NULL.
 * If not, close the descriptor and set the ptr to NULL.
 */
#define SAFE_FCLOSE(fd) \
    if (fd) { \
        fclose(fd); \
        fd = NULL; \
    } \

#endif
