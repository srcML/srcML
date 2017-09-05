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

#ifndef _MSC_BUILD
/** Macro to define the correct name of strdup on unix/windows */
#define STRDUP strdup
#else
#define STRDUP _strdup
#endif

#include <srcsax.hpp>

#include <libxml/parser.h>

#include <string.h>
#include <string>
#include <list>

/** Check if a copy was actually allocated */
#define CHECK_COPY(ORIGINAL, COPY) if(ORIGINAL && !COPY) { fprintf(stderr, "ERROR allocating memory"); srcsax_stop_parser(context); return; }

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
    srcml_element(srcsax_context * context, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                 int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                 const xmlChar** attributes) {

        // save all the info in case this is not a srcML archive
        this->localname = localname ? (xmlChar*) STRDUP((const char*) localname) : 0;
        CHECK_COPY(localname, this->localname);

        this->prefix = prefix ? (xmlChar*) STRDUP((const char*) prefix) : 0;
        CHECK_COPY(prefix, this->prefix);

        this->URI = URI ? (xmlChar*) STRDUP((const char*) URI) : 0;
        CHECK_COPY(URI, this->URI);

        this->nb_namespaces = nb_namespaces;
        int ns_length = nb_namespaces * 2;
        this->namespaces = (const xmlChar**) malloc(ns_length * sizeof(namespaces[0]));
        CHECK_COPY(namespaces, this->namespaces);
        memset(this->namespaces, 0, ns_length * sizeof(namespaces[0]));

        for (int i = 0; i < ns_length; ++i) {

            if(prefix && namespaces[i] && strcmp((const char *)prefix, (const char *)namespaces[i]) == 0)
                this->namespaces[i] = this->prefix;
            else if(URI && namespaces[i] && strcmp((const char *)URI, (const char *)namespaces[i]) == 0)
                this->namespaces[i] = this->URI;
            else {

                this->namespaces[i] = namespaces[i] ? (xmlChar*) STRDUP((const char*) namespaces[i]) : 0;
                CHECK_COPY(namespaces[i], this->namespaces[i]);
            }

        }

        this->nb_attributes = nb_attributes;
        this->nb_defaulted = nb_defaulted;

        int nb_length = nb_attributes * 5;
        this->attributes = (const xmlChar**) malloc(nb_length * sizeof(attributes[0]));
        CHECK_COPY(attributes, this->attributes);

        memset(this->attributes, 0, nb_length * sizeof(attributes[0]));

        for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
            this->attributes[index] = attributes[index] ? (xmlChar*) STRDUP((const char*) attributes[index]) : 0;
            CHECK_COPY(attributes[index], this->attributes[index]);
            this->attributes[index + 1] = attributes[index + 1] ? (xmlChar*) STRDUP((const char*) attributes[index + 1]) : 0;
            CHECK_COPY(attributes[index + 1], this->attributes[index + 1]);
            this->attributes[index + 2] = attributes[index + 2] ? (xmlChar*) STRDUP((const char*) attributes[index + 2]) : 0;
            CHECK_COPY(attributes[index + 2], this->attributes[index + 2]);

            int vallength = (int)(attributes[index + 4] - attributes[index + 3]);
            this->attributes[index + 3] = (const xmlChar*) malloc(vallength + 1);
            CHECK_COPY(attributes[index + 3], this->attributes[index + 3]);

            strncpy((char *) this->attributes[index + 3], (const char*) attributes[index + 3], vallength);
            ((char *)this->attributes[index + 3])[vallength] = 0;
            this->attributes[index + 4] = this->attributes[index + 3] + vallength;

        }

    }

    /** Copy constructor */
    srcml_element(const srcml_element & element)
        : srcml_element(element.context, element.localname, element.prefix, element.URI,
          element.nb_namespaces, element.namespaces,
          element.nb_attributes, element.nb_defaulted,
          element.attributes) {
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

        if(namespaces) {

            for(int i = 0; i < nb_namespaces * 2; ++i)
                if(namespaces[i] && namespaces[i] != prefix && namespaces[i] != URI)
                    free((void *)namespaces[i]);

            free((void *)namespaces);
        }

        if(localname) free((void *)localname);
        if(prefix) free((void *)prefix);
        if(URI) free((void *)URI);

        if(attributes) {

            for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
                if(attributes[index])
                    free((void *)attributes[index]);
                if(attributes[index + 1])
                    free((void *)attributes[index + 1]);
                if(attributes[index + 2])
                    free((void *)attributes[index + 2]);
                free((void *)attributes[index + 3]);
            }

            free((void *)attributes);

        }

    }

    xmlChar* stralloc(const char* s) {
    	salloc.push_back(s);
    	return (xmlChar*) salloc.back().c_str();
    }

    /** parser context */
    srcsax_context* context = nullptr;

    /** local name of an element*/
    const xmlChar* localname = nullptr;

    /** prefix of an element*/
    const xmlChar* prefix = nullptr;

    /** URI of an element*/
    const xmlChar* URI = nullptr;

    /** number of namespaces on an element*/
    int nb_namespaces = 0;

    /** namespaces on an element*/
    const xmlChar** namespaces = nullptr;

    /** number of attributes on an element*/
    int nb_attributes = 0;

    /** number of defaulted on an element*/
    int nb_defaulted = 0;

    /** attributes of an element*/
    const xmlChar** attributes = 0;

    std::list<std::string> salloc;
};

#endif
