// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcsax_handler.hpp
 *
 * @copyright Copyright (C) 2013-2024 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCSAX_HANDLER_HPP
#define INCLUDED_SRCSAX_HANDLER_HPP

#include <libxml/parser.h>

class srcsax_context;

/**
 * srcsax_handler
 *
 * Struct of srcSAX callback functions i.e. srcSAX handler.
 */
struct srcsax_handler {

    /**
     * start_document
     * @param context a srcSAX context
     *
     * Signature for srcSAX handler function for start of document.
     */
    void (*start_document)(srcsax_context* context);

    /**
     * end_document
     * @param context a srcSAX context
     *
     * Signature for srcSAX handler function for end of document.
     */
    void (*end_document)(srcsax_context* context);

    /**
     * start_root
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Signature for srcSAX handler function for start of the root element.
     */
    void (*start_root)(srcsax_context* context, const char* localname, const char* prefix, const char* URI,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes);

    /**
     * start_unit
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Signature srcSAX handler function for start of an unit.
     */
    void (*start_unit)(srcsax_context* context, const char* localname, const char* prefix, const char* URI,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes);

    /**
     * end_root
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * Signature for srcSAX handler function for end of the root element.
     */
    void (*end_root)(srcsax_context* context, const char* localname, const char* prefix, const char* URI);

    /**
     * end_unit
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     *
     * Signature for srcSAX handler function for end of an unit.
     */
    void (*end_unit)(srcsax_context* context, const char* localname, const char* prefix, const char* URI);

    /**
     * meta_tag
     * @param context a srcSAX context
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param num_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param num_attributes the number of attributes on the tag
     * @param attributes list of attributes
     *
     * Signature for srcSAX handler function for meta tags.
     */
    void (*meta_tag)(srcsax_context* context, const char* localname, const char* prefix, const char* URI,
                           int num_namespaces, const xmlChar** namespaces, int num_attributes,
                           const xmlChar** attributes);
};

#endif
