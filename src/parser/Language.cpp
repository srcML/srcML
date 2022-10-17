// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Language.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <Language.hpp>
#include <algorithm>

/** static size of lang2int */
int Language::lang2intcount = 6;

/** static array holding string/numeric language pairs */
language_pair Language::lang2int[] = {
    language_pair( LanguageName::LANGUAGE_C, LANGUAGE_C ),
    language_pair( LanguageName::LANGUAGE_CXX, LANGUAGE_CXX ),
    language_pair( LanguageName::LANGUAGE_JAVA, LANGUAGE_JAVA ),
    language_pair( LanguageName::LANGUAGE_CSHARP, LANGUAGE_CSHARP ),
    language_pair( LanguageName::LANGUAGE_OBJECTIVE_C, LANGUAGE_OBJECTIVE_C | LANGUAGE_C ),
    language_pair( LanguageName::LANGUAGE_NONE, LANGUAGE_NONE ),
};
