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

#include "../headers/l_lexical_unity.h"
#include "../headers/l_rules.h"
#include "../headers/l_first.h"

static int first[NON_TERMINAL_MAX + 1][TERMINAL_MAX + 1];

void l_init_first() {
    int i, j;

    for (i = 0; i <= NON_TERMINAL_MAX; i++) {
        for (j = 0; j <= TERMINAL_MAX; j++) {
            first[i][j] = 0;
        }
    }

    /*first[PG][EPSILON] = 1;*/
    first[PG][FCT_ID] = 1;
    first[PG][INTEGER] = 1;

    /*first[VDO][EPSILON] = 1;*/
    first[VDO][INTEGER] = 1;
    first[VDO][FCT_ID] = 1;

    first[VDL][INTEGER] = 1;

    /*first[VDLB][EPSILON] = 1;*/
    first[VDLB][COMMA] = 1;

    first[VD][INTEGER] = 1;

    /*first[OAS][EPSILON] = 1;*/
    first[OAS][OPENING_BRACKET] = 1;

    /*first[FDL][EPSILON] = 1;*/
    first[FDL][FCT_ID] = 1;

    first[FD][FCT_ID] = 1;

    first[PL][OPENING_PARENTHESIS] = 1;

    /*first[VDLO][EPSILON] = 1;*/
    first[VDLO][INTEGER] = 1;

    first[I][VAR_ID] = 1;
    first[I][OPENING_BRACE] = 1;
    first[I][WHILE] = 1;
    first[I][IF] = 1;
    first[I][RETURN] = 1;
    first[I][SEMICOLON] = 1;
    first[I][WRITE] = 1;
    first[I][FCT_ID] = 1;

    first[ASSI][VAR_ID] = 1;

    first[BI][OPENING_BRACE] = 1;

    /*first[IL][EPSILON] = 1;*/
    first[IL][VAR_ID] = 1;
    first[IL][OPENING_BRACE] = 1;
    first[IL][WHILE] = 1;
    first[IL][IF] = 1;
    first[IL][RETURN] = 1;
    first[IL][SEMICOLON] = 1;
    first[IL][WRITE] = 1;
    first[IL][FCT_ID] = 1;

    first[IFI][IF] = 1;

    /*first[ELSEO][EPSILON] = 1;*/
    first[ELSEO][ELSE] = 1;

    first[WHILEI][WHILE] = 1;

    first[CALLI][FCT_ID] = 1;

    first[RETI][RETURN] = 1;

    first[WRITEI][WRITE] = 1;

    first[EMPTYI][SEMICOLON] = 1;

    first[EXP][VAR_ID] = 1;
    first[EXP][OPENING_PARENTHESIS] = 1;
    first[EXP][NOT] = 1;
    first[EXP][NUMBER] = 1;
    first[EXP][FCT_ID] = 1;
    first[EXP][READ] = 1;

    /*first[EXPB][EPSILON] = 1;*/
    first[EXPB][OR] = 1;

    first[CONJ][VAR_ID] = 1;
    first[CONJ][OPENING_PARENTHESIS] = 1;
    first[CONJ][NOT] = 1;
    first[CONJ][NUMBER] = 1;
    first[CONJ][FCT_ID] = 1;
    first[CONJ][READ] = 1;

    /*first[CONJB][EPSILON] = 1;*/
    first[CONJB][AND] = 1;

    first[COMP][VAR_ID] = 1;
    first[COMP][OPENING_PARENTHESIS] = 1;
    first[COMP][NOT] = 1;
    first[COMP][NUMBER] = 1;
    first[COMP][FCT_ID] = 1;
    first[COMP][READ] = 1;

    first[COMPB][EQUAL] = 1;
    first[COMPB][OPENING_PARENTHESIS] = 1;

    first[E][VAR_ID] = 1;
    first[E][OPENING_PARENTHESIS] = 1;
    first[E][NOT] = 1;
    first[E][NUMBER] = 1;
    first[E][FCT_ID] = 1;
    first[E][READ] = 1;

    /*first[EB][EPSILON] = 1;*/
    first[EB][SUBSTRACT] = 1;
    first[EB][ADD] = 1;

    first[T][VAR_ID] = 1;
    first[T][OPENING_PARENTHESIS] = 1;
    first[T][NOT] = 1;
    first[T][NUMBER] = 1;
    first[T][FCT_ID] = 1;
    first[T][READ] = 1;

    /*first[TB][EPSILON] = 1;*/
    first[TB][MULTIPLY] = 1;
    first[TB][DIVIDE] = 1;

    first[NEG][VAR_ID] = 1;
    first[NEG][OPENING_PARENTHESIS] = 1;
    first[NEG][NOT] = 1;
    first[NEG][NUMBER] = 1;
    first[NEG][FCT_ID] = 1;
    first[NEG][READ] = 1;

    first[F][VAR_ID] = 1;
    first[F][OPENING_PARENTHESIS] = 1;
    first[F][NUMBER] = 1;
    first[F][FCT_ID] = 1;
    first[F][READ] = 1;

    first[VAR][VAR_ID] = 1;

    /*first[INDO][EPSILON] = 1;*/
    first[INDO][OPENING_BRACKET] = 1;

    first[FCALL][FCT_ID] = 1;

    /*first[LEXP][EPSILON] = 1;*/
    first[LEXP][VAR_ID] = 1;
    first[LEXP][OPENING_PARENTHESIS] = 1;
    first[LEXP][NOT] = 1;
    first[LEXP][NUMBER] = 1;
    first[LEXP][FCT_ID] = 1;
    first[LEXP][READ] = 1;

    /*first[LEXPB][EPSILON] = 1;*/
    first[LEXPB][COMMA] = 1;
}

bool l_is_first(int non_terminal, int terminal) {
    return first[non_terminal][terminal];
}
