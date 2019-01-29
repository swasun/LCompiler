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

#include "../headers/l_symbols_table.h"
#include "../headers/utils.h"
#include "../headers/stacktrace.h"
#include "../headers/alloc.h"

#include <string.h>
#include <stdlib.h>

static bool identifier_add(l_symbols_table *st, char *name, l_scope s, l_identifier_type type, int address, int complement) {
    if (st->current_identifier == st->max_identifiers) {
        SAFE_REALLOC(st->identifiers, l_identifier *, st->max_identifiers, st->max_identifiers + MAX_IDENTIF)
        st->max_identifiers += MAX_IDENTIF;
    }

    SAFE_ALLOC(st->identifiers[st->current_identifier], l_identifier, 1)
    st->identifiers[st->current_identifier]->name = string_create_from(name);
    st->identifiers[st->current_identifier]->current_scope = s;
    st->identifiers[st->current_identifier]->type = type;
    st->identifiers[st->current_identifier]->address = address;
    st->identifiers[st->current_identifier]->complement = complement;
    st->current_identifier++;
}

static void identifier_destroy(l_identifier *id) {
    if (id) {
        SAFE_FREE(id->name)
        SAFE_FREE(id)
    }
}

static l_symbols_table *l_symbols_table_create() {
    l_symbols_table *st;

    SAFE_ALLOC(st, l_symbols_table, 1)
    SAFE_ALLOC(st->identifiers, l_identifier *, MAX_IDENTIF)
    st->max_identifiers = MAX_IDENTIF;
    st->current_identifier = 0;

    return st;
}

static void l_symbols_table_destroy(l_symbols_table *st) {
    int i;

    if (st) {
        for (i = 0; i < st->current_identifier; i++) {
            identifier_destroy(st->identifiers[i]);
        }
        SAFE_FREE(st->identifiers)
        SAFE_FREE(st)
    }
}

static void clear_local_table(l_symbols_table_stream *stream) {
    int i;

    for (i = 0; i < stream->local_table->current_identifier; i++) {
        identifier_destroy(stream->local_table->identifiers[i]);
    }
    stream->local_table->current_identifier = 0;
}

l_symbols_table_stream *l_symbols_table_stream_create() {
    l_symbols_table_stream *stream;

    SAFE_ALLOC(stream, l_symbols_table_stream, 1)
    stream->global_table = l_symbols_table_create();
    stream->local_table = l_symbols_table_create();
    stream->current_scope = L_GLOBAL_SCOPE;
    stream->current_local_address = 0;
    stream->current_argument_address = 0;

    return stream;
}

void l_symbols_table_stream_destroy(l_symbols_table_stream *stream) {
    int i;

    if (stream) {
        l_symbols_table_destroy(stream->local_table);
        l_symbols_table_destroy(stream->global_table);
        SAFE_FREE(stream)
    }
}

void l_symbols_table_function_begin(l_symbols_table_stream *stream) {
    stream->current_scope = L_LOCAL_SCOPE;
    stream->current_local_address = 0;
    stream->current_argument_address = 0;
}

void l_symbols_table_function_end(l_symbols_table_stream *stream) {
    stream->current_scope = L_GLOBAL_SCOPE;
    clear_local_table(stream); 
}

void l_symbols_table_identifier_add(l_symbols_table_stream *stream, char *name, l_scope current_scope, l_identifier_type type, int address, int complement) {
    if (current_scope == L_LOCAL_SCOPE || current_scope == L_ARGUMENT_SCOPE) {
        identifier_add(stream->local_table, name, current_scope, type, address, complement);
    } else if (current_scope == L_GLOBAL_SCOPE) {
        identifier_add(stream->global_table, name, current_scope, type, address, complement);
    } else {
        printf("Error. Invalid scope.\n");
    }
}

int l_symbols_table_search_local(l_symbols_table_stream *stream, char *name) {
    int i;

    if (!name) {
        return -1;
    }

    for (i = 0; i < stream->local_table->current_identifier; i++) {
        if (strcmp(stream->local_table->identifiers[i]->name, name) == 0) {
            return i;
        }
    }

    return -1;
}

int l_symbols_table_search_global(l_symbols_table_stream *stream, char *name) {
    int i;

    if (!name) {
        return -1;
    }

    for (i = 0; i < stream->global_table->current_identifier; i++) {
        if (strcmp(stream->global_table->identifiers[i]->name, name) == 0) {
            return i;
        }
    }

    return -1;
}

void l_symbols_table_print(l_symbols_table_stream *stream, FILE *out) {
    int i;

    fprintf(out, "------------------------------------------\n");

    if (stream->current_scope == L_LOCAL_SCOPE || stream->current_scope == L_ARGUMENT_SCOPE) {
        for (i = 0; i < stream->local_table->current_identifier; i++) {
            fprintf(out, "%d ", i);
            fprintf(out, "%s ", stream->local_table->identifiers[i]->name);

            if (stream->current_scope == L_LOCAL_SCOPE) {
                fprintf(out, "LOCAL ");
            } else if (stream->current_scope == L_ARGUMENT_SCOPE) {
                fprintf(out, "ARGUMENT ");
            }
            if(stream->local_table->identifiers[i]->type == L_INTEGER_IDENTIFIER) {
                fprintf(out, "INTEGER ");
            } else if(stream->local_table->identifiers[i]->type == L_TABLE_IDENTIFIER) {
                fprintf(out, "TABLE ");
            }

            fprintf(out, "%d ", stream->local_table->identifiers[i]->address);
            fprintf(out, "%d\n", stream->local_table->identifiers[i]->complement);
        }
    } else if (stream->current_scope == L_GLOBAL_SCOPE) {
        for (i = 0; i < stream->global_table->current_identifier; i++) {
            fprintf(out, "%d ", i);
            fprintf(out, "%s ", stream->global_table->identifiers[i]->name);
            fprintf(out, "GLOBAL ");

            if(stream->global_table->identifiers[i]->type == L_INTEGER_IDENTIFIER) {
                fprintf(out, "INTEGER ");
            } else if(stream->global_table->identifiers[i]->type == L_TABLE_IDENTIFIER) {
                fprintf(out, "TABLE ");
            } else if(stream->global_table->identifiers[i]->type == L_FUNCTION_IDENTIFIER) {
                fprintf(out, "FUNCTION ");
            }

            fprintf(out, "%d ", stream->global_table->identifiers[i]->address);
            fprintf(out, "%d\n", stream->global_table->identifiers[i]->complement); 
        }
    } else {
        printf("Error. Invalid scope.\n");
    }

    fprintf(out, "------------------------------------------\n");
}
