/**
 * @file language_extension_registry.hpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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

#ifndef LANGUAGE_EXTENSION_REGISTRY_HPP
#define LANGUAGE_EXTENSION_REGISTRY_HPP

#include <Language.hpp>
#include <string>
#include <vector>

/** Declare type language_extension as string/int pair */
typedef std::pair<std::string, int> language_extension;

/** access the extension from language_extension */
#define get_extension(OBJ) OBJ.first

/** access the language from language_extension */
#define get_language(OBJ) OBJ.second

 /**
  * language_extension_registry_error
  *
  * Error class thrown for improper access of language_extension_registry.
  */
class language_extension_registry_error {};

/**
 * language_extension_registry
 *
 * Data class for language extensions.
 */
class language_extension_registry {

public:

    language_extension_registry();
    ~language_extension_registry();

    bool register_user_ext(const char* ext, int language);
    bool register_user_ext(const char* ext, const char* language);

    int get_language_from_filename(const char* const path);

    void register_standard_file_extensions();

    void c_is_cpp(bool use_cpp);

    unsigned int size() const;

    language_extension last() const;

    language_extension at(unsigned int pos) const;

    void append(language_extension_registry registry);

private:

    /** vector of language/extension pairs */
    std::vector<language_extension> registered_languages;

    /** bool to indicate that c extensions are to be treated as C++ false by default */
    bool use_cpp_for_c;

};

#endif
