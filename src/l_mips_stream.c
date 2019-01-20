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

#include "../headers/l_mips_stream.h"
#include "../headers/alloc.h"

l_mips_stream *l_mips_stream_create(const char *file_name) {
    l_mips_stream *stream;

    SAFE_ALLOC(stream, l_mips_stream, 1)
    stream->out = fopen(file_name, "w+");
    stream->current_register = 0;
    stream->else_counter = 0;
    stream->if_counter = 0;
    stream->while_counter = 0;
    stream->do_counter = 0;
    stream->return_value = -1;

    return stream;
}

void l_mips_stream_destroy(l_mips_stream *stream) {
    if (stream) {
        SAFE_FCLOSE(stream->out)
        SAFE_FREE(stream)
    }
}
