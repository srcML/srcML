 /**
 * @file xpath_query_units.hpp
 *
 * @copyright Copyright (C) 2008-2014 srcML, LLC. (www.srcML.org)
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

#ifndef INCLUDED_XPATH_QUERY_UNITS_HPP
#define INCLUDED_XPATH_QUERY_UNITS_HPP

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifdef WITH_LIBXSLT
#include <libexslt/exslt.h>
#endif

/** size of string then the literal */
#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s
/** literal followed by its size */
#define LITERALPLUSSIZE(s) s, sizeof(s) - 1

#include <srcexfun.hpp>
#include <transform_units.hpp>
#include <srcmlns.hpp>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

#include <srcml_translator.hpp>

extern std::vector<transform> global_transformations;

/**
 * srcml_xpath
 * @param input_buffer a parser input buffer
 * @param context_element a srcML element that is to be used as the context
 * @param xpath the xpath expression
 * @param prefix an element namepace prefix
 * @param uri an element namepace uri
 * @param element the tag name
 * @param attr_prefix an attribute namepace prefix
 * @param attr_uri an attribute namepace uri
 * @param attr_name the attribute name
 * @param attr_value the attribute value
 * @param options srcml options
 * @param oarchive output srcML archive
 *
 * XPath evaluation of the nested units.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_xpath(xmlParserInputBufferPtr input_buffer, const char * context_element, const char * xpath,
                const char * prefix, const char * uri, const char * element, const char * attr_prefix, const char * attr_uri, const char * attr_name, const char * attr_value,
                OPTION_TYPE options, srcml_archive* oarchive);

/**
 * xpath_query_units
 *
 * Extends unit_dom to execute XPath expression and write results.
 */
class xpath_query_units : public transform_units {
public :

    /**
     * xpath_query_units
     * @param options list of srcML options
     * @param compiled_xpath Combined XPath expression
     * @param prefix an element namepace prefix
     * @param uri an element namepace uri
     * @param element the tag name
     * @param attr_prefix an attribute namepace prefix
     * @param attr_uri an attribute namepace uri
     * @param attr_name the attribute name
     * @param attr_value the attribute value
     * @param fd file descriptor in which to write
     *
     * Constructor.
     */
    xpath_query_units(OPTION_TYPE options, srcml_archive* oarchive, 
                      const char* prefix, const char* uri, const char* element, const char* attr_prefix, const char* attr_uri, const char* attr_name, const char* attr_value);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * form_simple_xpath
     * @param root_result_node the root node form xpath query result
     *
     * Form a simple xpath expression that marks the location of the result.
     * @returns the simple xpath to the result node as a string.
     */
    void form_simple_xpath(xmlTextWriterPtr bufwriter, xmlNodePtr root_result_node);

    /**
     * child_offset
     * @param root_result_node the root node form xpath query result
     *
     * Find the child offset.
     * @returns the child offset number as a string.
     */
    int child_offset(xmlNodePtr root_result_node);

    /**
     * append_attribute_to_node
     * @param node an xmlNode to append the attribute
     * @param attr_name the attribute name
     * @param attr_value the attribute value
     *
     * Append an attribute to the given node.  Only the prefix and uri can vary.  The
     * rest are the same throughout all calls and are part of the class.
     */
    void append_attribute_to_node(xmlNodePtr node, const char* attr_prefix, const char* attr_uri);

    /**
     * start_output
     *
     * Create the output buffer and setup XPath.
     */
     // TODO: start_output needs an error return value
    virtual void start_output();
    
    virtual xmlXPathContextPtr set_context();

#pragma GCC diagnostic push

    /**
     * apply
     *
     * Apply XPath expression, writing results.
     * 
     * @returns true on success false on failure.
     */
    virtual bool apply();

    virtual bool applyxpath(std::vector<transform>::const_iterator tr, std::vector<transform>::const_iterator end, xmlXPathObjectPtr result_nodes);

    virtual bool apply(xmlXPathObjectPtr result_nodes);

    // process the resulting nodes
    virtual void outputXPathResultsWrap(xmlXPathObjectPtr result_nodes);

    // process the resulting nodes
    virtual void outputXPathResultsElement(xmlXPathObjectPtr result_nodes);

    // process the resulting nodes
    virtual void outputXPathResultsAttribute(xmlXPathObjectPtr result_nodes);

    virtual void outputXPathResultsNumber(xmlXPathObjectPtr result_nodes);

    virtual void outputXPathResultsBoolean(xmlXPathObjectPtr result_nodes);

    virtual void outputXPathResultsString(xmlXPathObjectPtr result_nodes);

    /**
     * end_output
     *
     * Finish the archive and close buffer.
     */
    virtual void end_output();

private :

    OPTION_TYPE options;
    const char* prefix;
    const char* uri;
    const char* element;
    const char* attr_prefix;
    const char* attr_uri;
    const char* attr_name;
    const char* attr_value;
    double total;
    bool result_bool;
    int nodetype;
    xmlOutputBufferPtr buf;
    xmlXPathContextPtr context;

    static const char* const simple_xpath_attribute_name;

};


const char* const simple_xpath_attribute_name = "location";

#endif
