 /**
 * @file xpathTransformation.hpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INCLUDED_XPATHTRANSFORMATION_HPP
#define INCLUDED_XPATHTRANSFORMATION_HPP

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#define DLLOAD
#else
#undef DLLOAD
#endif

#include <libxml/parser.h>

#include <srcmlns.hpp>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

#include <Transformation.hpp>
#include <srcml_translator.hpp>

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
 * xpathTransformation
 *
 * Extends unit_dom to execute XPath expression and write results.
 */
class xpathTransformation : public Transformation {
public :

    /**
     * xpathTransformation
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
    xpathTransformation(srcml_archive* oarchive, const char* xpath, const char* element_prefix, const char* element_uri, const char* element,
                        const char* attr_prefix, const char* attr_uri, const char* attr_name, const char* attr_value);

    /*
     * Destructor
     */
    ~xpathTransformation();

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
    void append_attribute_to_node(xmlNodePtr node, const char* attr_prefix, const char* attr_uri) const;

#pragma GCC diagnostic push

    /**
     * apply
     *
     * Apply XPath expression, writing results.
     *
     * @returns true on success false on failure.
     */
    virtual TransformationResult apply(xmlDocPtr doc, int position) const;

    void addElementXPathResults(xmlDocPtr doc, xmlXPathObjectPtr result_nodes) const;

    // element namespace
    xmlNsPtr element_ns = nullptr;

    // attribute namespace
    xmlNsPtr attr_ns = nullptr;

public:
    std::string xpath;
    std::string prefix;
    std::string uri;
    std::string element;
    std::string attr_prefix;
    std::string attr_uri;
    std::string attr_name;
    std::string attr_value;
    xmlXPathCompExprPtr compiled_xpath = nullptr;

    static const char* const simple_xpath_attribute_name;

private:
    xmlXPathContextPtr createContext(xmlDocPtr doc) const;
#ifdef DLLOAD
    void* handle = nullptr;
#endif

};

#endif
