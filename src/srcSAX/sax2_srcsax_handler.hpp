/**
 * @file sax2_srcsax_handler.hpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML SAX2 Framework.
 *
 * The srcML SAX2 Framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML SAX2 Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML SAX2 Framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_SAX2_SRCSAX_HANDLER_HPP
#define INCLUDED_SAX2_SRCSAX_HANDLER_HPP

#include <srcml_element.hpp>
#include <srcsax.h>

#include <libxml/parser.h>

#include <string>
#include <vector>

/**
 * srcMLMode
 * 
 * Enum of srcML parsing modes.
 */
enum srcMLMode {

    START,
    ROOT,
    UNIT,
    END_UNIT,
    END_ROOT

};

/**
 * declaration
 *
 * Data structure to hold a declaration.
 */
struct declaration {

    /** default constructor */
    declaration() : type(), name(), mode(TYPE) {}

    /** declaration type */
    std::string type;

    /** declaration name */
    std::string name;

    /** declaration parsing modes */
    enum { TYPE, NAME, INIT } mode;

};

/**
 * function_prototype
 *
 * Data structure to hold a function prototype.
 */
struct function_prototype {

    /**constructor */
    function_prototype(bool is_decl = false) : name(), return_type(), parameter_list(), mode(RETURN_TYPE), is_decl(is_decl) {}

    /** function name */
    std::string name;

    /** function return type */
    std::string return_type;

    /** function parameter list */
    std::vector<declaration> parameter_list;

    /** function prototype parsing modes */
    enum { RETURN_TYPE, NAME, PARAMETER_LIST, PARAMETER } mode;

    /** bool to indicate if function_decl or function */
    bool is_decl;

};

/**
 * sax2_srcsax_handler
 *
 * Data structure to hold process during
 * sax parsing.
 */
struct sax2_srcsax_handler {

    /** default constructor */
    sax2_srcsax_handler() : context(0), root(), meta_tags(), characters(), is_archive(false), mode(START), parse_function(false), in_function_header(false), current_function() {}

    /** hooks for processing */
    srcsax_context * context;

    /** temporary storage for root unit */
    srcml_element root;

    /** temporary storage of meta-tags */
    std::vector<srcml_element> meta_tags;

    /** temporary storate of root characters */
    std::string characters;

    /** used to detect root unit */
    bool is_archive;

    /** open srcMLElement stack */
    std::vector<const char *> srcml_element_stack;

    /** the current parsing mode */
    srcMLMode mode;

    /** bool to indicate if should do special function parsing */
    bool parse_function;

    /** bool to indicate if in funciton for special function parsing */
    bool in_function_header;

    /** store data for special function parsing */
    function_prototype current_function;

};

/**
 * srcsax_sax2_factory
 *
 * Create SAX handler.
 */
xmlSAXHandler srcsax_sax2_factory();

/**
 * start_document
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for start of document.
 * Immediately calls supplied handlers function.
 */
void start_document(void * ctx);

/**
 * end_document
 * @param ctx an xmlParserCtxtPtr
 *
 * SAX handler function for end of document.
 * Calls endRoot if needed then
 * immediately calls supplied handlers function.
 */
void end_document(void * ctx);

/**
 * start_root
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of root element.
 * Caches root info and immediately calls supplied handlers function.
 */
void start_root(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
               int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
               const xmlChar ** attributes);


/**
 * start_element_ns_first
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of first element after root
 * Detects archive and acts accordingly.
 */
void start_element_ns_first(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                         int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar ** attributes);
/**
 * start_unit
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of an unit.
 * Immediately calls supplied handlers function.
 */
void start_unit(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
               int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
               const xmlChar ** attributes);

/**
 * start_element_ns
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 * @param nb_namespaces number of namespaces definitions
 * @param namespaces the defined namespaces
 * @param nb_attributes the number of attributes on the tag
 * @param nb_defaulted the number of defaulted attributes
 * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
 *
 * SAX handler function for start of an element.
 * Immediately calls supplied handlers function.
 */
void start_element_ns(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI,
                    int nb_namespaces, const xmlChar ** namespaces, int nb_attributes, int nb_defaulted,
                    const xmlChar ** attributes);

/**
 * end_element_ns
 * @param ctx an xmlParserCtxtPtr
 * @param localname the name of the element tag
 * @param prefix the tag prefix
 * @param URI the namespace of tag
 *
 * SAX handler function for end of an element.
 * Detects end of unit and calls correct functions
 * for either end_root end_unit or end_element_ns.
 */
void end_element_ns(void * ctx, const xmlChar * localname, const xmlChar * prefix, const xmlChar * URI);

/**
 * characters_first
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling before we
 * know if we have an archive or not.
 * Immediately calls supplied handlers function.
 */
void characters_first(void * ctx, const xmlChar * ch, int len);

/**
 * characters_root
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling at the root level.
 * Immediately calls supplied handlers function.
 */
void characters_root(void * ctx, const xmlChar * ch, int len);

/**
 * characters_unit
 * @param ctx an xmlParserCtxtPtr
 * @param ch the characers
 * @param len number of characters
 *
 * SAX handler function for character handling within a unit.
 * Immediately calls supplied handlers function.
 */
void characters_unit(void * ctx, const xmlChar * ch, int len);

/**
 * comment
 * @param ctx an xmlParserCtxtPtr
 * @param value the comment content
 *
 * A comment has been parsed.
 * Immediately calls supplied handlers function.
 */
void comment(void * ctx, const xmlChar * value);

/**
 * cdata_block
 * @param ctx an xmlParserCtxtPtr
 * @param value the pcdata content
 * @param len the block length
 *
 * Called when a pcdata block has been parsed.
 * Immediately calls supplied handlers function.
 */
void cdata_block(void * ctx, const xmlChar * value, int len);

/**
 * processing_instruction
 * @param ctx an xmlParserCtxtPtr
 * @param target the processing instruction target.
 * @param data the processing instruction data.
 *
 * Called when a processing instruction has been parsed.
 * Immediately calls supplied handlers function.
 */
void processing_instruction(void * ctx, const xmlChar * target, const xmlChar * data);

#endif
