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

#ifndef L_MIPS_STREAM_H
#define L_MIPS_STREAM_H

#include <stdio.h>

typedef struct {
    int current_register;
    FILE *out;
    int else_counter;
    int if_counter;
    int while_counter;
    int do_counter;
    int return_value;
} l_mips_stream;

l_mips_stream *l_mips_stream_create(const char *file_name);

void l_mips_stream_destroy(l_mips_stream *stream);

#endif
