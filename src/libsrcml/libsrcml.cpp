/**
 * @file libsrcml.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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

#include <memory>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

// library constructor/destructor
#if defined(__GNUC__)

__attribute__((constructor))
static void constructor() {

    xmlInitParser();
}

__attribute__((destructor))
static void destructor() {

    xmlCleanupParser();
}

#else

static bool once = [](){

    xmlInitParser();

    return true;
}();

#endif

/**
 * @var global_archive
 *
 * global archive for use with srcml() function.  Defaulted values.
 * Archive is used for both read and write first call to srcml()
 * initializes other parameters.
 */
static srcml_archive global_archive;

/**
 * @var global_unit
 *
 * global unit for use with srcml() function.  Defaulted values.
 */
static srcml_unit global_unit;

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

    // automatic on library unloading, but this lets
    // it be done earlier
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

    if (!input_filename || !output_filename) {

        global_archive.error_string = "No input file provided";
        return SRCML_STATUS_INVALID_ARGUMENT;
    }

    if (srcml_check_extension(input_filename)) {

        // src->srcml
        std::unique_ptr<srcml_archive> archive(srcml_archive_clone(&global_archive));
        if (!archive) {
            global_archive.error_string = "Unable to create srcML archive";
            return SRCML_STATUS_ERROR;
        }

        int status = srcml_archive_write_open_filename(archive.get(), output_filename);
        if (status != SRCML_STATUS_OK)
            return status;

        std::unique_ptr<srcml_unit> unit(srcml_unit_create(archive.get()));
        if (!unit) {
            global_archive.error_string = "Unable to create srcML unit";
            return SRCML_STATUS_ERROR;
        }

        // single input file, so non-archive unit
        status = srcml_archive_enable_solitary_unit(archive.get());
        if (status != SRCML_STATUS_OK)
            return status;

        status = srcml_unit_set_language(unit.get(), srcml_archive_get_language(archive.get()));
        if (status != SRCML_STATUS_OK)
            return status;

        // unit filename is based on convenience functions or the input filename
        if (srcml_unit_get_filename(&global_unit) != 0)
            status = srcml_unit_set_filename(unit.get(), srcml_unit_get_filename(&global_unit));
        else
            status = srcml_unit_set_filename(unit.get(), input_filename);
        if (status != SRCML_STATUS_OK)
            return status;

        // get value possibly set by convenience functions
        status = srcml_unit_set_version(unit.get(), srcml_archive_get_version(archive.get()));
        if (status != SRCML_STATUS_OK)
            return status;

        status = srcml_unit_set_timestamp(unit.get(), srcml_unit_get_timestamp(&global_unit));
        if (status != SRCML_STATUS_OK)
            return status;

        // parse the unit
        status = srcml_unit_parse_filename(unit.get(), input_filename);
        if (status != SRCML_STATUS_OK)
            return status;

        // write the parse unit to the archive
        status = srcml_archive_write_unit(archive.get(), unit.get());
        if (status != SRCML_STATUS_OK)
            return status;

    } else {

        // check the extension for .xml or .srcml, non case-sensitive
        size_t len = strlen(input_filename);
        if (!((len > 4 && tolower(input_filename[len - 1]) == 'l' && tolower(input_filename[len - 2]) == 'm'
            && ((tolower(input_filename[len - 3]) == 'x' && input_filename[len - 4] == '.')
             || (tolower(input_filename[len - 3]) == 'c' && tolower(input_filename[len - 4]) == 'r' && tolower(input_filename[len - 5]) == 's' && tolower(input_filename[len - 6]) == '.')))
           || (global_archive.language && strcmp(global_archive.language->c_str(), "xml") == 0))) {

            if (global_archive.language) {
                global_archive.error_string = "Language '";
                global_archive.error_string += global_archive.language->c_str();
                global_archive.error_string += "' is not supported.";
            } else
                global_archive.error_string = "No language provided.";

            return SRCML_STATUS_INVALID_INPUT;
        }

        std::unique_ptr<srcml_archive> archive(srcml_archive_clone(&global_archive));
        if (!archive) {
            global_archive.error_string = "Unable to create srcML archive";
            return SRCML_STATUS_ERROR;
        }

        int status = srcml_archive_read_open_filename(archive.get(), input_filename);
        if (status != SRCML_STATUS_OK)
            return status;

        std::unique_ptr<srcml_unit> unit(srcml_archive_read_unit(archive.get()));
        if (!unit)
            return status;

        status = srcml_unit_unparse_filename(unit.get(), output_filename);
        if (status != SRCML_STATUS_OK)
            return status;
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
 * srcml_set_options
 * @param option a srcml options
 *
 * Set the srcml options.  Clears all previously set.
 *
 * @returns Return SRCML_STATUS_OK success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_set_options(size_t option) {

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
int srcml_enable_option(size_t option) {

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
int srcml_disable_option(size_t option) {

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
 * srcml_set_eol
 * @param eol the kind of eol to use for unparse
 *
 * Set the eol to be used for unparse.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT
 * on failure.
 */
int srcml_set_eol(size_t eol) {

    return srcml_unit_set_eol(&global_unit, eol);
}

/**
 * srcml_get_eol
 *
 * Set the eol to be used for unparse.
 *
 * @returns Returns eol for unparse, or NULL
 */
size_t srcml_get_eol() {

    return srcml_unit_get_eol(&global_unit);
}

/**
 * srcml_set_srcdiff_revision
 * @param revision_number
 *
 * Set what revision (0 = original, 1 = modified) in a srcDiff docuement to operate with.
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_set_srcdiff_revision(size_t revision_number) {

    return srcml_archive_set_srcdiff_revision(&global_archive, revision_number);
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
 * srcml_get_loc
 *
 * @returns Get the loc of source, and -1 on failure.
 */
int srcml_get_loc() {

    return srcml_unit_get_loc(&global_unit);
}

/**
 * srcml_get_options
 *
 * @returns Get the currently set options on success and NULL on failure.
 */
int srcml_get_options() {

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
 * srcml_get_srcdiff_revision
 *
 * Gets the srcdiff revision number that is being using for processing.
 *
 * @returns the srcdiff revision number is being using.
 */
size_t srcml_get_srcdiff_revision() {

    return srcml_archive_get_srcdiff_revision(&global_archive);
}

/******************************************************************************
 *                                                                            *
 *                           libsrcml utility functions                       *
 *                                                                            *
 ******************************************************************************/

static const char* langs[] = { "C", "C++", "C#", "Java", "Objective-C" };

/**
 * srcml_check_language
 * @param language a language
 *
 * Check if the current language is supported.
 *
 * @returns Return the numeric representation for that language if supported.
 * Not supported returns 0.
 */
int srcml_check_language(const char* language) {

    if (!language)
        return 0;

    // first find in public languages (ones in langs[], then get the number)
    for (size_t i = 0; i < srcml_get_language_list_size(); ++i)
        if (strcmp(language, langs[i]) == 0)
            return Language::getLanguage(language);

    return 0;
}

/**
 * srcml_get_language_list_size
 *
 * Gets the number of languages supported.
 *
 * @returns number of languages supported.
 */
size_t srcml_get_language_list_size() {

    return sizeof(langs) / sizeof(langs[0]);
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

    if (pos >= srcml_get_language_list_size()) return NULL;

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

    if (encoding == nullptr)
        return 0;

    /*
       valgrind produces the error "Conditional jump or move depends on uninitialised value(s)"
       based on a strcmp() inside of xml. For a long, invalid name, no error is produced
    */
    auto result = xmlParseCharEncoding(encoding);

    return result != XML_CHAR_ENCODING_NONE && result != XML_CHAR_ENCODING_ERROR;
}

/**
 * srcml_check_xslt
 *
 * Check whether xslt is available.
 * @returns Return 1 on success and 0 on failure.
 */
int srcml_check_xslt() {
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    void* handle = dlopen_libxslt();
    if (!handle)
        return 0;

    dlclose(handle);
#endif

    return 1;
}

/**
 * srcml_check_exslt
 *
 * Check whether exslt is available.
 * @returns Return 1 on success and 0 on failure.
 */
int srcml_check_exslt() {
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    void* handle = dlopen_libexslt();
    if (!handle)
        return 0;

    dlclose(handle);
#endif

    return 1;
}

/******************************************************************************
 *                                                                            *
 *                           libsrcml error functions                         *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_error_string
 *
 * @returns Return a string describing last recorded error for convenience functions.
 */
const char* srcml_error_string() { return global_archive.error_string.c_str(); }

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

    if (buffer == nullptr)
        return;

    free(buffer);
}
