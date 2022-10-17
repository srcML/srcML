// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file language_extension_registry.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#ifndef LANGUAGE_EXTENSION_REGISTRY_HPP
#define LANGUAGE_EXTENSION_REGISTRY_HPP

#include <Language.hpp>
#include <string>
#include <vector>

/** Declare type language_extension as string/int pair */
typedef std::pair<std::string, int> language_extension;

/** access the extension from language_extension */
#define get_extension(OBJ) (OBJ).first

/** access the language from language_extension */
#define get_language(OBJ) (OBJ).second

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

    int get_language_from_filename(const char* const path) const;

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
