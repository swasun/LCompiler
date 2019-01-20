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

#ifndef UTILS_H
#define UTILS_H

#include "bool.h"

bool last_char_is(char *str, char c);

/* Remove the first character of a string */
void remove_first_char(char *str);

void remove_last_char(char *str);

char *string_create_from(const char *str);

const char *get_file_name_extension(const char *file_name);

int get_digits_number(int number);

char *create_dump_file_name(const char *source_file_name, char *type);

char *get_file_name_from_path(char *path);

bool is_file_exists(const char *file_name);

bool is_dir_exists(const char *file_name);

unsigned short count_dir_files(const char *dir_name, bool recursively);

char **list_directory(char *dir_name, unsigned short *files, bool recursively);

#endif
