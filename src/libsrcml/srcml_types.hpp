/**
 * @file srcml_types.hpp
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

#ifndef INCLUDED_SRCML_TYPES_HPP
#define INCLUDED_SRCML_TYPES_HPP

/** string for language Objective-C */
#define SRCML_LANGUAGE_OBJECTIVE_C "Objective-C"

/** Private options */

/** Include any XML namespace declarations */
#define SRCML_OPTION_NAMESPACE_DECL    1<<5
/** Markup preprocessor elements (default for C, C++, C#) */
#define SRCML_OPTION_CPP_NOMACRO       1<<2
/** Wrap function/classes/etc with templates (default: on) */
#define SRCML_OPTION_WRAP_TEMPLATE     1<<11
/** Markups literal in special namespace */
#define SRCML_OPTION_LITERAL           1<<21
/** Markups modifiers in special namespace */
#define SRCML_OPTION_MODIFIER          1<<22
/** Markups operator in special namespace */
#define SRCML_OPTION_OPERATOR          1<<23
/** output is interactive (good for editing applications) */
#define SRCML_OPTION_INTERACTIVE       1<<12
/** Turn on optional ternary operator markup */
#define SRCML_OPTION_TERNARY           1<<18
/** Marks up one-line implicit blocks as psuedoblocks (default: on) */
#define SRCML_OPTION_PSEUDO_BLOCK      1<<19
/** Not sure what this used for */
#define SRCML_OPTION_XPATH_TOTAL       1<<13
/** Nest if in else if intead of elseif tag */
#define SRCML_OPTION_NESTIF            1<<9
/** Additional cpp:if/cpp:endif checking */
#define SRCML_OPTION_CPPIF_CHECK       1<<16
/** Extra processing of @code#line@endcode for position information */
#define SRCML_OPTION_LINE              1<<15
/** Markups OpenMP in special namespace */
#define SRCML_OPTION_OPENMP            1<<25
/** Expression mode */
#define SRCML_OPTION_EXPRESSION        1<<14
/** Apply transformations to the entire srcML file (default: each unit */
#define SRCML_OPTION_APPLY_ROOT        1<<8
/** Debug time attribute */
#define SRCML_OPTION_DEBUG_TIMER       1<<17
/** Parser output special tokens for debugging the parser */
#define SRCML_OPTION_DEBUG             1<<24
/** Turn on old optional markup behaviour */
#define SRCML_OPTION_OPTIONAL_MARKUP   1<<20

 /** Create an archive */
#define SRCML_OPTION_ARCHIVE           1<<0
 /** Output hash attribute on each unit (default: on) */
#define SRCML_OPTION_HASH              1<<10

/** All default enabled options */
#define SRCML_OPTION_DEFAULT (SRCML_OPTION_ARCHIVE | SRCML_OPTION_XML_DECL | SRCML_OPTION_HASH)
 
/** All default enabled options */
#define SRCML_OPTION_DEFAULT_INTERNAL (SRCML_OPTION_DEFAULT  | SRCML_OPTION_NAMESPACE_DECL | SRCML_OPTION_PSEUDO_BLOCK | SRCML_OPTION_TERNARY /* | SRCML_OPTION_OPENMP*/)

#include <libxml/xmlwriter.h>

#include <Language.hpp>
#include <language_extension_registry.hpp>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <boost/optional.hpp>
#include <boost/any.hpp>

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
 * SRCML_TRANSORM_TYPE
 *
 * Transformation types, xpath, xslt, and relaxng
 */
enum SRCML_TRANSFORM_TYPE { SRCML_XPATH, SRCML_XSLT, SRCML_RELAXNG };

/**
 * xpath_arguments
 *
 * Data structure to hold xpath arguments
 */
 struct xpath_arguments {

    /** the xpath expression */
    boost::optional<std::string> str;

    /** the optional element prefix */
    boost::optional<std::string> prefix;
    /** the element uri */
    boost::optional<std::string> uri;
    /** the tag name */
    boost::optional<std::string> element;

    /** the optional attribute prefix */
    boost::optional<std::string> attr_prefix;
    /** the attribute uri */
    boost::optional<std::string> attr_uri;
    /** the attribute name */
    boost::optional<std::string> attr_name;
    /** the optional attribute value */
    boost::optional<std::string> attr_value;


 };

/**
 * transform
 *
 * Struct to hold transformation information for latter application.
 */
struct transform {
    
    /** a transformation type */
    SRCML_TRANSFORM_TYPE type;

    /** XSLT parameters */
    std::vector<const char *> xsl_parameters;

    /** the transformation to perform for XPath */
    struct xpath_arguments arguments;

    /** the transformation to perform for XSLT and relaxng */
    xmlDocPtr doc;

    xmlXPathCompExprPtr compiled_xpath;
};

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
    SRCML_ARCHIVE_TYPE type;

    /** @todo rename to xml_encoding */
    /** an attribute for the xml encoding */
    boost::optional<std::string> encoding;
    /** source encoding */
    boost::optional<std::string> src_encoding;
    /** an attribute for a revision */
    boost::optional<std::string> revision;
    /** an attribute for a language */
    boost::optional<std::string> language;
    /** an attribute for a url path */
    boost::optional<std::string> url;
    /** an attribute for a version string */
    boost::optional<std::string> version;
    /** an array of name-value attribute pairs */
    std::vector<std::string> attributes;

    /** srcml options */
    OPTION_TYPE options;

    /** size of tabstop */
    size_t tabstop;

    /** an array of XML namespace prefixes */
    std::vector<std::string> prefixes;

    /** namespaces an array of XML namespaces */
    std::vector<std::string> namespaces;

    /** target/data pair for processing instruction */
    boost::optional<std::pair<std::string, std::string> > processing_instruction;

    /** an array of registered extension language pairs */
    language_extension_registry registered_languages;

    /** an array of user defined macros and their types */
    std::vector<std::string> user_macro_list;

    /** a srcMLTranslator for writing and parsing */
    srcml_translator * translator;

    /** a srcMLReader for reading */
    srcml_sax2_reader * reader;

    /** xmlParserInputBuffer for reading */
    xmlParserInputBufferPtr input;
 
    /** an array of transformations to apply */
    std::vector<transform> transformations;

    /** libxml2 callback wrapper context.  Only needed for IO functions */
    boost::any context;

    /** srcDiff revision number */
    boost::optional<size_t> revision_number;

};

/**
 * srcml_unit
 *
 * Hold data pertaining to a unit in an archive,
 * such as the transformed or collected unit.
 */
struct srcml_unit {
    /** the archive the unit is created from */
    srcml_archive* archive;

    /** source encoding */
    boost::optional<std::string> encoding;
    /** an attribute for a revision */
    boost::optional<std::string> revision;
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
    size_t eol;

    /** output buffer to hold streaming creation of unit */
    xmlBuffer * output_buffer;

    /** a unit srcMLTranslator for writing and parsing as a stream */
    srcml_translator * unit_translator;

    /** store if attributes have been read */
    bool read_header;

    /** a buffer to store srcml from read and after parsing */
    boost::optional<std::string> unit;

    /** libxml2 callback wrapper context.  Only needed for IO functions */
    boost::any context;

};

/** Set the hash attribute for the srcml unit
 * @param unit A srcml_unit
 * @param hash A hash string
 * @retval SRCML_STATUS_OK on success
 * @retval SRCML_STATUS_INVALID_ARGUMENT
 */
int srcml_unit_set_hash (struct srcml_unit* unit, const char* hash);


#endif
