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

#include "../headers/main.h"

static void print_usage(char **argv) {
    fprintf(stdout, "\n");
    fprintf(stdout, "Usage: %s -f <source_file_name> | --file <source_file_name> | -d <source_dir_name> --dir <source_dir_name> [--lex | --synt | --asynt | --symb | --stack | --tests]\n", argv[0]);
    fprintf(stdout, "-f: Mandatory argument. Spécifie le fichier source .l.\n");
    fprintf(stdout, "--lex: Optional argument. Create a file 'source_file_name.lex' that contains the detail of the lexical analysis.\n");
    fprintf(stdout, "--synt: Optional argument. Create a file 'source_file_name.synt' that contains the detail of the syntactic analysis.\n");
    fprintf(stdout, "--asynt: Optional argument. Create a file 'source_file_name.asynt' that contains the detail of the abstract syntax.\n");
    fprintf(stdout, "--symb: Optional argument. Create a file 'source_file_name.symb' that contains the detail of the symbol table.\n");
    fprintf(stdout, "--stack: Optional argument. Create a file 'stacktrace' that contains the evantual internal errors of the compiler.\n");
    fprintf(stdout, "--tests: Optional argument. Create a file 'tests' that contains the detail of the executation of the compilation tests, as well as eventual errors.\n");
    fprintf(stdout, "\n");
}

static struct option long_options[] = {
    { "file", required_argument, NULL, 'f' },
    { "dir", required_argument, NULL, 'd' },
    { "help", no_argument, NULL, 'h' },
    { "lex", no_argument, NULL, '0' },
    { "stack", no_argument, NULL, '1' },
    { "synt", no_argument, NULL, '2' },
    { "asynt", no_argument, NULL, '3' },
    { "symb", no_argument, NULL, '4' },
    { "tests", no_argument, NULL, '5' },
    { NULL, 0, NULL, 0 }
};

int main(int argc, char **argv) {
    int opt;
    char *source_name;
    bool source_file_name, source_dir_name;
    bool dump_lex, dump_stack, dump_synt, dump_asynt, dump_symb, dump_test;
    FILE *stacktrace_fd, *test_fd;
    l_test_ctx *test_ctx;

    if (argc < 3 || argc > 9) {
        print_usage(argv);
        return EXIT_FAILURE;
    }

    source_name = NULL;
    source_file_name = false;
    source_dir_name = false;

    dump_lex = false;
    dump_stack = false;
    dump_synt = false;
    dump_asynt = false;
    dump_symb = false;
    test_ctx = NULL;
    dump_test = false;

    while ((opt = getopt_long(argc, argv, "f:d:", long_options, NULL)) != -1) {
        switch (opt) {
            case 'f':
               source_name = optarg;
               source_file_name = true;
            break;

            case 'd':
                source_name = optarg;
                source_dir_name = true;
            break;

            case '0':
                dump_lex = true;
            break;

            case '1':
                dump_stack = true;
            break;

            case '2':
                dump_synt = true;
            break;

            case '3':
                dump_asynt = true;
            break;

            case '4':
                dump_symb = true;
            break;

            case '5':
                dump_test = true;
            break;

            default:
                print_usage(argv);
                return EXIT_FAILURE;
        }
    }

    if ((source_file_name && source_dir_name) || (!source_file_name && !source_dir_name)) {
        print_usage(argv);
        return EXIT_FAILURE;
    }

    thread_storage_init();

    if (source_file_name) {
        test_ctx = l_test_manager_create_from_file(source_name);
    } else if (source_dir_name) {
        test_ctx = l_test_manager_create_from_dir(source_name);
    } else {
        test_ctx = l_test_manager_create_default();
    }

    if (!test_ctx) {
        PUSH_STACK_MSG("Failed to create tests context")
        goto clean_up;
    }

    if (dump_lex) {
        l_test_manager_dump_lex(test_ctx);
    }

    if (dump_synt) {
        l_test_manager_dump_synt(test_ctx);
    }

    if (dump_asynt) {
        l_test_manager_dump_asynt(test_ctx);
    }

    if (dump_symb) {
        l_test_manager_dump_symb(test_ctx);
    }

    if (dump_test) {
        test_fd = fopen("tests", "w+");
        if (!test_fd) {
            PUSH_STACK_ERRNO();
            goto clean_up;
        }
        l_test_manager_process(test_ctx, test_fd);
        printf("The tests have been saved in the file '%s'.\n", "tests");
        SAFE_FCLOSE(test_fd)
    } else {
        l_test_manager_process(test_ctx, stdout);    
    }

clean_up:
    l_test_manager_destroy(test_ctx);

    stacktrace_print();
    if (dump_stack) {
        if (stacktrace_is_filled()) {
            stacktrace_fd = fopen("stacktrace", "w+");
            stacktrace_print_fd(stacktrace_fd);
            printf("The stacktrace have been saved in the file '%s'.\n", "stacktrace");
            SAFE_FCLOSE(stacktrace_fd)
        }
    }

    thread_storage_uninit();

    return EXIT_SUCCESS;
}
