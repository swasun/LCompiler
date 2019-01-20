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

#ifndef L_TOKEN_H
#define L_TOKEN_H

#include <stdio.h>

/*
 * A token is a structure that describes a word in the grammar of a language.
 * @example: word name: (
 *           word type: symbol
 *           unit name: OPENING_PARENTHESIS
 */
typedef struct {
    int unity;
    char *word_name;
    char *word_type;
    char *unity_name;
} l_token;

l_token *l_token_create(int unity, char *word_name, char *word_type, char *unity_name);

l_token *l_token_create_token(int unity, char *word_name);

l_token *l_token_create_func(int unity, char *word_name);

void l_token_destroy(l_token *lex);

void l_token_write(FILE *lex_fd, l_token *lex);

#endif
