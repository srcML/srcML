/**
 * @file Language.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "Language.hpp"
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
