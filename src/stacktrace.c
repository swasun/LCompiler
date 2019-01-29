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

#include "../headers/stacktrace.h"

#include <string.h>

#define MAX_STACK_SIZE 10

void stacktrace_create(stacktrace **stack) {
    unsigned short i;

    (*stack) = (stacktrace *)malloc(sizeof(stacktrace));
    if (errno == ENOMEM || !(*stack)) {
        free((void*)(*stack));
        return;
    }
    memset((*stack), 0, sizeof(stacktrace));


    (*stack)->errors = (error **)malloc(MAX_STACK_SIZE * sizeof(error *));
    memset((*stack)->errors, 0, MAX_STACK_SIZE * sizeof(error *));
    (*stack)->elements = 0;
}

void stacktrace_destroy(stacktrace *stack) {
    unsigned short i;

    if (stack) {
        if (stack->errors) {
            for (i = 0; i < MAX_STACK_SIZE; i++) {
                error_destroy(stack->errors[i]);
            }
            free((void *)stack->errors);
        }
        free((void*)stack);    
    }
}

void push_to_stacktrace(stacktrace *stack, error *e) {
    if (!stack) {
        /*fprintf(stderr, "Failed to push error to current stack at thread: %lu\n", get_current_thread_id());*/
        return;
    }

    if (stack->elements == MAX_STACK_SIZE) {
        return;
    }

    stack->errors[stack->elements] = e;

    stack->elements++;
}

char *stacktrace_to_string(stacktrace *stack) {
    int size;
    unsigned short i;
    char *stacktrace_buffer, *error_buffer;

    if (stack->elements == 0) {
        return NULL;
    }

	/* Display the most important error in top of the stacktrace display */
    stack->errors[0]->is_main_error = true;

    size = 0;
    size = strlen(stack->errors[stack->elements - 1]->description);
    size += strlen("\n");
    for (i = 0; i < stack->elements; i++) {
        error_buffer = internal_error_to_string(stack->errors[i]);
        size += strlen(error_buffer);
        size += strlen("\n");
        free((void*)error_buffer);
    }

    stacktrace_buffer = (char*)malloc((size + 1) * sizeof(char));
    if (errno == ENOMEM || !stacktrace_buffer) {
        free((void*)stacktrace_buffer);
        return NULL;
    }
    sprintf(stacktrace_buffer, "%s\n",
        stack->errors[stack->elements - 1]->description);
    for (i = 0; i < stack->elements; i++) {
        error_buffer = internal_error_to_string(stack->errors[i]);
        strcat(stacktrace_buffer, error_buffer);
        strcat(stacktrace_buffer, "\n");
        free((void*)error_buffer);
    }

    return stacktrace_buffer;
}

void stacktrace_print_this(stacktrace *stack) {
    if (!stack) {
        return;
    }

    stacktrace_print_fd_this(stack, stderr);
}

void stacktrace_print() {
    stacktrace_print_this(thread_storage_get_stacktrace());
}

void stacktrace_print_fd_this(stacktrace *stack, FILE *fd) {
    char *stacktrace_buffer;
    
    if (!stack) {
        return;
    }

    stacktrace_buffer = stacktrace_to_string(stack);
    if (stacktrace_buffer) {
        fprintf(fd, "%s", stacktrace_buffer);
        free((void*)stacktrace_buffer);
    }
}

void stacktrace_print_fd(FILE *fd) {
    stacktrace_print_fd_this(thread_storage_get_stacktrace(), fd);
}

char *stacktrace_get_cause_this(stacktrace *stack) {
    if (stack->elements == 0) {
        return NULL;
    }

    return stack->errors[0]->description;
}

char *stacktrace_get_cause() {
    return stacktrace_get_cause_this(thread_storage_get_stacktrace());
}

bool stacktrace_is_filled_this(stacktrace *stack) {
    return stack->elements > 0 ? true : false;
}

bool stacktrace_is_filled() {
    return stacktrace_is_filled_this(thread_storage_get_stacktrace());
}
