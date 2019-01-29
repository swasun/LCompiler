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

#include "../headers/l_abstract_syntax_tree.h"
#include "../headers/utils.h"
#include "../headers/alloc.h"
#include "../headers/check_parameter.h"

#include <stdio.h>
#include <stdlib.h>

void l_ast_n_call_destroy(n_call *n);

void l_ast_l_exp_destroy(n_l_exp *n);

void l_ast_n_instr_destroy(n_instr *n);

void l_ast_n_instr_write_destroy(n_instr *n);

void l_ast_n_instr_assign_destroy(n_instr *n);

void l_ast_n_exp_var_destroy(n_exp *n);


n_call *l_ast_n_call_create(char *function, n_l_exp *args) {
    n_call *n;

    SAFE_ALLOC(n, n_call, 1)
    n->function = function;
    n->args = args;

    return n;
}

void l_ast_n_call_exp_destroy(n_exp *n) {
    l_ast_n_call_destroy(n->u.call);
}

void l_ast_n_call_destroy(n_call *n) {
    if (n) {
        SAFE_FREE(n->function)
        l_ast_l_exp_destroy(n->args);
        SAFE_FREE(n)
    }
}

n_prog *l_ast_n_prog_create(n_l_dec *variables, n_l_dec *functions) {
    n_prog *n;

    SAFE_ALLOC(n, n_prog, 1)
    n->variables = variables;
    n->functions = functions;

    return n;
}

void l_ast_n_prog_destroy(n_prog *n) {
    if (n) {
        l_ast_l_dec_destroy(n->variables);
        l_ast_l_dec_destroy(n->functions);
        SAFE_FREE(n)
    }
}

n_var *l_ast_n_var_simple_create(char *name) {
    n_var *n;

    SAFE_ALLOC(n, n_var, 1)
    n->type = SIMPLE_VAR;
    n->name = name;

    return n;
}

n_var *l_ast_n_var_indicee_create(char *name, n_exp *indice) {
    n_var *n;

    SAFE_ALLOC(n, n_var, 1)
    n->type = INDICEE_VAR;
    n->name = name;
    n->u.indicee.indice = indice;

    return n;
}

void l_ast_n_var_destroy(n_var *n) {
    if (n) {
        SAFE_FREE(n->name)
        if (n->type == INDICEE_VAR) {
            l_ast_n_exp_destroy(n->u.indicee.indice);
        }
        SAFE_FREE(n)
    }
}

n_exp *l_ast_n_exp_op_create(operation op, n_exp *op1, n_exp *op2) {
    n_exp *n;

    SAFE_ALLOC(n, n_exp, 1)
    n->type = OP_EXP;
    n->u.op_exp.op = op;
    n->u.op_exp.op1 = op1;
    n->u.op_exp.op2 = op2;

    return n;
}

void l_ast_n_exp_destroy(n_exp *n) {
    if (!n) {
        return;
    }
    
    if (n->type == VAR_EXP) {
        l_ast_n_exp_var_destroy(n);
        SAFE_FREE(n)
    } else if (n->type == OP_EXP) {
        if (n->u.op_exp.op1 != NULL) {
            l_ast_n_exp_destroy(n->u.op_exp.op1);
        }
        if (n->u.op_exp.op2 != NULL) {
            l_ast_n_exp_destroy(n->u.op_exp.op2);
        }
        SAFE_FREE(n)
    } else if (n->type == INT_EXP) {
        SAFE_FREE(n)
    } else if (n->type == CALL_EXP) {
        l_ast_n_call_exp_destroy(n);
        SAFE_FREE(n)
    } else if (n->type == READ_EXP) {
        SAFE_FREE(n)
    }
}

n_exp *l_ast_n_exp_call_create(n_call *app) {
    n_exp *n;    

    SAFE_ALLOC(n, n_exp, 1)
    n->type = CALL_EXP;
    n->u.call = app;

    return n;
}

n_exp *l_ast_n_exp_var_create(n_var *var) {
    n_exp *n;

    SAFE_ALLOC(n, n_exp, 1)
    n->type = VAR_EXP;
    n->u.var = var;

    return n;
}

void l_ast_n_exp_var_destroy(n_exp *n) {
    if (n) {
        l_ast_n_var_destroy(n->u.var);
    }
}

n_exp *l_ast_n_exp_integer_create(int i) {
    n_exp *n;

    SAFE_ALLOC(n, n_exp, 1)
    n->type = INT_EXP;
    n->u.i = i;

    return n;
}

n_exp *l_ast_n_exp_read_create() {
    n_exp *n;

    SAFE_ALLOC(n, n_exp, 1)
    n->type = READ_EXP;

    return n;
}

n_l_exp *l_ast_n_l_exp_create(n_exp *head, n_l_exp *tail) {
    n_l_exp *n;

    SAFE_ALLOC(n, n_l_exp, 1)
    n->head = head;
    n->tail = tail;

    return n;
}

void l_ast_l_exp_destroy(n_l_exp *n) {
    if (n) {
        l_ast_n_exp_destroy(n->head);
        l_ast_l_exp_destroy(n->tail);
        SAFE_FREE(n)
    }
}

n_instr *l_ast_n_instr_if_create(n_exp *test, n_instr *then_instr, n_instr *else_instr) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = IF_INST;
    n->u.if_instr.test = test;
    n->u.if_instr.then_instr = then_instr;
    n->u.if_instr.else_instr = else_instr;

    return n;
}

void l_ast_n_instr_if_destroy(n_instr *n) {
    l_ast_n_exp_destroy(n->u.if_instr.test);
    l_ast_n_instr_destroy(n->u.if_instr.then_instr);
    if (n->u.if_instr.else_instr){
        l_ast_n_instr_destroy(n->u.if_instr.else_instr);
    }
}

n_instr *l_ast_n_instr_while_create(n_exp *test, n_instr *do_instr) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = WHILE_INST;
    n->u.while_instr.test = test;
    n->u.while_instr.do_instr = do_instr;

    return n;
}

void l_ast_n_instr_while_destroy(n_instr *n) {
    l_ast_n_exp_destroy(n->u.while_instr.test);
    l_ast_n_instr_destroy(n->u.while_instr.do_instr);   
}

n_instr *l_ast_n_instr_then_create(n_instr *do_instr, n_exp *test) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = DO_INST;
    n->u.while_instr.test = test;
    n->u.while_instr.do_instr = do_instr;

    return n;
}

n_instr *l_ast_n_instr_assign_create(n_var *var, n_exp *exp) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = ASSIGN_INST;
    n->u.assign_instr.var = var;
    n->u.assign_instr.exp = exp;

    return n;
}

n_l_instr *l_ast_n_l_instr_create(n_instr *head, n_l_instr *tail) {
    n_l_instr *n;

    SAFE_ALLOC(n, n_l_instr, 1)
    n->head = head;
    n->tail = tail;

    return n;
}

n_instr *l_ast_n_instr_bloc_create(n_l_instr *list) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = BLOC_INST;
    n->u.list = list;

    return n;
}

void l_ast_l_instr_destroy(n_l_instr *n) {
    if (n) {
        l_ast_n_instr_destroy(n->head);
        l_ast_l_instr_destroy(n->tail);
        SAFE_FREE(n)
    }
}

n_instr *l_ast_n_instr_call_create(n_call *app) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = CALL_INST;
    n->u.call = app;

    return n;
}

void l_ast_n_instr_call_destroy(n_instr *n) {
    l_ast_n_call_destroy(n->u.call);
}

n_instr *l_ast_n_instr_write_create(n_exp *expression) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = WRITE_INST;
    n->u.write_instr.expression = expression;

    return n;
}

void l_ast_n_instr_write_destroy(n_instr *n) {
    l_ast_n_exp_destroy(n->u.write_instr.expression);
}

n_instr *l_ast_n_instr_return_create(n_exp *expression) {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = RETURN_INST;
    n->u.return_instr.expression = expression;

    return n;
}

void l_ast_n_instr_return_destroy(n_instr *n) {
    l_ast_n_exp_destroy(n->u.return_instr.expression);
}

n_instr *l_ast_n_instr_empty_create() {
    n_instr *n;

    SAFE_ALLOC(n, n_instr, 1)
    n->type = EMPTY_INST;

    return n;
}

void l_ast_n_instr_destroy(n_instr *n) {
    if (n) {
        if (n->type == BLOC_INST) {
            l_ast_l_instr_destroy(n->u.list);
        } else if (n->type == ASSIGN_INST) {
            l_ast_n_instr_assign_destroy(n);
        } else if (n->type == IF_INST) {
            l_ast_n_instr_if_destroy(n);
        } else if (n->type == WHILE_INST) {
            l_ast_n_instr_while_destroy(n);
        } else if (n->type == CALL_INST) {
            l_ast_n_instr_call_destroy(n);  
        } else if (n->type == RETURN_INST) {
            l_ast_n_instr_return_destroy(n);
        } else if (n->type == WRITE_INST) {
            l_ast_n_instr_write_destroy(n);
        }
        SAFE_FREE(n)
    }
}

void l_ast_n_instr_assign_destroy(n_instr *n) {
    l_ast_n_var_destroy(n->u.assign_instr.var);
    l_ast_n_exp_destroy(n->u.assign_instr.exp);

}

n_dec *l_ast_n_dec_var_create(char *name) {
    n_dec *n;

    SAFE_ALLOC(n, n_dec, 1)
    n->type = VAR_DEC;
    n->name = name;

    return n;
}

void l_ast_n_var_dec_destroy(n_dec *n) {
    if (n) {
        SAFE_FREE(n->name)
        SAFE_FREE(n)
    }
}

n_dec *l_ast_n_dec_tab_create(char *name, int size) {
    n_dec *n;

    SAFE_ALLOC(n, n_dec, 1)
    n->type = TAB_DEC;
    n->name = name;
    n->u.tab_dec.size = size;

    return n;
}

void l_ast_tab_dec_destroy(n_dec *n) {
    SAFE_FREE(n->name)
    SAFE_FREE(n)
}

n_dec *l_ast_n_dec_func_create(char *name, n_l_dec *param, n_l_dec *variables, n_instr *body) {
    n_dec *n;

    SAFE_ALLOC(n, n_dec, 1)
    n->type = FUNC_DEC;
    n->name = name;
    n->u.func_dec.param = param;
    n->u.func_dec.variables = variables;
    n->u.func_dec.body = body;

    return n;
}

void l_ast_func_dec_destroy(n_dec *n) {
    SAFE_FREE(n->name)
    l_ast_l_dec_destroy(n->u.func_dec.param);
    l_ast_l_dec_destroy(n->u.func_dec.variables);
    l_ast_n_instr_destroy(n->u.func_dec.body);
    SAFE_FREE(n)
}

void l_ast_dec_destroy(n_dec *n) {
    if (n) {
        if (n->type == FUNC_DEC) {
            l_ast_func_dec_destroy(n);
        } else if (n->type == VAR_DEC) {
            l_ast_n_var_dec_destroy(n);
        } else if (n->type == TAB_DEC) {
            l_ast_tab_dec_destroy(n);
        }
    }
}

n_l_dec *l_ast_n_l_dec_create(n_dec *head, n_l_dec *tail) {
    n_l_dec *n;

    SAFE_ALLOC(n, n_l_dec, 1)
    n->head = head;
    n->tail = tail;

    return n;
}

void l_ast_l_dec_destroy(n_l_dec *n) {
    if (n) {
        l_ast_dec_destroy(n->head);
        l_ast_l_dec_destroy(n->tail);
        SAFE_FREE(n)
    }
}
