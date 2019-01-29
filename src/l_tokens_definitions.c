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

#include "../headers/l_tokens_definitions.h"
#include "../headers/l_lexical_unity.h"
#include "../headers/bool.h"

static bool loaded = false;

static void init_punctuation() {
    open_parenthesis_token = l_token_create_token(OPENING_PARENTHESIS, "(");
    close_parenthesis_token = l_token_create_token(CLOSING_PARENTHESIS, ")");
    open_brace_token = l_token_create_token(OPENING_BRACE, "{");
    close_brace_token = l_token_create_token(CLOSNG_BRACE, "}");
    semicolon_token = l_token_create_token(SEMICOLON, ";");
    open_bracket_token = l_token_create_token(OPENING_BRACKET, "[");
    close_bracket_token = l_token_create_token(CLOSING_BRACKET, "]");
    comma_token = l_token_create_token(COMMA, ",");
}

static void init_operators() {
    equal_token = l_token_create_token(EQUAL, "=");
    plus_token = l_token_create_token(ADD, "+");
    sub_token = l_token_create_token(SUBSTRACT, "-");
    star_token = l_token_create_token(MULTIPLY, "*");
    slash_token = l_token_create_token(DIVIDE, "/");
    inferior_token = l_token_create_token(INFERIOR, "<");
    superior_token = l_token_create_token(SUPERIOR, ">");
    not_token = l_token_create_token(NOT, "!");
    and_token = l_token_create_token(AND, "&");
    or_token = l_token_create_token(OR, "|");
}

static void init_control_instructions() {
    if_token = l_token_create_token(IF, "if");
    then_token = l_token_create_token(THEN, "then");
    else_token = l_token_create_token(ELSE, "else");
    while_token = l_token_create_token(WHILE, "while");
    do_token = l_token_create_token(DO, "do");
    return_token = l_token_create_token(RETURN, "return");
}

static void init_known_functions() {
    read_func_token = l_token_create_func(READ, "read");
    write_func_token = l_token_create_func(WRITE, "write");
    main_func_token = l_token_create_func(MAIN, "main");
}

static void clean_up_punctuation() {
    l_token_destroy(open_parenthesis_token);
    l_token_destroy(close_parenthesis_token);
    l_token_destroy(open_brace_token);
    l_token_destroy(close_brace_token);
    l_token_destroy(semicolon_token);
    l_token_destroy(open_bracket_token);
    l_token_destroy(close_bracket_token);
    l_token_destroy(comma_token);
}

static void clean_up_operators() {
    l_token_destroy(equal_token);
    l_token_destroy(plus_token);
    l_token_destroy(sub_token);
    l_token_destroy(star_token);
    l_token_destroy(slash_token);
    l_token_destroy(inferior_token);
    l_token_destroy(superior_token);
    l_token_destroy(not_token);
    l_token_destroy(and_token);
    l_token_destroy(or_token);
}

static void clean_up_control_instructions() {
    l_token_destroy(if_token);
    l_token_destroy(then_token);
    l_token_destroy(else_token);
    l_token_destroy(while_token);
    l_token_destroy(do_token);
    l_token_destroy(return_token);
}

static void clean_up_known_functions() {
    l_token_destroy(read_func_token);
    l_token_destroy(write_func_token);
    l_token_destroy(main_func_token);
}

void l_tokens_init() {
    if (loaded) {
        return;
    }

    variable_token = l_token_create(VAR_ID, "$", "var_id", "");
    function_token = l_token_create_func(FCT_ID, "function_id");

    number_token = l_token_create(NUMBER, "", "number", "");

    end_of_file_token = l_token_create(END, "END", "", "");

    comment_token = '#';

    integer_token = l_token_create_token(INTEGER, "integer");

    epsilon_token = l_token_create_token(EPSILON, "epsilon");

    init_punctuation();
    init_operators();
    init_control_instructions();
    init_known_functions();

    loaded = true;
}

void l_tokens_destroy() {
    if (!loaded) {
       return;
    }

    clean_up_punctuation();

    clean_up_operators();

    clean_up_control_instructions();

    clean_up_known_functions();

    l_token_destroy(end_of_file_token);

    l_token_destroy(variable_token);

    l_token_destroy(function_token);

    l_token_destroy(number_token);

    l_token_destroy(integer_token);

    l_token_destroy(epsilon_token);

    loaded = false;
}

char *l_token_get_name_from(int unity) {
    switch (unity) {
        case IF:
            return if_token->word_name;
        break;

        case THEN:
            return then_token->word_name;
        break;

        case ELSE:
            return else_token->word_name;
        break;

        case WHILE:
            return while_token->word_name;
        break;

        case DO:
            return do_token->word_name;
        break;

        case RETURN:
            return return_token->word_name;
        break;

        case READ:
            return read_func_token->word_name;
        break;

        case WRITE:
            return write_func_token->word_name;
        break;

        case MAIN:
            return main_func_token->word_name;
        break;

        case VAR_ID:
            return variable_token->word_name;
        break;

        case FCT_ID:
            return function_token->word_name;
        break;

        case OPENING_PARENTHESIS:
            return open_parenthesis_token->word_name;
        break;

        case CLOSING_PARENTHESIS:
            return close_parenthesis_token->word_name;
        break;

        case OPENING_BRACE:
            return open_brace_token->word_name;
        break;

        case CLOSNG_BRACE:
            return close_brace_token->word_name;
        break;

        case SEMICOLON:
            return semicolon_token->word_name;
        break;

        case OPENING_BRACKET:
            return open_bracket_token->word_name;
        break;

        case CLOSING_BRACKET:
            return close_bracket_token->word_name;
        break;

        case COMMA:
            return comma_token->word_name;
        break;

        case EQUAL:
            return equal_token->word_name;
        break;

        case ADD:
            return plus_token->word_name;
        break;

        case SUBSTRACT:
            return sub_token->word_name;
        break;

        case MULTIPLY:
            return star_token->word_name;
        break;

        case DIVIDE:
            return slash_token->word_name;
        break;

        case INFERIOR:
            return inferior_token->word_name;
        break;

        case SUPERIOR:
            return superior_token->word_name;
        break;

        case AND:
            return and_token->word_name;
        break;

        case OR:
            return or_token->word_name;
        break;

        case NOT:
            return not_token->word_name;
        break;

        case END:
            return end_of_file_token->word_name;
        break;

        case EPSILON:
            return epsilon_token->word_name;
        break;

        case INTEGER:
            return integer_token->word_name;
        break;
    }

    return NULL;
}
