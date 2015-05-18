/**
 * @file srcml_archive.cpp
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

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_sax2_reader.hpp>
#include <srcml_translator.hpp>
#include <libxml2_callback_wrappers.hpp>

#include <libxml/encoding.h>

#include <srcmlns.hpp>

/**
 * srcml_archive_check_extension
 * @param archive a srcml_archive
 * @param filename name of a file
 *
 * Retrieve the currently registered language for a file extension.
 * Full filename can be provided, and extension will be extracted.
 *
 * @returns the language for extension or if 0 if no language.
 */
const char * srcml_archive_check_extension(srcml_archive * archive, const char* filename) {

    if(archive == NULL || filename == NULL) return 0;

    Language language(archive->registered_languages.get_language_from_filename(filename));
    const char * lang_string = language.getLanguageString();
    return strcmp(lang_string, "") == 0 ? 0 : lang_string;

}

/******************************************************************************
 *                                                                            *
 *                        Allocation/cleanup functions                        *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_archive_create
 *
 * Create a new srcml archive.
 * Client will have to free it using srcml_archive_free().
 *
 * @returns the created archive.
 */
srcml_archive* srcml_archive_create()

{
    srcml_archive * archive;
    try {

        archive = new srcml_archive;

    } catch(...) { return 0; }

    xmlInitParser();

    archive->type = SRCML_ARCHIVE_INVALID;
    archive->options = SRCML_OPTION_DEFAULT;
    archive->revision = srcml_version_string();
    archive->tabstop = 8;
    archive->translator = 0;
    archive->reader = 0;
    archive->input = 0;

    // default prefixes
    srcml_archive_register_namespace(archive, SRCML_SRC_NS_PREFIX_DEFAULT, SRCML_SRC_NS_URI);

    archive->registered_languages.register_standard_file_extensions();

    return archive;

}

/**
 * srcml_archive_free
 * @param archive a srcml_archive
 *
 * Free a srcml archive that was previously
 * allocated by srcml_archive_create().
 * archive must be reallocated/re-created to use again.
 */
void srcml_archive_free(srcml_archive * archive) {

    if(archive == NULL) return;
    srcml_clear_transforms(archive);

    delete archive;
}

/**
 * srcml_archive_clone
 * @param archive a srcml_archive
 *
 * Clone the setup of an existing archive.
 * Client will have to free it using srcml_archive_free().
 *
 * @return the cloned archive
 */
srcml_archive* srcml_archive_clone(const struct srcml_archive* archive) {

    if(archive == NULL) return 0;

    srcml_archive * new_archive = srcml_archive_create();

    if(!new_archive) return 0;

    new_archive->encoding = archive->encoding;
    new_archive->revision = archive->revision;
    new_archive->language = archive->language;
    new_archive->url = archive->url;
    new_archive->version = archive->version;

    try {

        for(std::vector<std::string>::size_type pos = 0; pos < archive->attributes.size(); ++pos)
            new_archive->attributes.push_back(archive->attributes.at(pos));

    } catch(...) {}

    new_archive->options = archive->options;
    new_archive->tabstop = archive->tabstop;

    // clear out those added by srcml_archive_create
    new_archive->prefixes.clear();
    new_archive->namespaces.clear();
    for(std::vector<std::string>::size_type pos = 0; pos < archive->namespaces.size(); ++pos) {

        try {

            srcml_archive_register_namespace(new_archive, archive->prefixes.at(pos).c_str(), archive->namespaces.at(pos).c_str());

        } catch(...) {}

    }

    new_archive->processing_instruction = archive->processing_instruction;

    new_archive->registered_languages = archive->registered_languages;

    try {

        new_archive->user_macro_list.clear();
        for(std::vector<std::string>::size_type i = 0; i < archive->user_macro_list.size(); ++i)
            new_archive->user_macro_list.push_back(archive->user_macro_list.at(i));

    } catch(...) {}

    return new_archive;

}

/******************************************************************************
 *                                                                            *
 *                           Set up functions                                 *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_archive_set_src_encoding
 * @param archive a srcml_archive
 * @param src_encoding an src_encoding
 *
 * Set the default source encoding of the srcML Archive.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_set_src_encoding(srcml_archive* archive, const char* src_encoding) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->src_encoding = src_encoding ? std::string(src_encoding) : boost::optional<std::string>();

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_set_xml_encoding
 * @param archive a srcml_archive
 * @param encoding an encoding
 *
 * Set the xml encoding of the srcML Archive.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_set_xml_encoding(srcml_archive* archive, const char* encoding) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->encoding = encoding ? std::string(encoding) : boost::optional<std::string>();

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_set_language
 * @param archive a srcml_archive
 * @param language a language
 *
 * Set the language of the srcML Archive.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_set_language(srcml_archive* archive, const char* language) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->language = language ? std::string(language) : boost::optional<std::string>();

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_set_url
 * @param archive a srcml_archive
 * @param url a url path
 *
 * Set the root url attribute of the srcML Archive.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_set_url (srcml_archive* archive, const char* url) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->url = url ? std::string(url) : boost::optional<std::string>();

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_set_version
 * @param archive a srcml_archive
 * @param version a version string
 *
 * Set the root version attribute of the srcML Archive.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_set_version(srcml_archive* archive, const char* version) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->version = version ? std::string(version) : boost::optional<std::string>();

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_set_options
 * @param archive a srcml_archive
 * @param options a set of srcml options
 *
 * Set all options for processing an archive (erases all previous).
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_set_options(srcml_archive* archive, unsigned long long options) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->options = options;
    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_enable_option
 * @param archive a srcml_archive
 * @param option a srcml option
 *
 * Enable/set a srcml option, adding to the previously active options.
 * May set multiple options with same call by |ing each.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_enable_option(srcml_archive* archive, unsigned long long option) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->options |= option;
    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_disable_option
 * @param archive a srcml_archive
 * @param option a srcml option
 *
 * Remove a srcml option.
 * May clear multiple options with same call by |ing each.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_disable_option(srcml_archive* archive, unsigned long long option) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->options &= ~option;
    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_set_tabstop
 * @param archive a srcml_archive
 * @param tabstop size of a tabstop
 *
 * Set the tabstop size for position/column calculation.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_archive_set_tabstop(srcml_archive* archive, size_t tabstop) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->tabstop = tabstop;
    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_register_file_extension
 * @param archive a srcml_archive
 * @param extension a file extension
 * @param language a language
 *
 * Set the given extension to be associate with the given language.
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_register_file_extension(srcml_archive* archive, const char* extension, const char* language) {

    if(archive == NULL || extension == NULL || language == NULL)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if(archive->registered_languages.register_user_ext(extension, language))
        return SRCML_STATUS_OK;
    return SRCML_STATUS_INVALID_INPUT;
}

/**
 * srcml_archive_register_namespace
 * @param archive a srcml_archive
 * @param prefix a XML namespace prefix
 * @param uri a XML namespace uri
 *
 * Create a new namespace or change the prefix of an existing namespace.
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_register_namespace(srcml_archive* archive, const char* prefix, const char* uri) {

    if(archive == NULL || prefix == NULL || uri == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    try {

        for(std::vector<std::string>::size_type i = 0; i < archive->prefixes.size(); ++i)
            if(archive->namespaces.at(i) == uri) {

                archive->prefixes.at(i) = prefix;
                return SRCML_STATUS_OK;
            }

    } catch(...) { return SRCML_STATUS_ERROR; }

    archive->prefixes.push_back(prefix);
    archive->namespaces.push_back(uri);

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_set_processing_instruction
 * @param archive a srcml_archive
 * @param target the processing instruction's target
 * @param data the processing instruciton's data
 *
 * Set a processing instruction that will be output before root element of archive.
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_set_processing_instruction(srcml_archive* archive, const char* target, const char* data) {

    if(archive == NULL) return SRCML_STATUS_INVALID_ARGUMENT;


    archive->processing_instruction = std::pair<std::string,std::string>(target, data);

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_register_macro
 * @param archive a srcml_archive
 * @param token name of macro
 * @param type macro type
 *
 * Register a macro (token) to be processed as a special type.
 *
 * Here is a list of the currently supported special types:
 * src:macro     -> Treat the token as a standalone macro (will be marked with a macro tag)
 * src:type      -> Treat the token as an identifier (will still be marked as macro tag)
 * src:name      -> Treat the token as an identifier (will still be marked as macro tag)
 * src:specifier -> Treat the token as a specifier (will be marked with a specifier tag around macro tag)
 * src:label     -> Treat the token as a label (a goto label, will be marked with a label tag around macro tag)
 * src:case      -> Treat as a case label (mark with case tag) either case keyword or case keyword and label
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_register_macro(srcml_archive* archive, const char* token, const char* type) {

    if(archive == NULL || token == NULL || type == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    try {

        std::vector<std::string>::size_type user_macro_list_size = archive->user_macro_list.size() / 2;
        for(std::vector<std::string>::size_type i = 0; i < user_macro_list_size; ++i)
            if(archive->user_macro_list.at(i * 2) == token) {

                archive->user_macro_list.at(i * 2 + 1) = type;
                return SRCML_STATUS_OK;
            }

    } catch(...) { return SRCML_STATUS_ERROR; }

    archive->user_macro_list.push_back(token);
    archive->user_macro_list.push_back(type);

    return SRCML_STATUS_OK;

}

/******************************************************************************
 *                                                                            *
 *                           Accessor Functions                               *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_archive_get_src_encoding
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently default source encoding or NULL.
 */
const char* srcml_archive_get_src_encoding(const struct srcml_archive* archive) {

    return archive && archive->src_encoding ? archive->src_encoding->c_str() : 0;

}

/**
 * srcml_archive_get_xml_encoding
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently set xml encoding or NULL.
 */
const char* srcml_archive_get_xml_encoding(const struct srcml_archive* archive) {

    return archive && archive->encoding ? archive->encoding->c_str() : 0;

}

/**
 * srcml_archive_get_revision
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently set srcML revision or NULL.
 */
const char* srcml_archive_get_revision(const struct srcml_archive* archive) {

    return archive && archive->revision ? archive->revision->c_str() : 0;

}

/**
 * srcml_archive_get_language
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently set language or NULL.
 */
const char* srcml_archive_get_language(const struct srcml_archive* archive) {

    return archive && archive->language ? archive->language->c_str() : 0;

}

/**
 * srcml_archive_get_url
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently set root url attribute or NULL.
 */
const char* srcml_archive_get_url(const struct srcml_archive* archive) {

    return archive && archive->url ? archive->url->c_str() : 0;

}

/**
 * srcml_archive_get_version
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently set root version attribute or NULL.
 */
const char* srcml_archive_get_version(const struct srcml_archive* archive) {

    return archive && archive->version ? archive->version->c_str() : 0;

}

/**
 * srcml_archive_get_options
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently set options.
 */
unsigned long long srcml_archive_get_options(const struct srcml_archive* archive) {

    return archive ? archive->options : 0;

}

/**
 * srcml_archive_get_tabstop
 * @param archive a srcml_archive
 *
 * @returns Retrieve the currently set tabstop size.
 */
size_t srcml_archive_get_tabstop(const struct srcml_archive* archive) {

    return archive ? archive->tabstop : 0;

}

/**
 * srcml_archive_get_namespace_size
 * @param archive a srcml_archive
 *
 * @returns Get the number of currently defined namespaces or 0 if archive is NULL
 */
size_t srcml_archive_get_namespace_size(const struct srcml_archive* archive) {

    /** @todo may want to make ssize_t so can return -1 */
    return archive ? archive->namespaces.size() : 0;

}

/**
 * srcml_archive_get_namespace_prefix
 * @param archive a srcml_archive
 * @param pos namespace position
 *
 * @returns Get prefix for the given position on success
 * and NULL on failure.
 */
const char* srcml_archive_get_namespace_prefix(const struct srcml_archive* archive, size_t pos) {

    if(archive == NULL) return 0;

    try {

        return archive->prefixes.at(pos).c_str();

    } catch(...) {

        return 0;

    }

}

/**
 * srcml_archive_get_prefix_from_uri
 * @param archive a srcml_archive
 * @param namespace_uri an XML namespace
 *
 * @returns Get the registered prefix for the given namespace
 * on success and NULL on failure.
 */
const char* srcml_archive_get_prefix_from_uri(const struct srcml_archive* archive, const char* namespace_uri) {

    if(archive == NULL || namespace_uri == NULL) return 0;

    try {

        for(std::vector<std::string>::size_type i = 0; i < archive->prefixes.size(); ++i)
            if(archive->namespaces.at(i) == namespace_uri)
                return archive->prefixes.at(i).c_str();

    } catch(...) {}

    return 0;
}

/**
 * srcml_archive_get_namespace_uri
 * @param archive a srcml_archive
 * @param pos position in namespaces
 *
 * @returns Get the namespace at the given pos on succcess
 * and NULL on failure.
 */
const char* srcml_archive_get_namespace_uri(const struct srcml_archive* archive, size_t pos) {

    if(archive == NULL) return 0;

    try {

        return archive->namespaces.at(pos).c_str();

    } catch (...) {

        return 0;

    }

}

/**
 * srcml_archive_get_uri_from_prefix
 * @param archive a srcml_archive
 * @param prefix an XML prefix
 *
 * @returns Get the first namespace for the given prefix on success
 * and NULL on failure.
 */
const char* srcml_archive_get_uri_from_prefix(const struct srcml_archive* archive, const char* prefix) {

    if(archive == NULL || prefix == NULL) return 0;

    try {

        for(std::vector<std::string>::size_type i = 0; i < archive->namespaces.size(); ++i)
            if(archive->prefixes.at(i) == prefix)
                return archive->namespaces.at(i).c_str();

    } catch(...) {}

    return 0;

}

/**
 * srcml_archive_get_processing_instruction_target
 * @param archive a srcml_archive
 *
 * @returns Get the processing instruction target.
 */
const char* srcml_archive_get_processing_instruction_target(const struct srcml_archive* archive) {

    return archive->processing_instruction ? archive->processing_instruction->first.c_str() : 0;

}

/**
 * srcml_archive_get_processing_instruction_data
 * @param archive a srcml_archive
 *
 * @returns Get the processing instruction data.
 */
const char* srcml_archive_get_processing_instruction_data(const struct srcml_archive* archive) {

    return archive->processing_instruction ?  archive->processing_instruction->second.c_str() : 0;

}

/**
 * srcml_archive_get_macro_list_size
 * @param archive a srcml_archive
 *
 * @returns Get the number of currently defined macros or 0 if archive is NULL
 */
size_t srcml_archive_get_macro_list_size(const struct srcml_archive* archive) {

    /** @todo may want to make ssize_t so can return -1 */
    return archive ? (archive->user_macro_list.size() / 2) : 0;

}

/**
 * srcml_archive_get_macro_token
 * @param archive a srcml_archive
 * @param pos macro position
 *
 * @returns Get token for the given position on success
 * and NULL on failure.
 */
const char* srcml_archive_get_macro_token(const struct srcml_archive* archive, size_t pos) {

    if(archive == NULL) return 0;

    try {

        return archive->user_macro_list.at(pos * 2).c_str();

    } catch(...) {

        return 0;

    }

}

/**
 * srcml_archive_get_macro_token_type
 * @param archive a srcml_archive
 * @param token a macro token
 *
 * @returns Get the registered type for the given token
 * on success and NULL on failure.
 */
const char* srcml_archive_get_macro_token_type(const struct srcml_archive* archive, const char* token) {

    if(archive == NULL || token == NULL) return 0;

    try {

        std::vector<std::string>::size_type user_macro_list_size = archive->user_macro_list.size() / 2;
        for(std::vector<std::string>::size_type i = 0;  user_macro_list_size; ++i)
            if(archive->user_macro_list.at(i * 2) == token)
                return archive->user_macro_list.at(i * 2 + 1).c_str();

    } catch(...) {}

    return 0;
}

/**
 * srcml_archive_get_macro_type
 * @param archive a srcml_archive
 * @param pos position in macro list
 *
 * @returns Get the type at the given pos on succcess
 * and NULL on failure.
 */
const char* srcml_archive_get_macro_type(const struct srcml_archive* archive, size_t pos) {

    if(archive == NULL) return 0;

    try {

        return archive->user_macro_list.at(pos * 2 + 1).c_str();

    } catch (...) {

        return 0;

    }

}

/******************************************************************************
 *                                                                            *
 *                       Archive write open functions                         *
 *                                                                            *
 ******************************************************************************/

 int srcml_archive_write_open_internal(srcml_archive * archive, xmlOutputBufferPtr output_buffer) {

if(output_buffer == NULL) return SRCML_STATUS_IO_ERROR;

    archive->type = SRCML_ARCHIVE_WRITE;

    try {

        archive->translator = new srcml_translator(
                                                output_buffer,
                                                archive->encoding ? archive->encoding->c_str() : "UTF-8",
                                                archive->options,
                                                archive->prefixes,
                                                archive->namespaces,
                                                archive->processing_instruction,
                                                archive->tabstop,
                                                srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                                archive->revision ? archive->revision->c_str() : 0,
                                                archive->url ? archive->url->c_str() : 0,
                                                0,
                                                archive->version ? archive->version->c_str() : 0,
                                                archive->attributes, 0, 0, 0);
        archive->translator->set_macro_list(archive->user_macro_list);

    } catch(...) { 

        xmlOutputBufferClose(output_buffer);
        return SRCML_STATUS_IO_ERROR; 

    }

    return SRCML_STATUS_OK;

 }

/**
 * srcml_archive_open_filename
 * @param archive a srcml_archive
 * @param srcml_filename name of an output file
 * @param compression amount of compression 0 none to 9 max
 *
 * Open up a srcml_archive for writing.  Set the output
 * to go to the file srcml_filename.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_filename(srcml_archive* archive, const char* srcml_filename, unsigned short compression) {

    if(archive == NULL || srcml_filename == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    if(compression > 9) compression = 9;

    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(srcml_filename, 0, compression);

    return srcml_archive_write_open_internal(archive, output_buffer);
    
}

/**
 * srcml_archive_open_memory
 * @param archive a srcml_archive
 * @param buffer location to return output string
 * @param size the size of the resulting buffer
 *
 * Open up a srcml_archive for writing.  Set the output
 * to be to memory.  Buffer is allocated and set to the location
 * buffer points at.  Buffer must be freed after use.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_memory(srcml_archive* archive, char** buffer, size_t * size) {

    if(archive == NULL || buffer == NULL || size == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->type = SRCML_ARCHIVE_WRITE;

    try {

        archive->translator = new srcml_translator(
                                                buffer,
                                                size,
                                                archive->encoding ? archive->encoding->c_str() : "UTF-8",
                                                archive->options,
                                                archive->prefixes,
                                                archive->namespaces,
                                                archive->processing_instruction,
                                                archive->tabstop,
                                                srcml_check_language(archive->language ? archive->language->c_str() : 0),
                                                archive->revision ? archive->revision->c_str() : 0,
                                                archive->url ? archive->url->c_str() : 0,
                                                0,
                                                archive->version ? archive->version->c_str() : 0,
                                                archive->attributes, 0, 0, 0);

        archive->translator->set_macro_list(archive->user_macro_list);

    } catch(...) { return SRCML_STATUS_IO_ERROR; }

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_open_FILE
 * @param archive a srcml_archive
 * @param srcml_file FILE opened for writing
 *
 * Open up a srcml_archive for writing.  Set the output
 * to go to the FILE srcml_file.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_FILE(srcml_archive* archive, FILE* srcml_file) {

    if(archive == NULL || srcml_file == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));

    return srcml_archive_write_open_internal(archive, output_buffer);

}

/**
 * srcml_archive_open_fd
 * @param archive a srcml_archive
 * @param srcml_fd output file descriptor
 *
 * Open up a srcml_archive for writing.  Set the output
 * to go to the file descriptor srcml_fd.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_fd(srcml_archive* archive, int srcml_fd) {

    if(archive == NULL || srcml_fd < 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));

    return srcml_archive_write_open_internal(archive, output_buffer);

}

/**
 * srcml_archive_open_io
 * @param archive a srcml_archive
 * @param context an io context
 * @param write_callback a write callback function
 * @param close_callback a close callback function
 *
 * Open up a srcml_archive for writing.  Set the output
 * to go the opened io context written to using write callback
 * and closed using close callback.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_io(srcml_archive* archive, void * context, int (*write_callback)(void * context, const char * buffer, size_t len), int (*close_callback)(void * context)) {

    if(archive == NULL || context == NULL || write_callback == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlOutputBufferPtr output_buffer = 0;
    archive->context = libxml2_write_context{context, write_callback, close_callback};
    try {

        output_buffer = xmlOutputBufferCreateIO(write_callback_wrapper, write_close_callback_wrapper,
                                                               boost::any_cast<libxml2_write_context>(&archive->context), xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));

    } catch(boost::bad_any_cast cast) { return SRCML_STATUS_ERROR; }

    return srcml_archive_write_open_internal(archive, output_buffer);

}

/******************************************************************************
 *                                                                            *
 *                       Archive read open functions                          *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_read_internal
 * @param archive a srcml_archive
 *
 * Function used internally to the srcml_archive_read_open_* functions.
 * Reads and sets the open type as well as gathers the attributes
 * and sets the options from the opened srcML Archive.
 */
static int srcml_archive_read_open_internal(srcml_archive * archive) {

    try {

        archive->reader = new srcml_sax2_reader(archive->input);

    } catch(...) {

        xmlFreeParserInputBuffer(archive->input);
        return SRCML_STATUS_IO_ERROR;

    }

    archive->type = SRCML_ARCHIVE_READ;

    boost::optional<std::string> encoding, language, url, version;
    bool done = !archive->reader->read_root_unit_attributes(encoding, language, url, version,
                                                            archive->attributes, archive->prefixes,
                                                            archive->namespaces,
                                                            archive->processing_instruction,
                                                            archive->options,
                                                            archive->tabstop,
                                                            archive->user_macro_list);
    if(!done) {

        if(!archive->encoding) archive->encoding = encoding;
        archive->language = language;
        archive->url = url;
        archive->version = version;

    }

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_read_open_filename
 * @param archive a srcml_archive
 * @param srcml_filename name of an input file
 *
 * Open a srcML archive for reading.  Set the input to be read from
 * srcml_filename.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_read_open_filename(srcml_archive* archive, const char* srcml_filename) {

    if(archive == NULL || srcml_filename == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->input = xmlParserInputBufferCreateFilename(srcml_filename, archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);

    return srcml_archive_read_open_internal(archive);

}

/**
 * srcml_archive_read_open_memory
 * @param archive a srcml_archive
 * @param buffer an input buffer
 * @param buffer_size size of the input buffer
 *
 * Open a srcML archive for reading.  Set the input to be read from
 * the buffer up until buffer_size.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_read_open_memory(srcml_archive* archive, const char* buffer, size_t buffer_size) {

    if(archive == NULL || buffer == NULL || buffer_size <= 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlCharEncoding encoding = archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE;
    archive->input = xmlParserInputBufferCreateMem(buffer, (int)buffer_size, encoding);

    if(encoding != XML_CHAR_ENCODING_NONE && archive->input && archive->input->encoder) {

#ifdef LIBXML2_NEW_BUFFER
        xmlParserInputBufferPtr temp_parser = xmlAllocParserInputBuffer(encoding);
        xmlBufPtr save_buf = archive->input->raw;
        archive->input->raw = archive->input->buffer;
        archive->input->buffer = temp_parser->buffer;
        temp_parser->buffer = save_buf;
        xmlFreeParserInputBuffer(temp_parser);
#else
        if(archive->input->raw)
            xmlBufferFree(archive->input->raw);
        archive->input->raw = archive->input->buffer;
        archive->input->rawconsumed = 0;
        archive->input->buffer = xmlBufferCreate();
#endif

        xmlParserInputBufferGrow(archive->input, buffer_size > 4096 ? (int)buffer_size : 4096);

    }

    return srcml_archive_read_open_internal(archive);

}

/**
 * srcml_archive_read_open_FILE
 * @param archive a srcml_archive
 * @param srcml_file a FILE opened for reading
 *
 * Open a srcML archive for reading.  Set the input to be read from
 * the FILE srcml_file.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_read_open_FILE(srcml_archive* archive, FILE* srcml_file) {

    if(archive == NULL || srcml_file == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->input = xmlParserInputBufferCreateFile(srcml_file, archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);
    
    return srcml_archive_read_open_internal(archive);

    return SRCML_STATUS_OK;

}

/**
 * srcml_archive_read_open_fd
 * @param archive a srcml_archive
 * @param srcml_fd a file descriptor opened for reading
 *
 * Open a srcML archive for reading.  Set the input to be read from
 * the file descriptor srcml_fd.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_read_open_fd(srcml_archive* archive, int srcml_fd) {

    if(archive == NULL || srcml_fd < 0) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->input = xmlParserInputBufferCreateFd(srcml_fd, archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);
    archive->input->closecallback = 0;

    return srcml_archive_read_open_internal(archive);

}

/**
 * srcml_archive_read_open_io
 * @param archive a srcml_archive
 * @param context an io context
 * @param read_callback a read callback function
 * @param close_callback a close callback function
 *
 * Open a srcML archive for reading.  Set the input to be read from
 * the opened context accessed via read_callback and closed via close_callback.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_read_open_io(srcml_archive* archive, void * context, int (*read_callback)(void * context, char * buffer, size_t len), int (*close_callback)(void * context)) {

    if(archive == NULL || context == NULL || read_callback == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    archive->context = libxml2_read_context{context, read_callback, close_callback};
    try {

        archive->input = xmlParserInputBufferCreateIO(read_callback_wrapper, read_close_callback_wrapper, boost::any_cast<libxml2_read_context>(&archive->context), archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE);

    } catch(boost::bad_any_cast cast) {}

    return srcml_archive_read_open_internal(archive);

}

/******************************************************************************
 *                                                                            *
 *                       Archive read/write unit functions                    *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_write_unit
 * @param archive a srcml archive opened for writing
 * @param unit a srcml_unit to output
 *
 * Append the srcml_unit unit to the srcml_archive archive.
 * If copying from a read and only the attributes have been read
 * read in the xml and output.
 *
 * Can not mix with by element mode.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_write_unit(srcml_archive* archive, const struct srcml_unit* unit) {

    if(archive == NULL || unit == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    if(!unit->unit && !unit->read_header) return SRCML_STATUS_UNINITIALIZED_UNIT;

    boost::optional<std::string> read_unit;
    if(!unit->unit && (unit->archive->type == SRCML_ARCHIVE_READ || unit->archive->type == SRCML_ARCHIVE_RW))
        unit->archive->reader->read_srcml(read_unit);

    if(!unit->unit && !read_unit) return SRCML_STATUS_UNINITIALIZED_UNIT;

    if(archive->type != SRCML_ARCHIVE_WRITE && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_INVALID_IO_OPERATION;
    archive->translator->add_unit(unit, read_unit ? read_unit->c_str() : unit->unit->c_str());

    return SRCML_STATUS_OK;
}

/**
 * srcml_read_unit_header
 * @param archive a srcml archive open for reading
 *
 * Read the next unit from the archive.
 * unit contains read attribute.  xml is set to read only if needed
 * in a subsequent call.
 *
 * @returns Return the read srcml_unit on success.
 * On failure returns NULL.
 */
srcml_unit* srcml_read_unit_header(srcml_archive* archive) {

    if(archive == NULL) return 0;

    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return 0;

    srcml_unit * unit = srcml_unit_create(archive);
    int not_done = archive->reader->read_unit_attributes(unit->language, unit->filename, unit->url, unit->version, unit->timestamp, unit->hash, unit->attributes);

    if(!not_done) {
        srcml_unit_free(unit);
        return 0;
    }

    unit->read_header = true;

    return unit;
}

/**
 * srcml_read_unit_xml
 * @param archive a srcml archive open for reading
 *
 * Read the next unit from the archive.
 * unit contains the complete srcml only.
 *
 * @returns Return the read srcml_unit on success.
 * On failure returns NULL.
 */
srcml_unit* srcml_read_unit_xml(srcml_archive* archive) {

    if(archive == NULL) return 0;

    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return 0;

    srcml_unit * unit = srcml_unit_create(archive);
    int not_done = 0;
    if(!unit->read_header)
        not_done = archive->reader->read_unit_attributes(unit->language, unit->filename, unit->url, unit->version, unit->timestamp, unit->hash, unit->attributes);
    archive->reader->read_srcml(unit->unit);

    if(!not_done || !unit->unit) {
        srcml_unit_free(unit);
        unit = 0;
    }

    return unit;
}


/**
 * srcml_read_unit
 * @param archive a srcml archive open for reading
 *
 * Read the next unit from the archive.
 * unit contains read attributes and complete srcml.
 *
 * @returns Return the read srcml_unit on success.
 * On failure returns NULL.
 */
srcml_unit* srcml_read_unit(srcml_archive* archive) {

    if(archive == NULL) return 0;

    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return 0;

    srcml_unit * unit = srcml_unit_create(archive);
    int not_done = 0;
    if(!unit->read_header)
        not_done = archive->reader->read_unit_attributes(unit->language, unit->filename, unit->url, unit->version, unit->timestamp, unit->hash, unit->attributes);
    archive->reader->read_srcml(unit->unit);

    if(!not_done || !unit->unit) {
        srcml_unit_free(unit);
        unit = 0;
    }

    return unit;
}

/**
 * srcml_read_unit_revision
 * @param archive a srcml archive open for reading
 *
 * Read the next unit as the given srcDiff revision (0=original, 1=modified) from the archive.
 * unit contains read attributes and complete srcml.
 *
 * @returns Return the read srcml_unit on success.
 * On failure returns NULL.
 */
srcml_unit* srcml_read_unit_revision(struct srcml_archive* archive, size_t revision_number) {

    if(archive == NULL) return 0;

    if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return 0;

    archive->reader->revision_number(revision_number);

    srcml_unit * unit = srcml_unit_create(archive);
    int not_done = 0;
    if(!unit->read_header)
        not_done = archive->reader->read_unit_attributes(unit->language, unit->filename, unit->url, unit->version, unit->timestamp, unit->hash, unit->attributes);
    archive->reader->read_srcml(unit->unit);

    if(!not_done || !unit->unit) {
        srcml_unit_free(unit);
        unit = 0;
    }

    archive->reader->revision_number(boost::optional<size_t>());

    return unit;

}

/******************************************************************************
 *                                                                            *
 *                       Archive close function                               *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_archive_close
 * @param archive an open srcml archive
 *
 * Close a srcML archive opened using srcml_archive_read_open_*
 * or srcml_archive_write_open_*.
 * Archive can be reopened.
 */
void srcml_archive_close(srcml_archive * archive) {

    if(archive == NULL) return;

    if(archive->translator) archive->translator->close();

    /* FIXME: Crashes when deleted */
 //   if(archive->translator) delete archive->translator, archive->translator = 0;
    if(archive->reader) delete archive->reader, archive->reader = 0;
    if(archive->input) xmlFreeParserInputBuffer(archive->input), archive->input = 0;

    archive->type = SRCML_ARCHIVE_INVALID;

}
