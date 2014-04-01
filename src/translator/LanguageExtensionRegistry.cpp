/**
 * @file LanguageExtensionRegistry.cpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <LanguageExtensionRegistry.hpp>
#include <algorithm>
#include <boost/regex.hpp>

/** regular expression to match extension even if in an archive */
static const boost::regex extRegEx("(zx\\.|zg\\.|2zb\\.)*([^\\.]*)");

/**
* LanguageExtensionRegistry
*
* Constructor.
*/
LanguageExtensionRegistry::LanguageExtensionRegistry() : use_cpp_for_c(false) {}

/**
* ~LanguageExtensionRegistery
*
* Destructor.
*/
LanguageExtensionRegistry::~LanguageExtensionRegistry() {}

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
 * registerUserExt
 * @param ext the file extension
 * @param language interger representation of language to associated with extention
 *
 * Register a user extension overriding defaults.  @todo consider splitting this functionality off.
 *
 * @returns a bool indicating success.
 */
bool LanguageExtensionRegistry::registerUserExt(const char* ext, int language) {

    if(!language) return false;

    language_extension apair = language_extension(ext, language);
    registered_languages.push_back(apair);

 return true;

}

/** 
 * registerUserExt
 * @param ext the file extension
 * @param language string representation of language to associated with extention
 *
 * Register a user extension overriding defaults.
 *
 * @returns a bool indicating success.
 */
bool LanguageExtensionRegistry::registerUserExt(const char* ext, const char* language) {

    int nlanguage = Language::getLanguage(language);
    if (!nlanguage)
        return false;

    registerUserExt(ext, nlanguage);

    return true;
}

/**
 * getLanguageFromFilename
 * @param path filename with complete path
 *
 * Gets the current language based on the extenstion.
 *
 * @returns the numeric representation of the currently registered language from the given filename.
 */
int LanguageExtensionRegistry::getLanguageFromFilename(const char* const path) { 

    // extract the (pure) extension
    std::string extension;
    bool success = getLanguageExtension(path, extension);

    if (!success) return 0;

    // custom extensions
    for (int i = (int)(registered_languages.size() - 1); i >= 0; --i) {
        if (get_extension(registered_languages[i]) == extension)
            return get_language(registered_languages[i]) == Language::LANGUAGE_NONE ? 0 :
                get_language(registered_languages[i]) == Language::LANGUAGE_C && use_cpp_for_c ? Language::LANGUAGE_CXX : get_language(registered_languages[i]);
    }

    return 0;
}

/**
 * register_standar_file_extensions
  *
 * Register the standard file extensions for all languages. 
 */
void LanguageExtensionRegistry::register_standard_file_extensions()
{
    registerUserExt("c",    Language::LANGUAGE_C);
    registerUserExt("h",    Language::LANGUAGE_C);
    registerUserExt("i",    Language::LANGUAGE_C);

    registerUserExt("cpp",  Language::LANGUAGE_CXX);
    registerUserExt("CPP",  Language::LANGUAGE_CXX);
    registerUserExt("cp",   Language::LANGUAGE_CXX);
    registerUserExt("hpp",  Language::LANGUAGE_CXX);
    registerUserExt("cxx",  Language::LANGUAGE_CXX);
    registerUserExt("hxx",  Language::LANGUAGE_CXX);
    registerUserExt("cc",   Language::LANGUAGE_CXX);
    registerUserExt("hh",   Language::LANGUAGE_CXX);
    registerUserExt("c++",  Language::LANGUAGE_CXX);
    registerUserExt("h++",  Language::LANGUAGE_CXX);
    registerUserExt("C",    Language::LANGUAGE_CXX);
    registerUserExt("H",    Language::LANGUAGE_CXX);
    registerUserExt("tcc",  Language::LANGUAGE_CXX);
    registerUserExt("ii",   Language::LANGUAGE_CXX);

    registerUserExt("java", Language::LANGUAGE_JAVA);

    registerUserExt("aj",   Language::LANGUAGE_ASPECTJ);

    registerUserExt("cs",   Language::LANGUAGE_CSHARP);
}

/**
 * c_is_cpp
 * @param use_cpp bool to indicate if to use C++ for C.
 *
 * Indictae if to treat C as C++.
 */
void LanguageExtensionRegistry::c_is_cpp(bool use_cpp) {

    use_cpp_for_c = use_cpp;

}

/**
 * at
 * @param pos position to access
 *
 * Access the language/extension pair at given position.
 *
 * @returns the language/extension pair at given position.
 */
language_extension LanguageExtensionRegistry::at(unsigned int pos) const {

    if(pos >= size()) throw LanguageExtensionRegistryError();

    return registered_languages.at(pos);

}

/**
 * size
 *
 * Gets the size of the language extension registry.
 *
 * @returns the number of registered language/extension pairs.
 */
unsigned int LanguageExtensionRegistry::size() const {

    return (unsigned int)registered_languages.size();

}

/**
 * last
 *
 * Get the last language/extension pair at given position.
 * 
 * @returns the last language/extension pair at given position.
 */
language_extension LanguageExtensionRegistry::last() const {

    if(size() == 0) throw LanguageExtensionRegistryError();

    return registered_languages.back();

}

/**
 * append
 * @param registry another LanguageExtensionRegistry
 *
 * Append the contents of registry to this registry.
 */
void LanguageExtensionRegistry::append(LanguageExtensionRegistry registry) {

    for(std::vector<language_extension>::const_iterator itr = registry.registered_languages.begin(); itr != registry.registered_languages.end(); ++itr)
        registered_languages.push_back(*itr);

}
