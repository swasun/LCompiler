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

#include "../headers/l_parser.h"
#include "../headers/l_lexical_unity.h"
#include "../headers/l_lexical_analysis.h"
#include "../headers/utils.h"
#include "../headers/stacktrace.h"
#include "../headers/debug.h"
#include "../headers/alloc.h"
#include "../headers/error.h"
#include "../headers/l_token.h"
#include "../headers/l_first.h"
#include "../headers/l_follow.h"
#include "../headers/l_rules.h"
#include "../headers/l_tokens_definitions.h"
#include "../headers/xml_writer.h"
#include "../headers/check_parameter.h"
#include "../headers/l_abstract_syntax_tree.h"
#include "../headers/l_abstract_syntax_tree_print.h"
#include "../headers/l_symbols_table.h"
#include "../headers/thread_storage.h"
#include "../headers/l_error.h"
#include "../headers/l_mips_stream.h"
#include "../headers/l_mips.h"
#include "../headers/l_mips_sp.h"

#include <stdlib.h>

#define DEBUG_PRINT_CURRENT_LEX(ctx) DEBUG_PRINT("%s : %s %s \n", __func__, ctx->current_token->word_name, ctx->current_token->word_type);

#define CHECK_IF_TERMINATED(ctx) \
    if (ctx->eof_state || !ctx->current_token) { \
        return 0; \
    } \

#define CHECK_IF_TERMINATED_HERITE(ctx, herite) \
    if (ctx->eof_state) { \
        return herite; \
    } \

#define NEXT_LEXEME(ctx) \
    ctx->previous_token = ctx->current_token; \
    ctx->current_token = l_lexical_analysis_next_token(ctx); \
    if (ctx->dump_lex) { \
        l_token_write(ctx->lex_fd, ctx->current_token); \
    } \

#define SYNT_WRITE_OPENED_TAG(ctx) \
    if (ctx->dump_synt) { \
        xml_write_opened_tag(ctx->synt_writer, __func__); \
    } \

#define SYNT_WRITE_CLOSED_TAG(ctx) \
    if (ctx->dump_synt) { \
        xml_write_closed_tag(ctx->synt_writer, __func__); \
    } \

#define SYNT_WRITE_TERMINAL(ctx) \
    if (ctx->dump_synt) { \
        xml_write_element(ctx->synt_writer, ctx->current_token->word_type, ctx->current_token->unity_name); \
    } \

#define FORWARD(ctx) \
    SYNT_WRITE_TERMINAL(ctx) \
    NEXT_LEXEME(ctx) \

#define CONSUME(ctx, excepted_token) \
    if (ctx->current_token) { \
        if (ctx->current_token->unity == excepted_token) { \
            FORWARD(ctx) \
        } \
    } \

#define CONSUME_OR_ERROR(ctx, excepted_token) \
    if (ctx->current_token) { \
        if (ctx->current_token->unity == excepted_token) { \
            FORWARD(ctx) \
        } else { \
            ERROR_EXCEPTED(ctx, excepted_token) \
        } \
    } \

/* pg -> vdo fdl #(1) program -> variableDecOpt functionDecList */
static n_prog *pg(l_analysis_ctx *ctx);

/*
 * vdo -> vdl ';' #(2) variableDecOpt -> variableDecList ';'
 * vdo -> ε       #(3)                  | ε
 */
static n_l_dec *vdo(l_analysis_ctx *ctx);

/* vdl -> vd ldvb #(4) variableDecList -> variableDeclaration variableDecListBis */
static n_l_dec *vdl(l_analysis_ctx *ctx);

/*
 * ldvb -> ',' vd ldvb #(5) variableDecListBis -> variableDeclaration variableDecListBis
 * ldvb -> ε            #(6)                       | ε
 */
static n_l_dec *ldvb(l_analysis_ctx *ctx);

/* INT IVD oas #(7) variableDeclaration -> INTEGER VAR_ID optArraySize */
static n_dec *vd(l_analysis_ctx *ctx);

/*
 * oas -> [' NB '] #(8) optArraySize -> '[' NUMBER ']'
 * oas -> ε              #(9)                   | ε
 */
static n_dec *oas(l_analysis_ctx *ctx, char *herite);

/*
 * fdl -> fd fdl #(10) functionDecList -> functionDeclaration functionDecList
 * fdl -> ε      #(11)                    | ε
 */
static n_l_dec *fdl(l_analysis_ctx *ctx);

/* fd -> FCI pl vdo bi #(12) functionDeclaration -> FCT_ID paramList variableDecOpt blockInstruction */
static n_dec *fd(l_analysis_ctx *ctx);

/* pl -> '(' vdlo ')' #(13) paramList -> '(' variableDecListOpt ')'*/
static n_l_dec *pl(l_analysis_ctx *ctx);

/*
 * vdlo -> vdl #(14) variableDecListOpt -> variableDecList
 * vdlo -> ε   #(15)                       | ε
 */
static n_l_dec *vdlo(l_analysis_ctx *ctx);

/*
 * i -> assi   #(16) instruction -> assignInstruction
 * i -> bi     #(17)              | blockInstruction
 * i -> ifi    #(18)              | ifInstruction
 * i -> whilei    #(19)              | whileInstruction
 * i -> calli   #(20)              | callInstruction
 * i -> reti   #(21)              | returnInstruction
 * i -> writei   #(22)              | writeInstruction
 * i -> emptyi #(23)              | emptyInstruction
 */
static n_instr *i(l_analysis_ctx *ctx);

/* assi -> var '=' exp ';' #(24) assignInstruction -> var '=' expression ';' */
static n_instr *assi(l_analysis_ctx *ctx);

/* bi -> '{' il '}' #(25) blockInstruction -> '{' instructionList '}' */
static n_instr *bi(l_analysis_ctx *ctx);

/*
 * il -> i il #(26) instructionList -> instruction instructionList
 * il -> ε    #(27)                    | ε
 */
static n_l_instr *il(l_analysis_ctx *ctx);

/* ifi -> IF exp THEN bi elseo #(28) ifInstruction -> IF expression THEN blockInstruction optSinon */
static n_instr *ifi(l_analysis_ctx *ctx);

/*
 * elseo -> SIN bi #(29) optSinon -> ELSE blockInstruction
 * opelse -> ε     #(30)           | ε
 */
static n_instr *elseo(l_analysis_ctx *ctx);

/* whilei -> WHILE exp FR bi #(31) whileInstruction -> WHILE expression DO blockInstruction */
static n_instr *whilei(l_analysis_ctx *ctx);

/* calli -> callf ';' #(32) callInstruction -> fctCall ';' */
static n_instr *calli(l_analysis_ctx *ctx);

/* reti -> RET exp ';' #(33) returnInstruction -> RETURN expression ';' */
static n_instr *reti(l_analysis_ctx *ctx);

/* writei -> WRITE '(' exp ')' ';' #(34) writeInstruction -> WRITE '(' expression ')' ';' */
static n_instr *writei(l_analysis_ctx *ctx);

/* emptyi -> ';' #(35) emptyInstruction -> ';' */
static n_instr *emptyi(l_analysis_ctx *ctx);

/* exp -> conj expB #(36) expression -> conjonction expressionBis */
static n_exp *Exp(l_analysis_ctx *ctx);

/*
 * expB -> '|' conj expB #(37) expressionBis -> '|' conjonction expressionBis
 * expB -> ε             #(38)                | ε
 */
static n_exp *expB(l_analysis_ctx *ctx, n_exp *herite);

/* conj -> comp conjB #(39) conjonction -> comparaison conjonctionBis */
static n_exp *Conj(l_analysis_ctx *ctx);

/*
 * compB -> '=' e compB #(43) comparaisonBis -> '=' expArith comparaisonBis
 * compB -> '<' e compB #(44)                 | '<' expArith comparaisonBis
 * compB -> ε           #(45)                 | ε
 */
static n_exp *conjB(l_analysis_ctx *ctx, n_exp *herite);

/* comp -> e compb #(42) comparaison -> expArith comparaisonBis */
static n_exp *comp(l_analysis_ctx *ctx);

/*
 * compB -> '=' e compB #(43) comparaisonBis -> '=' expArith comparaisonBis
 * compB -> '<' e compB #(44)                 | '<' expArith comparaisonBis
 * compB -> ε           #(45)                 | ε
 */
static n_exp *compB(l_analysis_ctx *ctx, n_exp *herite);

/* e -> t eb #(46) expArith -> term expArithBis */
static n_exp *e(l_analysis_ctx *ctx);

/*
 * eb -> '+' t eb #(47) expArithBos -> '+' term expArithBis
 * eb -> '-' t eb #(48)              | '-' term expArithBis
 * eb -> ε        #(49)              | ε
 */
static n_exp *eB(l_analysis_ctx *ctx, n_exp *herite);

/* t -> neg tb #(50) term -> negation termBis */
static n_exp *t(l_analysis_ctx *ctx);

/*
 * tb -> '*' neg tb #(51) termBis -> '*' negation termBis
 * tb -> '/' neg tb #(52)           | '/' negation termBis
 * tb -> ε          #(53)           | ε
 */
static n_exp *tB(l_analysis_ctx *ctx, n_exp* herite);

/*
 * neg -> '!' neg #(54) negation -> '!' negation
 * neg -> f       #(55)           | facteur
 */
static n_exp *neg(l_analysis_ctx *ctx);

/*
 * f -> '(' exp ')'   #(56) facteur -> '(' expression ')'
 * f -> NB            #(57)          | NUMBER
 * f -> callf          #(58)          | fctCall
 * f -> var           #(59)          | var
 * f -> READL '(' ')' #(60)          | READL '(' ')'
 */
static n_exp *f(l_analysis_ctx *ctx);

/* var -> IVD indo #(61) var -> VAR_ID indiceOpt */
static n_var *var(l_analysis_ctx *ctx);

/*
 * indo -> '[' exp ']' #(62) indiceOpt -> '[' expression ']'
 * indo -> ε           #(63)            | ε
 */
static n_var *indo(l_analysis_ctx *ctx, char *herite);

/* FCI '(' lexp ')' #(64) fctCall -> FCT_ID '(' listeExpressions ')' */
static n_call *callf(l_analysis_ctx *ctx);

/*
 * lexp -> exp lexpb #(65) listeExpressions -> expression listeExpressionsBis
 * lexp -> ε         #(66)                   | ε
 */
static n_l_exp *lExp(l_analysis_ctx *ctx);

/*
 * lexpb -> ',' exp lexpb #(67) listeExpressionBis -> ',' expression listeExpressionBis
 * lexpb -> ε             #(68)                     |  ε
 */
static n_l_exp *lexpB(l_analysis_ctx *ctx);

static n_prog *pg(l_analysis_ctx *ctx) {
    n_prog *SS;
    n_l_dec *S1;
    n_l_dec *S2;
    int main_id;

    SS = NULL;
    S1 = NULL;
    S2 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(VDO, ctx->current_token->unity)) {
        ctx->symb_stream->current_scope = L_GLOBAL_SCOPE;
        S1 = vdo(ctx);
        S2 = fdl(ctx);
        main_id = l_symbols_table_search_global(ctx->symb_stream, "main");
        
        /* If there is no main function in the symbol, we record an error */
        if (main_id == -1) {
            ERROR_UNDEFINED_MAIN(ctx)
        }
        /* If the main function is present in the symbol table but contains arguments, we record an error */
        else if (ctx->symb_stream->global_table->identifiers[main_id]->complement != 0) {
            ERROR_TOO_MANY_ARGS(ctx, "main")
        }
        if (!(SS = l_ast_n_prog_create(S1, S2))) {
            PUSH_STACK_MSG("Failed to create n_prog")
            
        } else {
            /* If there is no error, we can convert the source code in MIPS */
            if (ctx->ae->errors_number == 0) {
                l_mips_pg(ctx->mips_stream, SS);
                /*l_mips_sp_pg(ctx->mips_stream, SS);*/
            }

            /* If the option is specified, we save in a file the abstract syntax tree (AST) */
            if (ctx->dump_asynt) {
                l_ast_n_prog_print(SS, ctx->asynt_writer);
            }

            l_ast_n_prog_destroy(SS);

            /* If the option is specified, we save in a file the symbol table (ST) */
            if (ctx->dump_symb) {
                l_symbols_table_print(ctx->symb_stream, ctx->symb_fd);
            }
        }
    } else {
        DEBUG_PRINT_STR("error pg\n");
        if (ctx->current_token->unity == CLOSING_PARENTHESIS) {
            ERROR_EXCEPTED(ctx, OPENING_PARENTHESIS)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_dec *vdo(l_analysis_ctx *ctx) {
    n_l_dec *SS;

    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(VDL, ctx->current_token->unity)) {
        SS = vdl(ctx);
        CONSUME(ctx, SEMICOLON)
    } else if (l_is_follow(VDO, ctx->current_token->unity)) {
        SS = NULL;
    } else {
        DEBUG_PRINT("error vdo: %s\n", ctx->current_token->word_name);
        if (ctx->current_token->unity == CLOSNG_BRACE) {
            ERROR_EXCEPTED(ctx, OPENING_BRACE)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_dec *vdl(l_analysis_ctx *ctx) {
    char *error_buffer;
    n_l_dec *SS, *S2;
    n_dec *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(VD, ctx->current_token->unity)) {
        S1 = vd(ctx);
        if (ctx->symb_stream->current_scope == L_ARGUMENT_SCOPE) {
            ctx->symb_stream->current_argument_address++;
        }
        S2 = ldvb(ctx);
        SS = l_ast_n_l_dec_create(S1, S2);    
    } else {
        DEBUG_PRINT("error vdl: %s\n", ctx->current_token->word_name);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_dec *ldvb(l_analysis_ctx *ctx) {
    char *error_buffer;
    n_l_dec *SS;
    n_dec *S1;
    n_l_dec *S2;

    S1 = NULL;
    S2 = NULL;
    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == COMMA) {
        FORWARD(ctx)
        S1 = vd(ctx);
        if (ctx->symb_stream->current_scope == L_ARGUMENT_SCOPE){
            ctx->symb_stream->current_argument_address++;
        }
        S2 = ldvb(ctx);
        SS = l_ast_n_l_dec_create(S1, S2);
    } else if (l_is_follow(VDLB, ctx->current_token->unity)) {
        /* ε */
        SS = NULL;
    } else {
        if (ctx->previous_token->unity == COMMA) {
            ERROR_EXCEPTED(ctx, SEMICOLON)
        } else {
            ERROR_EXCEPTED(ctx, COMMA)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_dec *vd(l_analysis_ctx *ctx) {
    char *error_buffer;
    n_dec *SS;
    char *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == INTEGER) {
        FORWARD(ctx)
        if (ctx->current_token->unity == VAR_ID) {
            S1 = string_create_from(ctx->current_token->word_name);
            FORWARD(ctx)
            SS = oas(ctx, S1);
            if (!SS) {
                SAFE_FREE(S1)
            }
        } else {
            ERROR_EXCEPTED(ctx, VAR_ID)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_dec *oas(l_analysis_ctx *ctx, char *herite) {
    char *error_buffer;
    n_dec *SS;
    int S1, var_id;

    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->symb_stream->current_scope == L_ARGUMENT_SCOPE || ctx->symb_stream->current_scope == L_LOCAL_SCOPE) {
        var_id = l_symbols_table_search_local(ctx->symb_stream, herite);
        if (var_id != -1) {
            ERROR_REDECLARED_VARIABLE(ctx, herite)
            
        } else {
            var_id = l_symbols_table_search_global(ctx->symb_stream, herite);
            if (var_id != -1) {
                WARNING_VARIABLE_GLOBAL_SCOPE(ctx, herite)
            }
        }
    }

    if (ctx->current_token->unity == OPENING_BRACKET) {
        FORWARD(ctx)
        if (ctx->current_token->unity == NUMBER) {
            S1 = atoi(ctx->current_token->word_name);
            if (ctx->symb_stream->current_scope == L_GLOBAL_SCOPE){
                l_symbols_table_identifier_add(ctx->symb_stream, herite, ctx->symb_stream->current_scope, L_TABLE_IDENTIFIER, ctx->symb_stream->current_local_address, S1);
                ctx->symb_stream->current_local_address += S1;

                SS = l_ast_n_dec_tab_create(herite, S1);
                FORWARD(ctx)
                CONSUME(ctx, CLOSING_BRACKET)
            } else {
                ERROR_WRONG_ARRAY_DECLARATION(ctx)
                
            }
        } else {
            ERROR_UNDECLARED_VARIABLE(ctx, ctx->current_token->word_name)
        }
    } else if (l_is_follow(OAS, ctx->current_token->unity)) {
        /* ε */
        l_symbols_table_identifier_add(ctx->symb_stream, herite, ctx->symb_stream->current_scope, L_INTEGER_IDENTIFIER, ctx->symb_stream->current_local_address, 0);
        ctx->symb_stream->current_local_address++;
        SS = l_ast_n_dec_var_create(herite);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_dec *fdl(l_analysis_ctx *ctx) {
    char *error_buffer;
    n_l_dec *SS, *S2;
    n_dec *S1;

    S1 = NULL;
    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(FD, ctx->current_token->unity)) {
        S1 = fd(ctx);
        if (S1) {
            S2 = fdl(ctx);
            SS = l_ast_n_l_dec_create(S1, S2);
        }
    } else if (l_is_follow(FDL, ctx->current_token->unity)) {
        /* ε */
        SS = NULL;
    } else {
        if (ctx->current_token->unity == CLOSNG_BRACE) {
            ERROR_EXCEPTED_EXPRESSION(ctx)    
        } else if (ctx->current_token->unity == CLOSING_PARENTHESIS && ctx->previous_token->unity != OPENING_PARENTHESIS) {
            ERROR_EXCEPTED(ctx, OPENING_PARENTHESIS)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_dec *fd(l_analysis_ctx *ctx) {
    n_dec *SS;
    char *S1;
    n_l_dec *S2, *S3;
    n_instr *S4;
    int func_id, func_args, func_addr;

    func_addr = ctx->symb_stream->current_local_address;
    SS = NULL;
    S1 = NULL;
    S2 = NULL;
    S3 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == FCT_ID) {
        S1 = string_create_from(ctx->current_token->word_name);
        ctx->current_function_name = S1;
        FORWARD(ctx)
        l_symbols_table_function_begin(ctx->symb_stream);
        S2 = pl(ctx);
        
        func_args = ctx->symb_stream->current_argument_address;
        func_id = l_symbols_table_search_global(ctx->symb_stream, S1);
        if (func_id != -1) {
            ERROR_FUNC_REDEFINITION(ctx, S1)
            
        } else {
            l_symbols_table_identifier_add(ctx->symb_stream, S1, L_GLOBAL_SCOPE, L_FUNCTION_IDENTIFIER, func_addr, func_args);
            func_addr++;
            ctx->symb_stream->current_local_address = func_addr;
            S3 = vdo(ctx);
            S4 = bi(ctx);
            SS = l_ast_n_dec_func_create(S1, S2, S3, S4);
        }
        
        l_symbols_table_function_end(ctx->symb_stream);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_dec *pl(l_analysis_ctx *ctx) {
    n_l_dec *SS;

    SS = NULL;
    ctx->symb_stream->current_argument_address = 0;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == OPENING_PARENTHESIS) {
        FORWARD(ctx)
        ctx->symb_stream->current_scope = L_ARGUMENT_SCOPE;
        SS = vdlo(ctx);

        if (ctx->current_token->unity == CLOSING_PARENTHESIS) {
            FORWARD(ctx)
            ctx->symb_stream->current_scope = L_LOCAL_SCOPE;
        } else {
            ERROR_EXCEPTED(ctx, CLOSING_PARENTHESIS)
        }
    } else {
        ERROR_EXCEPTED(ctx, OPENING_PARENTHESIS)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_dec *vdlo(l_analysis_ctx *ctx) {
    n_l_dec *SS;

    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(VDL, ctx->current_token->unity)) {
        ctx->symb_stream->current_scope = L_ARGUMENT_SCOPE;
        SS = vdl(ctx);
    } else if (l_is_follow(VDLO, ctx->current_token->unity)) {
        /* epsilon */
        SS = NULL;
    } else {
        DEBUG_PRINT("error vdlo: %s\n", ctx->current_token->word_name);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *i(l_analysis_ctx *ctx) {
    n_instr *SS;

    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(ASSI, ctx->current_token->unity)) {
        SS = assi(ctx);
    } else if (l_is_first(BI, ctx->current_token->unity)) {
        SS = bi(ctx);
    } else if (l_is_first(IFI, ctx->current_token->unity)) {
        SS = ifi(ctx);
    } else if (l_is_first(WHILEI, ctx->current_token->unity)) {
        SS = whilei(ctx);
    } else if (l_is_first(CALLI, ctx->current_token->unity)) {
        SS = calli(ctx);
    } else if (l_is_first(RETI, ctx->current_token->unity)) {
        SS = reti(ctx);
    } else if (l_is_first(WRITEI, ctx->current_token->unity)) {
        SS = writei(ctx);
    } else if (l_is_first(EMPTYI, ctx->current_token->unity)) {
        SS = emptyi(ctx);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *assi(l_analysis_ctx *ctx) {
    n_var *S1;
    n_exp *S2;
    n_instr *SS;
    int local_var_id, global_var_id;

    S1 = NULL;
    S2 = NULL;
    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(VAR, ctx->current_token->unity)) {
        S1 = var(ctx);
        if (S1) {
            local_var_id = l_symbols_table_search_local(ctx->symb_stream, S1->name);
            global_var_id = l_symbols_table_search_global(ctx->symb_stream, S1->name);
            if (local_var_id == -1 && global_var_id == -1) {
                ERROR_UNDECLARED_VARIABLE(ctx, S1->name)
                
            } else if (local_var_id != -1) {
                if (ctx->symb_stream->local_table->identifiers[local_var_id]->type == L_TABLE_IDENTIFIER && S1->u.indicee.indice == NULL){
                    ERROR_WRONG_ARRAY_ASSIGNMENT(ctx, S1->name)
                    
                } else if (ctx->symb_stream->local_table->identifiers[local_var_id]->type == L_INTEGER_IDENTIFIER && S1->u.indicee.indice != NULL){
                    ERROR_WRONG_INTEGER_ASSIGNMENT(ctx, S1->name)
                    
                }
            } else if (global_var_id != -1) {
                if (ctx->symb_stream->global_table->identifiers[global_var_id]->type == L_TABLE_IDENTIFIER && S1->u.indicee.indice == NULL){
                    ERROR_WRONG_ARRAY_ASSIGNMENT(ctx, S1->name)
                    
                } else if (ctx->symb_stream->global_table->identifiers[global_var_id]->type == L_INTEGER_IDENTIFIER && S1->u.indicee.indice != NULL){
                    ERROR_WRONG_INTEGER_ASSIGNMENT(ctx, S1->name)
                    
                }
            }

            CONSUME_OR_ERROR(ctx, EQUAL)
            S2 = Exp(ctx);
            CONSUME_OR_ERROR(ctx, SEMICOLON)

            SS = l_ast_n_instr_assign_create(S1, S2);

        } else {
            ERROR_EXCEPTED_BEFORE_EXPRESSION(ctx, "=")
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *bi(l_analysis_ctx *ctx) {
    n_instr *SS;
    n_l_instr *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == OPENING_BRACE) {
        FORWARD(ctx)
        S1 = il(ctx);
        CONSUME(ctx, CLOSNG_BRACE)
        SS = l_ast_n_instr_bloc_create(S1);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_instr *il(l_analysis_ctx *ctx) {
    n_l_instr *SS;
    n_instr *S1;
    n_l_instr *S2;

    SS = NULL;
    S1 = NULL;
    S2 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(I, ctx->current_token->unity)) {
        S1 = i(ctx);
        S2 = il(ctx);
        SS = l_ast_n_l_instr_create(S1, S2);    
    } else if (l_is_follow(IL, ctx->current_token->unity)) {
        /* ε */
          SS = NULL;
    } else {
        DEBUG_PRINT("error il %s %s %d\n", ctx->current_token->word_name, ctx->previous_token->word_name, ctx->current_line)
        if (ctx->current_token->unity == OPENING_PARENTHESIS) {
            ERROR_EXCEPTED(ctx, CLOSNG_BRACE)
        } else if (ctx->current_token->unity == DO) {
            ERROR_WHILE_KEYWORD(ctx, ctx->current_token->word_name)
        } else {
            ERROR_UNCORRECT_STATEMENT(ctx, ctx->current_token->word_name)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *ifi(l_analysis_ctx *ctx) {
    n_exp *S1;
    n_instr *SS;
    n_instr *S2;
    n_instr *S3;

    S1 = NULL;
    S2 = NULL;
    S3 = NULL;
    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == IF) {
        FORWARD(ctx)
        S1 = Exp(ctx);
        CONSUME(ctx, THEN)
        S2 = bi(ctx);
        S3 = elseo(ctx);
        SS = l_ast_n_instr_if_create(S1, S2, S3);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *elseo(l_analysis_ctx *ctx) {
    n_instr *SS;

    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == ELSE) {
        FORWARD(ctx)
        SS = bi(ctx);
    } else if (l_is_follow(ELSEO, ctx->current_token->unity)) {
        /* ε */
        SS = NULL;
    } else {
        ERROR_EXCEPTED(ctx, ELSE)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *whilei(l_analysis_ctx *ctx) {
    n_instr *SS;
    n_exp *S1;
    n_instr *S2;

    SS = NULL;
    S1 = NULL;
    S2 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == WHILE) {
        FORWARD(ctx)
        S1 = Exp(ctx);
        if (S1) {
            CONSUME(ctx, DO)
            S2 = bi(ctx);
            SS = l_ast_n_instr_while_create(S1, S2);
        } else {
            ERROR_EXCEPTED_EXPRESSION(ctx)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *calli(l_analysis_ctx *ctx) {
    n_instr *SS;
    n_call *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(FCALL, ctx->current_token->unity)) {
        S1 = callf(ctx);
        CONSUME(ctx, SEMICOLON)
        SS = l_ast_n_instr_call_create(S1);    
    } else {
        DEBUG_PRINT_STR("error calli\n");
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *reti(l_analysis_ctx *ctx) {
    n_instr *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == RETURN) {
        FORWARD(ctx)
        S1 = Exp(ctx);
        CONSUME(ctx, SEMICOLON)
        SS = l_ast_n_instr_return_create(S1);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *writei(l_analysis_ctx *ctx) {
    n_instr *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == WRITE) {
        FORWARD(ctx)
        CONSUME(ctx, OPENING_PARENTHESIS)
        S1 = Exp(ctx);
        CONSUME(ctx, CLOSING_PARENTHESIS)
        CONSUME(ctx, SEMICOLON)
        SS = l_ast_n_instr_write_create(S1);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_instr *emptyi(l_analysis_ctx *ctx) {
    n_instr *SS;

    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    CONSUME(ctx, SEMICOLON)
    SS = l_ast_n_instr_empty_create();

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *Exp(l_analysis_ctx *ctx) {
    n_exp *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(CONJ, ctx->current_token->unity)) {
        S1 = Conj(ctx);
        SS = expB(ctx, S1);
    } else {
        ERROR_EXCEPTED_ASSIGNMENT_OR_EXPRESSION_BEFORE(ctx, ctx->current_token->word_name)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *expB(l_analysis_ctx *ctx, n_exp *herite) {
    n_exp *SS;
    n_exp *S1;
    operation op;
    n_exp *herite_fils;

    SS = NULL;
    S1 = NULL;
    herite_fils = NULL;

    CHECK_IF_TERMINATED_HERITE(ctx, herite)
    SYNT_WRITE_OPENED_TAG(ctx)

    if (ctx->current_token && ctx->current_token->unity == OR) {
        DEBUG_PRINT_CURRENT_LEX(ctx)

        op = OR_OPERATION;

        FORWARD(ctx)
        S1 = Conj(ctx);
        herite_fils = l_ast_n_exp_op_create(op, herite, S1);
        SS = expB(ctx, herite_fils);
    } else if (l_is_follow(EXPB, ctx->current_token->unity)) {
        /* ε */
        SS = herite;
    } else {
        ERROR_EXCEPTED(ctx, OR)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *Conj(l_analysis_ctx *ctx) {
    n_exp *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(COMP, ctx->current_token->unity)) {
        S1 = comp(ctx);
        SS = conjB(ctx, S1);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *conjB(l_analysis_ctx *ctx, n_exp *herite) {
    n_exp *SS;
    n_exp *S1;
    operation op;
    n_exp *herite_fils;

    SS = NULL;
    S1 = NULL;
    herite_fils = NULL;

    CHECK_IF_TERMINATED_HERITE(ctx, herite)
    SYNT_WRITE_OPENED_TAG(ctx)

    if (ctx->current_token && ctx->current_token->unity == AND) {
        DEBUG_PRINT_CURRENT_LEX(ctx)

        op = AND_OPERATION;

        FORWARD(ctx)
        S1 = comp(ctx);
        herite_fils = l_ast_n_exp_op_create(op, herite, S1);
        SS = conjB(ctx, herite_fils);
    } else if (l_is_follow(CONJB, ctx->current_token->unity)) {
        /* ε */
        SS = herite;
    } else {
        ERROR_EXCEPTED(ctx, AND)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *comp(l_analysis_ctx *ctx) {
    n_exp *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(E, ctx->current_token->unity)) {
        S1 = e(ctx);
        SS = compB(ctx, S1);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *compB(l_analysis_ctx *ctx, n_exp *herite) {
    n_exp *SS;
    n_exp *S1;
    operation op;
    n_exp *herite_fils;

    SS = NULL;
    S1 = NULL;
    herite_fils = NULL;

    CHECK_IF_TERMINATED_HERITE(ctx, herite)
    SYNT_WRITE_OPENED_TAG(ctx)

    if (ctx->current_token &&
        (ctx->current_token->unity == INFERIOR ||
        ctx->current_token->unity == EQUAL)) {
        DEBUG_PRINT_CURRENT_LEX(ctx)

        if (ctx->current_token->unity == INFERIOR) {
            op = INF_OPERATION;
        } else if (herite->u.op_exp.op != INF_OPERATION) {
            op = EQUAL_OPERATION;
        } else {
            op = INFEQ_OPERATION;
        }

        FORWARD(ctx)
        S1 = e(ctx);
        herite_fils = l_ast_n_exp_op_create(op, herite, S1);
        SS = compB(ctx, herite_fils);
    } else if (l_is_follow(COMPB, ctx->current_token->unity)) {
        /* ε */
        SS = herite;
    } else {
        ERROR_EXCEPTED_TWICE(ctx, INFERIOR, EQUAL)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *e(l_analysis_ctx *ctx) {
    n_exp *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(T, ctx->current_token->unity)) {
        S1 = t(ctx);
        SS = eB(ctx, S1);
    } else {
        DEBUG_PRINT_STR("error e\n");
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *eB(l_analysis_ctx *ctx, n_exp *herite) {
    n_exp *SS;
    n_exp *S1;
    operation op;
    n_exp *herite_fils;

    SS = NULL;
    S1 = NULL;
    herite_fils = NULL;

    CHECK_IF_TERMINATED_HERITE(ctx, herite)
    SYNT_WRITE_OPENED_TAG(ctx)

    if (ctx->current_token &&
        (ctx->current_token->unity == ADD ||
        ctx->current_token->unity == SUBSTRACT)) {
        DEBUG_PRINT_CURRENT_LEX(ctx)

        if (ctx->current_token->unity == ADD) {
            op = ADD_OPERATION;
        } else {
            op = SUBSTRACT_OPERATION;
        }

        FORWARD(ctx)
        S1 = t(ctx);
        herite_fils = l_ast_n_exp_op_create(op, herite, S1);
        SS = eB(ctx, herite_fils);
        if (!SS) {
            l_ast_n_exp_destroy(herite_fils);
        }
    } else if (l_is_follow(EB, ctx->current_token->unity)) {
        /* ε */
        SS = herite;
    } else {
        ERROR_EXCEPTED_TWICE(ctx, ADD, SUBSTRACT)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *t(l_analysis_ctx *ctx) {
    n_exp *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(NEG, ctx->current_token->unity)) {
        S1 = neg(ctx);
        SS = tB(ctx, S1);
    } else {
        DEBUG_PRINT_STR("error t\n");
        ERROR_EXCEPTED_EXPRESSION(ctx)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *tB(l_analysis_ctx *ctx, n_exp *herite) {
    n_exp *SS;
    n_exp *S1;
    operation op;
    n_exp *herite_fils;

    SS = NULL;
    S1 = NULL;
    herite_fils = NULL;

    CHECK_IF_TERMINATED_HERITE(ctx, herite)
    SYNT_WRITE_OPENED_TAG(ctx)

    if (ctx->current_token &&
        (ctx->current_token->unity == MULTIPLY ||
        ctx->current_token->unity == DIVIDE)) {
        DEBUG_PRINT_CURRENT_LEX(ctx)

        if (ctx->current_token->unity == MULTIPLY) {
            op = MULTIPLY_OPERATION;
        } else {
            op = DIVIDE_OPERATION;
        }

        FORWARD(ctx)
        S1 = neg(ctx);
        herite_fils = l_ast_n_exp_op_create(op, herite, S1);
        SS = tB(ctx, herite_fils);
    } else if (l_is_follow(TB, ctx->current_token->unity)) {
        /* ε */
        SS = herite;
    } else {
        ERROR_EXCEPTED_TWICE(ctx, MULTIPLY, DIVIDE)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *neg(l_analysis_ctx *ctx) {
    n_exp *SS;

    SS = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)

    if (ctx->current_token && ctx->current_token->unity == NOT) {
        DEBUG_PRINT_CURRENT_LEX(ctx)
        FORWARD(ctx)
        SS = neg(ctx);
    } else if (l_is_first(F, ctx->current_token->unity)) {
        SS = f(ctx);
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_exp *f(l_analysis_ctx *ctx) {
    n_exp *SS;
    int S2, local_var_id, global_var_id;
    n_call *S3;
    n_var *S4;

    SS = NULL;
    S3 = NULL;
    S4 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == OPENING_PARENTHESIS) {
        FORWARD(ctx)
        SS = Exp(ctx);
        CONSUME(ctx, CLOSING_PARENTHESIS)
    } else if (ctx->current_token->unity == NUMBER) {
        S2 = atoi(ctx->current_token->word_name);
        FORWARD(ctx)
        SS = l_ast_n_exp_integer_create(S2);
    } else if (l_is_first(FCALL, ctx->current_token->unity)) {
        S3 = callf(ctx);
        SS = l_ast_n_exp_call_create(S3);
    } else if (l_is_first(VAR, ctx->current_token->unity)) {
        S4 = var(ctx);
        global_var_id = l_symbols_table_search_global(ctx->symb_stream, S4->name);
        local_var_id = l_symbols_table_search_local(ctx->symb_stream, S4->name);
        if (global_var_id == -1 && local_var_id == -1) {
            ERROR_UNDECLARED_VARIABLE(ctx, S4->name)
        
        } else if (global_var_id != -1) {
            if (ctx->symb_stream->global_table->identifiers[global_var_id]->type == L_TABLE_IDENTIFIER && S4->u.indicee.indice == NULL) {
                ERROR_WRONG_ARRAY_ASSIGNMENT(ctx, S4->name)
                
            } else if (ctx->symb_stream->global_table->identifiers[global_var_id]->type == L_INTEGER_IDENTIFIER && S4->u.indicee.indice != NULL) {
                ERROR_WRONG_INTEGER_ASSIGNMENT(ctx, S4->name)
                
            } else {
                SS = l_ast_n_exp_var_create(S4);
            }
        } else if (local_var_id != -1) {
            if (ctx->symb_stream->local_table->identifiers[local_var_id]->type == L_TABLE_IDENTIFIER && S4->u.indicee.indice == NULL) {
                ERROR_WRONG_ARRAY_ASSIGNMENT(ctx, S4->name)
                
            } else if (ctx->symb_stream->local_table->identifiers[local_var_id]->type == L_INTEGER_IDENTIFIER && S4->u.indicee.indice != NULL) {
                ERROR_WRONG_INTEGER_ASSIGNMENT(ctx, S4->name)
                l_ast_n_var_destroy(S4);

            } else {
                SS = l_ast_n_exp_var_create(S4);
            }
        }
    } else if (ctx->current_token->unity == READ) {
        FORWARD(ctx)
        if (ctx->current_token->unity == OPENING_PARENTHESIS) {
            FORWARD(ctx)
            if (ctx->current_token->unity == CLOSING_PARENTHESIS) {
                SS = l_ast_n_exp_read_create();
                FORWARD(ctx)
            } else {
                ERROR_EXCEPTED(ctx, CLOSING_PARENTHESIS)
            }
        } else {
            ERROR_EXCEPTED(ctx, OPENING_PARENTHESIS)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_var *var(l_analysis_ctx *ctx) {
    n_var *SS;
    char *S1;

    SS = NULL;
    S1 = NULL;

    SYNT_WRITE_OPENED_TAG(ctx)

    if (ctx->current_token->unity == VAR_ID) {
        S1 = string_create_from(ctx->current_token->word_name);
        FORWARD(ctx)
        SS = indo(ctx, S1);
        if (!SS) {
            SAFE_FREE(S1)
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_var *indo(l_analysis_ctx *ctx, char *herite) {
    n_var *SS;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == OPENING_BRACKET) {
        FORWARD(ctx)
        S1 = Exp(ctx);
        CONSUME(ctx, CLOSING_BRACKET)
        SS = l_ast_n_var_indicee_create(herite, S1);
    } else if (l_is_follow(INDO, ctx->current_token->unity)) {
        /* ε */
        SS = l_ast_n_var_simple_create(herite);
    } else {
        ERROR_EXCEPTED(ctx, OPENING_BRACKET)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_call *callf(l_analysis_ctx *ctx) {
    n_call *SS;
    n_l_exp *S2;
    char *S1;
    int func_id;

    CHECK_IF_TERMINATED(ctx)
    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    SS = NULL;
    S2 = NULL;
    S1 = NULL;

    if (ctx->current_token->unity == FCT_ID) {
        S1 = string_create_from(ctx->current_token->word_name);
        func_id = l_symbols_table_search_global(ctx->symb_stream, S1);
        if (func_id == -1) {
            ERROR_UNDECLARED_FUNCTION(ctx, S1)
            SAFE_FREE(S1)
            FORWARD(ctx)
        } else {
            FORWARD(ctx)
            if (ctx->current_token->unity == OPENING_PARENTHESIS) {
                FORWARD(ctx)
                S2 = lExp(ctx);
                if (ctx->symb_stream->current_argument_address < ctx->symb_stream->global_table->identifiers[func_id]->complement) {
                    ERROR_TOO_FEW_ARGS(ctx, S1)
                    SAFE_FREE(S1)
                } else if (ctx->symb_stream->current_argument_address > ctx->symb_stream->global_table->identifiers[func_id]->complement) {
                    ERROR_TOO_MANY_ARGS(ctx, S1)
                    SAFE_FREE(S1)
                } else if (ctx->current_token->unity == CLOSING_PARENTHESIS) {
                    FORWARD(ctx)
                    SS = l_ast_n_call_create(S1, S2);
                }
            }
        }
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_exp *lExp(l_analysis_ctx *ctx) {
    n_l_exp *SS, *S2;
    n_l_exp *herite_fils;
    n_exp *S1;

    SS = NULL;
    herite_fils = NULL;
    S1 = NULL;
    S2 = NULL;
    ctx->symb_stream->current_argument_address = 0;

    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (l_is_first(EXP, ctx->current_token->unity)) {
        S1 = Exp(ctx);
        ctx->symb_stream->current_argument_address++;
        S2 = lexpB(ctx);
        SS = l_ast_n_l_exp_create(S1, S2);
    } else if (l_is_follow(LEXP, ctx->current_token->unity)) {
        /* ε */
        SS = NULL;
    } else {
        DEBUG_PRINT_STR("error lexp\n");
        ERROR_EXCEPTED_EXPRESSION(ctx)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

static n_l_exp *lexpB(l_analysis_ctx *ctx) {
    n_l_exp *SS, *S2;
    n_exp *S1;

    SS = NULL;
    S1 = NULL;
    S2 = NULL;

    SYNT_WRITE_OPENED_TAG(ctx)
    DEBUG_PRINT_CURRENT_LEX(ctx)

    if (ctx->current_token->unity == COMMA) {
        FORWARD(ctx)
        S1 = Exp(ctx);
        ctx->symb_stream->current_argument_address++;
        S2 = lexpB(ctx);
        SS = l_ast_n_l_exp_create(S1, S2);
    } else if (l_is_follow(LEXPB, ctx->current_token->unity)) {
        /* ε */
        SS = NULL;
    } else {
        ERROR_EXCEPTED(ctx, COMMA)
    }

    SYNT_WRITE_CLOSED_TAG(ctx)

    return SS;
}

bool l_parser_init(l_analysis_ctx **ctx) {
    char *dump_file_name;

    (*ctx)->current_token = NULL;
    (*ctx)->previous_token = NULL;
    (*ctx)->synt_writer = NULL;
    (*ctx)->dump_synt = false;
    (*ctx)->asynt_writer = NULL;
    (*ctx)->dump_asynt = false;
    (*ctx)->symb_fd = NULL;
    (*ctx)->dump_symb = false;
    (*ctx)->symb_stream = l_symbols_table_stream_create();
    (*ctx)->current_function_name = NULL;

    dump_file_name = create_dump_file_name((*ctx)->source_file->path_name, "mips");
    (*ctx)->mips_stream = l_mips_stream_create(dump_file_name);
    SAFE_FREE(dump_file_name)

    l_init_first();
    l_init_follow();

    return true;
}

void l_parser_uninit(l_analysis_ctx *ctx) {
    if (ctx->dump_synt) {
        xml_writer_destroy(ctx->synt_writer);
    }
    if (ctx->dump_asynt) {
        xml_writer_destroy(ctx->asynt_writer);
    }
    if (ctx->dump_symb) {
        SAFE_FCLOSE(ctx->symb_fd);
    }
    l_symbols_table_stream_destroy(ctx->symb_stream);
    l_mips_stream_destroy(ctx->mips_stream);
}

bool l_parser_process(l_analysis_ctx *ctx) {
    NEXT_LEXEME(ctx)

    return pg(ctx) ? true : false;
}
