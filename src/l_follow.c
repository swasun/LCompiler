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
#include "../headers/l_follow.h"

static int follow[NON_TERMINAL_MAX + 1][TERMINAL_MAX + 1];

void l_init_follow() {
    int i,j;

    for(i = 0; i <= NON_TERMINAL_MAX; i++) {
        for(j = 0; j <= TERMINAL_MAX; j++) {
           follow[i][j] = 0;
        }
    }

    follow[PG][END] = 1;

    follow[VDO][FCT_ID] = 1;
    follow[VDO][END] = 1;
    follow[VDO][OPENING_BRACE] = 1;

    follow[VDL][CLOSING_PARENTHESIS] = 1;
    follow[VDL][SEMICOLON] = 1;

    follow[VDLB][CLOSING_PARENTHESIS] = 1;
    follow[VDLB][SEMICOLON] = 1;

    follow[VD][CLOSING_PARENTHESIS] = 1;
    follow[VD][SEMICOLON] = 1;
    follow[VD][COMMA] = 1;

    follow[OAS][CLOSING_PARENTHESIS] = 1;
    follow[OAS][SEMICOLON] = 1;
    follow[OAS][COMMA] = 1;

    follow[FDL][END] = 1;

    follow[FD][FCT_ID] = 1;
    follow[FD][END] = 1;

    follow[PL][OPENING_BRACE] = 1;
    follow[PL][INTEGER] = 1;

    follow[VDLO][CLOSING_PARENTHESIS] = 1;

    follow[I][VAR_ID] = 1;
    follow[I][OPENING_BRACE] = 1;
    follow[I][WHILE] = 1;
    follow[I][IF] = 1;
    follow[I][RETURN] = 1;
    follow[I][SEMICOLON] = 1;
    follow[I][WRITE] = 1;
    follow[I][FCT_ID] = 1;
    follow[I][CLOSNG_BRACE] = 1;

    follow[ASSI][VAR_ID] = 1;
    follow[ASSI][OPENING_BRACE] = 1;
    follow[ASSI][WHILE] = 1;
    follow[ASSI][IF] = 1;
    follow[ASSI][RETURN] = 1;
    follow[ASSI][SEMICOLON] = 1;
    follow[ASSI][WRITE] = 1;
    follow[ASSI][FCT_ID] = 1;
    follow[ASSI][CLOSNG_BRACE] = 1;

    follow[BI][VAR_ID] = 1;
    follow[BI][OPENING_BRACE] = 1;
    follow[BI][WHILE] = 1;
    follow[BI][IF] = 1;
    follow[BI][RETURN] = 1;
    follow[BI][ELSE] = 1;
    follow[BI][SEMICOLON] = 1;
    follow[BI][WRITE] = 1;
    follow[BI][FCT_ID] = 1;
    follow[BI][CLOSNG_BRACE] = 1;
    follow[BI][END] = 1;

    follow[IL][CLOSNG_BRACE] = 1;

    follow[IFI][VAR_ID] = 1;
    follow[IFI][OPENING_BRACE] = 1;
    follow[IFI][WHILE] = 1;
    follow[IFI][IF] = 1;
    follow[IFI][RETURN] = 1;
    follow[IFI][SEMICOLON] = 1;
    follow[IFI][WRITE] = 1;
    follow[IFI][FCT_ID] = 1;
    follow[IFI][CLOSNG_BRACE] = 1;

    follow[ELSEO][VAR_ID] = 1;
    follow[ELSEO][OPENING_BRACE] = 1;
    follow[ELSEO][WHILE] = 1;
    follow[ELSEO][IF] = 1;
    follow[ELSEO][RETURN] = 1;
    follow[ELSEO][SEMICOLON] = 1;
    follow[ELSEO][WRITE] = 1;
    follow[ELSEO][FCT_ID] = 1;
    follow[ELSEO][CLOSNG_BRACE] = 1;

    follow[WHILEI][VAR_ID] = 1;
    follow[WHILEI][OPENING_BRACE] = 1;
    follow[WHILEI][WHILE] = 1;
    follow[WHILEI][IF] = 1;
    follow[WHILEI][RETURN] = 1;
    follow[WHILEI][SEMICOLON] = 1;
    follow[WHILEI][WRITE] = 1;
    follow[WHILEI][FCT_ID] = 1;
    follow[WHILEI][CLOSNG_BRACE] = 1;

    follow[CALLI][VAR_ID] = 1;
    follow[CALLI][OPENING_BRACE] = 1;
    follow[CALLI][WHILE] = 1;
    follow[CALLI][IF] = 1;
    follow[CALLI][RETURN] = 1;
    follow[CALLI][SEMICOLON] = 1;
    follow[CALLI][WRITE] = 1;
    follow[CALLI][FCT_ID] = 1;
    follow[CALLI][CLOSNG_BRACE] = 1;

    follow[RETI][VAR_ID] = 1;
    follow[RETI][OPENING_BRACE] = 1;
    follow[RETI][WHILE] = 1;
    follow[RETI][IF] = 1;
    follow[RETI][RETURN] = 1;
    follow[RETI][SEMICOLON] = 1;
    follow[RETI][WRITE] = 1;
    follow[RETI][FCT_ID] = 1;
    follow[RETI][CLOSNG_BRACE] = 1;

    follow[WRITEI][VAR_ID] = 1;
    follow[WRITEI][OPENING_BRACE] = 1;
    follow[WRITEI][WHILE] = 1;
    follow[WRITEI][IF] = 1;
    follow[WRITEI][RETURN] = 1;
    follow[WRITEI][SEMICOLON] = 1;
    follow[WRITEI][WRITE] = 1;
    follow[WRITEI][FCT_ID] = 1;
    follow[WRITEI][CLOSNG_BRACE] = 1;

    follow[EMPTYI][VAR_ID] = 1;
    follow[EMPTYI][OPENING_BRACE] = 1;
    follow[EMPTYI][WHILE] = 1;
    follow[EMPTYI][IF] = 1;
    follow[EMPTYI][RETURN] = 1;
    follow[EMPTYI][SEMICOLON] = 1;
    follow[EMPTYI][WRITE] = 1;
    follow[EMPTYI][FCT_ID] = 1;
    follow[EMPTYI][CLOSNG_BRACE] = 1;

    follow[EXP][DO] = 1;
    follow[EXP][THEN] = 1;
    follow[EXP][COMMA] = 1;
    follow[EXP][CLOSING_PARENTHESIS] = 1;
    follow[EXP][CLOSING_BRACKET] = 1;
    follow[EXP][SEMICOLON] = 1;

    follow[EXPB][DO] = 1;
    follow[EXPB][THEN] = 1;
    follow[EXPB][COMMA] = 1;
    follow[EXPB][CLOSING_PARENTHESIS] = 1;
    follow[EXPB][CLOSING_BRACKET] = 1;
    follow[EXPB][SEMICOLON] = 1;

    follow[CONJ][DO] = 1;
    follow[CONJ][THEN] = 1;
    follow[CONJ][COMMA] = 1;
    follow[CONJ][CLOSING_PARENTHESIS] = 1;
    follow[CONJ][CLOSING_BRACKET] = 1;
    follow[CONJ][SEMICOLON] = 1;
    follow[CONJ][OR] = 1;

    follow[CONJB][DO] = 1;
    follow[CONJB][THEN] = 1;
    follow[CONJB][COMMA] = 1;
    follow[CONJB][CLOSING_PARENTHESIS] = 1;
    follow[CONJB][CLOSING_BRACKET] = 1;
    follow[CONJB][SEMICOLON] = 1;
    follow[CONJB][OR] = 1;

    follow[COMP][DO] = 1;
    follow[COMP][THEN] = 1;
    follow[COMP][COMMA] = 1;
    follow[COMP][CLOSING_PARENTHESIS] = 1;
    follow[COMP][CLOSING_BRACKET] = 1;
    follow[COMP][SEMICOLON] = 1;
    follow[COMP][OR] = 1;
    follow[COMP][AND] = 1;

    follow[COMPB][DO] = 1;
    follow[COMPB][THEN] = 1;
    follow[COMPB][COMMA] = 1;
    follow[COMPB][CLOSING_PARENTHESIS] = 1;
    follow[COMPB][CLOSING_BRACKET] = 1;
    follow[COMPB][SEMICOLON] = 1;
    follow[COMPB][OR] = 1;
    follow[COMPB][AND] = 1;

    follow[E][DO] = 1;
    follow[E][THEN] = 1;
    follow[E][COMMA] = 1;
    follow[E][CLOSING_PARENTHESIS] = 1;
    follow[E][CLOSING_BRACKET] = 1;
    follow[E][SEMICOLON] = 1;
    follow[E][OR] = 1;
    follow[E][AND] = 1;
    follow[E][INFERIOR] = 1;
    follow[E][EQUAL] = 1;

    follow[EB][DO] = 1;
    follow[EB][THEN] = 1;
    follow[EB][COMMA] = 1;
    follow[EB][CLOSING_PARENTHESIS] = 1;
    follow[EB][CLOSING_BRACKET] = 1;
    follow[EB][SEMICOLON] = 1;
    follow[EB][OR] = 1;
    follow[EB][AND] = 1;
    follow[EB][INFERIOR] = 1;
    follow[EB][EQUAL] = 1;

    follow[T][DO] = 1;
    follow[T][THEN] = 1;
    follow[T][COMMA] = 1;
    follow[T][CLOSING_PARENTHESIS] = 1;
    follow[T][CLOSING_BRACKET] = 1;
    follow[T][SEMICOLON] = 1;
    follow[T][OR] = 1;
    follow[T][AND] = 1;
    follow[T][INFERIOR] = 1;
    follow[T][EQUAL] = 1;
    follow[T][SUBSTRACT] = 1;
    follow[T][ADD] = 1;

    follow[TB][DO] = 1;
    follow[TB][THEN] = 1;
    follow[TB][COMMA] = 1;
    follow[TB][CLOSING_PARENTHESIS] = 1;
    follow[TB][CLOSING_BRACKET] = 1;
    follow[TB][SEMICOLON] = 1;
    follow[TB][OR] = 1;
    follow[TB][AND] = 1;
    follow[TB][INFERIOR] = 1;
    follow[TB][EQUAL] = 1;
    follow[TB][SUBSTRACT] = 1;
    follow[TB][ADD] = 1;

    follow[NEG][DO] = 1;
    follow[NEG][THEN] = 1;
    follow[NEG][COMMA] = 1;
    follow[NEG][CLOSING_PARENTHESIS] = 1;
    follow[NEG][CLOSING_BRACKET] = 1;
    follow[NEG][SEMICOLON] = 1;
    follow[NEG][OR] = 1;
    follow[NEG][AND] = 1;
    follow[NEG][INFERIOR] = 1;
    follow[NEG][EQUAL] = 1;
    follow[NEG][SUBSTRACT] = 1;
    follow[NEG][ADD] = 1;
    follow[NEG][MULTIPLY] = 1;
    follow[NEG][DIVIDE] = 1;

    follow[F][DO] = 1;
    follow[F][THEN] = 1;
    follow[F][COMMA] = 1;
    follow[F][CLOSING_PARENTHESIS] = 1;
    follow[F][CLOSING_BRACKET] = 1;
    follow[F][SEMICOLON] = 1;
    follow[F][OR] = 1;
    follow[F][AND] = 1;
    follow[F][INFERIOR] = 1;
    follow[F][EQUAL] = 1;
    follow[F][SUBSTRACT] = 1;
    follow[F][ADD] = 1;
    follow[F][MULTIPLY] = 1;
    follow[F][DIVIDE] = 1;

    follow[VAR][DO] = 1;
    follow[VAR][THEN] = 1;
    follow[VAR][COMMA] = 1;
    follow[VAR][CLOSING_PARENTHESIS] = 1;
    follow[VAR][CLOSING_BRACKET] = 1;
    follow[VAR][SEMICOLON] = 1;
    follow[VAR][OR] = 1;
    follow[VAR][AND] = 1;
    follow[VAR][INFERIOR] = 1;
    follow[VAR][EQUAL] = 1;
    follow[VAR][SUBSTRACT] = 1;
    follow[VAR][ADD] = 1;
    follow[VAR][MULTIPLY] = 1;
    follow[VAR][DIVIDE] = 1;

    follow[INDO][DO] = 1;
    follow[INDO][THEN] = 1;
    follow[INDO][COMMA] = 1;
    follow[INDO][CLOSING_PARENTHESIS] = 1;
    follow[INDO][CLOSING_BRACKET] = 1;
    follow[INDO][SEMICOLON] = 1;
    follow[INDO][OR] = 1;
    follow[INDO][AND] = 1;
    follow[INDO][INFERIOR] = 1;
    follow[INDO][EQUAL] = 1;
    follow[INDO][SUBSTRACT] = 1;
    follow[INDO][ADD] = 1;
    follow[INDO][MULTIPLY] = 1;
    follow[INDO][DIVIDE] = 1;

    follow[FCALL][DO] = 1;
    follow[FCALL][THEN] = 1;
    follow[FCALL][COMMA] = 1;
    follow[FCALL][CLOSING_PARENTHESIS] = 1;
    follow[FCALL][CLOSING_BRACKET] = 1;
    follow[FCALL][SEMICOLON] = 1;
    follow[FCALL][OR] = 1;
    follow[FCALL][AND] = 1;
    follow[FCALL][INFERIOR] = 1;
    follow[FCALL][EQUAL] = 1;
    follow[FCALL][SUBSTRACT] = 1;
    follow[FCALL][ADD] = 1;
    follow[FCALL][MULTIPLY] = 1;
    follow[FCALL][DIVIDE] = 1;

    follow[LEXP][CLOSING_PARENTHESIS] = 1;

    follow[LEXPB][CLOSING_PARENTHESIS] = 1;
}

bool l_is_follow(int non_terminal, int terminal) {
    return follow[non_terminal][terminal];
}
