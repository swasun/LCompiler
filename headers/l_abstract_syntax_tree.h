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

#ifndef L_ABSTRACT_SYNTAX_TREE_H
#define L_ABSTRACT_SYNTAX_TREE_H

typedef struct n_l_instr n_l_instr;
typedef struct n_instr n_instr;
typedef struct n_exp n_exp;
typedef struct n_l_exp n_l_exp;
typedef struct n_var n_var;
typedef struct n_l_dec n_l_dec;
typedef struct n_dec n_dec;
typedef struct n_prog n_prog;
typedef struct n_call n_call;

/*-------------------------------------------------------------------------*/

struct n_prog {
    n_l_dec *variables;
    n_l_dec *functions;
};

n_prog *l_ast_n_prog_create(n_l_dec *variables, n_l_dec *functions);

void l_ast_n_prog_destroy(n_prog *prog);

/*-------------------------------------------------------------------------*/

struct n_dec {
    enum {
        FUNC_DEC,
        VAR_DEC, 
        TAB_DEC
    } type;

    char *name;

    union {
        struct {
            n_l_dec *param; 
            n_l_dec *variables; 
            n_instr *body;
        } func_dec;

        struct {
            int type;
        } var_dec;

        struct {
            int size;
        } tab_dec;
    } u;
};

n_dec *l_ast_n_dec_var_create(char *name);
n_dec *l_ast_n_dec_tab_create(char *name, int size);
n_dec *l_ast_n_dec_func_create(char *name, n_l_dec *param, n_l_dec *variables, n_instr *body);

/*-------------------------------------------------------------------------*/

typedef enum {
    ADD_OPERATION,
    SUBSTRACT_OPERATION,
    MULTIPLY_OPERATION,
    DIVIDE_OPERATION,
    MODULO_OPERATION,
    EQUAL_OPERATION,
    DIFF_OPERATION,
    INF_OPERATION,
    SUP_OPERATION,
    INFEQ_OPERATION,
    SUPEQ_OPERATION,
    OR_OPERATION,
    AND_OPERATION,
    NOT_OPERATION
} operation; 

struct n_exp {
    enum { 
        VAR_EXP,
        OP_EXP,
        INT_EXP,
        CALL_EXP,
        READ_EXP
    } type;

    union {
        struct {
            operation op; 
            struct n_exp *op1; 
            struct n_exp *op2;
        } op_exp;
        
        n_var *var;
        int i;
        n_call *call;
    } u;
};

n_exp *l_ast_n_exp_op_create(operation type, n_exp *op1, n_exp *op2);
n_exp *l_ast_n_exp_integer_create(int i);
n_exp *l_ast_n_exp_var_create(n_var *var);
void l_ast_n_var_dec_destroy(n_dec *n);
void l_ast_call_exp_destroy(n_exp *n);
n_exp *l_ast_n_exp_call_create(n_call *app);
n_exp *l_ast_n_exp_read_create(void);
n_exp *l_ast_n_exp_incr_create(n_var *var);
void l_ast_n_exp_destroy(n_exp *n);

/*-------------------------------------------------------------------------*/

struct n_instr {
    enum {
        INCR_INST, 
        ASSIGN_INST, 
        IF_INST, 
        DO_INST, 
        WHILE_INST, 
        CALL_INST, 
        RETURN_INST, 
        WRITE_INST, 
        EMPTY_INST, 
        BLOC_INST
    } type;

    union {
        n_exp *incr;

        struct {
            n_exp *test; 
            struct n_instr *then_instr; 
            struct n_instr *else_instr;
        } if_instr;

        struct {
            n_exp *test; 
            struct n_instr *do_instr;
        } while_instr;

        struct {
            n_exp *test; 
            struct n_instr *do_instr;
        } do_instr;

        n_call *call;
        
        struct {
            n_var *var; 
            n_exp *exp;
        } assign_instr;

        struct {
            n_exp *expression;
        } return_instr;

        struct {
            n_exp *expression;
        } write_instr;

        n_l_instr *list;
    } u;
};

n_instr *l_ast_n_instr_incr_create(n_exp *incr);
n_instr *l_ast_n_instr_if_create(n_exp *test, n_instr *then_instr, n_instr *else_instr);
n_instr *l_ast_n_instr_bloc_create(n_l_instr *list);
n_instr *l_ast_n_instr_while_create(n_exp *test, n_instr *then_instr);
n_instr *l_ast_n_instr_then_create(n_instr *then_instr, n_exp *test);
n_instr *l_ast_n_instr_assign_create(n_var *var, n_exp *exp);
n_instr *l_ast_n_instr_call_create(n_call *call);
n_instr *l_ast_n_instr_return_create(n_exp *expression);
n_instr *l_ast_n_instr_write_create(n_exp *expression);
n_instr *l_ast_n_instr_empty_create();

/*-------------------------------------------------------------------------*/
struct n_call {
    char *function;
    n_l_exp *args;
};

n_call *l_ast_n_call_create(char *function, n_l_exp *args);

/*-------------------------------------------------------------------------*/
struct n_var {
    enum {
        SIMPLE_VAR, INDICEE_VAR
    } type;

    char *name;

    union {
        struct {
            n_exp *indice;
        } indicee;
    } u;
};

n_var *l_ast_n_var_simple_create(char *name);
n_var *l_ast_n_var_indicee_create(char *name, n_exp *indice);
void l_ast_n_var_destroy(n_var *n);

/*-------------------------------------------------------------------------*/
struct n_l_exp {
    n_exp *head;
    struct n_l_exp *tail;
};

n_l_exp *l_ast_n_l_exp_create(n_exp *head, n_l_exp *tail);

/*-------------------------------------------------------------------------*/
struct n_l_instr {
    n_instr *head;
    struct n_l_instr *tail;
};

n_l_instr *l_ast_n_l_instr_create(n_instr *head, n_l_instr *tail);

/*-------------------------------------------------------------------------*/

struct n_l_dec {
    n_dec *head;
    struct n_l_dec *tail;
};

n_l_dec *l_ast_n_l_dec_create(n_dec *head, n_l_dec *tail);
void l_ast_l_dec_destroy(n_l_dec *n);
/*-------------------------------------------------------------------------*/

#endif
