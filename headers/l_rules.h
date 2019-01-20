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

#ifndef L_RULES_H
#define L_RULES_H

#define NON_TERMINAL_MAX 38

typedef enum {
    PG,     /* pg -> vdo fdl                   #(1) program -> variableDecOpt functionDecList                                */
    VDO,    /* vdo -> vdl ';'                  #(2) variableDecOpt -> variableDecList ';'                                    */
            /* vdo -> ε                        #(3)                  | ε                                                     */
    VDL,    /* vdl -> vd ldvb                  #(4) variableDecList -> variableDeclaration variableDecListBis                */
    VDLB,   /* ldvb -> ',' vd ldvb             #(5) variableDecListBis -> variableDeclaration variableDecListBis             */
            /* ldvb -> ε                       #(6)                       | ε                                                */
    VD,     /* INT IVD oas                     #(7) variableDeclaration -> INTEGER VAR_ID optArraySize                       */
    OAS,    /* oas -> [' NB ']                 #(8) optArraySize -> '[' NUMBER ']'                                           */
            /* oas -> ε                        #(9)                   | ε                                                    */
    FDL,    /* fdl -> fd fdl                   #(10) functionDecList -> functionDeclaration functionDecList                  */
            /* fdl -> ε                        #(11)                    | ε                                                  */
    FD,     /* fd -> FCI pl vdo bi             #(12) functionDeclaration -> FCT_ID paramList variableDecOpt blockInstruction */
    PL,     /* pl -> '(' vdlo ')'              #(13) paramList -> '(' variableDecListOpt ')'                                 */
    VDLO,   /* vdlo -> vdl                     #(14) variableDecListOpt -> variableDecList                                   */
            /* vdlo -> ε                       #(15)                       | ε                                               */
    I,      /* i -> assi                       #(16) instruction -> assignmentInstruction                                    */
            /* i -> bi                         #(17)              | blockInstruction                                         */
            /* i -> ifi                        #(18)               | ifInstruction                                           */
            /* i -> whilei                     #(19)               | whileInstruction                                        */
            /* i -> calli                      #(20)               | callInstruction                                         */
            /* i -> reti                       #(21)               | returnInstruction                                       */
            /* i -> writei                     #(22)               | writeInstruction                                        */
            /* i -> emptyi                     #(23)               | emptyInstruction                                        */
    ASSI,   /* assi -> var '=' exp ';'         #(24) assignmentInstruction -> var '=' expression ';'                         */
    BI,     /* bi -> '{' il '}'                #(25) blockInstruction -> '{' instructionList '}'                             */
    IL,     /* il -> i il                      #(26) instructionList -> instruction instructionList                          */
            /* il -> ε                         #(27)                    | ε                                                  */
    IFI,    /* ifi -> IF exp THEN bi elseo     #(28) ifInstruction -> IF expression THEN blockInstruction elseOpt            */
    ELSEO,  /* elseo -> SIN bi                 #(29) elseOpt -> ELSE blockInstruction                                        */
            /* opelse -> ε                     #(30)           | ε                                                           */
    WHILEI, /* whilei -> WHILE exp FR bi       #(31) whileInstruction -> WHILE expression DO blockInstruction                */
    CALLI,  /* calli -> callf ';'              #(32) callInstruction -> fctCall ';'                                          */
    RETI,   /* reti -> RET exp ';'             #(33) returnInstruction -> RETURN expression ';'                              */
    WRITEI, /* writei -> WRITE '(' exp ')' ';' #(34) writeInstruction -> WRITE '(' expression ')' ';'                        */
    EMPTYI, /* emptyi -> ';'                   #(35) emptyInstruction -> ';'                                                 */
    EXP,    /* exp -> conj expB                #(36) expression -> conjunction expressionBis                                 */
    EXPB,   /* expB -> '|' conj expB           #(37) expressionBis -> '|' conjunction expressionBis                          */
            /* expB -> ε                       #(38)                | ε                                                      */
    CONJ,   /* conj -> comp conjB              #(39) conjunction -> comparaison conjunctionBis                               */
    CONJB,  /* conjB -> '&' comp conjB         #(40) conjunctionBis -> '&' comparaison conjunctionBis                        */
            /* conjB -> ε                      #(41)                 | ε                                                     */
    COMP,   /* comp -> e compb                 #(42) comparaison -> arithExp comparaisonBis                                  */
    COMPB,  /* compB -> '=' e compB            #(43) comparaisonBis -> '=' arithExp comparaisonBis                           */
            /* compB -> '<' e compB            #(44)                 | '<' arithExp comparaisonBis                           */
            /* compB -> ε                      #(45)                 | ε                                                     */
    E,      /* e -> t eb                       #(46) arithExp -> term arithExpBis                                            */
    EB,     /* eb -> '+' t eb                  #(47) arithExpBos -> '+' term arithExpBis                                     */
            /* eb -> '-' t eb                  #(48)              | '-' term arithExpBis                                     */
            /* eb -> ε                         #(49)              | ε                                                        */
    T,      /* t -> neg tb                     #(50) term -> negation termBis                                                */
    TB,     /* tb -> '*' neg tb                #(51) termBis -> '*' negation termBis                                         */
            /* tb -> '/' neg tb                #(52)           | '/' negation termBis                                        */
            /* tb -> ε                         #(53)           | ε                                                           */
    NEG,    /* neg -> '!' neg                  #(54) negation -> '!' negation                                                */
            /* neg -> f                        #(55)           | factor                                                      */
    F,      /* f -> '(' exp ')'                #(56) factor -> '(' expression ')'                                            */
            /* f -> NB                         #(57)          | NUMBER                                                       */
            /* f -> callf                      #(58)          | fctCall                                                      */
            /* f -> var                        #(59)          | var                                                          */
            /* f -> READL '(' ')'              #(60)          | READL '(' ')'                                                */
    VAR,    /* var -> IVD indo                 #(61) var -> VAR_ID indiceOpt                                                 */
    INDO,   /* indo -> '[' exp ']'             #(62) indiceOpt -> '[' expression ']'                                         */
            /* indo -> ε                       #(63)            | ε                                                          */
    FCALL,  /* FCI '(' lexp ')'                #(64) fctCall -> FCT_ID '(' expressionList ')'                                */
    LEXP,   /* lexp -> exp lexpb               #(65) expressionList -> expression expressionListBis                          */
            /* lexp -> ε                       #(66)                   | ε                                                   */
    LEXPB   /* lexpb -> ',' exp lexpb          #(67) expressionListBis -> ',' expression expressionListBis                   */
            /* lexpb -> ε                      #(68)                    |  ε                                                 */
} rule;

#endif
