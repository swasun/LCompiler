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

#include "../headers/l_mips.h"
#include "../headers/alloc.h"
#include "../headers/bool.h"

static int l_mips_exp(l_mips_stream *stream, n_exp *n, char *var, bool is_index);

static void l_mips_list_dec(l_mips_stream *stream, n_l_dec *n);

static void l_mips_instr(l_mips_stream *stream, n_instr *n);

static void l_mips_list_instr(l_mips_stream *stream, n_l_instr *n);

static int l_mips_make_operation(l_mips_stream *stream, operation o, char *var, int addr1, int addr2);

static int l_mips_exp(l_mips_stream *stream, n_exp *n, char *var, bool is_index);

static void l_mips_list_dec(l_mips_stream *stream, n_l_dec *n) {
    if (n == NULL) {
        return;
    }

    if (n->head->type == VAR_DEC) {
        fprintf(stream->out, "\t%s : .word 0\n", n->head->name);
    }
     
    if (n->head->type == TAB_DEC) {
        fprintf(stream->out, "\t%s : .space %d\n", n->head->name, (4 * n->head->u.tab_dec.size));
    }

    l_mips_list_dec(stream, n->tail);
}

static void l_mips_instr(l_mips_stream *stream, n_instr *n) {
    if (n->type == WRITE_INST) {
        n_exp * S1 = n->u.write_instr.expression;
        int addr_reg = l_mips_exp(stream, S1, "$a0", false);
        fprintf(stream->out, "\tli $v0, 4\n");
        fprintf(stream->out, "\tsyscall\n");
    }

    if (n->type == ASSIGN_INST) {
        n_var * S2 = n->u.assign_instr.var;
        int addr_reg = l_mips_exp(stream, n->u.assign_instr.exp, "$t", false);
        if (addr_reg != -1) {
            if (S2->type == SIMPLE_VAR) {
                fprintf(stream->out, "\tsw $t%d, %s\n", addr_reg, S2->name);
            } else {
                int addr_tab = l_mips_exp(stream, S2->u.indicee.indice, "$t", true);
                fprintf(stream->out, "\tsw $t%d, %s+%d\n", addr_reg, S2->name, addr_tab);
            }
        }
    }
    
    if(n->type == BLOC_INST){
        l_mips_list_instr(stream, n->u.list);
    }
    
    if(n->type == IF_INST) {
        int c = stream->if_counter;
        stream->if_counter++;
        int adr = l_mips_exp(stream, n->u.if_instr.test, "$t", false);
        if(n->u.if_instr.else_instr == NULL){
            fprintf(stream->out, "\tbeq $t%d, $0, after_si%d\n", adr, c);
            l_mips_instr(stream, n->u.if_instr.then_instr);
        }else{
            fprintf(stream->out, "\tbeq $t%d, $0, else%d\n", adr, c);
            l_mips_instr(stream, n->u.if_instr.then_instr);
            fprintf(stream->out, "\tj after_si%d\n", c);
            fprintf(stream->out, "else%d :", c);
            l_mips_instr(stream, n->u.if_instr.else_instr);
        }
        fprintf(stream->out, "after_si%d :", c);
    }
    
    if(n->type == WHILE_INST){
        int c = stream->while_counter;
        stream->while_counter++;
        fprintf(stream->out, "tq%d :", c);
        int adr = l_mips_exp(stream, n->u.while_instr.test, "$t", false);
        fprintf(stream->out, "\tbeq $t%d, $0, after_tq%d\n", adr, c);
        l_mips_instr(stream, n->u.while_instr.do_instr);
        fprintf(stream->out, "\tj tq%d\n", c);
        fprintf(stream->out, "after_tq%d :", c);
    }
}

static void l_mips_list_instr(l_mips_stream *stream, n_l_instr *n) {
    n_exp *S1;
    n_var *S2;
    int addr_reg, addr_tab;

    if (n == NULL || n->head == NULL) {
        return;
    }
    
    l_mips_instr(stream, n->head);

    l_mips_list_instr(stream, n->tail);
}

static int l_mips_make_operation(l_mips_stream *stream, operation o, char *var, int addr1, int addr2) {
    int addr = stream->current_register;

    switch (o) {
        case ADD_OPERATION:
            if (var == "$t") {
                fprintf(stream->out, "\tadd %s%d, $t%d, $t%d\n", var, addr, addr1, addr2); 
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tadd %s, $t%d, $t%d\n", var, addr1, addr2); 
            }
        break;

        case SUBSTRACT_OPERATION:
            if (var == "$t") {
                fprintf(stream->out, "\tsub %s%d, $t%d, $t%d\n", var, addr, addr1, addr2); 
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tsub %s, $t%d, $t%d\n", var, addr1, addr2); 
            }
        break;

        case MULTIPLY_OPERATION:
            fprintf(stream->out, "\tmult $t%d, $t%d\n", addr1, addr2);
            if (var == "$t") {
                fprintf(stream->out, "\tmflo %s%d\n", var, addr);
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tmflo %s\n", var);
            }
        break;

        case DIVIDE_OPERATION:
            fprintf(stream->out, "\tdiv $t%d, $t%d\n", addr1, addr2);
            if (var == "$t") {
                fprintf(stream->out, "\tmflo %s%d\n", var, addr);
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tmflo %s\n", var);
            }
        break;

        case MODULO_OPERATION:
            fprintf(stream->out, "\tdiv $t%d, $t%d\n", addr1, addr2);
            if (var == "$t") {
                fprintf(stream->out, "\tmfhi %s%d\n", var, addr);
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tmfhi %s\n", var);
            }
        break;

        case EQUAL_OPERATION:
            fprintf(stream->out, "\tli $t%d, -1\n", addr);
            fprintf(stream->out, "\tbeq $t%d, $t%d, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli $t%d, 0\n", addr);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            stream->current_register++;
        break;

        case DIFF_OPERATION:
            fprintf(stream->out, "\tli $t%d, -1\n", addr);
            fprintf(stream->out, "\tbne $t%d, $t%d, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli $t%d, 0\n", addr);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            stream->current_register++;
        break;

        case INF_OPERATION:
            fprintf(stream->out, "\tli $t%d, -1\n", addr);
            fprintf(stream->out, "\tblt $t%d, $t%d, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli $t%d, 0\n", addr);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            stream->current_register++;
        break;

        case SUP_OPERATION:
            fprintf(stream->out, "\tli $t%d, -1\n", addr);
            fprintf(stream->out, "\tbgt $t%d, $t%d, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli $t%d, 0\n", addr);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            stream->current_register++;
        break;

        case INFEQ_OPERATION:
            fprintf(stream->out, "\tli $t%d, -1\n", addr);
            fprintf(stream->out, "\tble $t%d, $t%d, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli $t%d, 0\n", addr);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            stream->current_register++;
        break;

        case SUPEQ_OPERATION:
            fprintf(stream->out, "\tli $t%d, -1\n", addr);
            fprintf(stream->out, "\tbge $t%d, $t%d, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli $t%d, 0\n", addr);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            stream->current_register++;
        break;

        case OR_OPERATION:
            fprintf(stream->out, "\tor $t%d, $t%d, $t%d\n", addr, addr1, addr2);
            stream->current_register++;
        break;

        case AND_OPERATION:
            fprintf(stream->out, "\tand $t%d, $t%d, $t%d\n", addr, addr1, addr2);
            stream->current_register++;
        break;

        case NOT_OPERATION:
            fprintf(stream->out, "\tnot $t%d, $t%d\n", addr, addr1);
            stream->current_register++;
        break;
    }
    
    return addr;
}

int l_mips_exp(l_mips_stream *stream, n_exp *n, char *var, bool is_index) {
    int result = stream->current_register;
    if (n == NULL) {
        return -1;
    }

    if (n->type == VAR_EXP) {
        if (n->u.var->type == SIMPLE_VAR) {
            if (var == "$t") {
                fprintf(stream->out, "\tlw %s%d, %s\n", var, result, n->u.var->name);
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tlw %s, %s\n", var, n->u.var->name);
            }
        } else {
            if (var == "$t") {
                fprintf(stream->out, "\tlw %s%d, %s+%d\n", var, result, n->u.var->name, l_mips_exp(stream, n->u.var->u.indicee.indice, "$t", true));
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tlw %s, %s+%d\n", var, n->u.var->name, l_mips_exp(stream, n->u.var->u.indicee.indice, "$t", true));
            }
        }
    }
    if (n->type == OP_EXP) {
        int addr1 = l_mips_exp(stream, n->u.op_exp.op1, "$t", false);
        if(n->u.op_exp.op == OR_OPERATION || n->u.op_exp.op == AND_OPERATION){
            if(n->u.op_exp.op == OR_OPERATION){
                fprintf(stream->out, "\tbne $t%d, $0, e%d\n", addr1, stream->else_counter);
            }else{
                fprintf(stream->out, "\tbeq $t%d, $0, e%d\n", addr1, stream->else_counter);
            }
        }
        int addr2 = l_mips_exp(stream, n->u.op_exp.op2, "$t", false);
        if(n->u.op_exp.op != OR_OPERATION && n->u.op_exp.op != AND_OPERATION){
            int addr3 = l_mips_make_operation(stream, n->u.op_exp.op, var, addr1, addr2);
            return addr3;
        }
        return addr2;
    }
    if (n->type == INT_EXP) {
        if (is_index != true) {
            if (var == "$t") {
                fprintf(stream->out, "\tli %s%d, %d\n", var, result, n->u.i);
                stream->current_register++;
            } else {
                fprintf(stream->out, "\tli %s, %d\n", var, n->u.i);
            }
        } else {
            return (4 * n->u.i);
        }
    }
    if (n->type == CALL_EXP) {
        fprintf(stream->out, "\tCALL : not implemented yet.\n");
        return -1;
    }
    if (n->type == READ_EXP) {
        fprintf(stream->out, "\tli $v0, 5\n");
        fprintf(stream->out, "\tsyscall\n");
        fprintf(stream->out, "\tmove $t%d, $v0\n", result);
        stream->current_register++;
    }
    
    return result;
}

void l_mips_pg(l_mips_stream *stream, n_prog *n) {
    n_l_dec *S1;

    S1 = n->functions;

    fprintf(stream->out, ".data\n");
    l_mips_list_dec(stream, n->variables);
    fprintf(stream->out, ".text\n");
    while (S1 != NULL && S1->head != NULL) {
        l_mips_list_instr(stream, S1->head->u.func_dec.body->u.list);
        S1 = S1->tail;
    }
}

