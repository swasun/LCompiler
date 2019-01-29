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

#include "../headers/l_mips_sp.h"
#include "../headers/alloc.h"
#include "../headers/utils.h"
#include "../headers/bool.h"

static void l_mips_main(l_mips_stream *stream, n_l_dec *n);

static void l_mips_list_dec(l_mips_stream *stream, n_l_dec *n, int global_state);

static bool l_mips_instr(l_mips_stream *stream, n_instr *n, int nb_args);

static void l_mips_list_instr(l_mips_stream *stream, n_l_instr *n, int nb_args);

static void l_mips_make_operation(l_mips_stream *stream, operation o, char *var, char* addr1, char* addr2);

static void l_mips_list_exp(l_mips_stream *stream, n_l_exp *n);

static int l_mips_exp(l_mips_stream *stream, n_exp *n, char *var);

static void push(l_mips_stream *stream, char *reg);

static void pop(l_mips_stream *stream, char *reg);

static char *create_register_label(l_mips_stream *stream);

static void l_mips_main(l_mips_stream *stream, n_l_dec *n){
    while (strcmp(n->head->name, "main") != 0) {
        n = n->tail;
    }

    fprintf(stream->out, "main:\n");
    push(stream, "$fp");
    fprintf(stream->out, "\tmove $fp, $sp\n");
    push(stream, "$ra");
    l_mips_list_dec(stream, n->head->u.func_dec.variables, 0);
    fprintf(stream->out, "\tsubu $sp, $sp, 4\n");
    l_mips_instr(stream, n->head->u.func_dec.body, 0);
    n_l_dec * S1 = n->head->u.func_dec.variables;
    while(S1 != NULL){
        fprintf(stream->out, "\taddu $sp, $sp, 4\n");
        S1 = S1->tail;
    }
    if(stream->return_value == -1){
        fprintf(stream->out, "\tsw $0, 4($fp)\n");
        pop(stream, "$ra");
        pop(stream, "$fp");
        fprintf(stream->out, "\tjr $ra\n");
    }else{
        stream->return_value = -1;
    }
}

static void l_mips_list_dec(l_mips_stream *stream, n_l_dec *n, int global_state) {
    if (n == NULL) {
        return;
    }
    
    if (n->head->type == FUNC_DEC){
        if (strcmp(n->head->name, "main") != 0){
            fprintf(stream->out, "%s:\n", n->head->name);
            push(stream, "$fp");
            fprintf(stream->out, "\tmove $fp, $sp\n");
            push(stream, "$ra");
            l_mips_list_dec(stream, n->head->u.func_dec.variables, 0);
            int nb_args = 0;
            n_l_dec * S2 = n->head->u.func_dec.param;
            while(S2 != NULL){
                nb_args ++;
                S2 = S2->tail;
            }
            fprintf(stream->out, "\tsubu $sp, $sp, 4\n");
            l_mips_instr(stream, n->head->u.func_dec.body, nb_args);
            n_l_dec * S1 = n->head->u.func_dec.variables;
            while(S1 != NULL){
                fprintf(stream->out, "\taddu $sp, $sp, 4\n");
                S1 = S1->tail;
            }
            if(stream->return_value == -1){
                fprintf(stream->out, "\tsw $0, %d($fp)\n", 4*(nb_args + 1));
                pop(stream, "$ra");
                pop(stream, "$fp");
                fprintf(stream->out, "\tjr $ra\n");
            }else{
                stream->return_value = -1;
            }
        }
    }
    
    else if (n->head->type == VAR_DEC) {
        if (global_state == 1){
            fprintf(stream->out, "\t%s : .word 0\n", n->head->name);
        } else {
            fprintf(stream->out, "\tsubu $sp, $sp, 4\n");
        }
    }
     
    else if (n->head->type == TAB_DEC) {
        fprintf(stream->out, "\t%s : .space %d\n", n->head->name, (4 * n->head->u.tab_dec.size));
    }
    
    l_mips_list_dec(stream, n->tail, global_state);
}

static bool l_mips_instr(l_mips_stream *stream, n_instr *n, int nb_args) {
    char *reg;
    int reg_addr, counter;
    n_exp *S1;
    n_var *S2;

    switch (n->type) {
        case WRITE_INST:
            S1 = n->u.write_instr.expression;
            l_mips_exp(stream, S1, "$a0");
            fprintf(stream->out, "\tli $v0, 4\n");
            fprintf(stream->out, "\tsyscall\n");    
        break;

        case ASSIGN_INST:
            S2 = n->u.assign_instr.var;
            reg_addr = l_mips_exp(stream, n->u.assign_instr.exp, "$t");
            if (reg_addr != -1) {
                reg = create_register_label(stream);
                pop(stream, reg);
                fprintf(stream->out, "\tsw %s, %d($fp)\n", reg, -8 /*+ 4 * <var address>*/);
                SAFE_FREE(reg)
            }
        break;

        case BLOC_INST:
            l_mips_list_instr(stream, n->u.list, nb_args);
        break;

        case IF_INST:
            counter = stream->if_counter;
            stream->if_counter++;
            reg_addr = l_mips_exp(stream, n->u.if_instr.test, "$t");
            reg = create_register_label(stream);
            pop(stream, reg);
            if (n->u.if_instr.else_instr == NULL) {
                fprintf(stream->out, "\tbeq %s, $0, after_si%d\n", reg, counter);
                l_mips_instr(stream, n->u.if_instr.then_instr, nb_args);
            } else {
                fprintf(stream->out, "\tbeq %s, $0, else%d\n", reg, counter);
                l_mips_instr(stream, n->u.if_instr.then_instr, nb_args);
                fprintf(stream->out, "\tj after_si%d\n", counter);
                fprintf(stream->out, "else%d :", counter);
                l_mips_instr(stream, n->u.if_instr.else_instr, nb_args);
            }
            fprintf(stream->out, "after_si%d :", counter);
            SAFE_FREE(reg)
        break;

        case WHILE_INST:
            counter = stream->while_counter;
            stream->while_counter++;
            fprintf(stream->out, "tq%d :", counter);
            reg_addr = l_mips_exp(stream, n->u.while_instr.test, "$t");
            reg = create_register_label(stream);
            pop(stream, reg);
            fprintf(stream->out, "\tbeq %s, $0, after_tq%d\n", reg, counter);
            l_mips_instr(stream, n->u.while_instr.do_instr, nb_args);
            fprintf(stream->out, "\tj tq%d\n", counter);
            fprintf(stream->out, "after_tq%d :", counter);
            SAFE_FREE(reg)
        break;
        
        case EMPTY_INST:
            fprintf(stream->out, "\t# empty instruction\n");
        break;
        
        case INCR_INST:
            l_mips_exp(stream, n->u.incr, "$t");
        break;
        
        case DO_INST:
            counter = stream->do_counter;
            stream->do_counter++;
            fprintf(stream->out, "do%d :", counter);
            reg_addr = l_mips_exp(stream, n->u.do_instr.test, "$t");
            reg = create_register_label(stream);
            pop(stream, reg);
            fprintf(stream->out, "\tbeq %s, $0, after_do%d\n", reg, counter);
            l_mips_instr(stream, n->u.do_instr.do_instr, nb_args);
            fprintf(stream->out, "\tj do%d\n", counter);
            fprintf(stream->out, "after_do%d :", counter);
            SAFE_FREE(reg)
        break;
        
        case CALL_INST:
            push(stream, "$ra");
            l_mips_list_exp(stream, n->u.call->args);
            fprintf(stream->out, "\tjal %s\n", n->u.call->function);
        break;
        
        case RETURN_INST:
            l_mips_exp(stream, n->u.return_instr.expression, "$t");
            reg = create_register_label(stream);
            pop(stream, reg);
            fprintf(stream->out, "\tsw %s, %d($fp)\n", reg, 4 * (nb_args + 1));
            pop(stream, "$ra");
            pop(stream, "$fp");
            fprintf(stream->out, "\tjr $ra\n");
            stream->return_value = 1;
            SAFE_FREE(reg)
        break;
    }
}

static void l_mips_list_instr(l_mips_stream *stream, n_l_instr *n, int nb_args) {
    if (n == NULL || n->head == NULL) {
        return;
    }
    
    l_mips_instr(stream, n->head, nb_args);

    l_mips_list_instr(stream, n->tail, nb_args);
}

static void l_mips_make_operation(l_mips_stream *stream, operation o, char *var, char* addr1, char* addr2) {
    char *str;
    
    if (var == "$t") {
        str = create_register_label(stream);
    } else {
        str = NULL;
    }

    switch (o) {
        case ADD_OPERATION:
            if (var == "$t") {
                fprintf(stream->out, "\tadd %s, %s, %s\n", str, addr1, addr2); 
                push(stream, str);
            } else {
                fprintf(stream->out, "\tadd %s, %s, %s\n", var, addr1, addr2); 
            }
        break;

        case SUBSTRACT_OPERATION:
            if (var == "$t") {
                fprintf(stream->out, "\tsub %s, %s, %s\n", str, addr1, addr2); 
                push(stream, str);
            } else {
                fprintf(stream->out, "\tsub %s, %s, %s\n", var, addr1, addr2); 
            }
        break;

        case MULTIPLY_OPERATION:
            fprintf(stream->out, "\tmult %s, %s\n", addr1, addr2);
            if (var == "$t") {
                fprintf(stream->out, "\tmflo %s\n", str);
                push(stream, str);
            } else {
                fprintf(stream->out, "\tmflo %s\n", var);
            }
        break;

        case DIVIDE_OPERATION:
            fprintf(stream->out, "\tdiv %s, %s\n", addr1, addr2);
            if (var == "$t") {
                fprintf(stream->out, "\tmflo %s\n", str);
                push(stream, str);
            } else {
                fprintf(stream->out, "\tmflo %s\n", var);
            }
        break;

        case MODULO_OPERATION:
            fprintf(stream->out, "\tdiv %s, %s\n", addr1, addr2);
            if (var == "$t") {
                fprintf(stream->out, "\tmfhi %s\n", str);
                push(stream, str);
            } else {
                fprintf(stream->out, "\tmfhi %s\n", var);
            }
        break;

        case EQUAL_OPERATION:
            fprintf(stream->out, "\tli %s, -1\n", str);
            fprintf(stream->out, "\tbeq %s, %s, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli %s, 0\n", str);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            push(stream, str);
        break;

        case DIFF_OPERATION:
            fprintf(stream->out, "\tli %s, -1\n", str);
            fprintf(stream->out, "\tbne %s, %s, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli %s, 0\n", str);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            push(stream, str);
        break;

        case INF_OPERATION:
            fprintf(stream->out, "\tli %s, -1\n", str);
            fprintf(stream->out, "\tblt %s, %s, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli %s, 0\n", str);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            push(stream, str);
        break;

        case SUP_OPERATION:
            fprintf(stream->out, "\tli %s, -1\n", str);
            fprintf(stream->out, "\tbgt %s, %s, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli %s, 0\n", str);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            push(stream, str);
        break;

        case INFEQ_OPERATION:
            fprintf(stream->out, "\tli %s, -1\n", str);
            fprintf(stream->out, "\tble %s, %s, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli %s, 0\n", str);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            push(stream, str);
        break;

        case SUPEQ_OPERATION:
            fprintf(stream->out, "\tli %s, -1\n", str);
            fprintf(stream->out, "\tbge %s, %s, e%d\n", addr1, addr2, stream->else_counter);
            fprintf(stream->out, "\tli %s, 0\n", str);
            fprintf(stream->out, "e%d:", stream->else_counter);
            stream->else_counter ++;
            push(stream, str);
        break;

        case OR_OPERATION:
            fprintf(stream->out, "\tor %s, %s, %s\n", str, addr1, addr2);
            push(stream, str);
        break;

        case AND_OPERATION:
            fprintf(stream->out, "\tand %s, %s, %s\n", str, addr1, addr2);
            push(stream, str);
        break;

        case NOT_OPERATION:
            fprintf(stream->out, "\tnot %s, %s\n", str, addr1);
            push(stream, str);
        break;
    }

    SAFE_FREE(str)
}

static void l_mips_list_exp(l_mips_stream *stream, n_l_exp *n) {
    if (n == NULL || n->head == NULL) {
        return;
    }
    
    l_mips_exp(stream, n->head, "$t");
    
    
    l_mips_list_exp(stream, n->tail);
}

int l_mips_exp(l_mips_stream *stream, n_exp *n, char *var) {
    char *str2, *reg, *addrtab, *number, *indice_i_buffer, *current_register_buffer;
    
    if (n == NULL) {
        return -1;
    }

    switch (n->type) {
        case VAR_EXP:
            if (n->u.var->type == SIMPLE_VAR) {
                if (var == "$a0"){
                    fprintf(stream->out, "\tlw %s, %s\n", var, n->u.var->name);
                } else {
                    push(stream, n->u.var->name);
                }
            } else if (n->u.var->u.indicee.indice->type == INT_EXP) {
                if (var == "$a0"){
                    fprintf(stream->out, "\tlw %s, %s+%d\n", var, n->u.var->name, 4 * n->u.var->u.indicee.indice->u.i);
                } else {
                    /* Copy in a string of '4 * n->u.var->u.indicee.indice->u.i' */
                    SAFE_ALLOC(indice_i_buffer, char, get_digits_number(4 * n->u.var->u.indicee.indice->u.i) + 1)
                    sprintf(indice_i_buffer, "%d", 4 * n->u.var->u.indicee.indice->u.i);

                    SAFE_ALLOC(reg, char, strlen(n->u.var->name) + strlen("+") + strlen(indice_i_buffer) + strlen("\n") + 1)
                    sprintf(reg, "%s+%s\n", n->u.var->name, indice_i_buffer);
                    push(stream, reg);

                    SAFE_FREE(indice_i_buffer)
                    SAFE_FREE(reg)
                }
            } else {
                number = create_register_label(stream);
                str2 = create_register_label(stream);
                addrtab = create_register_label(stream);

                int t = l_mips_exp(stream, n->u.var->u.indicee.indice, "$t");
                pop(stream, str2);
                fprintf(stream->out, "\tli %s, 4\n", number);
                fprintf(stream->out, "\tmult %s, %s\n", str2, number);
                fprintf(stream->out, "\tmflo %s\n", addrtab);

                /* Copy the current register into a string */
                SAFE_ALLOC(current_register_buffer, char, get_digits_number(stream->current_register))
                sprintf(current_register_buffer, "%d", stream->current_register);

                SAFE_ALLOC(reg, char, strlen(addrtab) + strlen("($t") + strlen(current_register_buffer) + strlen(")\n"))
                sprintf(reg, "%s($t%s)\n", addrtab, current_register_buffer);
                if (var == "$a0") {
                    fprintf(stream->out, "\tlw %s, %s\n", var, reg);
                } else {
                    push(stream, reg);
                }
                SAFE_FREE(reg)
            }
        break;

        case OP_EXP:
            reg = create_register_label(stream);
            stream->current_register ++;
            int t = l_mips_exp(stream, n->u.op_exp.op1, "$t");
            pop(stream, reg);
            
            if (n->u.op_exp.op == OR_OPERATION || n->u.op_exp.op == AND_OPERATION) {
                if (n->u.op_exp.op == OR_OPERATION) {
                    fprintf(stream->out, "\tbne %s, $0, e%d\n", reg, stream->else_counter);
                } else {
                    fprintf(stream->out, "\tbeq %s, $0, e%d\n", reg, stream->else_counter);
                }
            }
            
            str2 = create_register_label(stream);
            stream->current_register ++;
            t = l_mips_exp(stream, n->u.op_exp.op2, "$t");
            pop(stream, str2);
            if (n->u.op_exp.op != OR_OPERATION && n->u.op_exp.op != AND_OPERATION) {
                l_mips_make_operation(stream, n->u.op_exp.op, var, reg, str2);
                SAFE_FREE(str2)
                SAFE_FREE(reg)
                return 0;
            }

            push(stream, str2);

            SAFE_FREE(str2)
            SAFE_FREE(reg)
        break;

        case INT_EXP:
            if (var == "$t") {
                str2 = create_register_label(stream);
                fprintf(stream->out, "\tli %s, %d\n", str2, n->u.i);
                push(stream, str2);
                SAFE_FREE(str2)
            } else {
                fprintf(stream->out, "\tli %s, %d\n", var, n->u.i);
            }
        break;

        case CALL_EXP:
            /*push(stream, "$ra");*/
            l_mips_list_exp(stream, n->u.call->args);
            fprintf(stream->out, "\tjal %s\n", n->u.call->function);
            n_l_exp * S1 = n->u.call->args;
            while(S1 != NULL){
                fprintf(stream->out, "\taddu $sp, $sp, 4\n");
                S1 = S1->tail;
            }
        break;

        case READ_EXP:
            str2 = create_register_label(stream);
            fprintf(stream->out, "\tli $v0, 5\n");
            fprintf(stream->out, "\tsyscall\n");
            fprintf(stream->out, "\tmove %s, $v0\n", str2);
            push(stream, str2);
            SAFE_FREE(str2)
        break;
    }
    
    return 0;
}

/* Allocate a word on the queue, then copy reg to the top of the queue */
static void push(l_mips_stream *stream, char *reg){
    fprintf(stream->out, "\tsubu $sp, $sp, 4\n");
    fprintf(stream->out, "\tsw %s, 0($sp)\n", reg);
}

/* Copy the top of the queue to reg, then deallocate the word on top of queue */
static void pop(l_mips_stream *stream, char *reg){
    fprintf(stream->out, "\tlw %s, 0($sp)\n", reg);
    fprintf(stream->out, "\taddu $sp, $sp, 4\n");
}

static char *create_register_label(l_mips_stream *stream) {
    char *label, *current_register_buffer;

    /* Copy the current register into a string */
    SAFE_ALLOC(current_register_buffer, char, get_digits_number(stream->current_register) + 1)
    sprintf(current_register_buffer, "%d", stream->current_register);

    /**
     * Allocation of the necessary memory of the label: the size of '$t' + the size of
     * the current register + 1 for the '\0'
     */
    SAFE_ALLOC(label, char, strlen("$t") + strlen(current_register_buffer) + 1)
    
    sprintf(label, "$t%s", current_register_buffer);

    SAFE_FREE(current_register_buffer)

    return label;
}

void l_mips_sp_pg(l_mips_stream *stream, n_prog *n) {
    fprintf(stream->out, ".data\n");
    l_mips_list_dec(stream, n->variables, 1);
    fprintf(stream->out, ".text\n");
    fprintf(stream->out, "__start:\n");
    fprintf(stream->out, "\tjal main\n");
    fprintf(stream->out, "\tli $v0, 10\n");
    fprintf(stream->out, "\tsyscall\n");
    l_mips_main(stream, n->functions);
    l_mips_list_dec(stream, n->functions, 0);
}
