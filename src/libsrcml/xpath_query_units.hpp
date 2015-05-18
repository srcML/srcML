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
#include <unit_dom.hpp>
#include <srcmlns.hpp>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

#ifdef _MSC_BUILD
#include <io.h>
#define snprintf _snprintf
#endif

#include <srcml_translator.hpp>

extern std::vector<transform> global_transformations;

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
    xpath_query_units(OPTION_TYPE options, xmlXPathCompExprPtr compiled_xpath, srcml_archive* out_archive, 
                      const char * prefix = 0, const char * uri = 0, const char * element = 0, const char * attr_prefix = 0, const char * attr_uri = 0, const char * attr_name = 0, const char * attr_value = 0)
        : unit_dom(options), options(options), compiled_xpath(compiled_xpath),
          prefix(prefix), uri(uri), element(element), attr_prefix(attr_prefix), attr_uri(attr_uri), attr_name(attr_name), attr_value(attr_value),
          total(0), context(0), result_count(0), output_archive(out_archive) {
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
     // TODO: start_output needs an error return value
    virtual void start_output() {

        buf = output_archive->translator->output_buffer();

      //  bufwriter = xmlNewTextWriter(buf);

        // TODO:  Detect error

#ifdef _MSC_BUILD
        buf->writecallback = (xmlOutputWriteCallback)_write;
#endif
    }
    
    virtual xmlXPathContextPtr set_context() {

        // compile all the inner transformations
        for (unsigned long i = 1; i < global_transformations.size(); ++i) {

            transform& thistransform = global_transformations[i];
            thistransform.compiled_xpath = xmlXPathCompile(BAD_CAST thistransform.arguments.str->c_str());
        }

        xmlXPathContextPtr context = xmlXPathNewContext(ctxt->myDoc);
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
                return 0; //SRCML_STATUS_ERROR;

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
    virtual bool apply() {

        if (!context)
            context = set_context();

        // evaluate the xpath
        xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
        if (result_nodes == 0) {
            fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
            return false;
        }

        std::vector<transform>::const_iterator tr = global_transformations.begin();

        applyxpath(++tr, global_transformations.end(), result_nodes);

        // finished with the result nodes
        //xmlXPathFreeObject(result_nodes);

        return true;
    }

    virtual bool applyxpath(std::vector<transform>::const_iterator tr, std::vector<transform>::const_iterator end, xmlXPathObjectPtr result_nodes) {

        if (tr == end || xmlXPathNodeSetGetLength(result_nodes->nodesetval) == 0) {
            apply(result_nodes);
            return true;
        }

        xmlNodePtr savectxt = ctxt->node;

        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i]; 

            ctxt->node = onode;

            xmlXPathObjectPtr result_nodes2 = xmlXPathCompiledEval(tr->compiled_xpath, context);


            applyxpath(++tr, end, result_nodes2);

            //xmlXPathFreeObject(result_nodes2);
        }

        ctxt->node = savectxt;

        return true;
    }

    virtual bool apply(xmlXPathObjectPtr result_nodes) {

        nodetype = result_nodes->type;

        switch (nodetype) {

        case XPATH_NODESET:

            // root element if first result
            // TODO: This should move to start_output(), but that will have to wait because it is shared.
       //     if (result_count == 0) {
//                outputRoot(xmlDocGetRootElement(ctxt->myDoc));
//            }

            if (!element && !attr_name)
                outputXPathResultsWrap(result_nodes);
            else if (element)
                outputXPathResultsElement(result_nodes);
            else if (attr_name)
                outputXPathResultsAttribute(result_nodes);
            break;

        case XPATH_NUMBER:
            output_archive->translator->set_text_only();
            outputXPathResultsNumber(result_nodes);
            break;

        case XPATH_BOOLEAN:
            output_archive->translator->set_text_only();
            outputXPathResultsBoolean(result_nodes);
            break;

        case XPATH_STRING:
            output_archive->translator->set_text_only();
            outputXPathResultsString(result_nodes);
            break;

        default :
            break;
        }

        return true;
    }

    // removes the namespace from the element
    xmlNsPtr* xmlRemoveNs(xmlNodePtr a_node, xmlNsPtr hrefptr) {

        if (!hrefptr)
            return 0;

        xmlNsPtr* skip = 0;
        for (xmlNsPtr* pns = &a_node->nsDef; pns; pns = &((*pns)->next)) {
            if ((*pns) == hrefptr) {
                skip = pns;
                *skip = (*skip)->next;
                break;
            }
        }

        return skip;
    }

    // process the resulting nodes
    virtual void outputXPathResultsWrap(xmlXPathObjectPtr result_nodes) {

        if (xmlXPathNodeSetGetLength(result_nodes->nodesetval) == 0)
            return;

        bool isunit = strcmp((const char*) result_nodes->nodesetval->nodeTab[0]->name, "unit") == 0;

        // using the internal unit node to serve as the wrapper
        xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);

        // remove src namespace
        //xmlNsPtr hrefptr = xmlSearchNsByHref(a_node->doc, a_node, BAD_CAST SRCML_SRC_NS_URI);
        //xmlNsPtr* skip = xmlRemoveNs(a_node, hrefptr);

        // hash doesn't make sense anymore because contents changed
        // so remove hash attribute
        //if (xmlAttrPtr curattr = xmlHasProp(a_node, BAD_CAST "hash")) {
        //    xmlRemoveProp(curattr);
        //}

        srcml_unit* punit = srcml_unit_create(output_archive);

        // copy all the attributes from the current node
        for (xmlAttrPtr pAttr = a_node->properties; pAttr; pAttr = pAttr->next) {
            if (std::string((const char*) pAttr->name) == std::string("filename"))
                srcml_unit_set_filename(punit, (const char*) pAttr->children->content);
            else if (std::string((const char*) pAttr->name) == std::string("url"))
                srcml_unit_set_url(punit, (const char*) pAttr->children->content);
            else if (std::string((const char*) pAttr->name) == std::string("version"))
                srcml_unit_set_version(punit, (const char*) pAttr->children->content);
            else if (std::string((const char*) pAttr->name) == std::string("timestamp"))
                srcml_unit_set_timestamp(punit, (const char*) pAttr->children->content);
            else if (std::string((const char*) pAttr->name) == std::string("language"))
                srcml_unit_set_language(punit, (const char*) pAttr->children->content);
            else if (std::string((const char*) pAttr->name) == std::string("hash"))
                /* ignore hash */ ;
            else if (std::string((const char*) pAttr->name) == std::string("revision"))
                /* ignore revision */ ;
            else {
                punit->attributes.push_back((const char*) pAttr->name);
                punit->attributes.push_back((const char*) pAttr->children->content);
            }
        }

        if (!isunit) {
            punit->attributes.push_back("item");
            punit->attributes.push_back("");
        }

        /*
        for (xmlAttrPtr pAttr = a_node->properties; pAttr; pAttr = pAttr->next)
            srcml_unit_
            xmlTextWriterWriteAttribute(bufwriter, pAttr->name, pAttr->children->content);
*/
        // output all the found nodes
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            if (isunit) {

                static xmlBufferPtr lbuffer = xmlBufferCreate();

                for (xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i]->children; onode; onode = onode->next) {

                    xmlNodeDump(lbuffer, ctxt->myDoc, onode, 0, 0);
                }

                output_archive->translator->add_unit_content(punit, (const char*) xmlBufferContent(lbuffer), xmlBufferLength(lbuffer));

                xmlBufferEmpty(lbuffer);

                ++result_count;
                continue;
            }

            // item attribute on wrapping node
            static char s[100];
            sprintf(s, "%d", i + 1);
            punit->attributes.back() = s;

            // location attribute on wrapping node
            if (false) {
                const char* s = "/src:unit";
                xmlSetProp(a_node, BAD_CAST "location", BAD_CAST s);
            }

            // index into results
            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            // unlink this result node and link to the master parent
            //xmlUnlinkNode(onode);
            //xmlAddChild(a_node, onode);

            // output the result
            outputResult(punit, onode);

            // put the result node back into place
            //xmlUnlinkNode(onode);
            // xmlAddChild(onode_parent, onode);
        }

        //if (skip)
            //*skip = hrefptr;

        //xmlFreeNode(a_node);

        srcml_unit_free(punit);
    }

    virtual void outputResult(srcml_unit* punit, xmlNodePtr a_node) {

        static xmlBufferPtr lbuffer = xmlBufferCreate();
        int size = xmlNodeDump(lbuffer, ctxt->myDoc, a_node, 0, 0);
        if (size == 0)
            return;

        output_archive->translator->add_unit_content(punit, (const char*) xmlBufferContent(lbuffer), xmlBufferLength(lbuffer));

        xmlBufferEmpty(lbuffer);

        ++result_count;
    }

    virtual void outputResult(xmlNodePtr a_node) {

        static xmlBufferPtr lbuffer = xmlBufferCreate();
        int size = xmlNodeDump(lbuffer, ctxt->myDoc, a_node, 0, 0);
        if (size == 0)
            return;

        output_archive->translator->add_unit_raw((const char*) xmlBufferContent(lbuffer), size);

        xmlBufferEmpty(lbuffer);

        ++result_count;
    }

    // process the resulting nodes
    virtual void outputXPathResultsElement(xmlXPathObjectPtr result_nodes) {

    	if (!result_nodes || !(result_nodes->type == 1) || !(result_nodes->nodesetval))
    		return;

        xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);

        // remove src namespace
        xmlNsPtr hrefptr = xmlSearchNsByHref(a_node->doc, a_node, BAD_CAST SRCML_SRC_NS_URI);
        xmlNsPtr* skip = xmlRemoveNs(a_node, hrefptr);

        // output all the found nodes
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            // set up namespace
            xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar*) uri, (const xmlChar*) prefix);

            xpath_arguments& thisarguments = global_transformations[0].arguments;

            // set up node to insert
            xmlNodePtr element_node = xmlNewNode(ns, (const xmlChar*) thisarguments.element->c_str());

            if(attr_name) append_attribute_to_node(element_node, thisarguments.attr_uri ? thisarguments.attr_prefix->c_str() : thisarguments.prefix->c_str(), thisarguments.attr_uri->c_str() ? thisarguments.attr_uri->c_str() : thisarguments.uri->c_str());

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

        if (skip)
            *skip = hrefptr;
    }

    // process the resulting nodes
    virtual void outputXPathResultsAttribute(xmlXPathObjectPtr result_nodes) {

        xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);

        // remove src namespace
        xmlNsPtr hrefptr = xmlSearchNsByHref(a_node->doc, a_node, BAD_CAST SRCML_SRC_NS_URI);
        xmlNsPtr* skip = xmlRemoveNs(a_node, hrefptr);

        // output all the found nodes
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            append_attribute_to_node(onode, attr_uri ? attr_prefix : prefix, attr_uri ? attr_uri : uri);
        }

        // output the result
        outputResult(a_node);

        if (skip)
            *skip = hrefptr;
    }
/*
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
*/
    virtual void outputXPathResultsNumber(xmlXPathObjectPtr result_nodes) {

        total += result_nodes->floatval;

        if (isoption(options, SRCML_OPTION_XPATH_TOTAL))
            return;

        char buffer[200];
        if ((int)result_nodes->floatval == result_nodes->floatval) {
            sprintf(buffer, "%d\n", (int)result_nodes->floatval);
        } else {
            sprintf(buffer, "%lf\n", result_nodes->floatval);
        }
        xmlOutputBufferWriteString(buf, buffer);
    }

    virtual void outputXPathResultsBoolean(xmlXPathObjectPtr result_nodes) {

        result_bool |= (result_nodes->boolval != 0);

        if (isoption(options, SRCML_OPTION_XPATH_TOTAL))
            return;

        xmlOutputBufferWriteString(buf, result_nodes->boolval ? "true\n" : "false\n");
    }

    virtual void outputXPathResultsString(xmlXPathObjectPtr result_nodes) {

        char* pcur = (char*) result_nodes->stringval;
        char* start = pcur;
        while (*pcur) {

            if (pcur[0] == '&') {
                if (pcur[1] == 'l' && pcur[2] == 't' && pcur[3] == ';') {

                    xmlOutputBufferWrite(buf, (int)(pcur - start), start);
                    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<"));
                    pcur += 4;
                    start = pcur;

                } else if (pcur[1] == 'g' && pcur[2] == 't' && pcur[3] == ';') {

                    xmlOutputBufferWrite(buf, (int)(pcur - start), start);
                    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">"));
                    pcur += 4;
                    start = pcur;

                } else if (pcur[1] == '#' && isdigit(pcur[2]) && isdigit(pcur[3])) {

                    xmlOutputBufferWrite(buf, (int)(pcur - start), start);

                    int end = 4;
                    if(pcur[4] == ';')
                        end = 5;
                    else if (isdigit(pcur[4]) && pcur[5] == ';')
                        end = 6;

                    if(end > 4) {
                        pcur[end - 1] = '\0';

                        int c = atoi(pcur + 2);
                        xmlOutputBufferWrite(buf, 1, (const char*) &c);

                        pcur[end - 1] = ';';
                    } else
                        xmlOutputBufferWrite(buf, 4, (const char*) pcur);

                    pcur += end;
                    start = pcur;
                } else {
                    ++pcur;
                }
            } else {
                ++pcur;
            }
        }
        xmlOutputBufferWrite(buf, (int)(pcur - start), start);

        // TODO: Is this a separator? Or just to get the code to end on a line?
        // If just to get the code to end on a line, then maybe not put in if the string already has it
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n"));
    }

    /**
     * end_output
     *
     * Finish the archive and close buffer.
     */
    virtual void end_output() {

        // finalize results
        switch (nodetype) {

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
    }

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
    xmlOutputBufferPtr buf;
    xmlXPathContextPtr context;
    int result_count;
    srcml_archive* output_archive;

    static const char * const simple_xpath_attribute_name;

};


const char * const xpath_query_units::simple_xpath_attribute_name = "location";

#endif
