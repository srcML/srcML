/**
 * @file LanguageExtensionRegistry.hpp
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

#ifndef LANGUAGE_EXTENSION_REGISTRY_HPP
#define LANGUAGE_EXTENSION_REGISTRY_HPP

#include <Language.hpp>
#include <string>
#include <vector>

/** Declarare type language pair as string/int pair */
typedef std::pair<std::string, int> language_pair;

 /**
  * LanguageExtensionRegistryError
  *
  * Error class thrown for improper access of LanguageExtensionRegistry.
  */
class LanguageExtensionRegistryError {};

/**
 * LanguageExtensionRegistry
 *
 * Data class for language extensions.
 */
class LanguageExtensionRegistry {

public:

    LanguageExtensionRegistry();
    ~LanguageExtensionRegistry();

    bool registerUserExt(const char* ext, int language);
    bool registerUserExt(const char* ext, const char* language);

    int getLanguageFromFilename(const char* const path);

    void register_standard_file_extensions();

    void c_is_cpp(bool use_cpp);

    unsigned int size() const;

    language_pair last() const;

    language_pair at(unsigned int pos) const;

    void append(LanguageExtensionRegistry registry);

private:

    /** vector of language/extension pairs */
    std::vector<language_pair> registered_languages;

    /** bool to indicate that c extensions are to be treated as C++ false by default */
    bool use_cpp_for_c;

};

#endif
