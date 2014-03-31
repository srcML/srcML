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
#define LANGUAGE_EXTENSIONS_REGISTRY_HPP

//#include <fnmatch.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

/**
 * LanguageExtensionRegistry
 *
 * Data class for language extensions
 */
class LanguageExtensionRegistry {

public:

     /**
     * LanguageExtensionRegistry
     *
     * Constructor.
     */
    LanguageExtensionRegistery() {}

    static bool registerUserExt(const char* ext, int language, std::vector<pair> & registered_languages);
    static bool registerUserExt(const char* ext, const char* language, std::vector<pair> & registered_languages);

    // gets the current language based on the extenstion
    static int getLanguageFromFilename(const char* const path, std::vector<pair> & registered_languages);

    // register the standard language file extensions
    static void register_standard_file_extensions(std::vector<pair> & registered_languages);

    /**
     * ~LanguageExtensionRegistery
     *
     * Destructor.
     */
    ~LanguageExtensionRegistry() {}

private:

    /** the current langauge */
    const int language;

    static int lang2intcount;
    static pair lang2int[];
    static bool use_cpp_for_c;
};

#endif
