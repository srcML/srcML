// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_types.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCML_TYPES_HPP
#define INCLUDED_SRCML_TYPES_HPP

#include <srcml.h>
#include <srcmlns.hpp>
#include <srcml_options.hpp>

#include <libxml/tree.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>

#include <Transformation.hpp>

#include <memory>

#include <libxml/xmlwriter.h>

#include <Language.hpp>
#include <language_extension_registry.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <vector>

class srcml_sax2_reader;
class srcml_translator;

/**
 * SRCML_ARCHIVE_TYPE
 *
 * Archive type read, write, or read/write.
 */
enum SRCML_ARCHIVE_TYPE { SRCML_ARCHIVE_INVALID, SRCML_ARCHIVE_RW, SRCML_ARCHIVE_READ, SRCML_ARCHIVE_WRITE };

/**
 * srcml_archive
 *
 * Holds data for a srcML archive read/write.
 */
struct srcml_archive {

    /** type archive type read/write */
    SRCML_ARCHIVE_TYPE type = SRCML_ARCHIVE_INVALID;

    /** an attribute for the xml encoding */
    std::optional<std::string> encoding;
    /** source encoding */
    std::optional<std::string> src_encoding;
    /** an attribute for a revision */
    std::optional<std::string> revision = srcml_version_string();
    /** an attribute for a language */
    std::optional<std::string> language;
    /** an attribute for a url path */
    std::optional<std::string> url;
    /** an attribute for a version string */
    std::optional<std::string> version;
    /** an array of name-value attribute pairs */
    Attributes attributes;

    /** srcml options */
    OPTION_TYPE options = SRCML_OPTION_DEFAULT_INTERNAL;

    /** size of tabstop */
    size_t tabstop = 8;

    /**  new namespace structure */
    Namespaces namespaces = starting_namespaces;

    /** target/data pair for processing instruction */
    std::optional<std::pair<std::string, std::string> > processing_instruction;

    /** an array of registered extension language pairs */
    language_extension_registry registered_languages = language_extension_registry();

    /** an array of user defined macros and their types */
    std::vector<std::string> user_macro_list;

    /** a srcMLTranslator for writing and parsing */
    srcml_translator* translator = nullptr;

    /** a srcMLReader for reading */
    srcml_sax2_reader* reader = nullptr;

    std::vector<std::shared_ptr<Transformation>> transformations;

    /** srcDiff revision number */
    std::optional<size_t> revision_number;

    /** output buffer for io, filename, FILE*, and fd */
    xmlOutputBuffer* output_buffer = nullptr;
    xmlBuffer* xbuffer = nullptr;

    /** output for memory */
    char** buffer = nullptr;
    size_t* size = nullptr;

    /** raw writes were made */
    bool rawwrites = false;

    /** error reporting */
    std::string error_string;
    int error_number = 0;
};

/**
 * srcml_unit
 *
 * Hold data pertaining to a unit in an archive,
 * such as the transformed or collected unit.
 */
struct srcml_unit {

    /** the archive the unit is created from */
    srcml_archive* archive = nullptr;

    /** source encoding */
    std::optional<std::string> encoding;
    /** an attribute for a revision */
    std::optional<std::string> revision = srcml_version_string();
    /** an attribute for a language */
    std::optional<std::string> language;
    /** an attribute name for a file */
    std::optional<std::string> filename;
    /** an attribute for a url path */
    std::optional<std::string> url;
    /** an attribute for a version string */
    std::optional<std::string> version;
    /** an attribute for a timestamp string */
    std::optional<std::string> timestamp;
    /** an attribute for a hash string */
    std::optional<std::string> hash;
    /** a collection of attributes */
    Attributes attributes;

    /** the type of eol to output with source code */
    size_t eol = SOURCE_OUTPUT_EOL_AUTO;

    /** language decided for the unit */
    int derived_language = SRCML_LANGUAGE_NONE;

    /** output buffer to hold streaming creation of unit */
    xmlBuffer* output_buffer = nullptr;

    /** a unit srcMLTranslator for writing and parsing as a stream */
    srcml_translator* unit_translator = nullptr;

    std::optional<Namespaces> namespaces;

    // if header attributes have been read
    bool read_header = false;

    // if body has been read
    bool read_body = false;

    /** srcml from read and after parsing */
    std::string srcml;
    std::optional<std::string> srcml_revision;
    int currevision = -1;
    std::optional<std::string> srcml_fragment;
    std::optional<std::string> srcml_fragment_revision;
    std::optional<std::string> srcml_raw;
    std::optional<std::string> srcml_raw_revision;

    /** src from read */
    std::optional<std::string> src;

    /** record the begin and end of the actual content */
    // int instead of size_t since used with libxml2
    int content_begin = 0;
    int content_end = 0;
    int insert_begin = 0;
    int insert_end = 0;

    int loc = -1;

    /** error reporting */
    std::string error_string;
    int error_number = 0;
};

/** Set the hash attribute for the srcml unit
 * Note: Not publicly available, so declared here instead of srcml.h
 * @param unit A srcml_unit
 * @param hash A hash string
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
int srcml_unit_set_hash (struct srcml_unit* unit, const char* hash);

// helper conversions for std::optional<std::string>
inline const char* optional_to_c_str(const std::optional<std::string>& s) {
    return s ? s->data() : 0;
}

inline const char* optional_to_c_str(const std::optional<std::string>& s, const char* value) {
    return s ? s->data() : value;
}

// RAII for archives and units with std::unique_ptr<>
namespace std {
    template<>
    struct default_delete<srcml_archive> {
        void operator()(srcml_archive* arch) {
            srcml_archive_close(arch);
            srcml_archive_free(arch);
        }
    };

    template<>
    struct default_delete<srcml_unit> {
        void operator()(srcml_unit* unit) {
            srcml_unit_free(unit);
        }
    };
}

#endif
