/**
 * @file Language.hpp
 *
 * @copyright Copyright (C) 2004-2014 SDML (www.srcML.org)
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

/**
 * Namespace for language names
 */
namespace LanguageName {

    /** No language is empty string */
    const char* const LANGUAGE_NONE = "";

    /** String constant for C language */
    const char* const LANGUAGE_C = "C";

    /** String constant for C# language */
    const char* const LANGUAGE_CSHARP = "C#";

    /** String constant for C++ language */
    const char* const LANGUAGE_CXX = "C++";

    /** String constant for Java language */
    const char* const LANGUAGE_JAVA = "Java";

    /** String constant for AspectJ language */
    const char* const LANGUAGE_ASPECTJ = "AspectJ";


}

/**
 * pair
 *
 * Holds a Language string/numeral pair
 */
struct pair {
    std::string s;
    int n;
};

/**
* Language
*
* Class representing a source code language.  Also, provides static members
* for determining a language from an extension.
*/
class Language {

public:

    /**
     * Languages
     *
     * Enum constants for each handled language.
     * Each language is a bit.
     */
    enum Languages {

        /** Interger Constant for No language */
        LANGUAGE_NONE = 0,

        /** Interger Constant for C language */
        LANGUAGE_C = 1,

        /** Interger Constant for C++ language */
        LANGUAGE_CXX = 2,

        /** Interger Constant for Java language */
        LANGUAGE_JAVA = 4,

        /** Interger Constant for AspectJ language */
        LANGUAGE_ASPECTJ = 8,

        /** Interger Constant for C# language */
        LANGUAGE_CSHARP = 16,

        /** Interger Constant for languages in C++ family i.e. C++/C#  */
        LANGUAGE_CXX_FAMILY = LANGUAGE_CXX | LANGUAGE_CSHARP,

        /** Interger Constant for languages in C family i.e. C/C++/C# */
        LANGUAGE_C_FAMILY = LANGUAGE_C | LANGUAGE_CXX_FAMILY,

        /** Interger Constant for languages in Java family i.e. Java/AspectJ */
        LANGUAGE_JAVA_FAMILY = LANGUAGE_JAVA | LANGUAGE_ASPECTJ,

        /** Interger Constant for Object-Oriented languages i.e. C++/C#/Java */
        LANGUAGE_OO = LANGUAGE_CXX_FAMILY | LANGUAGE_JAVA_FAMILY,

        /** Interger Constant for all languages */
        LANGUAGE_ALL = ~0
    };

    /**
     * Language
     * @param lang numeric represenation to for language object
     *
     * Constructor taking int to set language.
     */
    Language(int lang)
        : language(lang) {

    }

    static bool filledLang();

    /**
     * inLanguage
     *
     * Predicate method to test if in a current language
     *
     * @returns if in the current language.
     */
    inline bool inLanguage(int l) const {
        return (l & language) > 0;
    }

    /**
     * getLanguage
     *
     * Accessor method that gets the current language.
     *
     * @returns the numberic representation of the language.
     */
    inline int getLanguage() const {
        return language;
    }

    /**
     * getLanguage
     *
     * Static method to translate a string representation of a language
     * into numeric version
     *
     * @returns the numberic representation of the language.
     */
    static int getLanguage(const char* const s) {

        for(int i = 0; i < lang2intcount; ++i)
            if (lang2int[i].s == s)
                return lang2int[i].n;

        return 0;
    }

    /**
     * getLanguageString
     *
     * Get the current language as a string.
     *
     * @returns the current language as a string.
     */
    const char* getLanguageString() const {

        for(int i = 0; i < lang2intcount; ++i)
            if (lang2int[i].n == language)
                return lang2int[i].s.c_str();

        return "";
    }

    static bool registerUserExt(const char* ext, int language, std::vector<pair> & registered_languages);
    static bool registerUserExt(const char* ext, const char* language, std::vector<pair> & registered_languages);

    // gets the current language based on the extenstion
    static int getLanguageFromFilename(const char* const path, std::vector<pair> & registered_languages);

    // register the standard language file extensions
    static void register_standard_file_extensions(std::vector<pair> & registered_languages);

    static void c_is_cpp(bool use_cpp);

    /**
     * ~Language
     *
     * Destructor.
     */
    ~Language() {}

private:

    /** the current langauge */
    const int language;

    static int lang2intcount;
    static pair lang2int[];
    static bool use_cpp_for_c;
};

#endif
