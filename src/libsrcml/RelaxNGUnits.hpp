/*
  RelaxNGUnits.cpp

  @copyright Copyright (C) 2008-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef INCLUDED_RelaxNGUNITS_HPP
#define INCLUDED_RelaxNGUNITS_HPP

#include <libxml/parser.h>
#include <libxml/relaxng.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s
#define LITERALPLUSSIZE(s) s, sizeof(s) - 1

#include <srcexfun.hpp>

#include <UnitDOM.hpp>

class RelaxNGUnits : public UnitDOM {
public :

    RelaxNGUnits(OPTION_TYPE options, xmlRelaxNGValidCtxtPtr rngctx, int fd = 0)
        : UnitDOM(options), options(options), rngctx(rngctx), fd(fd), found(false), root_prefix(0) {
    }

    virtual ~RelaxNGUnits() {}

    virtual void startOutput() {

        buf = xmlOutputBufferCreateFd(fd, NULL);
        // TODO:  Detect error

#ifdef LIBSRCML_COMPILER_IS_MSVC
        buf->writecallback = (xmlOutputWriteCallback)write;
#endif

    }

    virtual bool apply() {

        // validate
        int n = xmlRelaxNGValidateDoc(rngctx, ctxt->myDoc);

        // output if it validates
        if (n == 0) {

            // output the xml declaration, if needed
            if (!found && isoption(options, OPTION_XMLDECL))
                xmlOutputBufferWriteXMLDecl(ctxt, buf);

            // output the root unit start tag
            // this is only if in per-unit mode and this is the first result found
            // have to do so here because it may be empty
            if (is_archive && !found && !isoption(options, OPTION_APPLY_ROOT)) {

                // output a root element, just like the one read in
                // note that this has to be ended somewhere
                xmlOutputBufferWriteElementNs(buf, root->localname, root->prefix, root->URI,
                                              root->nb_namespaces, root->namespaces,
                                              is_archive ? root->nb_attributes : 0, root->nb_defaulted, root->attributes);

                xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">"));

                for(std::vector<std::string>::size_type i = 0; i < meta_tags->size(); ++i) {
                    xmlOutputBufferWriteElementNs(buf, meta_tags->at(i).localname, meta_tags->at(i).prefix, meta_tags->at(i).URI,
                                                  meta_tags->at(i).nb_namespaces, meta_tags->at(i).namespaces,
                                                  meta_tags->at(i).nb_attributes, meta_tags->at(i).nb_defaulted, meta_tags->at(i).attributes);
		    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("/>"));
                }

                xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));
                root_prefix = root->prefix;
            }
            found = true;

            // get the root node of current unit
            xmlNodePtr node = xmlDocGetRootElement(ctxt->myDoc);
            // output start unit tag
            if(node)
                xmlOutputBufferWriteElementNodeNs(buf, *node, is_archive);

            // output any children
            if(node && node->children) {

                for(xmlNodePtr child = node->children; child; child = child->next)
                    xmlNodeDumpOutput(buf, ctxt->myDoc, child, 0, 0, 0);

                std::string end_unit = "</";
                const char * unit_prefix = (const char *)root_prefix;
                if(!unit_prefix) unit_prefix = node->nsDef && node->nsDef->prefix ? (const char *)node->nsDef->prefix : 0;

                // output full end tag if children
                if(unit_prefix) {
                    end_unit += unit_prefix;
                    end_unit += ":";
                }
                end_unit += "unit>";

                xmlOutputBufferWrite(buf, (int)end_unit.size(), end_unit.c_str());

            } else
                // output end as empty element if no children
                xmlOutputBufferWrite(buf, 2, "/>");

            if(is_archive) xmlOutputBufferWrite(buf, 2, "\n\n");
            else  xmlOutputBufferWrite(buf, 1, "\n");

        }

        return true;
    }

    virtual void endOutput() {

        // root unit end tag
        if (is_archive && !isoption(options, OPTION_APPLY_ROOT)) {
            std::string end_unit = "</";
            if(root_prefix) {
                end_unit += (const char *)root_prefix;
                end_unit += ":";

            }
            end_unit += "unit>\n";

            xmlOutputBufferWriteString(buf, found || meta_tags->size() ? end_unit.c_str() : "/>\n");
        }

        // all done with the buffer
        xmlOutputBufferClose(buf);
    }

    static void xmlOutputBufferWriteXMLDecl(xmlParserCtxtPtr ctxt, xmlOutputBufferPtr buf) {

        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<?xml version=\""));
        xmlOutputBufferWriteString(buf, (const char*) ctxt->version);
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\" encoding=\""));
        xmlOutputBufferWriteString(buf, (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding));
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\" standalone=\""));
        xmlOutputBufferWriteString(buf, ctxt->standalone ? "yes" : "no");
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\"?>\n"));
    }

    static void xmlOutputBufferWriteElementNodeNs(xmlOutputBufferPtr buf, xmlNode & node, bool isarchive) {

        // record if this is an empty element since it will be erased by the attribute copying
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<"));
        // start the element
        {

            std::string s = "";
            if(node.ns && node.ns->prefix) {

                s += ((char*) node.ns->prefix);
                s += ":";
            }
            s += (char*) node.name;

            xmlOutputBufferWrite(buf, (int)s.size(), s.c_str());

        }

        {

            xmlNsPtr xmlns = node.nsDef;
            while(xmlns) {

                std::string ns = xmlns->href ? (const char *)xmlns->href : "";
                if(isarchive && ns == SRCML_SRC_NS_URI) {

                    xmlns = xmlns->next;
                    continue;

                }

                std::string prefix = " xmlns";
                if(xmlns->prefix) {

                    prefix += ":";
                    prefix += (const char *)xmlns->prefix;

                }

                std::string xmlns_string = prefix + "=\"" + ns + "\"";

                if(xmlns->href)
                    xmlOutputBufferWrite(buf, (int)xmlns_string.size(), xmlns_string.c_str());

                xmlns = xmlns->next;
            }
        }

        // copy all the attributes
        {
            xmlAttrPtr attribute = node.properties;
            while (attribute) {

                std::string s = " ";
                if(attribute->ns && attribute->ns->prefix) {
                    s += (const char *)attribute->ns->prefix;
                    s += ":";

                }
                s += (const char *)attribute->name;

                std::string attribute_string = s + "=\"";
                attribute_string += (const char *)attribute->children->content;
                attribute_string += "\"";

                xmlOutputBufferWrite(buf, (int)attribute_string.size(), attribute_string.c_str());
                attribute = attribute->next;
            }
        }


        // start the element
        // end now if this is an empty element
        if (node.children)
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">"));

    }

    static void xmlOutputBufferWriteElementNs(xmlOutputBufferPtr buf, const xmlChar* localname, const xmlChar* prefix,
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

    static void xmlOutputBufferWriteElementNs(std::string& s, const xmlChar* localname, const xmlChar* prefix,
                                              const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces,
                                              int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

        s.append(LITERALPLUSSIZE("<"));
        if (prefix != NULL) {
            s.append((const char*) prefix);
            s.append(LITERALPLUSSIZE(":"));
        }
        s.append((const char*) localname);

        // output the namespaces
        for (int i = 0; i < nb_namespaces; ++i) {

            s.append(LITERALPLUSSIZE(" xmlns"));
            if (namespaces[i * 2]) {
                s.append(LITERALPLUSSIZE(":"));
                s.append((const char*) namespaces[i * 2]);
            }
            s.append(LITERALPLUSSIZE("=\""));
            s.append((const char*) namespaces[i * 2 + 1]);
            s.append(LITERALPLUSSIZE("\""));
        }

        // output the attributes
        for (int i = 0; i < nb_attributes; ++i) {

            s.append(LITERALPLUSSIZE(" "));
            if (attributes[i * 5 + 1]) {
                s.append((const char*) attributes[i * 5 + 1]);
                s.append(LITERALPLUSSIZE(":"));
            }
            s.append((const char*) attributes[i * 5]);
            s.append(LITERALPLUSSIZE("=\""));

            s.append((const char*) attributes[i * 5 + 3], attributes[i * 5 + 4] - attributes[i * 5 + 3] + 1);

            s.append(LITERALPLUSSIZE("\""));
        }
    }

private :

    OPTION_TYPE options;
    xmlOutputBufferPtr buf;
    xmlRelaxNGValidCtxtPtr rngctx;
    int fd;
    bool found;
    const xmlChar * root_prefix;

};

#endif
