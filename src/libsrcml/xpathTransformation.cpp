 /**
 * @file xpathTransformation.cpp
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
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#ifdef WITH_LIBXSLT
#include <libexslt/exslt.h>
#endif

#include <xpathTransformation.hpp>

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
#include <srcml_sax2_utilities.hpp>

const char* const xpathTransformation::simple_xpath_attribute_name = "location";

#define stringOrNull(m) (m ? m : "")

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
xpathTransformation::xpathTransformation(srcml_archive* oarchive, const char* xpath, 
                        const char* element_prefix, const char* element_uri, const char* element,
                        const char* attr_prefix, const char* attr_uri, const char* attr_name, const char* attr_value)
    : xpath(xpath), prefix(stringOrNull(element_prefix)), uri(stringOrNull(element_uri)), element(stringOrNull(element)), attr_prefix(stringOrNull(attr_prefix)), attr_uri(stringOrNull(attr_uri)), attr_name(stringOrNull(attr_name)), attr_value(stringOrNull(attr_value)) {

//    xsltsrcMLRegister();

    compiled_xpath = xmlXPathCompile(BAD_CAST xpath);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/**
 * form_simple_xpath
 * @param root_result_node the root node form xpath query result
 *
 * Form a simple xpath expression that marks the location of the result.
 * @returns the simple xpath to the result node as a string.
 */
void xpathTransformation::form_simple_xpath(xmlTextWriterPtr bufwriter, xmlNodePtr root_result_node) {

    if ((!root_result_node) || (root_result_node->type != XML_ELEMENT_NODE) || (!root_result_node->name) || (strcmp((const char*) root_result_node->name, "unit") == 0)) {
        return;
    }

    form_simple_xpath(bufwriter, root_result_node->parent);

    xmlTextWriterWriteFormatString(bufwriter, "/%s:%s[%d]",
     ((root_result_node->ns && root_result_node->ns->prefix) ? (const char*) root_result_node->ns->prefix : "src"),
     root_result_node->name,
     child_offset(root_result_node)
     );

}

/**
 * child_offset
 * @param root_result_node the root node form xpath query result
 *
 * Find the child offset.
 * @returns the child offset number as a string.
 */
int xpathTransformation::child_offset(xmlNodePtr root_result_node) {

    int child_count = 1;
    for(xmlNodePtr sibling_node = root_result_node->prev; sibling_node; sibling_node = sibling_node->prev) {

        if (sibling_node->type != XML_ELEMENT_NODE)
            continue;

        if ((strcmp((const char*) root_result_node->name, (const char*) sibling_node->name) == 0) &&
            ((root_result_node->ns == NULL && sibling_node->ns == NULL) || (root_result_node->ns->prefix == sibling_node->ns->prefix))) {
            ++child_count;

        }
    }

    return child_count;
}

/**
 * append_attribute_to_node
 * @param node an xmlNode to append the attribute
 * @param attr_name the attribute name
 * @param attr_value the attribute value
 *
 * Append an attribute to the given node.  Only the prefix and uri can vary.  The
 * rest are the same throughout all calls and are part of the class.
 */
void xpathTransformation::append_attribute_to_node(xmlNodePtr node, const char* attr_prefix, const char* attr_uri) {

        // grab current value
    const char* value = (char*) xmlGetNsProp(node, BAD_CAST attr_name.c_str(), BAD_CAST attr_uri);
    const char* newvalue = attr_value.c_str();

        // previous property
    std::string curvalue;
    if (value && strcmp(value, newvalue)) {

        curvalue = value;
        curvalue += ' ';
        curvalue += attr_value;
        newvalue = curvalue.c_str();
    }

    static xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar *) attr_uri, (const xmlChar *) attr_prefix);
    xmlSetNsProp(node, ns, (const xmlChar *) attr_name.c_str(), (const xmlChar *) newvalue);
}

xmlXPathContextPtr xpathTransformation::set_context() {


#if LIBEXSLT_VERSION > 813
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
        typedef int (*exsltXpathCtxtRegister)(xmlXPathContextPtr, const xmlChar*);

        /** create a variable for dynamically load from library */
        typedef void * __attribute__ ((__may_alias__)) VOIDPTR;
#define dlsymvar(type, name) type name;  *(VOIDPTR *)(&name) = dlsym(handle, #name)

    void* handle = dlopen_libexslt();
    if (handle) {

        dlerror();
        dlsymvar(exsltXpathCtxtRegister,exsltDateXpathCtxtRegister);
        if (dlerror() == NULL)  {
            // register exslt functions for XPath usage
            if (exsltDateXpathCtxtRegister(context, BAD_CAST "date") == -1) {
                fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
                    "libsrcml", "date");
            }
        }

        dlerror();
        dlsymvar(exsltXpathCtxtRegister,exsltMathXpathCtxtRegister);
        if (dlerror() == NULL)  {
            if (exsltMathXpathCtxtRegister(context, BAD_CAST "math") == -1) {
                fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
                    "libsrcml", "math");
            }
        }

        dlerror();
        dlsymvar(exsltXpathCtxtRegister,exsltSetsXpathCtxtRegister);
        if (dlerror() == NULL)  {

            if (exsltSetsXpathCtxtRegister(context, BAD_CAST "set") == -1) {
                fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
                    "libsrcml", "set");
            }
        }

        dlerror();
        dlsymvar(exsltXpathCtxtRegister,exsltStrXpathCtxtRegister);
        if (dlerror() == NULL)  {

            if (exsltStrXpathCtxtRegister(context, BAD_CAST "str") == -1) {
                fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
                    "libsrcml", "str");
            }
        }
    }

#undef dlsymvar
#endif
#endif

    return context;
}

#pragma GCC diagnostic push

bool xpathTransformation::hasUnitWrapper() { return unitWrapped; }

/**
 * apply
 *
 * Apply XPath expression, writing results.
 * 
 * @returns true on success false on failure.
 */
xmlNodeSetPtr xpathTransformation::apply(xmlDocPtr doc, int position) {

    xmlXPathContextPtr context = xmlXPathNewContext(doc);

//    xpathsrcMLRegister(context);
    // TODO:  Detect error

    // register standard prefixes for standard namespaces
    for (const auto& ns : default_namespaces) {

        const char* uri = ns.uri.c_str();
        const char* prefix = ns.prefix.c_str();
        if (ns.uri == SRCML_SRC_NS_URI)
            prefix = "src";

        if (xmlXPathRegisterNs(context, BAD_CAST prefix, BAD_CAST uri) == -1) {
            fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "libsrcml", prefix, uri);
        }
    }
/*
    // register namespaces from input archive, which have been setup on the output archive
    for (unsigned int i = 1; i < srcml_archive_get_namespace_size(oarchive); ++i) {

        const char* uri = srcml_archive_get_namespace_uri(oarchive, i);
        const char* prefix = srcml_archive_get_namespace_prefix(oarchive, i);

        if (xmlXPathRegisterNs(context, BAD_CAST prefix, BAD_CAST uri) == -1) {
            fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "libsrcml", prefix, uri);
            throw; //SRCML_STATUS_ERROR;
        }
    }
*/

    // evaluate the xpath
    xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
    if (result_nodes == 0) {
        fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
        return nullptr;
    }

    if (result_nodes->type != XPATH_NODESET) {
        const char* s = (const char*) xmlXPathCastToString(result_nodes);

        auto result = xmlXPathNodeSetCreate(xmlNewTextLen(BAD_CAST s, (int) strlen(s)));

        return result;
    }

    if (!result_nodes->nodesetval)
        return nullptr;

    if (!result_nodes->nodesetval->nodeNr)
        return nullptr;

    if (!element.empty()) {

        addElementXPathResults(doc, result_nodes);

        unitWrapped = true;

        auto all = xmlXPathNodeSetCreate(xmlDocGetRootElement(doc));
        return all;
    }

    // convert all the found nodes
    if (!attr_name.empty()) {
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            append_attribute_to_node(onode, attr_prefix.c_str(), attr_value.c_str());
        }

        unitWrapped = true;

        auto all = xmlXPathNodeSetCreate(xmlDocGetRootElement(doc));
        return all;
    }

    unitWrapped = false;
    if (result_nodes && result_nodes->nodesetval && result_nodes->nodesetval->nodeTab[0] &&
        result_nodes->nodesetval->nodeTab[0]->children && result_nodes->nodesetval->nodeTab[0]->children->name &&
        strcmp((const char*) result_nodes->nodesetval->nodeTab[0]->children->name, "unit"))
        unitWrapped = true;

    auto all = xmlXPathNodeSetCreate(result_nodes->nodesetval->nodeTab[0]);
    for (int i = 1; i < result_nodes->nodesetval->nodeNr; ++i)
        xmlXPathNodeSetAdd(all, result_nodes->nodesetval->nodeTab[i]);

    return all;
}

// process the resulting nodes
void xpathTransformation::addElementXPathResults(xmlDocPtr doc, xmlXPathObjectPtr result_nodes) {

    if (!result_nodes || !(result_nodes->type == 1) || !(result_nodes->nodesetval))
        return;

    xmlNodePtr a_node = xmlDocGetRootElement(doc);

    // set up namespace
    static xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar*) uri.c_str(), (const xmlChar*) prefix.c_str());

    // add the element to all nodes
    for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

        xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

//        xpath_arguments& thisarguments = oarchive->transformations[0].arguments;

        // set up node to insert
        xmlNodePtr element_node = xmlNewNode(ns, (const xmlChar*) element.c_str());

        if (!attr_name.empty())
            append_attribute_to_node(element_node, !attr_uri.empty() ? attr_prefix.c_str() : prefix.c_str(), 
                !attr_uri.empty() ? attr_uri.c_str() : uri.c_str());

        // result node is not a unit
        if (a_node != onode) {

            xmlReplaceNode(onode, element_node);
            xmlAddChild(element_node, onode);

        // result node is a unit
        } else {

            element_node->children = onode->children;
            element_node->last = onode->last;
            element_node->parent = onode;
            element_node->next = 0;
            element_node->prev = 0;
            onode->children = element_node;
            onode->last = element_node;
        }
    }
}
