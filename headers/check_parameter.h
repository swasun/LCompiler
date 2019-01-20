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

#ifndef CHECK_PARAMETER_H
#define CHECK_PARAMETER_H

#include "stacktrace.h"

#define UNUSED(x) (void)(x);

#define CHECK_PARAMETER(p) \
    if (!(p)) { \
        PUSH_STACK(INVALID_PARAMETER) \
        return; \
    } \

#define CHECK_PARAMETER_OR_RETURN(p) \
    if (!(p)) { \
        PUSH_STACK(INVALID_PARAMETER) \
        return 0; \
    } \

#define CHECK_PARAMETER_OR_GOTO(p, label) \
    if (!(p)) { \
        PUSH_STACK(INVALID_PARAMETER) \
        goto label; \
    } \

#endif
