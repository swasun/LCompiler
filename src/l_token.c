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

#include "../headers/l_token.h"
#include "../headers/alloc.h"
#include "../headers/utils.h"
#include "../headers/check_parameter.h"
#include "../headers/l_lexical_unity.h"

#include <string.h>

static char *get_lexical_unity_name_by_value(int value) {
    #define NAME(V) case V: return #V;
      switch (value) {
         NAME(IF)
         NAME(THEN)
         NAME(ELSE)
         NAME(WHILE)
         NAME(DO)
         NAME(RETURN)
         NAME(READ)
         NAME(WRITE)
         NAME(MAIN)
         NAME(VAR_ID)
         NAME(FCT_ID)
         NAME(OPENING_PARENTHESIS)
         NAME(CLOSING_PARENTHESIS)
         NAME(OPENING_BRACE)
         NAME(CLOSNG_BRACE)
         NAME(SEMICOLON)
         NAME(OPENING_BRACKET)
         NAME(CLOSING_BRACKET)
         NAME(COMMA)
         NAME(EQUAL)
         NAME(ADD)
         NAME(SUBSTRACT)
         NAME(MULTIPLY)
         NAME(DIVIDE)
         NAME(INFERIOR)
         NAME(SUPERIOR)
         NAME(AND)
         NAME(OR)
         NAME(NOT)
         NAME(END)
         NAME(EPSILON)
         NAME(INTEGER)
      }
      return NULL;
   #undef NAME
}

l_token *l_token_create(int unity, char *word_name, char *word_type, char *unity_name) {
   l_token *lex;

   CHECK_PARAMETER_OR_RETURN(unity >= 0)
   CHECK_PARAMETER_OR_RETURN(word_name)
   CHECK_PARAMETER_OR_RETURN(word_type)
   CHECK_PARAMETER_OR_RETURN(unity_name)

   SAFE_ALLOC(lex, l_token, 1)
   lex->unity = unity;
   SAFE_ALLOC(lex->word_name, char, strlen(word_name) + 1)
   strcpy(lex->word_name, word_name);
   SAFE_ALLOC(lex->word_type, char, strlen(word_type) + 1)
   strcpy(lex->word_type, word_type);
   SAFE_ALLOC(lex->unity_name, char, strlen(unity_name) + 1)
   strcpy(lex->unity_name, unity_name);

   return lex;
}

l_token *l_token_create_token(int unity, char *word_name) {
   return l_token_create(unity, word_name, "symbol", get_lexical_unity_name_by_value(unity));
}

l_token *l_token_create_func(int unity, char *word_name) {
   return l_token_create(unity, word_name, "function_id", word_name);
}

void l_token_destroy(l_token *lex) {
   if (lex) {
      SAFE_FREE(lex->word_name)
      SAFE_FREE(lex->word_type)
      SAFE_FREE(lex->unity_name)
      SAFE_FREE(lex)
   }
}

void l_token_write(FILE *lex_fd, l_token *lex) {
   if (!lex_fd || !lex) {
      return;
   }

   fprintf(lex_fd, "%s\t%s %s\n", lex->word_name, lex->word_type, lex->unity_name);
}
