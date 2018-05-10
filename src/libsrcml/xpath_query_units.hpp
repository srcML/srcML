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

#include <srcml_translator.hpp>

extern std::vector<transform> global_transformations;

template<typename T>
class save_restore {
public:
    save_restore(T& item) : saved_item(&item), saved_value(item) {}
    save_restore(bool save, T& item) {
        saved_item = save ? &item : 0;
        if (save)
            saved_value = item;
    }
    ~save_restore() { if (saved_item) *saved_item = saved_value; }
    operator T() { return saved_value; }
private:
    T* saved_item;
    T saved_value;
};

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
    xpath_query_units(OPTION_TYPE options, xmlXPathCompExprPtr /* compiled_xpath */, srcml_archive* out_archive, 
                      const char* prefix = 0, const char* uri = 0, const char* element = 0, const char* attr_prefix = 0, const char* attr_uri = 0, const char* attr_name = 0, const char* attr_value = 0)
        : unit_dom(options), options(options), /* compiled_xpath(compiled_xpath) ,*/
          prefix(prefix), uri(uri), element(element), attr_prefix(attr_prefix), attr_uri(attr_uri), attr_name(attr_name), attr_value(attr_value),
          total(0), context(0), /* result_count(0),*/ output_archive(out_archive) {
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
    void append_attribute_to_node(xmlNodePtr node, const char* attr_prefix, const char* attr_uri) {

        // grab current value
        const char* value = (char*) xmlGetNsProp(node, BAD_CAST attr_name, BAD_CAST attr_uri);
        const char* newvalue = attr_value;

        // previous property
        std::string curvalue;
        if (value && strcmp(value, newvalue)) {

            curvalue = value;
            curvalue += ' ';
            curvalue += attr_value;
            newvalue = curvalue.c_str();
        }

        static xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar *) attr_uri, (const xmlChar *) attr_prefix);
        xmlSetNsProp(node, ns, (const xmlChar *) attr_name, (const xmlChar *) newvalue);
    }

    /**
     * start_output
     *
     * Create the output buffer and setup XPath.
     */
     // TODO: start_output needs an error return value
    virtual void start_output() {

        // @todo detect error
        buf = output_archive->translator->output_buffer();
#if 0
#ifdef _MSC_BUILD
        buf->writecallback = (xmlOutputWriteCallback)_write;
#endif
#endif
    }
    
    virtual xmlXPathContextPtr set_context() {

        // compile all the inner transformations
        for (auto& thistransform : global_transformations)
            thistransform.compiled_xpath = xmlXPathCompile(BAD_CAST thistransform.arguments.str->c_str());

        xmlXPathContextPtr context = xmlXPathNewContext(doc);
        // TODO:  Detect error

        xpathsrcMLRegister(context);
        // TODO:  Detect error

        // register standard prefixes for standard namespaces
        for (const auto& ns : default_namespaces) {

            const char* uri = ns.uri.c_str();
            const char* prefix = ns.prefix.c_str();
            if (ns.uri == SRCML_SRC_NS_URI)
                prefix = "src";

            if (xmlXPathRegisterNs(context, BAD_CAST prefix, BAD_CAST uri) == -1) {
                fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "libsrcml", prefix, uri);
                return 0; //SRCML_STATUS_ERROR;
            }
        }

        // register namespaces from input archive, which have been setup on the output archive
        for (unsigned int i = 1; i < srcml_archive_get_namespace_size(output_archive); ++i) {

            const char* uri = srcml_archive_get_namespace_uri(output_archive, i);
            const char* prefix = srcml_archive_get_namespace_prefix(output_archive, i);

            if (xmlXPathRegisterNs(context, BAD_CAST prefix, BAD_CAST uri) == -1) {
                fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "libsrcml", prefix, uri);
                return 0; //SRCML_STATUS_ERROR;
            }
        }

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
    virtual bool apply() {

        if (!context)
            context = set_context();
        context->doc = doc;

        // handle new elements and individual results the previous way
        if (element || !attr_name) {

            auto tr = global_transformations.begin();
            // evaluate the xpath
            xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(tr->compiled_xpath, context);
            if (result_nodes == 0) {
                fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
                return false;
            }
            applyxpath(++tr, global_transformations.end(), result_nodes);

            return true;
        }

        // apply all XPath transformations on the same base code, and save all the results
        for (auto& tr : global_transformations) {

            // evaluate the xpath
            tr.result_nodes = xmlXPathCompiledEval(tr.compiled_xpath, context);
            if (tr.result_nodes == 0) {
                fprintf(stderr, "%s: Error in executing xpath\n", "libsrcml");
                return false;
            }
        }

        // process all the xpath transform results for everything except the last
        for (auto& tr : global_transformations) {

            if (&tr == &global_transformations.back())
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
        attr_uri = global_transformations.back().arguments.attr_uri->c_str();
        attr_prefix = global_transformations.back().arguments.attr_prefix->c_str();
        attr_name = global_transformations.back().arguments.attr_name->c_str();
        attr_value = global_transformations.back().arguments.attr_value->c_str();
        apply(global_transformations.back().result_nodes);

//        applyxpath(++tr, global_transformations.end(), result_nodes);

        // finished with the result nodes
        //xmlXPathFreeObject(result_nodes);

        return true;
    }

    virtual bool applyxpath(std::vector<transform>::const_iterator tr, std::vector<transform>::const_iterator end, xmlXPathObjectPtr result_nodes) {

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

    virtual bool apply(xmlXPathObjectPtr result_nodes) {

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

    // process the resulting nodes
    virtual void outputXPathResultsWrap(xmlXPathObjectPtr result_nodes) {

        if (xmlXPathNodeSetGetLength(result_nodes->nodesetval) == 0)
            return;

        // using the internal unit node to serve as the wrapper
        xmlNodePtr a_node = xmlDocGetRootElement(doc);

        // special case for a single result for a unit, and the result is the entire unit
        if ((result_nodes->nodesetval->nodeNr == 1) && (strcmp((const char*) result_nodes->nodesetval->nodeTab[0]->name, "unit") == 0)) {
            outputResult(a_node);
            return;
        }

        // save the children
        save_restore<xmlNodePtr> a_node_children(a_node->children);
        a_node->children = 0;

        // create the item property
        if (xmlNewProp(a_node, BAD_CAST "item", BAD_CAST "0") == 0)
            return;

        // save the hash attribute
        xmlAttrPtr hashprop = xmlHasProp(a_node, BAD_CAST "hash");
        if (hashprop)
            xmlRemoveProp(hashprop);

        // output all the found nodes
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            // item attribute on wrapping node
            if (xmlSetProp(a_node, BAD_CAST "item", BAD_CAST std::to_string(i + 1).c_str()) == 0)
                return;

            // one of the multiple query results is an entire unit, then output directly
            if (strcmp((const char*) result_nodes->nodesetval->nodeTab[i]->name, "unit") == 0) {
                a_node->children = a_node_children;
                outputResult(a_node);
                a_node->children = 0;
                continue;
            }

            // location attribute on wrapping node
            if (false) {
                const char* s = "/src:unit";
                xmlSetProp(a_node, BAD_CAST "location", BAD_CAST s);
            }

            // index into results
            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];
            xmlNodePtr save_onode = 0;
            if (onode->type == 2) {
                std::string attr = (const char*) onode->name;
                attr += "=";
                attr += "\"";
                attr += (const char*) onode->children->content;
                attr += "\"";

                save_onode = onode;

                onode = xmlNewText((const xmlChar*) attr.c_str());
            }

            // unlink this result node and link to the master parent
            xmlNodePtr onode_parent = onode->parent;
            xmlNodePtr onode_next = onode->next;
            xmlNodePtr onode_prev = onode->prev;
            onode->parent = a_node;
            onode->next = 0;
            onode->prev = 0;
            a_node->children = onode;

            // output the result
            outputResult(a_node);

            // unlink from the master parent back to where it came from
            onode->parent = onode_parent;
            onode->next = onode_next;
            onode->prev = onode_prev;
            a_node->children = 0;

            if (save_onode) {
                xmlFreeNode(onode);
                onode = save_onode;
                save_onode = 0;
            }
        }

        if (hashprop) {
            ;
        }
        //xmlFreeProp(itemprop);
    }

    virtual void outputResult(xmlNodePtr a_node) {

        bool is_archive = (output_archive->options & SRCML_OPTION_ARCHIVE) > 0;

        // remove src namespace
        xmlNsPtr hrefptr = xmlSearchNsByHref(a_node->doc, a_node, BAD_CAST SRCML_CPP_NS_URI);

        save_restore<xmlNsPtr> save(a_node->nsDef); //skip = is_archive ? xmlRemoveNs(a_node, hrefptr) : 0;
        save_restore<xmlNsPtr> nextsave(hrefptr, hrefptr->next);
        if (is_archive) {
            if (!hrefptr)
                a_node->nsDef = 0;
            else {
                a_node->nsDef = hrefptr;
                hrefptr->next = 0;
            }
        }

        output_archive->translator->add_unit(a_node, doc);
    }

    // process the resulting nodes
    virtual void outputXPathResultsElement(xmlXPathObjectPtr result_nodes) {

    	if (!result_nodes || !(result_nodes->type == 1) || !(result_nodes->nodesetval))
    		return;

        xmlNodePtr a_node = xmlDocGetRootElement(doc);

        // set up namespace
        static xmlNsPtr ns = xmlNewNs(NULL, (const xmlChar*) uri, (const xmlChar*) prefix);

        // output all the found nodes
        for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            xpath_arguments& thisarguments = global_transformations[0].arguments;

            // set up node to insert
            xmlNodePtr element_node = xmlNewNode(ns, (const xmlChar*) thisarguments.element->c_str());

            if (attr_name)
                append_attribute_to_node(element_node, thisarguments.attr_uri ? thisarguments.attr_prefix->c_str() : thisarguments.prefix->c_str(), thisarguments.attr_uri->c_str() ? thisarguments.attr_uri->c_str() : thisarguments.uri->c_str());

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
    }

    // process the resulting nodes
    virtual void outputXPathResultsAttribute(xmlXPathObjectPtr result_nodes) {

        // use the root element to wrap the result ?
        xmlNodePtr a_node = xmlDocGetRootElement(doc);

        // append the attribute to the nodes
        for (int i = 0; result_nodes->nodesetval && i < result_nodes->nodesetval->nodeNr; ++i) {

            xmlNodePtr onode = result_nodes->nodesetval->nodeTab[i];

            append_attribute_to_node(onode, attr_uri ? attr_prefix : prefix, attr_uri ? attr_uri : uri);
        }

        // output the result
        outputResult(a_node);
    }

    virtual void outputXPathResultsNumber(xmlXPathObjectPtr result_nodes) {

        total += result_nodes->floatval;

        if (isoption(options, SRCML_OPTION_XPATH_TOTAL))
            return;

        xmlOutputBufferWriteString(buf, (int)result_nodes->floatval == result_nodes->floatval ?
            std::to_string((int)result_nodes->floatval).c_str() : std::to_string(result_nodes->floatval).c_str());
        xmlOutputBufferWriteString(buf, "\n");
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
                xmlOutputBufferWriteString(buf, (int) total == total ? std::to_string((int)total).c_str() :
                        std::to_string(total).c_str());
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

        if (context)
            xmlXPathFreeContext(context);
        context = 0;
    }

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
    srcml_archive* output_archive;

    static const char* const simple_xpath_attribute_name;

};


const char* const xpath_query_units::simple_xpath_attribute_name = "location";

#endif
