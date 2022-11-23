// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Language.hpp
 *
 * @copyright Copyright (C) 2004-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

//#include <fnmatch.h>
#include <cstdio>
#include <string>
#include <vector>

/** set language_pair as type that is a pair of string/int */
typedef std::pair<std::string, int> language_pair;

/** get the string representation of a language from a language pair */
#define get_language_string(OBJ) OBJ.first

/** get the number representation of a language from a language pair */
#define get_language_number(OBJ) OBJ.second

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

    /** String constant for Objective-C language */
    const char* const LANGUAGE_OBJECTIVE_C = "Objective-C";

}

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

        /** No language */
        LANGUAGE_NONE = 0,

        /** C language */
        LANGUAGE_C = 1,

        /** C++ language */
        LANGUAGE_CXX = 2,

        /** Java language */
        LANGUAGE_JAVA = 4,

        /** C# language */
        LANGUAGE_CSHARP = 8,

        /** Objective-C language */
        LANGUAGE_OBJECTIVE_C = 16,

        /** C++ family i.e. C++/C#  */
        LANGUAGE_CXX_FAMILY = LANGUAGE_CXX | LANGUAGE_CSHARP,

        /** C family i.e. C/C++/C# */
        LANGUAGE_C_FAMILY = LANGUAGE_C | LANGUAGE_CXX_FAMILY | LANGUAGE_OBJECTIVE_C,

        /** Java family i.e. Java/AspectJ */
        LANGUAGE_JAVA_FAMILY = LANGUAGE_JAVA,

        /** Object-Oriented languages i.e. C++/C#/Java */
        LANGUAGE_OO = LANGUAGE_CXX_FAMILY | LANGUAGE_JAVA_FAMILY,

        /** All languages */
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

    Language& operator=(const Language&) = delete;

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
            if (get_language_string(lang2int[i]) == s)
                return get_language_number(lang2int[i]);

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
            if (get_language_number(lang2int[i]) == language)
                return get_language_string(lang2int[i]).data();

        return "";
    }

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
    static language_pair lang2int[];
};

#endif
