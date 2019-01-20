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

#ifndef L_ANALYSIS_CTX_H
#define L_ANALYSIS_CTX_H

#include "l_analysis_errors.h"
#include "xml_writer.h"
#include "bool.h"
#include "l_mips_stream.h"
#include "l_token.h"
#include "l_symbols_table.h"
#include "l_source_file.h"

#include <stdio.h>
#include <stddef.h>

typedef struct {
    /* Source file to compile */
    l_source_file *source_file;

    /* List of lexical/syntactic/semantic errors detected during the compilation of the program */
    l_analysis_errors *ae;

    bool eof_state;

    /* Below the fields used to by the lexical analysis */

    /**
     * If a character isn't a punctuation symbol,
     * it is added to the buffer, which is empty after
     * being correctly analyzed.
     */
    char *current_buf;

    /* The current line, for error purpose */
    int current_line;

    /* By default 100 */
    size_t variable_max_size;
    size_t function_max_size;

    /* Persistence of the lexical analysis */
    bool dump_lex;
    FILE *lex_fd;

    /* Bellow the fields used by the syntactic/semantic analysis */

    l_token *current_token;
    l_token *previous_token;
    l_symbols_table_stream *symb_stream;
    bool dump_synt, dump_asynt, dump_symb;
    FILE *symb_fd;
    xml_writer *synt_writer;
    xml_writer *asynt_writer;
    char *current_function_name;

    /* Character stream used to transcript in MIPS assembly */
    l_mips_stream *mips_stream;

} l_analysis_ctx;

#endif
