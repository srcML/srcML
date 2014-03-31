/**
 * @file Language.cpp
 *
 * @copyright Copyright (C) 2008-2014 SDML (www.srcML.org)
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

#include "Language.hpp"
#include <algorithm>
#include <boost/regex.hpp>

/** bool to indicate that c extensions are to be treated as C++ false by default */
bool Language::use_cpp_for_c = false;

/** static size of lang2int */
int Language::lang2intcount = 7;

/** static array holding string/numeric language pairs */
pair Language::lang2int[] = {
    { LanguageName::LANGUAGE_C, LANGUAGE_C },
    { LanguageName::LANGUAGE_CXX, LANGUAGE_CXX },
    { LanguageName::LANGUAGE_JAVA, LANGUAGE_JAVA },
    { LanguageName::LANGUAGE_ASPECTJ, LANGUAGE_ASPECTJ },
    { LanguageName::LANGUAGE_CSHARP, LANGUAGE_CSHARP },
    { LanguageName::LANGUAGE_NONE, LANGUAGE_NONE },
};

/** regular expression to match extension even if in an archive */
static const boost::regex extRegEx("(zx\\.|zg\\.|2zb\\.)*([^\\.]*)");

/** 
 * registerUserExt
 * @param ext the file extension
 * @param language interger representation of language to associated with extention
 * @param registered_languages structure to hold registered_languages
 *
 * Register a user extension overriding defaults.  @todo consider splitting this functionality off.
 *
 * @returns a bool indicating success.
 */
bool Language::registerUserExt(const char* ext, int language,
                              std::vector<pair> & registered_languages) {

    pair apair = { ext, language };
    registered_languages.push_back(apair);

 return true;

}

/** 
 * registerUserExt
 * @param ext the file extension
 * @param language string representation of language to associated with extention
 * @param registered_languages structure to hold registered_languages
 *
 * Register a user extension overriding defaults.  @todo consider splitting this functionality off.
 *
 * @returns a bool indicating success.
 */
bool Language::registerUserExt(const char* ext, const char* language,
                               std::vector<pair> & registered_languages) {

    int nlanguage = Language::getLanguage(language);
    if (!nlanguage)
        return false;

    registerUserExt(ext, nlanguage, registered_languages);

    return true;
}

/**
 * getLanguageExtension
 * @param inpath a complete filename with path
 * @param extension the found extension returned passed by reference
 *
 * Gets the language extension from a filename with path.
 * 
 * @returns if successsful.
 */
bool getLanguageExtension(const char * const inpath, std::string & extension)
{

    // reversed copy of the path
    std::string path(inpath);
    std::reverse(path.begin(), path.end());

    std::string::const_iterator start = path.begin();
    std::string::const_iterator end = path.end();
    boost::match_results<std::string::const_iterator> what;
    boost::match_flag_type flags = boost::match_default;

    if(boost::regex_search(start, end, what, extRegEx, flags)) {

        std::string temp = what[2].str();
        extension.assign(temp.rbegin(), temp.rend());
        return true;

    } else
        return false;

}

/**
 * getLanguageFromFilename
 * @param path filename with complete path
 * @param registered_languages the currently registered languages
 *
 * Gets the current language based on the extenstion.
 *
 * @returns the numeric representation of the currently registered language from the given filename.
 */
int Language::getLanguageFromFilename(const char* const path, std::vector<pair> & registered_languages) { 

    // extract the (pure) extension
    std::string extension;
    bool success = getLanguageExtension(path, extension);

    if (!success) return 0;

    // custom extensions
    for (int i = (int)(registered_languages.size() - 1); i >= 0; --i) {
        if (registered_languages[i].s== extension)
            return registered_languages[i].n == LANGUAGE_NONE ? 0 :
                registered_languages[i].n == LANGUAGE_C && use_cpp_for_c ? LANGUAGE_CXX : registered_languages[i].n;
    }

    return 0;
}

/**
 * register_standar_file_extensions
 * @param registered_languages currently registered languages
 *
 * Register the standard file extensions for all languages. 
 */
void Language::register_standard_file_extensions(std::vector<pair> & registered_languages)
{
    Language::registerUserExt("c",    LANGUAGE_C, registered_languages );
    Language::registerUserExt("h",    LANGUAGE_C, registered_languages );
    Language::registerUserExt("i",    LANGUAGE_C, registered_languages );

    Language::registerUserExt("cpp",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("CPP",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("cp",   LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("hpp",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("cxx",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("hxx",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("cc",   LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("hh",   LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("c++",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("h++",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("C",    LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("H",    LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("tcc",  LANGUAGE_CXX, registered_languages );
    Language::registerUserExt("ii",   LANGUAGE_CXX, registered_languages );

    Language::registerUserExt("java", LANGUAGE_JAVA, registered_languages );

    Language::registerUserExt("aj",   LANGUAGE_ASPECTJ, registered_languages );

    Language::registerUserExt("cs",   LANGUAGE_CSHARP, registered_languages );
}

/**
 * c_is_cpp
 * @param use_cpp bool to indicate if to use C++ for C.
 *
 * Indictae if to treat C as C++.
 */
void Language::c_is_cpp(bool use_cpp) {

    Language::use_cpp_for_c = use_cpp;
}
