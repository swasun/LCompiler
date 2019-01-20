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

#ifndef XML_WRITER_H
#define XML_WRITER_H

#include <stdio.h>

typedef struct {
    int indent_xml;
    int indent_step;
    FILE *out;
} xml_writer;

xml_writer *xml_writer_create(const char *file_name);

void xml_writer_destroy(xml_writer *writer);

void xml_write_opened_tag(xml_writer *writer, const char *tag_name);

void xml_write_closed_tag(xml_writer *writer, const char *tag_name);

void xml_write_element(xml_writer *writer, const char *tag_name, char *value);

void xml_write_text(xml_writer *writer, char *text);

#endif
