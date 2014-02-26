/**
 * @file Language.hpp
 *
 * @copyright Copyright (C) 2004-2014  SDML (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

//#include <fnmatch.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace LanguageName {

    const char* const LANGUAGE_NONE = "";
    const char* const LANGUAGE_C = "C";
    const char* const LANGUAGE_CS = "C#";
    const char* const LANGUAGE_CXX = "C++";
    const char* const LANGUAGE_CXX_11 = "C++11";
    const char* const LANGUAGE_JAVA = "Java";
    const char* const LANGUAGE_ASPECTJ = "AspectJ";
    const char* const LANGUAGE_CSHARP = "C#";

}

struct pair {
    std::string s;
    int n;
};

class Language {

public:

    enum Languages {
        LANGUAGE_C = 1,
        LANGUAGE_CXX = 2,
        LANGUAGE_CXX_11 = 4,
        LANGUAGE_JAVA = 8,
        LANGUAGE_ASPECTJ = 16,
        LANGUAGE_CSHARP = 32,
        LANGUAGE_CXX_FAMILY = LANGUAGE_CXX | LANGUAGE_CXX_11 | LANGUAGE_CSHARP,
        LANGUAGE_CXX_ONLY = LANGUAGE_CXX | LANGUAGE_CXX_11,
        LANGUAGE_C_FAMILY = LANGUAGE_C | LANGUAGE_CXX_FAMILY,
        LANGUAGE_JAVA_FAMILY = LANGUAGE_JAVA | LANGUAGE_ASPECTJ,
        LANGUAGE_OO = LANGUAGE_CXX_FAMILY | LANGUAGE_JAVA_FAMILY,
        LANGUAGE_NONE = 64,
        LANGUAGE_ALL = ~0
    };

    Language(int lang)
        : language(lang) {

    }

    static bool filledLang();

    // gets the current language
    inline bool inLanguage(int l) const {
        return (l & language) > 0;
    }

    // gets the current language
    inline int getLanguage() const {
        return language;
    }

    // gets the current language
    static int getLanguage(const char* const s) {

        for(int i = 0; i < lang2intcount; ++i)
            if (lang2int[i].s == s)
                return lang2int[i].n;

        return 0;
    }

    // gets the current language
    const char* getLanguageString() const {

        for(int i = 0; i < lang2intcount; ++i)
            if (lang2int[i].n == language)
                return lang2int[i].s.c_str();

        return "";
    }

    static bool registerUserExt(const char* ext, int language);
    static bool registerUserExt(const char* ext, const char* language);
    static bool registerUserExt(const char* ext, int language, std::vector<pair> & registered_languages);
    static bool registerUserExt(const char* ext, const char* language, std::vector<pair> & registered_languages);

    // gets the current language based on the extenstion
    static int getLanguageFromFilename(const char* const path);
    static int getLanguageFromFilename(const char* const path, std::vector<pair> & registered_languages);

    // register the standard language file extensions
    static void register_standard_file_extensions();
    static void register_standard_file_extensions(std::vector<pair> & registered_languages);

    static void c_is_cpp(bool use_cpp);

    ~Language() {}

private:

    const int language;

    static int lang2intcount;
    static pair lang2int[];

    static pair userext2int[];

    static bool use_cpp_for_c;
};

#endif
