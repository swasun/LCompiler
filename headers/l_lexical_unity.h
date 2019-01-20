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

#ifndef L_LEXICAL_UNITY_H
#define L_LEXICAL_UNITY_H

#define TERMINAL_MAX 33

/*
 * Control instructions
 * IF
 * THEN
 * ELSE
 * WHILE
 * DO
 * RETURN
 *
 * Already known functions
 * READ
 * WRITE
 * MAIN
 *
 * Code of identifier types and numbers
 * VAR_ID \$[0-9a-zA-Z_\$]*
 * FCT_ID [a-zA-Z_][0-9a-zA-Z_\$]*
 * NUMBER [0-9]+
 *
 * Punctuations
 * OPENING_PARENTHESIS
 * CLOSING_PARENTHESIS
 * OPENING_BRACE
 * CLOSNG_BRACE
 * SEMICOLON
 * CROCHET_OUVERT
 * CROCHET_FERME
 * COMMA
 *
 * Arithmetic operators
 * EQUAL
 * ADD
 * SUBSTRACT
 * MULTIPLY
 * DIVIDE
 * INFERIOR
 * SUPERIOR
 * AND
 * OR
 * NOT
 *
 * End of file
 * END
 *
 * Type of variables
 * INTEGER
 */

#define IF                  0
#define THEN                1
#define ELSE                2
#define WHILE               3
#define DO                  4
#define RETURN              5
#define READ                6
#define WRITE               7
#define MAIN                8
#define VAR_ID              9
#define FCT_ID              10
#define NUMBER              11
#define OPENING_PARENTHESIS 12
#define CLOSING_PARENTHESIS 13
#define OPENING_BRACE       14
#define CLOSNG_BRACE        15
#define SEMICOLON           16
#define OPENING_BRACKET     17
#define CLOSING_BRACKET     18
#define COMMA               19
#define EQUAL               20
#define ADD                 21
#define SUBSTRACT           22
#define MULTIPLY            23
#define DIVIDE              24
#define INFERIOR            25
#define SUPERIOR            26
#define AND                 27
#define OR                  28
#define NOT                 29
#define END                 30
#define INTEGER             31
#define EPSILON             32

#endif
