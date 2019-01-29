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
