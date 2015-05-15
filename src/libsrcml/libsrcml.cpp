/**
 * @file libsrcml.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

/*
  Implementation of srcml simple API (global archive) functions from the namespace srcml_*
*/

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml_sax2_utilities.hpp>

#include <Language.hpp>
#include <language_extension_registry.hpp>
#include <srcmlns.hpp>

#include <cstring>
#include <stdlib.h>

#include <vector>
#include <string>
#include <fstream>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

/**
 * @var srcml_error
 *
 * Global structure to hold error string.
 */
#ifdef STATIC_GLOBALS
static
#endif
std::string srcml_error;

/**
 * @var global_archive
 *
 * global archive for use with srcml() function.  Defaulted values.
 * Archive is used for both read and write first call to srcml()
 * initializes other parameters.
 */
#ifdef STATIC_GLOBALS
static
#endif
srcml_archive global_archive = { SRCML_ARCHIVE_RW, boost::optional<std::string>(), boost::optional<std::string>(),
                                 std::string(SRCML_VERSION_STRING), boost::optional<std::string>(), boost::optional<std::string>(), boost::optional<std::string>(),
                                 std::vector<std::string>(),
                                 SRCML_OPTION_XML_DECL | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_HASH | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY,
                                 8, std::vector<std::string>(), std::vector<std::string>(), boost::optional<std::pair<std::string, std::string> >(),
                                 language_extension_registry(), std::vector<std::string>(), 0, 0, 0, std::vector<transform>(), boost::any() };

/**
 * @var global_unit
 *
 * global unit for use with srcml() function.  Defaulted values.
 */
#ifdef STATIC_GLOBALS
static
#endif
srcml_unit global_unit = { &global_archive, boost::optional<std::string>(), std::string(SRCML_VERSION_STRING), boost::optional<std::string>(),
                           boost::optional<std::string>(), boost::optional<std::string>(), boost::optional<std::string>(), boost::optional<std::string>(),
                           boost::optional<std::string>(), std::vector<std::string>(), 0, 0, 0, 0, boost::optional<std::string>(), boost::any() };

/**
 * @var register_languages
 *
 * Global variable tracking if need to register default languages.
 */
static bool register_languages = true;
/******************************************************************************
 *                                                                            *
 *                           Global Cleanup function                          *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_cleanup_globals
 *
 * Cleanup and free globals allocated at the global level
 * usually by libxml2.
 */
void srcml_cleanup_globals() {

    xmlCleanupCharEncodingHandlers();
    xmlCleanupGlobals();
    xmlDictCleanup();
    xmlCleanupParser();

}

/******************************************************************************
 *                                                                            *
 *                           Version functions                                *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_version_string
 *
 * @returns Return version of libsrcml as string.
 */
const char * srcml_version_string() {

    return SRCML_VERSION_STRING;

}

/**
 * srcml_version_number
 *
 * @returns Return version of libsrcml as number.
 */
int srcml_version_number() {

    return SRCML_VERSION_NUMBER;

}

/******************************************************************************
 *                                                                            *
 *                           srcML Convenience                                *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml
 * @param input_filename input file to turn to srcML or source code.
 * @param output_filename file to output srcML or source code
 *
 * Translates to/from srcML
 * Input files with extension xml and language set to xml result in srcml2src behaviour.
 * All other ending extensions result in src2srcml.
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml(const char* input_filename, const char* output_filename) {

    if(!input_filename || !output_filename) {

        srcml_error = "No input file provided";
        return  SRCML_STATUS_INVALID_ARGUMENT;

    }

    xmlInitParser();

    if(register_languages) {

        register_languages = false;
        language_extension_registry registry = global_archive.registered_languages;

        global_archive.registered_languages = language_extension_registry();

        global_archive.registered_languages.register_standard_file_extensions();

        global_archive.registered_languages.append(registry);

        std::vector<std::string> save_prefix;
        std::vector<std::string> save_ns;
        try {
            for(std::vector<std::string>::size_type i = 0; i < global_archive.prefixes.size(); ++i) {
                save_prefix.push_back(global_archive.prefixes.at(i));
                save_ns.push_back(global_archive.namespaces.at(i));

            }

        } catch(...) {
            return SRCML_STATUS_ERROR;
        }

        srcml_archive_register_namespace(&global_archive, SRCML_SRC_NS_PREFIX_DEFAULT, SRCML_SRC_NS_URI);

        for(std::vector<std::string>::size_type i = 0; i < save_prefix.size(); ++i) {
            try {
                srcml_archive_register_namespace(&global_archive, save_prefix.at(i).c_str(), save_ns.at(i).c_str());
            } catch(...) {
                return SRCML_STATUS_ERROR;
            }
        }

    }

    if(srcml_check_extension(input_filename)) {

        srcml_archive_write_open_filename(&global_archive, output_filename, 0);
        srcml_unit * unit = srcml_unit_create(&global_archive);

        int status = srcml_unit_set_language(unit, srcml_archive_get_language(&global_archive));
        if(status != SRCML_STATUS_OK) {

            srcml_unit_free(unit);
            return status;

        }
        
        if(srcml_unit_get_filename(&global_unit) != 0)
            srcml_unit_set_filename(unit, srcml_unit_get_filename(&global_unit));
        else
            srcml_unit_set_filename(unit, input_filename);

        srcml_unit_set_url(unit, srcml_archive_get_url(&global_archive));
        srcml_unit_set_version(unit, srcml_archive_get_version(&global_archive));
        srcml_unit_set_timestamp(unit, srcml_unit_get_timestamp(&global_unit));
        srcml_unit_set_hash(unit, srcml_unit_get_hash(&global_unit));

        status = srcml_unit_parse_filename(unit, input_filename);
        if(status != SRCML_STATUS_OK) {

            srcml_unit_free(unit);
            return status;

        }

        srcml_write_unit(&global_archive, unit);

        srcml_unit_free(unit);
        srcml_archive_close(&global_archive);

    } else {

        bool is_xml = false;
        size_t len = strlen(input_filename);
        if((len > 4 && tolower(input_filename[len - 1]) == 'l' && tolower(input_filename[len - 2]) == 'm'
            && ((tolower(input_filename[len - 3]) == 'x' && input_filename[len - 4] == '.')
             || (tolower(input_filename[len - 3]) == 'c' && tolower(input_filename[len - 4]) == 'r' && tolower(input_filename[len - 5]) == 's' && tolower(input_filename[len - 6]) == '.')))
           || (global_archive.language && strcmp(global_archive.language->c_str(), "xml") == 0))
            is_xml = true;

        // not xml or handled language
        if(!is_xml) {

            if(global_archive.language) {
                srcml_error = "Language '";
                srcml_error += global_archive.language->c_str();
                srcml_error += "' is not supported.";
            } else
                srcml_error = "No language provided.";

            return SRCML_STATUS_INVALID_INPUT;

        }

        srcml_extract_text_filename(input_filename, output_filename, global_archive.encoding ? global_archive.encoding->c_str() : "ISO-8859-1", 0);

    }

    return SRCML_STATUS_OK;
}

/******************************************************************************
 *                                                                            *
 *                           Global set functions                             *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_set_src_encoding
 * @param encoding an output encoding
 *
 * Set the source encoding.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_src_encoding(const char* encoding) {

    return srcml_archive_set_src_encoding(&global_archive, encoding);

}

/**
 * srcml_set_xml_encoding
 * @param encoding an output encoding
 *
 * Set the xml encoding.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_xml_encoding(const char* encoding) {

    return srcml_archive_set_xml_encoding(&global_archive, encoding);

}

/**
 * srcml_set_language
 * @param language a language
 *
 * Set the language to use to parse.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_language(const char* language) {

    return srcml_archive_set_language(&global_archive, language);

}

/**
 * srcml_set_filename
 * @param filename name of a file
 *
 * Set the filename attribute for the root unit.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_filename(const char* filename) {

    return srcml_unit_set_filename(&global_unit, filename);

}

/**
 * srcml_set_url
 * @param url a url path
 *
 * Set the url attribute for the root unit.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_url(const char* url) {

    return srcml_archive_set_url(&global_archive, url);

}

/**
 * srcml_set_version
 * @param version a version string
 *
 * Set the version attribute.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_version(const char* version) {

    return srcml_archive_set_version(&global_archive, version);

}

/**
 * srcml_set_timestamp
 * @param timestamp a timestamp string
 *
 * Set the timestamp attribute.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_timestamp(const char* timestamp) {

    return srcml_unit_set_timestamp(&global_unit, timestamp);

}

/**
 * srcml_set_hash
 * @param hash a hash string
 *
 * Set the hash attribute.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_hash(const char* hash) {

    return srcml_unit_set_hash(&global_unit, hash);

}

/**
 * srcml_set_options
 * @param option a srcml options
 *
 * Set the srcml options.  Clears all previously set.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_options(unsigned long long option) {

    return srcml_archive_set_options(&global_archive, option);

}

/**
 * srcml_enable_option
 * @param option a srcml option
 *
 * Enable/set the srcml options.  Multiple may be enabled.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_enable_option(unsigned long long option) {

    return srcml_archive_enable_option(&global_archive, option);

}

/**
 * srcml_disable_option
 * @param option a srcml option
 *
 * Remove an option.  May use multiple option with the same call.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_disable_option(unsigned long long option) {

    return srcml_archive_disable_option(&global_archive, option);

}

/**
 * srcml_set_tabstop
 * @param tabstop tabstop size
 *
 * Set the size of the tabstop.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_tabstop(size_t tabstop) {

    return srcml_archive_set_tabstop(&global_archive, tabstop);

}

/**
 * srcml_register_file_extension
 * @param extension a file extension
 * @param language a language
 *
 * Associate the given extension with the given language.
 *
 * @returns Return SRCML_STATUS_OK success and a status error code on failure.
 */
int srcml_register_file_extension(const char* extension, const char* language) {

    return srcml_archive_register_file_extension(&global_archive, extension, language);

}

/**
 * srcml_register_namespace
 * @param prefix a XML namespace prefix
 * @param ns a XML namespace
 *
 * Add a new namespace or change the prefix of an existing namespace.
 *
 * @returns Return SRCML_STATUS_OK success and a status error code on failure.
 */
int srcml_register_namespace(const char* prefix, const char* ns) {

    return srcml_archive_register_namespace(&global_archive, prefix, ns);

}

/**
 * srcml_archive_set_processing_instruction
 * @param target the processing instruction's target
 * @param data the processing instruciton's data
 *
 * Set a processing instruction that will be output before root element of archive.
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_set_processing_instruction(const char* target, const char* data) {

    return srcml_archive_set_processing_instruction(&global_archive, target, data);

}

/**
 * srcml_archive_register_macro
 * @param token name of macro
 * @param type macro type
 *
 * Register a macro (token) to be processed as a special type
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_register_macro(const char* token, const char* type) {

    return srcml_archive_register_macro(&global_archive, token, type);

}

/**
 * srcml_unparse_set_eol
 * @param eol the kind of eol to use for unparse
 *
 * Set the eol to be used for unparse.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_unparse_set_eol(size_t eol) {

    return srcml_unit_unparse_set_eol(&global_unit, eol);

}

/******************************************************************************
 *                                                                            *
 *                           Global get functions                             *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_get_src_encoding
 *
 * @returns Get the src encoding on success and NULL on failure.
 */
const char* srcml_get_src_encoding() {

    return srcml_archive_get_src_encoding(&global_archive);

}

/**
 * srcml_get_xml_encoding
 *
 * @returns Get the xml encoding on success and NULL on failure.
 */
const char* srcml_get_xml_encoding() {

    return srcml_archive_get_xml_encoding(&global_archive);

}

/**
 * srcml_get_revision
 *
 * @returns Get the srcML revision on success and NULL on failure.
 */
const char* srcml_get_revision() {

    return srcml_archive_get_revision(&global_archive);

}

/**
 * srcml_get_language
 *
 * @returns Get the language on success and NULL on failure.
 */
const char* srcml_get_language() {

    return srcml_archive_get_language(&global_archive);

}

/**
 * srcml_get_filename
 *
 * @returns Get the filename attribute for the root unit on success
 * and NULL on failure.
 */
const char* srcml_get_filename() {

    return srcml_unit_get_filename(&global_unit);

}

/**
 * srcml_get_url
 *
 * @returns Get the url attribute for the root unit on success
 * and NULL on failure
 */
const char* srcml_get_url() {

    return srcml_archive_get_url(&global_archive);

}

/**
 * srcml_get_version
 *
 * @returns Get the version attribute on success and NULL on failure.
 */
const char* srcml_get_version() {

    return srcml_archive_get_version(&global_archive);

}

/**
 * srcml_get_timestamp
 *
 * @returns Get the timestamp attribute on success and NULL on failure.
 */
const char* srcml_get_timestamp() {

    return srcml_unit_get_timestamp(&global_unit);

}

/**
 * srcml_get_hash
 *
 * @returns Get the hash attribute on success and NULL on failure.
 */
const char* srcml_get_hash() {

    return srcml_unit_get_hash(&global_unit);

}

/**
 * srcml_get_options
 *
 * @returns Get the currently set options on success and NULL on failure.
 */
unsigned long long srcml_get_options() {

    return srcml_archive_get_options(&global_archive);

}

/**
 * srcml_get_tabstop
 *
 * @returns Get the tabstop size on success and NULL On failure.
 */
size_t srcml_get_tabstop() {

    return srcml_archive_get_tabstop(&global_archive);

}

/**
 * srcml_get_namespace_size
 *
 * @returns Get the number of currently defined namespaces.
 */
size_t srcml_get_namespace_size() {

    return srcml_archive_get_namespace_size(&global_archive);

}

/**
 * srcml_get_namespace_prefix
 * @param pos namespace position
 *
 * @returns Get prefix for the given position on success
 * and NULL on failure.
 */
const char* srcml_get_namespace_prefix(size_t pos) {

    return srcml_archive_get_namespace_prefix(&global_archive, pos);

}

/**
 * srcml_get_prefix_from_uri
 * @param namespace_uri an XML namespace
 *
 * @returns Get the registered prefix for the given namespace
 * on success and NULL on failure.
 */
const char* srcml_get_prefix_from_uri(const char* namespace_uri) {

    return srcml_archive_get_prefix_from_uri(&global_archive, namespace_uri);

}

/**
 * srcml_get_namespace_uri
 * @param pos position in namespaces
 *
 * @returns Get the namespace at the given pos on succcess
 * and NULL on failure.
 */
const char* srcml_get_namespace_uri(size_t pos) {

    return srcml_archive_get_namespace_uri(&global_archive, pos);

}

/**
 * srcml_get_uri_from_prefix
 * @param prefix an XML prefix
 *
 * @returns Get the first namespace for the given prefix on success
 * and NULL on failure.
 */
const char* srcml_get_uri_from_prefix(const char* prefix) {

    return srcml_archive_get_uri_from_prefix(&global_archive, prefix);

}

/**
 * srcml_get_processing_instruction_target
 *
 * @returns Get the processing instruction target.
 */
const char* srcml_get_processing_instruction_target() {

    return srcml_archive_get_processing_instruction_target(&global_archive);

}

/**
 * srcml_get_processing_instruction_data
 * @param archive a srcml
 *
 * @returns Get the processing instruction data.
 */
const char* srcml_get_processing_instruction_data() {

    return srcml_archive_get_processing_instruction_data(&global_archive);

}

/**
 * srcml_archive_get_macro_list_size
 *
 * @returns Get the number of currently defined macros.
 */
size_t srcml_get_macro_list_size() {

    return srcml_archive_get_macro_list_size(&global_archive);

}

/**
 * srcml_archive_get_macro_token
 * @param pos macro position
 *
 * @returns Get token for the given position on success
 * and NULL on failure.
 */
const char* srcml_get_macro_token(size_t pos) {

    return srcml_archive_get_macro_token(&global_archive, pos);

}

/**
 * srcml_archive_get_macro_token_type
 * @param token a macro token
 *
 * @returns Get the registered type for the given token
 * on success and NULL on failure.
 */
const char* srcml_get_macro_token_type(const char* token) {

    return srcml_archive_get_macro_token_type(&global_archive, token
                                              );

}

/**
 * srcml_archive_get_macro_type
 * @param pos position in macro list
 *
 * @returns Get the type at the given pos on succcess
 * and NULL on failure.
 */
const char* srcml_get_macro_type(size_t pos) {

    return srcml_archive_get_macro_type(&global_archive, pos);

}

/******************************************************************************
 *                                                                            *
 *                           libsrcml utility functions                       *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_check_language
 * @param language a language
 *
 * Check if the current language is supported.
 *
 * @returns Return the numeric representation for that language if supported.
 * Not supported returns 0.
 */
int srcml_check_language(const char* language) { return language == 0 ? 0 : Language::getLanguage(language); }

/**
 * srcml_get_language_list_size
 *
 * Gets the number of languages supported.
 *
 * @returns number of languages supported.
 */
size_t srcml_get_language_list_size() {

    return 5;
}

/**
 * srcml_get_language_list
 * @param pos position of a language list
 *
 * Gets the name of the supported language at pos.
 *
 * @returns Get the supported language name at pos
 * on success and NULL on failure.
 */
const char * srcml_get_language_list(size_t pos) {

    if(pos >= srcml_get_language_list_size()) return NULL;

    static const char* langs[] = { "C", "C++", "C#", "Objective-C", "Java", 0 };
    return langs[pos];
}

/**
 * srcml_check_extension
 * @param filename name of a file
 *
 * Get the currently registered language for a file extension
 * Full filename can be provided, and extension will be extracted.
 * @returns Returns language on success and NULL on failure.
 */
const char * srcml_check_extension(const char* filename) {

    if(register_languages) {

        register_languages = false;
        language_extension_registry registry = global_archive.registered_languages;

        global_archive.registered_languages = language_extension_registry();

        global_archive.registered_languages.register_standard_file_extensions();

        global_archive.registered_languages.append(registry);

        std::vector<std::string> save_prefix;
        std::vector<std::string> save_ns;
        try {
            for(std::vector<std::string>::size_type i = 0; i < global_archive.prefixes.size(); ++i) {
                save_prefix.push_back(global_archive.prefixes.at(i));
                save_ns.push_back(global_archive.namespaces.at(i));

            }

        } catch(...) {
            return 0;
        }

    }

    return srcml_archive_check_extension(&global_archive, filename);

}

/**
 * srcml_check_encoding
 * @param encoding an encoding
 *
 * Check if the particular encoding is supported, both for input and output.
 * @returns Return if encoding is supported.
 */
int srcml_check_encoding(const char* encoding) {

    return xmlParseCharEncoding(encoding) > 0;

}

/**
 * srcml_check_xslt
 *
 * Check whether xslt is available.
 * @returns Return 1 on success and 0 on failure.
 */
int srcml_check_xslt() {
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    void * handle = dlopen("libxslt.so", RTLD_LAZY);
    if (!handle) {
        handle = dlopen("libxslt.so.1", RTLD_LAZY);
        if (!handle) {
            handle = dlopen("libxslt.dylib", RTLD_LAZY);
            if (!handle) return 0;

        }
    }

    dlclose(handle);
    return 1;
#else
    return 1;
#endif

}

/**
 * srcml_check_exslt
 *
 * Check whether exslt is available.
 * @returns Return 1 on success and 0 on failure.
 */
int srcml_check_exslt() {
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    void* handle = dlopen("libexslt.so", RTLD_LAZY);
    if (!handle) {
        handle = dlopen("libexslt.so.0", RTLD_LAZY);
        if (!handle) {
            handle = dlopen("libexslt.dylib", RTLD_LAZY);
            if (!handle) return 0;
        }
    }

    dlclose(handle);
    return 1;
#else
    return 1;
#endif
}

/******************************************************************************
 *                                                                            *
 *                           libsrcml error functions                         *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_error_string
 *
 * @returns Return a string describing last recorded error.
 */
const char* srcml_error_string() { return srcml_error.c_str(); }


/******************************************************************************
 *                                                                            *
 *                           libsrcml misc functions                          *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_memory_free
 *
 * Free a buffer allocated by functions such as srcml_archive_write_open_memory and srcml_unit_unparse_memory.
 */
void srcml_memory_free(char * buffer) {

    if(buffer == 0) return;

    free((void*)buffer);

}
