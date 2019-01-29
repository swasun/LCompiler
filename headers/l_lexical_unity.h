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
