 // SPDX-License-Identifier: GPL-3.0-only
/**
 * @file xpathTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libexslt/exslt.h>

#include <xpathTransformation.hpp>

#include <srcml_translator.hpp>
#include <srcml_sax2_utilities.hpp>
#include <libxml2_utilities.hpp>
#include <memory>

#include <qli_extensions.hpp>
#include <srcql.hpp>

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
xpathTransformation::xpathTransformation(srcml_archive* /* oarchive */, const char* xpath,
                        const char* element_prefix, const char* element_uri, const char* element,
                        const char* attr_prefix, const char* attr_uri, const char* attr_name, const char* attr_value)
    : xpath(xpath), prefix(stringOrNull(element_prefix)), uri(stringOrNull(element_uri)), element(stringOrNull(element)), attr_prefix(stringOrNull(attr_prefix)), attr_uri(stringOrNull(attr_uri)), attr_name(stringOrNull(attr_name)), attr_value(stringOrNull(attr_value)) {

    // compile the xpath expression
    // errors will show up when it is first used
    if (std::string_view(xpath).compare(0, "srcql:"sv.size(), "srcql:") != 0) {
        compiled_xpath = xmlXPathCompile(BAD_CAST xpath);
    }

    // create a namespace for the new attribute (if needed)
    if (attr_uri) {
        attr_ns = xmlNewNs(NULL, (const xmlChar *) attr_uri, (const xmlChar *) attr_prefix);
    }

    // create a namespace for the new element (if needed)
    if (element_uri) {
        element_ns = xmlNewNs(NULL, (const xmlChar*) uri.data(), (const xmlChar*) prefix.data());
    }
}

xpathTransformation::~xpathTransformation() {

    // free the compiled xpath
    if (!compiled_xpath)
        xmlXPathFreeCompExpr(compiled_xpath);

    // free the namespace for any added attributes
    if (attr_ns)
        xmlFreeNs(attr_ns);

    // free the namespace for any added elements
    if (element_ns)
        xmlFreeNs(element_ns);
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

    if ((!root_result_node) || (root_result_node->type != XML_ELEMENT_NODE) || (!root_result_node->name) || ((const char*) root_result_node->name == "unit"sv)) {
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

    std::string_view rootName = (const char*) root_result_node->name;
    int child_count = 1;
    for(xmlNodePtr sibling_node = root_result_node->prev; sibling_node; sibling_node = sibling_node->prev) {

        if (sibling_node->type != XML_ELEMENT_NODE)
            continue;

        if (rootName == (const char*) sibling_node->name &&
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
void xpathTransformation::append_attribute_to_node(xmlNodePtr node, const char* /* attr_prefix */, const char* append_attr_uri) const {

    // grab current value
    const char* value = (char*) xmlGetNsProp(node, BAD_CAST attr_name.data(), BAD_CAST append_attr_uri);
    const char* newvalue = attr_value.data();

    // previous property
    std::string curvalue;
    if (value && std::string_view(value) != newvalue) {

        curvalue = value;
        curvalue += ' ';
        curvalue += attr_value;
        newvalue = curvalue.data();
    }

    xmlNewNsProp(node, attr_ns, (const xmlChar *) attr_name.data(), (const xmlChar *) newvalue);
}

xmlXPathContextPtr xpathTransformation::createContext(xmlDocPtr doc) const {

    auto context = xmlXPathNewContext(doc);

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
TransformationResult xpathTransformation::apply(xmlDocPtr doc, int /* position */) const {

    std::unique_ptr<xmlXPathContext> context(createContext(doc));
    if (!context) {
        fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
        return TransformationResult();
    }

    // register standard prefixes for standard namespaces
    for (const auto& ns : default_namespaces) {

        const char* registerURI = ns.uri.data();
        const char* registerPrefix = ns.prefix.data();
        if (ns.uri == SRCML_SRC_NS_URI)
            registerPrefix = "src";

        if (xmlXPathRegisterNs(context.get(), BAD_CAST registerPrefix, BAD_CAST registerURI) == -1) {
            fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "libsrcml", registerPrefix, registerURI);
        }
    }

    // register prefixes from the doc
    for (auto p = doc->children->nsDef; p; p = p->next) {

        xmlXPathRegisterNs(context.get(), p->prefix, p->href);
    }

    // register srcQL extension functions
    xmlXPathRegisterNs(context.get(),(xmlChar*)"qli",(xmlChar*)"http://www.srcML.org/srcML/srcQLImplementation");
    xmlXPathRegisterFuncNS(context.get(), (const xmlChar*)"add-element",(xmlChar*)"http://www.srcML.org/srcML/srcQLImplementation",&add_element);
    xmlXPathRegisterFuncNS(context.get(), (const xmlChar*)"clear",(xmlChar*)"http://www.srcML.org/srcML/srcQLImplementation",&clear_elements);
    xmlXPathRegisterFuncNS(context.get(), (const xmlChar*)"is-valid-element",(xmlChar*)"http://www.srcML.org/srcML/srcQLImplementation",&is_valid_element);

    auto localCompiledXPath = compiled_xpath;
    // process srcQL query
    if (!localCompiledXPath) {
        std::string_view srcql_string(xpath);
        srcql_string.remove_prefix("srcql:"sv.size());
        const auto srcqlXPath = srcql_convert_query_to_xpath(srcql_string.data(), "C++");
        localCompiledXPath = xmlXPathCompile(BAD_CAST srcqlXPath);
    }

    // evaluate the xpath
    std::unique_ptr<xmlXPathObject> result_nodes(xmlXPathCompiledEval(localCompiledXPath, context.get()));
    if (!result_nodes) {
        fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
        return TransformationResult();
    }

    TransformationResult tresult;
    tresult.unitWrapped = false;

    // xpath evaluation produces a nodeset result, even if there are no results
    tresult.nodeType = result_nodes->type == XPATH_NODESET && (result_nodes->nodesetval == nullptr || result_nodes->nodesetval->nodeNr == 0)? 0 : result_nodes->type;

    // update scalar values, if the type is right
    if (result_nodes->type == XPATH_NUMBER) {
        tresult.numberValue = result_nodes->floatval;
    }
    if (result_nodes->type == XPATH_BOOLEAN) {
        tresult.boolValue = result_nodes->boolval != 0;
    }
    if (result_nodes->type == XPATH_STRING) {
        tresult.stringValue = (const char*) result_nodes->stringval;
    }

    // when result is not a nodeset, then return nullptr, and the calling code will check the other values
    if (result_nodes->type != XPATH_NODESET)
        return tresult;

    if (!element.empty()) {

        addElementXPathResults(doc, result_nodes.get());

        tresult.unitWrapped = true;
        tresult.nodeType = SRCML_RESULT_UNITS;
        tresult.nodeset.reset(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc)));

        return tresult;
    }

    // convert all the found nodes
    if (!attr_name.empty()) {
        if (result_nodes->nodesetval) {
            for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

                xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

                append_attribute_to_node(onode, attr_prefix.data(), attr_value.data());
            }
        }
        tresult.unitWrapped = true;
        tresult.nodeType = SRCML_RESULT_UNITS;
        tresult.nodeset.reset(xmlXPathNodeSetCreate(xmlDocGetRootElement(doc)));

        return tresult;
    }

    if (!result_nodes->nodesetval)
        return tresult;

    if (!result_nodes->nodesetval->nodeNr)
        return tresult;

    if (result_nodes->nodesetval->nodeTab[0] && result_nodes->nodesetval->nodeTab[0]->name &&
        (const char*) result_nodes->nodesetval->nodeTab[0]->name == "unit"sv)
        tresult.unitWrapped = true;

    tresult.nodeset.reset(result_nodes->nodesetval);
    result_nodes->nodesetval = nullptr;

    return tresult;
}

// process the resulting nodes
void xpathTransformation::addElementXPathResults(xmlDocPtr doc, xmlXPathObjectPtr result_nodes) const {

    if (!result_nodes || !(result_nodes->type == 1) || !(result_nodes->nodesetval))
        return;

    xmlNodePtr a_node = xmlDocGetRootElement(doc);

    // add the element to all nodes
    for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

        xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

        // set up node to insert
        xmlNodePtr element_node = xmlNewNode(element_ns, (const xmlChar*) element.data());

        if (!attr_name.empty())
            append_attribute_to_node(element_node, !attr_uri.empty() ? attr_prefix.data() : prefix.data(),
                !attr_uri.empty() ? attr_uri.data() : uri.data());

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
