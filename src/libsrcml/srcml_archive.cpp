/**
 * @file srcml_archive.cpp
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

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>
#include <srcml_translator.hpp>
#include <srcml_sax2_reader.hpp>
#include <libxml/encoding.h>

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
const char* srcml_archive_check_extension(const srcml_archive* archive, const char* filename) {

    if (archive == nullptr || filename == nullptr)
        return 0;

    Language language(archive->registered_languages.get_language_from_filename(filename));
    const char* lang_string = language.getLanguageString();
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
srcml_archive* srcml_archive_create() {

    srcml_archive* archive;
    try {

        archive = new srcml_archive;

    } catch(...) { return nullptr; }

    archive->registered_languages.register_standard_file_extensions();

    return archive;
}

/** Provides a code of the last error to occur for an archive
 * @param archive A srcml_archive
 * @return Error code for last recorded error
 */
int srcml_archive_error_number(const struct srcml_archive* archive) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    return archive->error_number;
}

/** Provides a description of the last error to occur for an archive
 * @param archive A srcml_archive
 * @return A string describing last recorded error
 */
const char* srcml_archive_error_string(const struct srcml_archive* archive) {

    if (archive == nullptr)
        return "Archive does not exist";

    return archive->error_string.c_str();
}

/**
 * srcml_archive_free
 * @param archive a srcml_archive
 *
 * Free a srcml archive that was previously
 * allocated by srcml_archive_create().
 * archive must be reallocated/re-created to use again.
 */
void srcml_archive_free(struct srcml_archive* archive) {

    if (archive->translator) {
        delete archive->translator;
        archive->translator = nullptr;
        archive->output_buffer = nullptr;
    }

    if (archive->output_buffer) {
        xmlOutputBufferClose(archive->output_buffer);
        archive->output_buffer = nullptr;
    }

    if (archive->xbuffer)
        xmlBufferFree(archive->xbuffer);

    if (archive->reader) {
        delete archive->reader;
        archive->reader = nullptr;
    }

    if (archive == nullptr)
        return;

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

    if (archive == nullptr)
        return nullptr;

    std::unique_ptr<srcml_archive> new_archive(srcml_archive_create());
    if (!new_archive)
        return nullptr;

    *new_archive = *archive;

    new_archive->type = SRCML_ARCHIVE_INVALID;
    new_archive->translator = nullptr;
    new_archive->reader = nullptr;
    new_archive->output_buffer = nullptr;
    new_archive->xbuffer = nullptr;
    new_archive->buffer = nullptr;
    new_archive->size = nullptr;
    new_archive->rawwrites = false;
    new_archive->error_string.clear();
    new_archive->error_number = 0;

    return new_archive.release();
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
int srcml_archive_set_src_encoding(struct srcml_archive* archive, const char* src_encoding) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

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
int srcml_archive_set_xml_encoding(struct srcml_archive* archive, const char* encoding) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

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
int srcml_archive_set_language(struct srcml_archive* archive, const char* language) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->language = language ? std::string(language) : decltype(archive->language)();

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
int srcml_archive_set_url (struct srcml_archive* archive, const char* url) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->url = url ? std::string(url) : decltype(archive->url)();

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
int srcml_archive_set_version(struct srcml_archive* archive, const char* version) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (version)
        archive->version = version;
    else
        archive->version = boost::none;

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
int srcml_archive_set_options(struct srcml_archive* archive, size_t options) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // if the user specified cpp, then mark that it was declared
    if (options & SRCML_OPTION_CPP) {
        archive->options |= SRCML_OPTION_CPP_DECLARED;
    }

    int modoption = options % (1<<7);

    archive->options = modoption;

    return SRCML_STATUS_OK;
}

/**
 * @param archive a srcml_archive
 */
int srcml_archive_is_solitary_unit(const struct srcml_archive* archive) {
    return (archive->options & SRCML_OPTION_ARCHIVE) == 0;
}

/**
 * @param archive a srcml_archive
 */
int srcml_archive_has_hash(const struct srcml_archive* archive) {

    return (archive->options & SRCML_OPTION_HASH) != 0;
}

/**
 * @param archive a srcml_archive
 */
int srcml_archive_disable_solitary_unit(struct srcml_archive* archive) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->options |= (unsigned long long)(SRCML_OPTION_ARCHIVE);

    return SRCML_STATUS_OK;
}

/**
 * @param archive a srcml_archive
 */
int srcml_archive_enable_solitary_unit(struct srcml_archive* archive) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->options &= ~(unsigned long long)(SRCML_OPTION_ARCHIVE);

    return SRCML_STATUS_OK;
}

/**
 * @param archive a srcml_archive
 */
int srcml_archive_enable_hash(struct srcml_archive* archive) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->options |= (unsigned long long)(SRCML_OPTION_HASH);

    return SRCML_STATUS_OK;
}

/**
 * @param archive a srcml_archive
 */
int srcml_archive_disable_hash(struct srcml_archive* archive) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->options &= ~(unsigned long long)(SRCML_OPTION_HASH);

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
int srcml_archive_enable_option(struct srcml_archive* archive, size_t option) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // if the user specified cpp, then mark that it was declared
    if (option & SRCML_OPTION_CPP) {
        archive->options |= SRCML_OPTION_CPP_DECLARED;
    }

    int modoption = option % (1<<7);

    archive->options |= modoption;

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
int srcml_archive_disable_option(struct srcml_archive* archive, size_t option) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    int modoption = option % (1<<7);

    archive->options &= ~modoption;

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
int srcml_archive_set_tabstop(struct srcml_archive* archive, size_t tabstop) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

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
int srcml_archive_register_file_extension(struct srcml_archive* archive, const char* extension, const char* language) {

    if (archive == nullptr || extension == nullptr || language == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (archive->registered_languages.register_user_ext(extension, language))
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
int srcml_archive_register_namespace(struct srcml_archive* archive, const char* prefix, const char* uri) {

    if (archive == nullptr || prefix == nullptr || uri == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // lookup by uri, if it already exists, update the prefix. If it doesn't exist, add it
    auto& view = archive->namespaces.get<nstags::uri>();
    auto it = view.find(uri);
    if (it != view.end()) {
        // change prefix of existing namespace
        view.modify(it, [prefix](Namespace& ns) { ns.prefix = prefix; });
    } else {
        // add new namespace
        archive->namespaces.push_back({ prefix, uri, NS_REGISTERED });
    }

    // namespaces for options enable the options automatically
    std::string suri = uri;
    if (suri == SRCML_CPP_NS_URI) {
        archive->options |= SRCML_OPTION_CPP;
        archive->options |= SRCML_OPTION_CPP_DECLARED;
    } else if (suri == SRCML_ERROR_NS_URI) {
        archive->options |= SRCML_OPTION_DEBUG;
    } else if (suri == SRCML_POSITION_NS_URI) {
        archive->options |= SRCML_OPTION_POSITION;
    }

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
int srcml_archive_set_processing_instruction(struct srcml_archive* archive, const char* target, const char* data) {

    if (archive == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

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
int srcml_archive_register_macro(struct srcml_archive* archive, const char* token, const char* type) {

    if (archive == nullptr || token == nullptr || type == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    try {

        std::vector<std::string>::size_type user_macro_list_size = archive->user_macro_list.size() / 2;
        for(std::vector<std::string>::size_type i = 0; i < user_macro_list_size; ++i)
            if (archive->user_macro_list.at(i * 2) == token) {

                archive->user_macro_list.at(i * 2 + 1) = type;
                return SRCML_STATUS_OK;
            }

    } catch(...) { return SRCML_STATUS_ERROR; }

    archive->user_macro_list.push_back(token);
    archive->user_macro_list.push_back(type);

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_set_srcdiff_revision
 * @param archive a srcml_archive
 * @param revision_number
 *
 * Set what revision (0 = original, 1 = modified) in a srcDiff archive to operate with.
 *
 * @returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_set_srcdiff_revision(struct srcml_archive* archive, size_t revision_number) {

    if (archive == nullptr
        || (revision_number != SRCDIFF_REVISION_ORIGINAL && revision_number != SRCDIFF_REVISION_MODIFIED))
            return SRCML_STATUS_INVALID_ARGUMENT;

    archive->revision_number = revision_number;

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
 * Note: Only public options, as defined in srcml.h, are returned
 *
 * @returns Retrieve the currently set options.
 */
int srcml_archive_get_options(const struct srcml_archive* archive) {

    return archive ? (archive->options % (1<<7)) : 0;
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

    if (archive == nullptr)
        return nullptr;

    if (pos > archive->namespaces.size())
        return nullptr;

    return archive->namespaces[pos].prefix.c_str();
}

/**
 * srcml_archive_get_prefix_from_uri
 * @param archive a srcml_archive
 * @param namespace_uri an XML namespace
 *
 * @returns Get the registered prefix for the given namespace
 * on success and NULL on failure.
 */
const char* srcml_archive_get_prefix_from_uri(const struct srcml_archive* archive, const char* uri) {

    if (archive == nullptr || uri == nullptr)
        return 0;

    const auto& view = archive->namespaces.get<nstags::uri>();
    const auto it = view.find(uri);

    return it != view.end() ? it->prefix.c_str() : 0;
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

    if (archive == nullptr)
        return nullptr;

    if (pos >= archive->namespaces.size())
        return nullptr;

    return archive->namespaces[pos].uri.c_str();
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

    if (archive == nullptr || prefix == nullptr)
        return 0;

    const auto& view = archive->namespaces.get<nstags::prefix>();
    const auto& it = view.find(prefix);

    return it != view.end() ? it->uri.c_str() : 0;
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

    if (archive == nullptr)
        return 0;

    if (pos * 2 >= archive->user_macro_list.size())
        return 0;

    return archive->user_macro_list[pos * 2].c_str();
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

    if (archive == nullptr || token == nullptr)
        return 0;

    try {

        std::vector<std::string>::size_type user_macro_list_size = archive->user_macro_list.size() / 2;
        for(std::vector<std::string>::size_type i = 0;  user_macro_list_size; ++i)
            if (archive->user_macro_list.at(i * 2) == token)
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

    if (archive == nullptr)
        return 0;

    if (pos * 2 + 1 >= archive->user_macro_list.size())
        return 0;

    return archive->user_macro_list[pos * 2 + 1].c_str();
}

/**
 * srcml_archive_get_srcdiff_revision
 * @param archive a srcml_archive
 *
 * Gets the srcdiff revision number that the archive is using for processing.
 *
 * @returns the srcdiff revision number the archive is using.
 */
size_t srcml_archive_get_srcdiff_revision(const struct srcml_archive* archive) {

    if (archive == nullptr)
        return SRCDIFF_REVISION_INVALID;

    return archive->revision_number ? *archive->revision_number : SRCDIFF_REVISION_INVALID;
}

/******************************************************************************
 *                                                                            *
 *                       Archive write open functions                         *
 *                                                                            *
 ******************************************************************************/

static int srcml_archive_write_create_translator_xml_buffer(struct srcml_archive* archive) {

    try {

        archive->translator = new srcml_translator(
                                                archive->output_buffer,
                                                optional_to_c_str(archive->encoding, "UTF-8"),
                                                archive->options,
                                                archive->namespaces,
                                                archive->processing_instruction,
                                                archive->tabstop,
                                                srcml_check_language(optional_to_c_str(archive->language)),
                                                optional_to_c_str(archive->revision),
                                                optional_to_c_str(archive->url),
                                                0,
                                                optional_to_c_str(archive->version),
                                                archive->attributes, 0, 0, 0);
        archive->translator->set_macro_list(archive->user_macro_list);

    } catch(...) {

        xmlOutputBufferClose(archive->output_buffer);
        archive->output_buffer = nullptr;
        return SRCML_STATUS_IO_ERROR;
    }

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_write_open_filename
 * @param archive a srcml_archive
 * @param srcml_filename name of an output file
 *
 * Open up a srcml_archive for writing.  Set the output
 * to go to the file srcml_filename.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_filename(struct srcml_archive* archive, const char* srcml_filename) {

    if (archive == nullptr || srcml_filename == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->type = SRCML_ARCHIVE_WRITE;
    archive->output_buffer = xmlOutputBufferCreateFilename(srcml_filename, 0, 0);

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_write_open_memory
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
int srcml_archive_write_open_memory(struct srcml_archive* archive, char** buffer, size_t * size) {

    if (archive == nullptr || buffer == nullptr || size == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->type = SRCML_ARCHIVE_WRITE;

    archive->buffer = buffer;
    archive->size = size;

    archive->xbuffer = xmlBufferCreate();
    archive->output_buffer = xmlOutputBufferCreateBuffer(archive->xbuffer, 0);

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_write_open_FILE
 * @param archive a srcml_archive
 * @param srcml_file FILE opened for writing
 *
 * Open up a srcml_archive for writing.  Set the output
 * to go to the FILE srcml_file.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_FILE(struct srcml_archive* archive, FILE* srcml_file) {

    if (archive == nullptr || srcml_file == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->type = SRCML_ARCHIVE_WRITE;

    archive->output_buffer = xmlOutputBufferCreateFile(srcml_file, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_write_open_fd
 * @param archive a srcml_archive
 * @param srcml_fd output file descriptor
 *
 * Open up a srcml_archive for writing.  Set the output
 * to go to the file descriptor srcml_fd.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_archive_write_open_fd(struct srcml_archive* archive, int srcml_fd) {

    if (archive == nullptr || srcml_fd < 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->type = SRCML_ARCHIVE_WRITE;

    archive->output_buffer = xmlOutputBufferCreateFd(srcml_fd, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_write_open_io
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
int srcml_archive_write_open_io(struct srcml_archive* archive, void * context, int (*write_callback)(void * context, const char* buffer, int len), int (*close_callback)(void * context)) {

    if (archive == nullptr || context == nullptr || write_callback == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    archive->type = SRCML_ARCHIVE_WRITE;

    archive->output_buffer = xmlOutputBufferCreateIO(write_callback, close_callback, context, xmlFindCharEncodingHandler(archive->encoding ? archive->encoding->c_str() : 0));

    return SRCML_STATUS_OK;
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
static int srcml_archive_read_open_internal(struct srcml_archive* archive, std::unique_ptr<xmlParserInputBuffer> input) {

    if (!input)
        return SRCML_STATUS_IO_ERROR;

    try {

        archive->reader = new srcml_sax2_reader(archive, std::move(input));

    } catch(...) {

        return SRCML_STATUS_IO_ERROR;
    }

    archive->type = SRCML_ARCHIVE_READ;

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
int srcml_archive_read_open_filename(struct srcml_archive* archive, const char* srcml_filename) {

    if (archive == nullptr || srcml_filename == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlParserInputBuffer> input(xmlParserInputBufferCreateFilename(srcml_filename, archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE));

    return srcml_archive_read_open_internal(archive, std::move(input));
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
int srcml_archive_read_open_memory(struct srcml_archive* archive, const char* buffer, size_t buffer_size) {

    if (archive == nullptr || buffer == nullptr || buffer_size <= 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    xmlCharEncoding encoding = archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE;
    std::unique_ptr<xmlParserInputBuffer> input(xmlParserInputBufferCreateMem(buffer, (int)buffer_size, encoding));

    // buffer stuff
    if (encoding != XML_CHAR_ENCODING_NONE && input && input->encoder) {

#ifdef LIBXML2_NEW_BUFFER
        {
            std::unique_ptr<xmlParserInputBuffer> temp_parser(xmlAllocParserInputBuffer(encoding));
            xmlBufPtr save_buf = input->raw;
            input->raw = input->buffer;
            input->buffer = temp_parser.get()->buffer;
            temp_parser.get()->buffer = save_buf;
        }
#else
        if (input->raw)
            xmlBufferFree(input->raw);
        input->raw = input->buffer;
        input->rawconsumed = 0;
        input->buffer = xmlBufferCreate();
#endif

        xmlParserInputBufferGrow(input.get(), buffer_size > 4096 ? (int)buffer_size : 4096);
    }

    return srcml_archive_read_open_internal(archive, std::move(input));
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
int srcml_archive_read_open_FILE(struct srcml_archive* archive, FILE* srcml_file) {

    if (archive == nullptr || srcml_file == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlParserInputBuffer> input(xmlParserInputBufferCreateFile(srcml_file, archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE));

    return srcml_archive_read_open_internal(archive, std::move(input));
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
int srcml_archive_read_open_fd(struct srcml_archive* archive, int srcml_fd) {

    if (archive == nullptr || srcml_fd < 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlParserInputBuffer> input(xmlParserInputBufferCreateFd(srcml_fd, archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE));

    return srcml_archive_read_open_internal(archive, std::move(input));
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
int srcml_archive_read_open_io(struct srcml_archive* archive, void * context, int (*read_callback)(void * context, char* buffer, int len), int (*close_callback)(void * context)) {

    if (archive == nullptr || context == nullptr || read_callback == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    std::unique_ptr<xmlParserInputBuffer> input(xmlParserInputBufferCreateIO(read_callback, close_callback, context, archive->encoding ? xmlParseCharEncoding(archive->encoding->c_str()) : XML_CHAR_ENCODING_NONE));

    return srcml_archive_read_open_internal(archive, std::move(input));
}

/******************************************************************************
 *                                                                            *
 *                       Archive read/write unit functions                    *
 *                                                                            *
 ******************************************************************************/

/**
 * srcml_archive_write_unit
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
int srcml_archive_write_unit(struct srcml_archive* archive, struct srcml_unit* unit) {

    if (archive == nullptr || unit == nullptr)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (!unit->read_body && !unit->read_header)
        return SRCML_STATUS_UNINITIALIZED_UNIT;

    // if we haven't read a unit yet, go ahead and try
    if (!unit->read_body && (unit->archive->type == SRCML_ARCHIVE_READ || unit->archive->type == SRCML_ARCHIVE_RW))
        unit->archive->reader->read_body(unit);
    if (!unit->read_body) {
        return SRCML_STATUS_UNINITIALIZED_UNIT;
    }

    if (archive->type != SRCML_ARCHIVE_WRITE && archive->type != SRCML_ARCHIVE_RW)
        return SRCML_STATUS_INVALID_IO_OPERATION;

    // if we haven't opened the translator yet, do so now
    if (archive->translator == nullptr) {
        int status = srcml_archive_write_create_translator_xml_buffer(archive);
        if (status != SRCML_STATUS_OK)
            return status;
    }

    archive->translator->add_unit(unit);

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_write
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
int srcml_archive_write_string(struct srcml_archive* archive, const char* s, int len) {

    if (archive == nullptr || s == nullptr || len < 0)
        return SRCML_STATUS_INVALID_ARGUMENT;

    if (archive->output_buffer)
        xmlOutputBufferWrite(archive->output_buffer, len, s);

    archive->rawwrites = true;

    return SRCML_STATUS_OK;
}

/**
 * srcml_archive_read_unit
 * @param archive a srcml archive open for reading
 *
 * Read the next unit from the archive.
 * unit contains read attribute.  xml is set to read only if needed
 * in a subsequent call.
 *
 * @returns Return the read srcml_unit on success.
 * On failure returns NULL.
 */
struct srcml_unit* srcml_archive_read_unit(struct srcml_archive* archive) {

    if (archive == nullptr)
        return nullptr;

    if (archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW)
        return nullptr;

    std::unique_ptr<srcml_unit> unit(srcml_unit_create(archive));
    int not_done = 0;
    if (!unit->read_header)
        not_done = archive->reader->read_header(unit.get());

    archive->reader->read_body(unit.get());

    if (!not_done || !unit->read_body) {
        return nullptr;
    }

    return unit.release();
}

/**
 * srcml_archive_skip_unit
 * @param archive a srcml archive open for reading
 *
 * Skip the next unit from the archive.
 *
 * @returns 1 on success
 * @returns 0 on failure
 */
int srcml_archive_skip_unit(struct srcml_archive* archive) {

    if (archive == nullptr)
        return 0;

    if (archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW)
        return 0;

    // read the header only of a temporary unit
    std::unique_ptr<srcml_unit> unit(srcml_unit_create(archive));

    int not_done = archive->reader->read_header(unit.get());
    if (!not_done) {
        return 0;
    }

    return 1;
}

/******************************************************************************
 *                                                                            *
 *                       Archive close function                               *
 *                                                                            *
 ******************************************************************************/

/**
 * 3bf7e136e0a6c7494c6f0030a5d0e42502989ae7
 * @param archive an open srcml archive
 *
 * Close a srcML archive opened using srcml_archive_read_open_*
 * or srcml_archive_write_open_*.
 * Archive can be reopened.
 */
void srcml_archive_close(struct srcml_archive* archive) {

    if (archive == nullptr)
        return;

    // if we haven't opened the translator yet, do so now. This will create an empty unit/archive
    if (archive->type == SRCML_ARCHIVE_WRITE && !archive->rawwrites && archive->translator == nullptr) {
        srcml_archive_write_create_translator_xml_buffer(archive);
    }

    if (archive->translator) {
        archive->translator->close();
    }

    if (archive->rawwrites && archive->output_buffer) {
        xmlOutputBufferClose(archive->output_buffer);
        archive->output_buffer = nullptr;
    }

    // Give the user the completed buffer if opened using srcml_archive_write_open_memory()
    if (archive->buffer && archive->size) {

        // record the size before the buffer is detached
        *archive->size = (size_t) archive->xbuffer->use;
        (*archive->buffer) = (char *) xmlBufferDetach(archive->xbuffer);
    }

    archive->type = SRCML_ARCHIVE_INVALID;
}
