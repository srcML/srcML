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

    if (!result_nodes->nodesetval)
        return nullptr;

    if (!result_nodes->nodesetval->nodeNr)
        return nullptr;

    if (!element.empty()) {

        addElementXPathResults(doc, result_nodes);

        auto all = xmlXPathNodeSetCreate(xmlDocGetRootElement(doc));
        return all;
    }

    // convert all the found nodes
    if (!attr_name.empty()) {
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            append_attribute_to_node(onode, attr_prefix.c_str(), attr_value.c_str());
        }
    }

    auto all = xmlXPathNodeSetCreate(result_nodes->nodesetval->nodeTab[0]);
    for (int i = 1; i < result_nodes->nodesetval->nodeNr; ++i)
        xmlXPathNodeSetAdd(all, result_nodes->nodesetval->nodeTab[i]);


    return all;


}

#if 0
bool xpathTransformation::apply() {

    // handle new elements and individual results the previous way
    if (element || !attr_name) {

//        auto tr = oarchive->transformations.begin();
        // evaluate the xpath
        xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(tr->compiled_xpath, context);
        if (result_nodes == 0) {
            fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
            return false;
        }
//        applyxpath(++tr, oarchive->transformations.end(), result_nodes);

        return true;
    }

    // apply all XPath transformations on the same base code, and save all the results
//    for (auto& tr : oarchive->transformations) {

        // evaluate the xpath
        tr.result_nodes = xmlXPathCompiledEval(tr.compiled_xpath, context);
        if (tr.result_nodes == 0) {
            fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
            return false;
        }
    }

    // process all the xpath transform results for everything except the last
//    for (auto& tr : oarchive->transformations) {

//        if (&tr == &oarchive->transformations.back())
            break;

        if (!tr.result_nodes->nodesetval)
            continue;

        attr_uri = tr.arguments.attr_uri->c_str();
        attr_prefix = tr.arguments.attr_prefix->c_str();
        attr_name = tr.arguments.attr_name->c_str();
        attr_value = tr.arguments.attr_value->c_str();

        // convert all the found nodes
        for (int i = 0; i < tr.result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = tr.result_nodes->nodesetval->nodeTab[i];

            append_attribute_to_node(onode, attr_prefix, attr_uri);
        }
    }

    // apply regularly to the last. Note this will be what outputs the node
//    attr_uri = oarchive->transformations.back().arguments.attr_uri->c_str();
//    attr_prefix = oarchive->transformations.back().arguments.attr_prefix->c_str();
//    attr_name = oarchive->transformations.back().arguments.attr_name->c_str();
//    attr_value = oarchive->transformations.back().arguments.attr_value->c_str();
//    apply(oarchive->transformations.back().result_nodes);

////        applyxpath(++tr, oarchive->transformations.end(), result_nodes);

    // finished with the result nodes
    //xmlXPathFreeObject(result_nodes);
    return true;
}
#endif

/*
bool xpathTransformation::applyxpath(std::vector<transform>::const_iterator tr, std::vector<transform>::const_iterator end, xmlXPathObjectPtr result_nodes) {

    if (tr == end || xmlXPathNodeSetGetLength(result_nodes->nodesetval) == 0) {
        apply(result_nodes);
        return true;
    }

    save_restore<xmlNodePtr> savectxt = ctxt->node;

    for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

        xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i]; 

        ctxt->node = onode;

        xmlXPathObjectPtr result_nodes2 = xmlXPathCompiledEval(tr->compiled_xpath, context);

        applyxpath(++tr, end, result_nodes2);

        xmlXPathFreeObject(result_nodes2);
    }

    return true;
}
*/
/*
bool xpathTransformation::apply(xmlXPathObjectPtr result_nodes) {

    // record the node type here because it will be needed at the end
    nodetype = result_nodes->type;

    switch (nodetype) {

        case XPATH_NODESET:
        if (element)
            outputXPathResultsElement(result_nodes);
        else if (attr_name)
            outputXPathResultsAttribute(result_nodes);
        else
            outputXPathResultsWrap(result_nodes);
        break;

        case XPATH_NUMBER:
//        oarchive->translator->set_text_only();
        outputXPathResultsNumber(result_nodes);
        break;

        case XPATH_BOOLEAN:
//        oarchive->translator->set_text_only();
        outputXPathResultsBoolean(result_nodes);
        break;

        case XPATH_STRING:
//        oarchive->translator->set_text_only();
        outputXPathResultsString(result_nodes);
        break;

        default :
        break;
    }

    return true;
}
*/
// process the resulting nodes
void xpathTransformation::outputXPathResultsWrap(xmlXPathObjectPtr result_nodes) {}

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

// process the resulting nodes
void xpathTransformation::outputXPathResultsElement(xmlXPathObjectPtr result_nodes) {

#if 0
	if (!result_nodes || !(result_nodes->type == 1) || !(result_nodes->nodesetval))
		return;

    xmlNodePtr a_node = xmlDocGetRootElement(doc);

    // set up namespace
    static xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar*) uri, (const xmlChar*) prefix);

    // output all the found nodes
    for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

        xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

//        xpath_arguments& thisarguments = oarchive->transformations[0].arguments;

        // set up node to insert
        xmlNodePtr element_node = xmlNewNode(ns, (const xmlChar*) thisarguments.element->c_str());

//        if (attr_name)
//            append_attribute_to_node(element_node, thisarguments.attr_uri ? thisarguments.attr_prefix->c_str() : thisarguments.prefix->c_str(), thisarguments.attr_uri->c_str() ? thisarguments.attr_uri->c_str() : thisarguments.uri->c_str());

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

    // output the result
    outputResult(a_node);
#endif
}

// process the resulting nodes
void xpathTransformation::outputXPathResultsAttribute(xmlXPathObjectPtr result_nodes) {
/*
    // use the root element to wrap the result ?
    xmlNodePtr a_node = xmlDocGetRootElement(doc);

    // append the attribute to the nodes
    for (int i = 0; result_nodes->nodesetval && i < result_nodes->nodesetval->nodeNr; ++i) {

        xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

        append_attribute_to_node(onode, attr_uri ? attr_prefix : prefix, attr_uri ? attr_uri : uri);
    }

    // output the result
    outputResult(a_node);
*/
}

void xpathTransformation::outputXPathResultsNumber(xmlXPathObjectPtr result_nodes) {

    total += result_nodes->floatval;

    if (isoption(options, SRCML_OPTION_XPATH_TOTAL))
        return;

//    xmlOutputBufferWriteString(buf, (int)result_nodes->floatval == result_nodes->floatval ?
//        std::to_string((int)result_nodes->floatval).c_str() : std::to_string(result_nodes->floatval).c_str());
//    xmlOutputBufferWriteString(buf, "\n");
}

void xpathTransformation::outputXPathResultsBoolean(xmlXPathObjectPtr result_nodes) {

    result_bool |= (result_nodes->boolval != 0);

    if (isoption(options, SRCML_OPTION_XPATH_TOTAL))
        return;

//    xmlOutputBufferWriteString(buf, result_nodes->boolval ? "true\n" : "false\n");
}

void xpathTransformation::outputXPathResultsString(xmlXPathObjectPtr result_nodes) {

//     xmlOutputBufferWriteString(buf, (char*) result_nodes->stringval);

    // TODO: Is this a separator? Or just to get the code to end on a line?
    // If just to get the code to end on a line, then maybe not put in if the string already has it
//    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n"));
}

/**
 * end_output
 *
 * Finish the archive and close buffer.
 */
int xpathTransformation::end_output() {

    // finalize results
    switch (nodetype) {

        case XPATH_NUMBER:
        if (isoption(options, SRCML_OPTION_XPATH_TOTAL)) {
//            xmlOutputBufferWriteString(buf, (int) total == total ? std::to_string((int)total).c_str() :
//                std::to_string(total).c_str());
//            xmlOutputBufferWriteString(buf, "\n");
        }
        break;

        // boolean result
        case XPATH_BOOLEAN:
        if (isoption(options, SRCML_OPTION_XPATH_TOTAL))
//            xmlOutputBufferWriteString(buf, result_bool ? "true\n" : "false\n");
        break;

        default:
        break;
    }

    if (context)
        xmlXPathFreeContext(context);
    context = 0;

    return 0;
}
