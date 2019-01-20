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

#ifndef DEFINES_H
#define DEFINES_H

/* Uncomment to print debug messages */
/* #define DEBUG */

#ifdef DEBUG
    #include <stdio.h>
#endif

#ifdef DEBUG
    #define DEBUG_PRINT(format, ...) fprintf(stdout, format, ##__VA_ARGS__);
    #define DEBUG_PRINT_STR(str) fprintf(stdout, "%s", str);
#else
    #define DEBUG_PRINT(format, ...)
    #define DEBUG_PRINT_STR(str)
#endif

#endif
