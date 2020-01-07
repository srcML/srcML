
/**
 * @file srcml_types.hpp
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

#ifndef INCLUDED_SRCML_TYPES_HPP
#define INCLUDED_SRCML_TYPES_HPP

#include <srcml.h>
#include <srcmlns.hpp>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libexslt/exslt.h>

#include <Transformation.hpp>

#include <memory>

/** Private options */

/** Include any XML namespace declarations */
const unsigned int SRCML_OPTION_NAMESPACE_DECL    = 1<<8;
/** Not sure what this used for */
const unsigned int SRCML_OPTION_XPATH_TOTAL       = 1<<9;
/** Extra processing of @code#line@endcode for position information */
const unsigned int SRCML_OPTION_LINE              = 1<<10;
/** Parser output special tokens for debugging the parser */
const unsigned int SRCML_OPTION_DEBUG             = 1<<11;
/** Is a fragment, i.e., no unit element */
const unsigned int SRCML_OPTION_FRAGMENT          = 1<<12;
/** User requested cpp */
const unsigned int SRCML_OPTION_CPP_DECLARED      = 1<<13;
 /** Create an archive */
const unsigned int SRCML_OPTION_ARCHIVE           = 1<<14;
 /** Output hash attribute on each unit (default: on) */
const unsigned int SRCML_OPTION_HASH              = 1<<15;

/** All default enabled options */
const unsigned int SRCML_OPTION_DEFAULT_INTERNAL  = (SRCML_OPTION_ARCHIVE | SRCML_OPTION_HASH | SRCML_OPTION_NAMESPACE_DECL);

#include <libxml/xmlwriter.h>

#include <Language.hpp>
#include <language_extension_registry.hpp>

#include <boost/optional.hpp>

#include <string>
#include <vector>

#ifdef __GNUC__

/** size of option type in GNU */
typedef unsigned long long OPTION_TYPE;

#else

/** size of option type in non-GNU */
typedef unsigned __int64 OPTION_TYPE;

#endif

#ifdef __GNUC__

/** size of options literal in GNU */
#define ull(a) a##ULL

#else

/** size of options literal in non-GNU */
#define ull(a) a##i64

#endif

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
    boost::optional<std::string> encoding;
    /** source encoding */
    boost::optional<std::string> src_encoding;
    /** an attribute for a revision */
    boost::optional<std::string> revision = std::string(srcml_version_string());
    /** an attribute for a language */
    boost::optional<std::string> language;
    /** an attribute for a url path */
    boost::optional<std::string> url;
    /** an attribute for a version string */
    boost::optional<std::string> version;
    /** an array of name-value attribute pairs */
    std::vector<std::string> attributes;

    /** srcml options */
    OPTION_TYPE options = SRCML_OPTION_DEFAULT_INTERNAL;

    /** size of tabstop */
    size_t tabstop = 8;

    /**  new namespace structure */
    Namespaces namespaces = starting_namespaces;

    /** target/data pair for processing instruction */
    boost::optional<std::pair<std::string, std::string> > processing_instruction;

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
    boost::optional<size_t> revision_number;

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
    boost::optional<std::string> encoding;
    /** an attribute for a revision */
    boost::optional<std::string> revision = std::string(srcml_version_string());
    /** an attribute for a language */
    boost::optional<std::string> language;
    /** an attribute name for a file */
    boost::optional<std::string> filename;
    /** an attribute for a url path */
    boost::optional<std::string> url;
    /** an attribute for a version string */
    boost::optional<std::string> version;
    /** an attribute for a timestamp string */
    boost::optional<std::string> timestamp;
    /** an attribute for a hash string */
    boost::optional<std::string> hash;
    /** an array of name-value attribute pairs */
    std::vector<std::string> attributes;
    /** the type of eol to output with source code */
    size_t eol = SOURCE_OUTPUT_EOL_AUTO;

    /** language decided for the unit */
    int derived_language = SRCML_LANGUAGE_NONE;

    /** output buffer to hold streaming creation of unit */
    xmlBuffer* output_buffer = nullptr;

    /** a unit srcMLTranslator for writing and parsing as a stream */
    srcml_translator* unit_translator = nullptr;

    boost::optional<Namespaces> namespaces;

    // if header attributes have been read
    bool read_header = false;

    // if body has been read
    bool read_body = false;

    /** srcml from read and after parsing */
    std::string srcml;
    boost::optional<std::string> srcml_revision;
    int currevision = -1;
    boost::optional<std::string> srcml_fragment;
    boost::optional<std::string> srcml_fragment_revision;
    boost::optional<std::string> srcml_raw;
    boost::optional<std::string> srcml_raw_revision;

    /** src from read */
    boost::optional<std::string> src;

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

// helper conversions for boost::optional<std::string>
inline const char* optional_to_c_str(const boost::optional<std::string>& s) {
    return s ? s->c_str() : 0;
}

inline const char* optional_to_c_str(const boost::optional<std::string>& s, const char* value) {
    return s ? s->c_str() : value;
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
