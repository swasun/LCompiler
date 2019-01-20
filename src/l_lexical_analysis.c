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

#include "../headers/l_lexical_analysis.h"
#include "../headers/alloc.h"
#include "../headers/bool.h"
#include "../headers/utils.h"
#include "../headers/l_lexical_unity.h"
#include "../headers/l_tokens_definitions.h"
#include "../headers/stacktrace.h"
#include "../headers/error.h"
#include "../headers/check_parameter.h"
#include "../headers/l_analysis_errors.h"
#include "../headers/l_error.h"

#include <stdlib.h>
#include <string.h>

/* Maximum size of a variable name */
#define DEFAULT_VARIABLE_MAX_SIZE 100

/* Maximum size of a function name */
#define DEFAULT_FUNCTION_MAX_SIZE 100

static l_token *is_punctuation_token(char c) {
    if (open_parenthesis_token->word_name[0] == c) {
        return open_parenthesis_token;
    } else if (close_parenthesis_token->word_name[0] == c) {
        return close_parenthesis_token;
    } else if (open_brace_token->word_name[0] == c) {
        return open_brace_token;
    } else if (close_brace_token->word_name[0] == c) {
        return close_brace_token;
    } else if (semicolon_token->word_name[0] == c) {
        return semicolon_token;
    } else if (open_bracket_token->word_name[0] == c) {
        return open_bracket_token;
    } else if (close_bracket_token->word_name[0] == c) {
        return close_bracket_token;
    } else if (comma_token->word_name[0] == c) {
        return comma_token;
    }

    return NULL;
}

static l_token *is_operator_token(char c) {
    if (equal_token->word_name[0] == c) {
        return equal_token;
    } else if (plus_token->word_name[0] == c) {
        return plus_token;
    } else if (sub_token->word_name[0] == c) {
        return sub_token;
    } else if (star_token->word_name[0] == c) {
        return star_token;
    } else if (slash_token->word_name[0] == c) {
        return slash_token;
    } else if (inferior_token->word_name[0] == c) {
        return inferior_token;
    } else if (superior_token->word_name[0] == c) {
        return superior_token;
    } else if (not_token->word_name[0] == c) {
        return not_token;
    } else if (and_token->word_name[0] == c) {
        return and_token;
    } else if (or_token->word_name[0] == c) {
        return or_token;
    }

    return NULL;
}

/**
 * Check if the specified buffer is a simple symbol
 * while traveling through the table of simple symbols (punctuation, operators).
 */
static l_token *is_simple_token(char c) {
    l_token *tok;

    tok = is_punctuation_token(c);
    if (tok) {
        return tok;
    }

    tok = is_operator_token(c);

    return tok;
}

/**
 * Check if the specified buffer is a control instruction
 * while traveling through the table of control instructions.
 */
static l_token *is_control_instruction(l_analysis_ctx *ctx) {
    CHECK_PARAMETER_OR_RETURN(ctx->current_buf)

    if (strcmp(if_token->word_name, ctx->current_buf) == 0) {
       return if_token;
    } else if (strcmp(then_token->word_name, ctx->current_buf) == 0) {
        return then_token;
    } else if (strcmp(else_token->word_name, ctx->current_buf) == 0) {
        return else_token;
    } else if (strcmp(while_token->word_name, ctx->current_buf) == 0) {
        return while_token;
    } else if (strcmp(do_token->word_name, ctx->current_buf) == 0) {
        return do_token;
    } else if (strcmp(return_token->word_name, ctx->current_buf) == 0) {
        return return_token;
    }

    return NULL;
}

/**
 * Check if the specified buffer a variable type
 * while traveling through the table of known variables.
 */
static l_token *is_type(l_analysis_ctx *ctx) {
    CHECK_PARAMETER_OR_RETURN(ctx->current_buf)

    if (strcmp(integer_token->word_name, ctx->current_buf) == 0) {
        return integer_token;
    }

    return NULL;
}

/**
 * Check if the specified buffer is a known file while
 * traveling through the table of known functions.
 */
static l_token *is_known_function(l_analysis_ctx *ctx) {
    CHECK_PARAMETER_OR_RETURN(ctx->current_buf)

    if (strcmp(read_func_token->word_name, ctx->current_buf) == 0) {
        return read_func_token;
    } else if (strcmp(write_func_token->word_name, ctx->current_buf) == 0) {
        return write_func_token;
    }

    return NULL;
}

static bool is_useless_char(l_analysis_ctx *ctx, char c) {
    if (c == '\n') {
        ctx->current_line++;
        return true;
    }

    return c == ' ' || c == '\t' ? true : false;
}

/**
 * Check if the specified buffer is variable name.
 * If it's the case, it returned the corresponding token, else it returned NULL.
 * We specify the file descriptor of the source code to verify
 * the following characters, in order to determine if we reach the end of variable name.
 */ 
static l_token *is_variable_name(l_analysis_ctx *ctx) {
    char c, *tmp_buf;
    unsigned i;

    CHECK_PARAMETER_OR_RETURN(ctx->current_buf)

   /**
    * If there is a variable identifier of one character in
    * this language, and that the first character of the buffer isn't
    * equal to this identifier, we returned NULL because it isn't a
    * variable name.
    */
    if (variable_token && ctx->current_buf[0] != variable_token->word_name[0]) {
        return NULL;
    }

    c = ' ';
    SAFE_ALLOC(tmp_buf, char, ctx->variable_max_size)
    
    if (variable_token) {
        tmp_buf[0] = variable_token->word_name[0];
        i = 1;
    }

   /* We advance through the file until the end of the variable name */
    c = fgetc(ctx->source_file->fd);
    while (c != EOF &&
           !is_useless_char(ctx, c) &&
           !is_simple_token(c) &&
           i < ctx->variable_max_size) {
        tmp_buf[i] = c;
        i++;
        c = fgetc(ctx->source_file->fd);
    }

   /**
    * If the character after the variable name is equal to the end of file character,
    * we returned the end of file token.
    */
    if (c == EOF) {
        return end_of_file_token;
    } else if (i > ctx->variable_max_size) {
        printf("error: Variable '%s' size is too long.\n", ctx->current_buf);
    }
   
   /**
    * If the character after the variable name is a space or a simple symbol,
    * we create a token of variable name.
    */
    else if (is_useless_char(ctx, c) || is_simple_token(c)) {
        ungetc(c, ctx->source_file->fd);
        SAFE_FREE(variable_token->word_name)
        SAFE_ALLOC(variable_token->word_name, char, strlen(tmp_buf) + 1)
        strcpy(variable_token->word_name, tmp_buf);

        SAFE_FREE(variable_token->unity_name)
        SAFE_ALLOC(variable_token->unity_name, char, strlen(tmp_buf) + 1)
        strcpy(variable_token->unity_name, tmp_buf);

        SAFE_FREE(tmp_buf)

        return variable_token;
    }

    SAFE_FREE(tmp_buf)

    return NULL;
}

/**
 * Check if the specified buffer is a function name.
 * If it's the case, it returned the corresponding token, else it returned NULL.
 * We specify the file descriptor of the source code to verify
 * the following characters, in order to determine if we reach the end of the function name.
 */
static l_token *is_function_name(l_analysis_ctx *ctx) {
   char c;

   CHECK_PARAMETER_OR_RETURN(ctx)

   CHECK_PARAMETER_OR_RETURN(ctx->current_buf)

   /* We get the next character in the source code file */
   c = fgetc(ctx->source_file->fd);
   ungetc(c, ctx->source_file->fd);

   /* If the next character is the end of file character, we returned the end of file token */
   if (c == EOF) {
      return end_of_file_token;
   } else if (c == '(' && strlen(ctx->current_buf) > ctx->function_max_size) {
      printf("Function '%s' size is too long.\n", ctx->current_buf);
   }
   /* If the next character is opening parenthesis, it's a function name */
   else if (c == '(') {
      /* We fill the token of function name */
      SAFE_FREE(function_token->word_name)
      SAFE_ALLOC(function_token->word_name, char, strlen(ctx->current_buf) + 1)
      strcpy(function_token->word_name, ctx->current_buf);

      SAFE_FREE(function_token->unity_name)
      SAFE_ALLOC(function_token->unity_name, char, strlen(ctx->current_buf) + 1)
      strcpy(function_token->unity_name, ctx->current_buf);

      return function_token;
   }

   return NULL;
}

/* Check if the character is a digit */
static bool is_digit(char c) {
    switch(c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return true;
    }

    return false;
}

/**
 * Check if the specified buffer is a number.
 * If it's the case, it returned the corresponding token, else it returned NULL.
 * We specify the file descriptor of the source code to verify
 * the following characters, in order to determine if we're at the end of the number.
 */
static l_token *is_number(l_analysis_ctx *ctx) {
    unsigned i;
    char c;

    CHECK_PARAMETER_OR_RETURN(ctx)

    CHECK_PARAMETER_OR_RETURN(ctx->current_buf)

    /* We check if each character of the buffer is a digit */
    for (i = 0; i < strlen(ctx->current_buf); i++) {
        if (!is_digit(ctx->current_buf[i])) {
            return NULL;
        }
    }

    /* We get the next character in the file of the source code */
    c = fgetc(ctx->source_file->fd);
    ungetc(c, ctx->source_file->fd);

    /* If the next character is the character of the end of file, we returned the token of end of file */
    if (c == EOF) {
        return end_of_file_token;
    }
    /* Else if it's not a number, it means that our number is complete */
    else if (!is_digit(c)) {
        /* Clean-up the potential previous number */
        SAFE_FREE(number_token->word_name)

        SAFE_ALLOC(number_token->word_name, char, strlen(ctx->current_buf) + 1)
        strcpy(number_token->word_name, ctx->current_buf);

        SAFE_FREE(number_token->unity_name)
        SAFE_ALLOC(number_token->unity_name, char, strlen(ctx->current_buf) + 1)
        strcpy(number_token->unity_name, ctx->current_buf);

        return number_token;
    }

    return NULL;
}

bool l_lexical_analysis_init(l_analysis_ctx **ctx) {

    CHECK_PARAMETER_OR_RETURN((*ctx)->source_file->path_name)

    (*ctx)->variable_max_size = DEFAULT_VARIABLE_MAX_SIZE;
    (*ctx)->function_max_size = DEFAULT_FUNCTION_MAX_SIZE;

    (*ctx)->current_line = 1;

    (*ctx)->current_buf = NULL;

    (*ctx)->dump_lex = false;

    (*ctx)->lex_fd = NULL;

    return true;
}

void l_lexical_analysis_uninit(l_analysis_ctx *ctx) {
    if (!ctx) {
        return;
    }

    SAFE_FREE(ctx->current_buf)
    SAFE_FCLOSE(ctx->lex_fd);
}

void l_lexical_analysis_set_variable_max_size(l_analysis_ctx *ctx, size_t size) {
    ctx->variable_max_size = size;
}

void l_lexical_analysis_set_function_max_size(l_analysis_ctx *ctx, size_t size) {
    ctx->function_max_size = size;
}

l_token *l_lexical_analysis_next_token(l_analysis_ctx *ctx) {
    char c;
    l_token *tok;

    tok = NULL;

    /**
     * If the buffer isn't empty, we try to determine which
     * type of symbol it is.
     */
    if (ctx->current_buf)
    {
        /* We check the next character in order to improve the heuristic */
        c = fgetc(ctx->source_file->fd);
        ungetc(c, ctx->source_file->fd);
        if (c == EOF ||
            is_useless_char(ctx, c) ||
            is_simple_token(c)) {
            /* Is the buffer a control instruction ? */
            tok = is_control_instruction(ctx);
            if (tok) {
                /* We empty the buffer and we move in found-token state in order to stop the recursion */
                SAFE_FREE(ctx->current_buf)
                return tok;
            }

            /* Is the buffer a variable type ? */
            tok = is_type(ctx);
            if (tok) {
                /* We empty the buffer and we move in found-token state in order to stop recursion */
                SAFE_FREE(ctx->current_buf)
                return tok;
            }

            /* Is the buffer is a known function ? */
            tok = is_known_function(ctx);
            if (tok) {
                /* We empty the buffer and we move in found-token state in order to stop recursion */
                SAFE_FREE(ctx->current_buf)
                return tok;
            }
        }

        /* Is the buffer a variable name ? */
        tok = is_variable_name(ctx);
        if (tok) {
            /* We empty the buffer */
            SAFE_FREE(ctx->current_buf)
            if (tok == end_of_file_token) {
                ctx->eof_state = true;
            }
            return tok;
        }

        /* Is the buffer a function name ? */
        tok = is_function_name(ctx);
        if (tok) {
            /* We empty the buffer and we move in found-token state in order to stop recursion */
            SAFE_FREE(ctx->current_buf)
            return tok;
        }

        /* Is the buffer a number ? */
        tok = is_number(ctx);
        if (tok) {
            SAFE_FREE(ctx->current_buf)
            if (tok == end_of_file_token) {
                ctx->eof_state = true;
            }
            return tok;
        }
    }

    /* We get the current character of the file */
    c = fgetc(ctx->source_file->fd);

    /* If the character is the comment symbol, we move to the next line */
    if (c == comment_token) {
        while (c != '\n' && c != EOF) {
            c = fgetc(ctx->source_file->fd);
        }
        if (c != EOF) {
            ctx->current_line++;
            c = fgetc(ctx->source_file->fd);
        }
    }

    /* If we reach the end of file, we change the state to stop an eventual recursion */
    if (c == EOF) {
        SAFE_FREE(ctx->current_buf)
        ctx->eof_state = true;
        return end_of_file_token;
    }

    /* We skip the spaces, new lines and tabulations */
    if (!is_useless_char(ctx, c)) {
        /**
         * If the character isn't a simple symbol (punctuation, operators),
         * we add the character to the buffer and we call recursively the lexical
         * analysis until find that the buffer means.
         */
        if ((tok = is_simple_token(c)) == NULL) {
            /* If the buffer already exists, we allocate additional memory */
            if (ctx->current_buf) {
                SAFE_REALLOC(ctx->current_buf, char, strlen(ctx->current_buf) + 1, 1)
                sprintf(ctx->current_buf, "%s%c", ctx->current_buf, c);
            }
            /* Else we allocate the buffer */
            else {
                SAFE_ALLOC(ctx->current_buf, char, 2)
                ctx->current_buf[0] = c;
                ctx->current_buf[1] = '\0';
            }

            /**
             * While we didn't find a valid token,
             * we continue recursively the analysis.
             */
            while (!tok) {
                tok = l_lexical_analysis_next_token(ctx);
                if (ctx->eof_state) {
                    SAFE_FREE(ctx->current_buf)
                }
            }
            return tok;
        } else {
            return tok;
        }
    }
    /**
     * If we just skipped a space or new line, et we still doesn't know
     * what the buffer means, it's probably because it is not known in
     * the grammar.
     */
    else if (ctx->current_buf) {
        ERROR_UNDECLARED_VARIABLE(ctx, ctx->current_buf)
        return NULL;
    }

    return l_lexical_analysis_next_token(ctx);
}
