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
