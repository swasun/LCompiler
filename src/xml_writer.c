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

#include "../headers/xml_writer.h"
#include "../headers/alloc.h"

static void indent(xml_writer *writer) {
    int i;

    for (i = 0; i < writer->indent_xml; i++) {
        fprintf(writer->out, "  " );
    }
}

xml_writer *xml_writer_create(const char *file_name) {
    xml_writer *writer;

    SAFE_ALLOC(writer, xml_writer, 1)
    writer->out = fopen(file_name, "w+");
    writer->indent_xml = 0;
    writer->indent_step = 1;

    return writer;
}

void xml_writer_destroy(xml_writer *writer) {
    if (writer) {
        SAFE_FCLOSE(writer->out)
        SAFE_FREE(writer)
    }
}

void xml_write_opened_tag(xml_writer *writer, const char *tag_name) {
    indent(writer);
    writer->indent_xml += writer->indent_step;
    fprintf(writer->out, "<%s>\n", tag_name);
}

void xml_write_closed_tag(xml_writer *writer, const char *tag_name) {
    writer->indent_xml -= writer->indent_step;
    indent(writer);
    fprintf(writer->out, "</%s>\n", tag_name);
}

void xml_write_element(xml_writer *writer, const char *tag_name, char *text) {
    int i = 0;

    indent(writer);
    fprintf(writer->out, "<%s>", tag_name);

    while(text[i] != '\0') {
        if (text[i] == '<') {
            fprintf(writer->out, "&lt;");
        } else if (text[ i ] == '>') {
            fprintf(writer->out, "&gt;" );
        } else if (text[ i ] == '&') {
            fprintf(writer->out, "&amp;");
        } else {
            fprintf(writer->out, "%c", text[i]);
        }
        i++;
    }

    fprintf(writer->out, "</%s>\n", tag_name);
}

void xml_write_text(xml_writer *writer, char *text) {
    indent(writer);
    fprintf(writer->out, "%s\n", text);
}
