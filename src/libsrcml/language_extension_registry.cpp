/**
 * @file language_extension_registry.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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

#include <language_extension_registry.hpp>
#include <algorithm>

/**
* language_extension_registry
*
* Constructor.
*/
language_extension_registry::language_extension_registry() : registered_languages({

    { "c",    Language::LANGUAGE_C },
    { "h",    Language::LANGUAGE_C },
    { "i",    Language::LANGUAGE_C },
    { "cpp",  Language::LANGUAGE_CXX },
    { "CPP",  Language::LANGUAGE_CXX },
    { "cp",   Language::LANGUAGE_CXX },
    { "hpp",  Language::LANGUAGE_CXX },
    { "cxx",  Language::LANGUAGE_CXX },
    { "hxx",  Language::LANGUAGE_CXX },
    { "cc",   Language::LANGUAGE_CXX },
    { "hh",   Language::LANGUAGE_CXX },
    { "c++",  Language::LANGUAGE_CXX },
    { "h++",  Language::LANGUAGE_CXX },
    { "C",    Language::LANGUAGE_CXX },
    { "H",    Language::LANGUAGE_CXX },
    { "tcc",  Language::LANGUAGE_CXX },
    { "ii",   Language::LANGUAGE_CXX },
    { "java", Language::LANGUAGE_JAVA },
    { "aj",   Language::LANGUAGE_JAVA },
    { "cs",   Language::LANGUAGE_CSHARP },
    /* { "m",    Language::LANGUAGE_OBJECTIVE_C | Language::LANGUAGE_C } */ }), use_cpp_for_c(false)
    {}

/**
* ~LanguageExtensionRegistery
*
* Destructor.
*/
language_extension_registry::~language_extension_registry() {}

/**
 * get_language_extension
 * @param inpath a complete filename with path
 * @param extension the found extension returned passed by reference
 *
 * Gets the language extension from a filename with path.
 *
 * @returns if successsful.
 */
bool get_language_extension(const char * const inpath, std::string & extension)
{
    std::string path(inpath);

    // remove any .gz extension
    // FIXME: Why are we doing this? Could be many types of these kind of extensions
    if ((path.size() > 3) && (path.substr(path.size() - 3) == ".gz"))
        path.resize(path.size() - 3);

    // get the proper extension, not including the '.'
    size_t pos = path.find_last_of(".");
    if (pos != std::string::npos) {
        extension = path.substr(pos + 1);
        return true;
    } else {
        return false;
    }
}

/**
 * register_user_ext
 * @param ext the file extension
 * @param language interger representation of language to associated with extention
 *
 * Register a user extension overriding defaults.
 *
 * @returns a bool indicating success.
 */
bool language_extension_registry::register_user_ext(const char* ext, int language) {

    if (!language)
        return false;

    language_extension apair = language_extension(ext, language);
    registered_languages.push_back(apair);

    return true;
}

/**
 * register_user_ext
 * @param ext the file extension
 * @param language string representation of language to associated with extention
 *
 * Register a user extension overriding defaults.
 *
 * @returns a bool indicating success.
 */
bool language_extension_registry::register_user_ext(const char* ext, const char* language) {

    int nlanguage = Language::getLanguage(language);
    if (!nlanguage)
        return false;

    register_user_ext(ext, nlanguage);

    return true;
}

/**
 * get_language_from_filename
 * @param path filename with complete path
 *
 * Gets the current language based on the extenstion.
 *
 * @returns the numeric representation of the currently registered language from the given filename.
 */
int language_extension_registry::get_language_from_filename(const char* const path) const {

    // extract the (pure) extension
    std::string extension;
    bool success = get_language_extension(path, extension);
    if (!success)
        return 0;

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
void language_extension_registry::register_standard_file_extensions() {

    register_user_ext("c",    Language::LANGUAGE_C);
    register_user_ext("h",    Language::LANGUAGE_C);
    register_user_ext("i",    Language::LANGUAGE_C);

    register_user_ext("cpp",  Language::LANGUAGE_CXX);
    register_user_ext("CPP",  Language::LANGUAGE_CXX);
    register_user_ext("cp",   Language::LANGUAGE_CXX);
    register_user_ext("hpp",  Language::LANGUAGE_CXX);
    register_user_ext("cxx",  Language::LANGUAGE_CXX);
    register_user_ext("hxx",  Language::LANGUAGE_CXX);
    register_user_ext("cc",   Language::LANGUAGE_CXX);
    register_user_ext("hh",   Language::LANGUAGE_CXX);
    register_user_ext("c++",  Language::LANGUAGE_CXX);
    register_user_ext("h++",  Language::LANGUAGE_CXX);
    register_user_ext("C",    Language::LANGUAGE_CXX);
    register_user_ext("H",    Language::LANGUAGE_CXX);
    register_user_ext("tcc",  Language::LANGUAGE_CXX);
    register_user_ext("ii",   Language::LANGUAGE_CXX);

    register_user_ext("java", Language::LANGUAGE_JAVA);
    register_user_ext("aj",   Language::LANGUAGE_JAVA);

    register_user_ext("cs",   Language::LANGUAGE_CSHARP);
}

/**
 * c_is_cpp
 * @param use_cpp bool to indicate if to use C++ for C.
 *
 * Indictae if to treat C as C++.
 */
void language_extension_registry::c_is_cpp(bool use_cpp) {

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
language_extension language_extension_registry::at(unsigned int pos) const {

    if(pos >= size()) throw language_extension_registry_error();

    return registered_languages.at(pos);
}

/**
 * size
 *
 * Gets the size of the language extension registry.
 *
 * @returns the number of registered language/extension pairs.
 */
unsigned int language_extension_registry::size() const {

    return (unsigned int)registered_languages.size();
}

/**
 * last
 *
 * Get the last language/extension pair at given position.
 *
 * @returns the last language/extension pair at given position.
 */
language_extension language_extension_registry::last() const {

    if (size() == 0)
        throw language_extension_registry_error();

    return registered_languages.back();
}

/**
 * append
 * @param registry another language_extension_registry
 *
 * Append the contents of registry to this registry.
 */
void language_extension_registry::append(language_extension_registry registry) {

    for(auto& language : registry.registered_languages)
        registered_languages.push_back(language);
}
