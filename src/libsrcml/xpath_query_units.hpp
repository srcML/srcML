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

#include <libexslt/exslt.h>

/** size of string then the literal */
#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s
/** literal followed by its size */
#define LITERALPLUSSIZE(s) s, sizeof(s) - 1

#include <srcexfun.hpp>
#include <unit_dom.hpp>
#include <srcmlns.hpp>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

#ifdef _MSC_BUILD
#include <io.h>
#define snprintf _snprintf
#endif

/**
 * xpath_query_units
 *
 * Extends unit_dom to execute XPath expression and write results.
 */
class xpath_query_units : public unit_dom {
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
    xpath_query_units(OPTION_TYPE options, xmlXPathCompExprPtr compiled_xpath, xmlOutputBufferPtr output,
                      const char * prefix = 0, const char * uri = 0, const char * element = 0, const char * attr_prefix = 0, const char * attr_uri = 0, const char * attr_name = 0, const char * attr_value = 0)
        : unit_dom(options), options(options), compiled_xpath(compiled_xpath),
          prefix(prefix), uri(uri), element(element), attr_prefix(attr_prefix), attr_uri(attr_uri), attr_name(attr_name), attr_value(attr_value),
          total(0), found(false), needroot(true), closetag(false), context(0), output(output), result_count(0) {
    }

    /**
     * ~xpath_query_units
     *
     * Destructor.
     */
    virtual ~xpath_query_units() {}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * form_simple_xpath
     * @param root_result_node the root node form xpath query result
     *
     * Form a simple xpath expression that marks the location of the result.
     * @returns the simple xpath to the result node as a string.
     */
    void form_simple_xpath(xmlTextWriterPtr bufwriter, xmlNodePtr root_result_node) {

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
    int child_offset(xmlNodePtr root_result_node) {

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
    void append_attribute_to_node(xmlNodePtr node, const char * attr_prefix, const char * attr_uri) {

        xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar *) attr_uri, (const xmlChar *) attr_prefix);
        xmlNewNsProp(node, ns, (const xmlChar *) attr_name, (const xmlChar *) attr_value);
    }

    /**
     * start_output
     *
     * Create the output buffer and setup XPath.
     */
    virtual void start_output() {

        buf = output;

        bufwriter = xmlNewTextWriter(buf);

        // TODO:  Detect error

#ifdef _MSC_BUILD
        buf->writecallback = (xmlOutputWriteCallback)_write;
#endif

    }

#pragma GCC diagnostic push

    /**
     * apply
     *
     * Apply XPath expression, writing results.
     * 
     * @returns true on success false on failure.
     */
    virtual bool apply() {

        if (!context) {

            context = xmlXPathNewContext(ctxt->myDoc);
            // TODO:  Detect error

            xpathsrcMLRegister(context);
            // TODO:  Detect error

            // register standard prefixes for standard namespaces
            const char* prefixes[] = {
                SRCML_SRC_NS_URI, "src",
                SRCML_CPP_NS_URI, SRCML_CPP_NS_PREFIX_DEFAULT,
                SRCML_ERR_NS_URI, SRCML_ERR_NS_PREFIX_DEFAULT,
                SRCML_EXT_LITERAL_NS_URI, SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
                SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
                SRCML_EXT_MODIFIER_NS_URI, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
                SRCML_EXT_POSITION_NS_URI, SRCML_EXT_POSITION_NS_PREFIX_DEFAULT,
                SRCML_DIFF_NS_URI, SRCML_DIFF_NS_PREFIX_DEFAULT,
                0, 0
            };

            for (unsigned int i = 0; prefixes[i] != 0; i += 2){

                if (xmlXPathRegisterNs(context, BAD_CAST prefixes[i + 1], BAD_CAST prefixes[i]) == -1) {

                    fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "libsrcml", prefixes[i + 1], prefixes[i]);
                    return SRCML_STATUS_ERROR;

                }

            }

#if LIBEXSLT_VERSION > 813
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
            typedef int (*exsltXpathCtxtRegister)(xmlXPathContextPtr, const xmlChar*);

            /** create a variable for dynamically load from library */
            typedef void * __attribute__ ((__may_alias__)) VOIDPTR;
#define dlsymvar(type, name) type name;  *(VOIDPTR *)(&name) = dlsym(handle, #name)


            void* handle = dlopen("libexslt.so", RTLD_LAZY);
            if (!handle) {
                handle = dlopen("libexslt.so.0", RTLD_LAZY);
                if (!handle) {
                    handle = dlopen("libexslt.dylib", RTLD_LAZY);
                    if (!handle)
                        fprintf(stderr, "Unable to open libexslt library\n");
                }
            }

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
        }
        // evaluate the xpath
        xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
        if (result_nodes == 0) {
            fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
            return false;
        }

        // root element if first result
        // TODO: This should move to start_output(), but that will have to wait because it is shared.
        if (result_count == 0) {
            outputRoot(xmlDocGetRootElement(ctxt->myDoc));
        }

        if (result_nodes && xmlXPathNodeSetGetLength(result_nodes->nodesetval)) {
            if (result_nodes->type == XPATH_NODESET && !element && !attr_name)
                outputXPathResultsWrap(result_nodes);
            else
                outputXPathResults(result_nodes);
        }

        // finished with the result nodes
        xmlXPathFreeObject(result_nodes);

        return true;
    }

    // process the resulting nodes
    virtual void outputXPathResultsWrap(xmlXPathObjectPtr result_nodes) {

        nodetype = result_nodes->type;

        // if this is the first real result, then we need space for this internal unit
        if (result_count == 0)
            xmlTextWriterWriteString(bufwriter, BAD_CAST "\n\n");

        // using the internal unit node to serve as the wrapper
        xmlNodePtr a_node = xmlCopyNode(xmlDocGetRootElement(ctxt->myDoc), 2);

        // remove src namespace and save for reassignment
        // TODO: Just remove it directly
        xmlNsPtr src_ns = 0;
        if (false && a_node->nsDef && strcmp((const char *)a_node->nsDef->href, "http://www.sdml.info/srcML/src") == 0) {

            src_ns = a_node->nsDef;
            a_node->nsDef = a_node->nsDef->next;
        }

        // output all the found nodes
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            ++result_count;

            // item attribute on wrapping node
            static char s[100];
            sprintf(s, "%d", i + 1);
            xmlSetProp(a_node, BAD_CAST "item", BAD_CAST s);

            // index into results
            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            // unlink this result node and link to the master parent
            xmlUnlinkNode(onode);
            xmlAddChild(a_node, onode);

            // output the result, with special handling for attribute nodes
            xmlNodeDumpOutput(buf, ctxt->myDoc, a_node, 0, 0, 0);

            // space between result units
            xmlTextWriterWriteString(bufwriter, BAD_CAST "\n\n");

            // put the result node back into place
            xmlUnlinkNode(onode);
            // xmlAddChild(onode_parent, onode);
        }

        if (src_ns)
            a_node->nsDef = src_ns;

        xmlFreeNode(a_node);
    }

    virtual void outputRoot(xmlNodePtr a_node) {

        // xml declaration
        if (isoption(options, SRCML_OPTION_XML_DECL))
            xmlTextWriterStartDocument(bufwriter,
                                       (const char*) ctxt->version,
                                       (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding), 
                                       ctxt->standalone ? "yes" : "no");

        // processing instruction
        if (processing_instruction)
            xmlTextWriterWritePI(bufwriter, BAD_CAST processing_instruction->first.c_str(), BAD_CAST processing_instruction->second.c_str());

        // output a root element, just like the one read in
        // note that this has to be ended somewhere
        xmlTextWriterStartElementNS(bufwriter, NULL, root->localname, root->URI);

        // copy all namespaces from the current unit
        for (size_t pos = 0; pos < (size_t)context->nsNr; ++pos) {
            //                    xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST namespaces[i], BAD_CAST "", BAD_CAST namespaces[i+1]);
        }

        // add in the element namespace if needed
        bool found_element_ns = false;
        if (uri) {

            for (size_t pos = 0; pos < (size_t)context->nsNr; ++pos)
                if (strcmp((const char *)context->namespaces[pos], uri) == 0) {
                    found_element_ns = true;
                    break;
                }

            if (!found_element_ns)
                xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST prefix, BAD_CAST "", BAD_CAST uri);
        }

        // add in the attribute namespace if needed
        bool found_attribute_ns = attr_uri && found_element_ns && (strcmp(uri, attr_uri) == 0);
        if (attr_uri && !found_attribute_ns) {

            for (size_t pos = 0; pos < (size_t)context->nsNr; ++pos)
                if (strcmp((const char *)context->namespaces[pos], uri) == 0) {
                    found_attribute_ns = true;
                    break;
                }
            if (!found_attribute_ns)
                xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST attr_prefix, BAD_CAST "", BAD_CAST attr_uri);
        }

        // copy all the attributes from the current unit
        for (xmlAttrPtr pAttr = a_node->properties; pAttr; pAttr = pAttr->next)
            xmlTextWriterWriteAttribute(bufwriter, pAttr->name, pAttr->children->content);

        closetag = true;

        // meta tags
        for(std::vector<std::string>::size_type i = 0; i < meta_tags.size(); ++i) {

            xmlTextWriterStartElementNS(bufwriter, meta_tags[i].localname, meta_tags[i].prefix, meta_tags[i].URI);

            for (int j = 0; j < meta_tags[i].nb_namespaces; j += 2)
                xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST meta_tags[i].namespaces[j], BAD_CAST "", BAD_CAST meta_tags[i].namespaces[j+1]);

            for (int j = 0; j < meta_tags[i].nb_attributes; j += 2)
                xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST meta_tags[i].attributes[j], BAD_CAST "", BAD_CAST meta_tags[i].attributes[j+1]);

            xmlTextWriterEndElement(bufwriter);
        }

    }

    virtual void outputXPathResults(xmlXPathObjectPtr result_nodes) {

        // process the resulting nodes
        xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);
        bool outputunit = false;
        xmlNodePtr onode = 0;
        int result_size = 0;
        nodetype = result_nodes->type;

        switch (nodetype) {

            // node set result
        case XPATH_NODESET:

            if (needroot /*&& !isoption(options, SRCML_OPTION_APPLY_ROOT)*/) {

                // xml declaration
                if (isoption(options, SRCML_OPTION_XML_DECL))
                    xmlTextWriterStartDocument(bufwriter, (const char*) ctxt->version, (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding), 
                                               ctxt->standalone ? "yes" : "no");

                // processing instruction
                if (processing_instruction)
                    xmlTextWriterWritePI(bufwriter, BAD_CAST processing_instruction->first.c_str(), BAD_CAST processing_instruction->second.c_str());
                //                xml_output_buffer_write_processing_instruction(buf, processing_instruction);

                // output a root element, just like the one read in
                // note that this has to be ended somewhere
                xmlTextWriterStartElementNS(bufwriter, NULL, root->localname, root->URI);

                // copy all namespaces from the current unit
                for (size_t pos = 0; pos < (size_t)context->nsNr; ++pos) {
                    //                    xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST namespaces[i], BAD_CAST "", BAD_CAST namespaces[i+1]);
                }

                // add in the element namespace if needed
                bool found_element_ns = false;
                if (uri) {

                    for (size_t pos = 0; pos < (size_t)context->nsNr; ++pos)
                        if (strcmp((const char *)context->namespaces[pos], uri) == 0) {
                            found_element_ns = true;
                            break;
                        }
                }

                if (uri && !found_element_ns)
                    xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST prefix, BAD_CAST "", BAD_CAST uri);

                // add in the attribute namespace if needed
                bool found_attribute_ns = attr_uri && found_element_ns && (strcmp(uri, attr_uri) == 0);
                if (attr_uri && !found_attribute_ns) {

                    for (size_t pos = 0; pos < (size_t)context->nsNr; ++pos)
                        if (strcmp((const char *)context->namespaces[pos], uri) == 0) {
                            found_attribute_ns = true;
                            break;
                        }
                }
                if (attr_uri && !found_attribute_ns)
                    xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST attr_prefix, BAD_CAST "", BAD_CAST attr_uri);

                // copy all the attributes from the current unit
                for (xmlAttrPtr pAttr = a_node->properties; pAttr; pAttr = pAttr->next) {
                    xmlTextWriterWriteAttribute(bufwriter, pAttr->name, pAttr->children->content);
                }

                closetag = true;

                if(meta_tags.size()) {

                    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">"));
                    for(std::vector<std::string>::size_type i = 0; i < meta_tags.size(); ++i) {

                        xmlTextWriterStartElementNS(bufwriter, meta_tags[i].localname, meta_tags[i].prefix, meta_tags[i].URI);

                        for (int j = 0; j < meta_tags[i].nb_namespaces; j += 2) {
                            xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST meta_tags[i].namespaces[j], BAD_CAST "", BAD_CAST meta_tags[i].namespaces[j+1]);
                        }

                        for (int j = 0; j < meta_tags[i].nb_attributes; j += 2) {
                            xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST meta_tags[i].attributes[j], BAD_CAST "", BAD_CAST meta_tags[i].attributes[j+1]);
                        }

                        xmlTextWriterEndElement(bufwriter);
                    }

                }

            }

            // may not have any values or results
            result_size = xmlXPathNodeSetGetLength(result_nodes->nodesetval);
            if (isoption(options, SRCML_OPTION_APPLY_ROOT) && result_size == 0) {

                xmlTextWriterEndElement(bufwriter);
            }

            if (result_size != 0)
                xmlTextWriterWriteString(bufwriter, BAD_CAST "\n\n");

            needroot = false;

            if (result_size == 0 && !element && !attr_name)
                break;


            // opened the root start element before, now need to close it.
            // why not do this when it is started?  May not have any results, and
            // need an empty element
            /*
              if (closetag) {
              if(meta_tags.size())
              xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));
              else
              xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">\n\n"));

              closetag = false;
              }
            */
            found = true;

            if(element) {

                for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

                    onode = result_nodes->nodesetval->nodeTab[i];

                    // set up namespace
                    xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar*) uri, (const xmlChar*) prefix);

                    // set up node to insert
                    xmlNodePtr element_node = xmlNewNode(ns, (const xmlChar*) element);

                    if(attr_name) append_attribute_to_node(element_node, attr_uri ? attr_prefix : prefix, attr_uri ? attr_uri : uri);

                    if(a_node != onode) {                    

                        element_node->children = onode;
                        element_node->last = onode;
                        element_node->parent = onode->parent;
                        onode->parent = element_node;
                        element_node->next = onode->next;
                        element_node->prev = onode->prev;
                        onode->next = 0;
                        onode->prev = 0;

                        // update former root siblings
                        if(element_node->parent) {

                            if(element_node->parent->children == onode)
                                element_node->parent->children = element_node;
                            else
                                element_node->prev->next = element_node;

                            if(element_node->next)
                                element_node->next->prev = element_node;

                        } 

                    } else {

                        element_node->children = onode->children;
                        element_node->last = onode->last;
                        element_node->parent = onode;
                        element_node->next = 0;
                        element_node->prev = 0;
                        onode->children = element_node;
                        onode->last = element_node;

                    }

                    element_node->doc = onode->doc;

                }

                // remove src namespace and save for reassignment
                xmlNsPtr src_ns = 0;
                if(a_node->nsDef && strcmp((const char *)a_node->nsDef->href, "http://www.sdml.info/srcML/src") == 0) {

                    src_ns = a_node->nsDef;
                    a_node->nsDef = a_node->nsDef->next;

                }

                xmlTextWriterWriteString(bufwriter, BAD_CAST "");
                xmlNodeDumpOutput(buf, ctxt->myDoc, a_node, 0, 0, 0);
                xmlTextWriterWriteString(bufwriter, BAD_CAST "\n\n");
                //                xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));

                // reassign src namespace
                if(src_ns) {

                    a_node->nsDef = src_ns;

                }

            } else if(attr_name) {


                for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

                    onode = result_nodes->nodesetval->nodeTab[i];

                    append_attribute_to_node(onode, attr_prefix, attr_uri);

                }

                // remove src namespace and save for reassignment
                xmlNsPtr src_ns = 0;
                if(a_node->nsDef && strcmp((const char *)a_node->nsDef->href, "http://www.sdml.info/srcML/src") == 0) {

                    src_ns = a_node->nsDef;
                    a_node->nsDef = a_node->nsDef->next;

                }

                xmlNodeDumpOutput(buf, ctxt->myDoc, a_node, 0, 0, 0);
                xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));

                // reassign src namespace
                if(src_ns) {

                    a_node->nsDef = src_ns;

                }

            } else {

                // output all the found nodes
                for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

                    // index into results
                    onode = result_nodes->nodesetval->nodeTab[i];

                    // output a unit element around the fragment, unless
                    // is is already a unit
                    outputunit = !onode->name || strcmp("unit", (const char*) onode->name) != 0;

                    // if we need a unit, output the start tag.  Line number starts at 1, not 0
                    if (outputunit) {

                        // output a wrapping element, just like the one read in
                        // note that this has to be ended somewhere
                        xmlTextWriterStartElement(bufwriter, root->localname);

                        // copy the cpp namespace from the current if it has one unit
                        if (is_archive) {
                            for(std::vector<const xmlChar *>::size_type pos = 1; pos < data.size(); pos += 2)
                                if(strcmp((const char *)data[pos], "http://www.sdml.info/srcML/cpp") == 0) {
                                    xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST "cpp", BAD_CAST "", BAD_CAST "http://www.sdml.info/srcML/cpp");
                                    break;
                                }
                        }

                        // copy all namespaces from the current unit
                        for(std::vector<const xmlChar *>::size_type i = rootsize; i < data.size(); i+=2) {
                            //                                xmlTextWriterWriteAttributeNS(bufwriter, BAD_CAST "xmlns", BAD_CAST data[i], BAD_CAST "", BAD_CAST data[i+1]);
                        }

                        // copy all the attributes from the current unit
                        for (xmlAttrPtr pAttr = a_node->properties; pAttr; pAttr = pAttr->next)
                            xmlTextWriterWriteAttribute(bufwriter, pAttr->name, pAttr->children->content);

                        // append line item number
                        xmlTextWriterWriteFormatAttribute(bufwriter, BAD_CAST "item", "%d", i + 1);

                        // append path
                        xmlTextWriterStartAttribute(bufwriter, BAD_CAST simple_xpath_attribute_name);
                        form_simple_xpath(bufwriter, onode);
                        xmlTextWriterEndAttribute(bufwriter);

                        xmlTextWriterWriteString(bufwriter, BAD_CAST "");
                        xmlTextWriterFlush(bufwriter);
                    }

                    /*
                      Three possibilities:

                      - Input was an archive, and XPath result is a unit
                      Resulting namespaces are those that were on the original unit

                      - Input was not an archive, and XPath result is a unit
                      Need to split the name

                      - XPath result was a node, but not a unit
                    */

                    // input was an archive, xpath result is a unit
                    if (onode->type == XML_ELEMENT_NODE && is_archive && !outputunit) {

                        // create a new list of namespaces
                        // skip over the namespaces on the root
                        xmlNsPtr savens = onode->nsDef;
                        if(!isoption(options, SRCML_OPTION_APPLY_ROOT)) {
                            onode->nsDef = savens;
                            for (std::vector<const xmlChar*>::size_type i = 0; i < unit_dom::rootsize / 2; ++i)
                                onode->nsDef = onode->nsDef->next;
                        }
                        // dump the namespace-modified tree
                        xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);

                        // restore original namespaces
                        onode->nsDef = savens;

                    } else if (onode->type == XML_ELEMENT_NODE && !is_archive && !outputunit) {

                        // input was not an archive, xpath result is a unit

                        // namespace list only need the cpp namespace, if it exists
                        if(!isoption(options, SRCML_OPTION_APPLY_ROOT)) {
                            xmlNsPtr savens = onode->nsDef;
                            for (onode->nsDef = savens; onode->nsDef; onode->nsDef = onode->nsDef->next)
                                if (strcmp((const char*) onode->nsDef->href, SRCML_CPP_NS_URI) == 0)
                                    break;

                            // if we found it, then
                            xmlNsPtr keepcppnext = 0;
                            if (onode->nsDef) {
                                keepcppnext = onode->nsDef->next;
                                onode->nsDef->next = 0;
                            }

                            // dump the namespace-modified tree
                            xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);

                            // restore original namespaces
                            if (onode->nsDef)
                                onode->nsDef->next = keepcppnext;
                            onode->nsDef = savens;

                        } else {

                            // dump the namespace-modified tree
                            xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);
                        }

                    } else {

                        // xpath of nodeset, that is not a unit, or the value of an attribute

                        // dump the namespace-modified tree
                        xmlNodeDumpOutput(buf, ctxt->myDoc, onode->type == XML_ATTRIBUTE_NODE ? onode->children : onode, 0, 0, 0);

                        // wrapped in a unit, so end the wrapping unit
                        xmlTextWriterEndElement(bufwriter);
                    }

                    // space between internal units
                    xmlTextWriterWriteString(bufwriter, BAD_CAST "\n\n");
                }

            }

            break;

            // numeric result
        case XPATH_NUMBER:
            if (!isoption(options, SRCML_OPTION_XPATH_TOTAL)) {
                std::ostringstream out;
                if ((int)result_nodes->floatval == result_nodes->floatval)
                    out << (int)result_nodes->floatval;
                else
                    out << result_nodes->floatval;

                xmlOutputBufferWriteString(buf, out.str().c_str());
                xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n"));
            }
            total += result_nodes->floatval;
            break;

            // boolean result
        case XPATH_BOOLEAN:
            if (!isoption(options, SRCML_OPTION_XPATH_TOTAL))
                xmlOutputBufferWriteString(buf, result_nodes->boolval ? "true\n" : "false\n");

            result_bool |= (result_nodes->boolval != 0);
            break;

            // string
        case XPATH_STRING:
            {
                char* p = (char*) result_nodes->stringval;
                char* pos = p;
                while (*p) {

                    if (p[0] == '&') {
                        if (p[1] == 'l' && p[2] == 't' && p[3] == ';') {

                            xmlOutputBufferWrite(buf, (int)(p - pos), pos);
                            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<"));
                            p += 4;
                            pos = p;

                        } else if (p[1] == 'g' && p[2] == 't' && p[3] == ';') {

                            xmlOutputBufferWrite(buf, (int)(p - pos), pos);
                            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">"));
                            p += 4;
                            pos = p;

                        } else if (p[1] == '#' && isdigit(p[2]) && isdigit(p[3])) {

                            xmlOutputBufferWrite(buf, (int)(p - pos), pos);

                            int end = 4;
                            if(p[4] == ';')
                                end = 5;
                            else if (isdigit(p[4]) && p[5] == ';')
                                end = 6;

                            if(end > 4) {
                                p[end - 1] = '\0';

                                int c = atoi(p + 2);
                                xmlOutputBufferWrite(buf, 1, (const char*) &c);

                                p[end - 1] = ';';
                            } else
                                xmlOutputBufferWrite(buf, 4, (const char*) p);

                            p += end;
                            pos = p;
                        }
                    } else {

                        ++p;
                    }
                }
                xmlOutputBufferWrite(buf, (int)(p - pos), pos);
            }

            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n"));

            break;

        default:
            fprintf(stderr, "Unhandled type %d\n", nodetype);
            break;
        };


    }

    /**
     * end_output
     *
     * Finish the archive and close buffer.
     */
    virtual void end_output() {

        // finalize results
        switch (nodetype) {
        case XPATH_NODESET:

            {

                std::string full_unit = "</";
                if(root->prefix) {
                    full_unit += (const char *)root->prefix;
                    full_unit += ":";
                }
                full_unit += "unit>\n";

                // root unit end tag
                if (!isoption(options, SRCML_OPTION_APPLY_ROOT))
                    xmlOutputBufferWriteString(buf, found || meta_tags.size() ? full_unit.c_str() : "/>\n");
                else if(found)
                    xmlOutputBufferWriteString(buf, full_unit.c_str());
                else
                    xmlOutputBufferWriteString(buf, "\n");

                break;

            }

        case XPATH_NUMBER:
            if (isoption(options, SRCML_OPTION_XPATH_TOTAL)) {
                std::ostringstream out;
                if ((int)total == total)
                    out << (int)total;
                else
                    out << total;

                xmlOutputBufferWriteString(buf, out.str().c_str());
                xmlOutputBufferWriteString(buf, "\n");
            }
            break;

            // boolean result
        case XPATH_BOOLEAN:
            if (isoption(options, SRCML_OPTION_XPATH_TOTAL))
                xmlOutputBufferWriteString(buf, result_bool ? "true\n" : "false\n");
            break;

        default:
            break;
        }

        if(context) xmlXPathFreeContext(context);
        context = 0;

        xmlTextWriterEndDocument(bufwriter);

        xmlFreeTextWriter(bufwriter);

    }

    /**
     * xml_output_buffer_write_element_ns
     * @param buf output buffer to write element
     * @param localname the name of the element tag
     * @param prefix the tag prefix
     * @param URI the namespace of tag
     * @param nb_namespaces number of namespaces definitions
     * @param namespaces the defined namespaces
     * @param nb_attributes the number of attributes on the tag
     * @param nb_defaulted the number of defaulted attributes
     * @param attributes list of attribute name value pairs (localname/prefix/URI/value/end)
     *
     * Write an element to output buffer.
     */
    /*
      void xml_output_buffer_write_element_ns(xmlOutputBufferPtr buf, const xmlChar* localname, const xmlChar* prefix,
      const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces,
      int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<"));
      if (prefix != NULL) {
      xmlOutputBufferWriteString(buf, (const char*) prefix);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
      }
      xmlOutputBufferWriteString(buf, (const char*) localname);

      // output the namespaces
      for (int i = 0; i < nb_namespaces; ++i) {

      // don't put cpp namespace on the root for a non-archive
      if (!is_archive && strcmp((const char*) namespaces[2 * i + 1], SRCML_CPP_NS_URI) == 0)
      continue;

      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" xmlns"));
      if (namespaces[i * 2]) {
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
      xmlOutputBufferWriteString(buf, (const char*) namespaces[i * 2]);
      }
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("=\""));
      xmlOutputBufferWriteString(buf, (const char*) namespaces[i * 2 + 1]);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
      }

      // output the attributes
      for (int i = 0; i < nb_attributes; ++i) {

      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" "));
      if (attributes[i * 5 + 1]) {
      xmlOutputBufferWriteString(buf, (const char*) attributes[i * 5 + 1]);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
      }
      xmlOutputBufferWriteString(buf, (const char*) attributes[i * 5]);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("=\""));

      xmlOutputBufferWrite(buf, (int)(attributes[i * 5 + 4] - attributes[i * 5 + 3]),
      (const char*) attributes[i * 5 + 3]);

      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
      }
      }
    */

private :

    OPTION_TYPE options;
    xmlXPathCompExprPtr compiled_xpath;
    const char * prefix;
    const char * uri;
    const char * element;
    const char * attr_prefix;
    const char * attr_uri;
    const char * attr_name;
    const char * attr_value;
    double total;
    bool result_bool;
    int nodetype;
    bool found;
    xmlOutputBufferPtr buf;
    xmlTextWriterPtr bufwriter;
    bool needroot;
    bool closetag;
    xmlXPathContextPtr context;
    xmlOutputBufferPtr output;
    int result_count;

    static const char * const simple_xpath_attribute_name;

};


const char * const xpath_query_units::simple_xpath_attribute_name = "location";

#endif
