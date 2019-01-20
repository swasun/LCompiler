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

#include "../headers/l_abstract_syntax_tree_print.h"
#include "../headers/alloc.h"

static void l_instr_print(n_l_instr *n, xml_writer *writer);

static void instr_print(n_instr *n, xml_writer *writer);

static void instr_if_print(n_instr *n, xml_writer *writer);

static void instr_while_print(n_instr *n, xml_writer *writer);

static void instr_assign_print(n_instr *n, xml_writer *writer);

static void instr_call_print(n_instr *n, xml_writer *writer);

static void instr_return_print(n_instr *n, xml_writer *writer);

static void instr_write_print(n_instr *n, xml_writer *writer);

static void l_exp_print(n_l_exp *n, xml_writer *writer);

static void exp_print(n_exp *n, xml_writer *writer);

static void var_exp_print(n_exp *n, xml_writer *writer);

static void op_exp_print(n_exp *n, xml_writer *writer);

static void int_exp_print(n_exp *n, xml_writer *writer);

static void read_exp_print(n_exp *n, xml_writer *writer);

static void call_exp_print(n_exp *n, xml_writer *writer);

static void l_dec_print(n_l_dec *n, xml_writer *writer);

static void dec_print(n_dec *n, xml_writer *writer);

static void func_dec_print(n_dec *n, xml_writer *writer);

static void var_dec_print(n_dec *n, xml_writer *writer);

static void tab_dec_print(n_dec *n, xml_writer *writer);

static void var_print(n_var *n, xml_writer *writer);

static void var_simple_print(n_var *n, xml_writer *writer);

static void var_indicee_print(n_var *n, xml_writer *writer);

static void call_print(n_call *n, xml_writer *writer);

void l_ast_n_prog_print(n_prog *n, xml_writer *writer) {
    char *fct = "prog";

    xml_write_opened_tag(writer, fct);
    l_dec_print(n->variables, writer);
    l_dec_print(n->functions, writer);
    xml_write_closed_tag(writer, fct);
}

void l_instr_print(n_l_instr *n, xml_writer *writer) {
    char *fct = "l_instr";

    if (n) {
        xml_write_opened_tag(writer, fct);
        instr_print(n->head, writer);
        l_instr_print(n->tail, writer);
        xml_write_closed_tag(writer, fct);
    }
}

void instr_print(n_instr *n, xml_writer *writer) {
    if (n) {
        if (n->type == BLOC_INST) {
            l_instr_print(n->u.list, writer);
        } else if (n->type == ASSIGN_INST) {
            instr_assign_print(n, writer);
        } else if (n->type == IF_INST) {
            instr_if_print(n, writer);
        } else if (n->type == WHILE_INST) {
            instr_while_print(n, writer);
        } else if (n->type == CALL_INST) {
            instr_call_print(n, writer);
        } else if (n->type == RETURN_INST) {
            instr_return_print(n, writer);
        } else if (n->type == WRITE_INST) {
            instr_write_print(n, writer);
        }
    }
}

void instr_if_print(n_instr *n, xml_writer *writer) {
    char *fct = "instr_if";

    xml_write_opened_tag(writer, fct);
    exp_print(n->u.if_instr.test, writer);
    instr_print(n->u.if_instr.then_instr, writer);
    if (n->u.if_instr.else_instr){
        instr_print(n->u.if_instr.else_instr, writer);
    }
    xml_write_closed_tag(writer, fct);
}

void instr_while_print(n_instr *n, xml_writer *writer) {
    char *fct = "instr_while";

    xml_write_opened_tag(writer, fct);
    exp_print(n->u.while_instr.test, writer);
    instr_print(n->u.while_instr.do_instr, writer);
    xml_write_closed_tag(writer, fct);
}

void instr_assign_print(n_instr *n, xml_writer *writer) {
    char *fct = "instr_assign";

    xml_write_opened_tag(writer, fct);
    var_print(n->u.assign_instr.var, writer);
    exp_print(n->u.assign_instr.exp, writer);
    xml_write_closed_tag(writer, fct);
}

void instr_call_print(n_instr *n, xml_writer *writer) {
    char *fct = "instr_call";

    xml_write_opened_tag(writer, fct);
    call_print(n->u.call, writer);
    xml_write_closed_tag(writer, fct);
}

void call_print(n_call *n, xml_writer *writer) {
    char *fct = "call";

    xml_write_opened_tag(writer, fct);
    if (n) {
        xml_write_text(writer, n->function);
        l_exp_print(n->args, writer);
    }
    xml_write_closed_tag(writer, fct);
}

void instr_return_print(n_instr *n, xml_writer *writer) {
    char *fct = "instr_return";

    xml_write_opened_tag(writer, fct);
    exp_print(n->u.return_instr.expression, writer);
    xml_write_closed_tag(writer, fct);
}

void instr_write_print(n_instr *n, xml_writer *writer) {
    char *fct = "instr_write";

    xml_write_opened_tag(writer, fct);
    exp_print(n->u.write_instr.expression, writer);
    xml_write_closed_tag(writer, fct);
}

void l_exp_print(n_l_exp *n, xml_writer *writer) {
    char *fct = "l_exp";

    xml_write_opened_tag(writer, fct);
    if (n) {
        exp_print(n->head, writer);
        l_exp_print(n->tail, writer);
    }
    xml_write_closed_tag(writer, fct);
}

void exp_print(n_exp *n, xml_writer *writer) {
    if (!n) {
        return;
    }

    if (n->type == VAR_EXP) {
        var_exp_print(n, writer);
    } else if (n->type == OP_EXP) {
        op_exp_print(n, writer);
    } else if (n->type == INT_EXP) {
        int_exp_print(n, writer);
    } else if (n->type == CALL_EXP) {
        call_exp_print(n, writer);
    } else if (n->type == READ_EXP) {
        read_exp_print(n, writer);
    }
}

void var_exp_print(n_exp *n, xml_writer *writer) {
    char *fct = "varExp";

    xml_write_opened_tag(writer, fct);
    var_print(n->u.var, writer);
    xml_write_closed_tag(writer, fct);
}

void op_exp_print(n_exp *n, xml_writer *writer) {
    char *fct = "op_exp";

    xml_write_opened_tag(writer, fct);
    if (n->u.op_exp.op == ADD_OPERATION) {
        xml_write_text(writer, "add");
    } else if (n->u.op_exp.op == SUBSTRACT_OPERATION) {
        xml_write_text(writer, "substract");
    } else if (n->u.op_exp.op == MULTIPLY_OPERATION) {
        xml_write_text(writer, "multiply");
    } else if (n->u.op_exp.op == DIVIDE_OPERATION) {
        xml_write_text(writer, "divide");
    } else if (n->u.op_exp.op == EQUAL_OPERATION) {
        xml_write_text(writer, "equal");
    } else if (n->u.op_exp.op == DIFF_OPERATION) {
        xml_write_text(writer, "diff");
    } else if (n->u.op_exp.op == INF_OPERATION) {
        xml_write_text(writer, "inf");
    } else if (n->u.op_exp.op == INFEQ_OPERATION) {
        xml_write_text(writer, "infeq");
    } else if (n->u.op_exp.op == OR_OPERATION) {
        xml_write_text(writer, "or");
    } else if (n->u.op_exp.op == AND_OPERATION) {
        xml_write_text(writer, "and");
    } else if (n->u.op_exp.op == NOT_OPERATION) {
        xml_write_text(writer, "not");
    }
    if (n->u.op_exp.op1 != NULL) {
        exp_print(n->u.op_exp.op1, writer);
    }
    if (n->u.op_exp.op2 != NULL) {
        exp_print(n->u.op_exp.op2, writer);
    }
    xml_write_closed_tag(writer, fct);
}

void int_exp_print(n_exp *n, xml_writer *writer) {
    char text[50]; /* Max 50 digits */

    sprintf(text, "%d", n->u.i);
    xml_write_element(writer, "intExp", text);
}

void read_exp_print(n_exp *n, xml_writer *writer) {
    char *fct = "readExp";

    xml_write_opened_tag(writer, fct);
    xml_write_closed_tag(writer, fct);
}

void call_exp_print(n_exp *n, xml_writer *writer) {
    char *fct = "callExp";

    xml_write_opened_tag(writer, fct);
    call_print(n->u.call, writer);
    xml_write_closed_tag(writer, fct);
}

void l_dec_print(n_l_dec *n, xml_writer *writer) {
    char *fct = "l_dec";

    if (n) {
        xml_write_opened_tag(writer, fct);
        dec_print(n->head, writer);
        l_dec_print(n->tail, writer);
        xml_write_closed_tag(writer, fct);
    }
}

void dec_print(n_dec *n, xml_writer *writer) {
    if (n) {
        if (n->type == FUNC_DEC) {
            func_dec_print(n, writer);
        } else if (n->type == VAR_DEC) {
            var_dec_print(n, writer);
        } else if (n->type == TAB_DEC) {
            tab_dec_print(n, writer);
        }
    }
}

void func_dec_print(n_dec *n, xml_writer *writer) {
    char *fct = "funcDec";

    xml_write_opened_tag(writer, fct);
    xml_write_text(writer, n->name);
    l_dec_print(n->u.func_dec.param, writer);
    l_dec_print(n->u.func_dec.variables, writer);
    instr_print(n->u.func_dec.body, writer);
    xml_write_closed_tag(writer, fct);
}

void var_dec_print(n_dec *n, xml_writer *writer) {
    xml_write_element(writer, "varDec", n->name);
}

void tab_dec_print(n_dec *n, xml_writer *writer) {
    char text[100]; /* Max. 100 chars name tab + size */
    sprintf(text, "%s[%d]", n->name, n->u.tab_dec.size);
    xml_write_element(writer, "tabDec", text);
}

void var_print(n_var *n, xml_writer *writer) {
    if (n->type == SIMPLE_VAR) {
        var_simple_print(n, writer);
    } else if (n->type == INDICEE_VAR) {
        var_indicee_print(n, writer);
    }
}

void var_simple_print(n_var *n, xml_writer *writer) {
    xml_write_element(writer, "var_simple", n->name);
}

void var_indicee_print(n_var *n, xml_writer *writer) {
    char *fct = "var_indicee";

    xml_write_opened_tag(writer, fct);
    xml_write_element(writer, "var_base_tableau", n->name);
    exp_print(n->u.indicee.indice, writer);
    xml_write_closed_tag(writer, fct);
}
