/**
 * @file srcmlexfun_impl.cpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
#include <srcexfun_impl.hpp>
#include <libxml/xpathInternals.h>
#include <libxml/dict.h>

/*
VectorNodeSet forALLDescendentsAndSelfOptimized(xmlNodePtr input, char const* ns, char const* nodeName) {
    xmlNodePtr currentNode = input;
    xmlNodePtr temp;
    VectorNodeSet ret;
    xmlDocPtr docPtr = input->doc;
    xmlChar const* expectedNodeNamePtr = nullptr;
    xmlNsPtr expectedNS = nullptr;

    xmlNsPtr currentNs = docPtr->oldNs;
    while(currentNs) {
        if(xmlStrEqual(currentNs->href, BAD_CAST ns) == 0){
            expectedNS = currentNs;
            break;
        }
        currentNs = currentNs->next;
    }
    typedef const xmlChar* ConstCharPtr;
    expectedNodeNamePtr = xmlDictExists(docPtr->dict, ConstCharPtr(nodeName), -1);
    if(expectedNodeNamePtr == nullptr) {
        throw std::runtime_error("Unable to locate tag name within dictionary.");
    }

    START:
        if(currentNode == nullptr) {
            goto EXIT;
        }
    VISIT:
        if(currentNode->type == XML_ELEMENT_NODE) {
            if(currentNode->ns == expectedNS) {
                if(currentNode->name == expectedNodeNamePtr) {
                    ret.push_back(currentNode);
                }
            }
        }
    DESCENDING:
        temp = currentNode->children;
        if (temp != nullptr) {
            currentNode = temp;
            goto VISIT;
        } 
    STRAIF_SIBLINGS:
        temp = currentNode->next;
        if (temp != nullptr && currentNode != input) {
            currentNode = temp;
            goto VISIT;
        }
    ASCENDING:
        if (currentNode->parent != input) {
            currentNode = currentNode->parent;
            goto STRAIF_SIBLINGS;
        }
    EXIT:
        return ret;
}
*/
void xpath_exfun_has_return(xmlXPathParserContextPtr ctxt, int nargs) {
    
}


void xpath_exfun_is_nested(xmlXPathParserContextPtr ctxt, int nargs) {

}

void xpath_exfun_is_partial_template_specialization(xmlXPathParserContextPtr ctxt, int nargs) {

}

void xpath_exfun_is_local(xmlXPathParserContextPtr ctxt, int nargs) {
    
}