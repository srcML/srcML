/**
 * @file srcml_element.hpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
 *
 * srcSAX is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * srcSAX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef INCLUDED_SRCML_ELEMENT_HPP
#define INCLUDED_SRCML_ELEMENT_HPP

class srcsax_context;

#include <libxml/parser.h>

#include <vector>

/**
 * srcml_element
 *
 * Data structure to hold an element
 * mainly root element
 */
struct srcml_element {

    /** Default constructor to Zero out srcml_element */
    srcml_element() {}

    /** Constructor to initialize using start element items */
    srcml_element(srcsax_context* context, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                 int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) : context(context) {

        // save all the info in case this is not a srcML archive
       	this->localname = xmlStrdup(localname);
        this->prefix = xmlStrdup(prefix);
        this->URI = xmlStrdup(URI);

        this->nb_namespaces = nb_namespaces;
        int ns_length = nb_namespaces * 2;
        this->namespaces.reserve(ns_length);

        for (int i = 0; i < ns_length; ++i) {
            this->namespaces[i] = xmlStrdup(namespaces[i]);
        }

        this->nb_attributes = nb_attributes;
        this->nb_defaulted = nb_defaulted;

        int nb_length = nb_attributes * 5;
        this->attributes.reserve(nb_length);

        for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
            this->attributes[index] = xmlStrdup(attributes[index]);
            this->attributes[index + 1] = xmlStrdup(attributes[index + 1]);
            this->attributes[index + 2] = xmlStrdup(attributes[index + 2]);

            int vallength = (int)(attributes[index + 4] - attributes[index + 3]);
            this->attributes[index + 3] = (const xmlChar*) xmlStrndup(attributes[index + 3], vallength);
            this->attributes[index + 4] = this->attributes[index + 3] + vallength;
        }
    }

    /** Copy constructor */
    srcml_element(const srcml_element& element)
        : srcml_element(element.context, element.localname, element.prefix, element.URI,
          element.nb_namespaces, (const xmlChar**) element.namespaces.data(),
          element.nb_attributes, element.nb_defaulted,
          (const xmlChar**) element.attributes.data()) {
    }

    /** Overloaded assignment operator */
    srcml_element & operator=(srcml_element element) {

        swap(element);
        return *this;

    }

    /** swap operator */
    void swap(srcml_element & element) {

        std::swap(localname, element.localname);
        std::swap(prefix, element.prefix);
        std::swap(URI, element.URI);
        std::swap(nb_namespaces, element.nb_namespaces);
        std::swap(namespaces, element.namespaces);
        std::swap(nb_attributes, element.nb_attributes);
        std::swap(nb_defaulted, element.nb_defaulted);
        std::swap(attributes, element.attributes);

    }

    /** destructor */
    ~srcml_element() {

        if (localname)
           xmlFree(localname);

        if (prefix)
            xmlFree(prefix);

        if (URI)
            xmlFree(URI);

        for (auto p : namespaces) {
            xmlFree((xmlChar*) p);
        }

        for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
            xmlFree((xmlChar*) attributes[index]);
            xmlFree((xmlChar*) attributes[index + 1]);
            xmlFree((xmlChar*) attributes[index + 2]);
            xmlFree((xmlChar*) attributes[index + 3]);
        }
    }

    /** parser context */
    srcsax_context* context = nullptr;

    /** local name of an element*/
    xmlChar* localname = nullptr;

    /** prefix of an element*/
    xmlChar* prefix = nullptr;

    /** URI of an element*/
    xmlChar* URI = nullptr;

    /** number of namespaces on an element*/
    int nb_namespaces = 0;

    /** namespaces on an element*/
    std::vector<const xmlChar*> namespaces;

    /** number of attributes on an element*/
    int nb_attributes = 0;

    /** number of defaulted on an element*/
    int nb_defaulted = 0;

    /** attributes of an element*/
    std::vector<const xmlChar*> attributes;
};

#endif
