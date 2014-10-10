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
#include <libxml/xmlstring.h>
#include <libxml/tree.h>
#include <libxml/xmlerror.h>

#include <srcmlns.hpp>

#include <iostream> // for debugging only
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
/* Static constants used by this file that don't need to be visible to other areas of the language. */
namespace {
    typedef const xmlChar* ConstXmlCharPtr;
    typedef const char* ConstCharPtr;

    /* Utility function for extracting a namespace from an XPath context. */
    inline xmlNsPtr locate_ns(xmlXPathParserContextPtr ctxt, char const* const nsUrl) {
        xmlNsPtr srcNs = 0;
        xmlNsPtr currentNs = xmlFirstElementChild(xmlNodePtr(ctxt->context->doc))->ns;
        while(currentNs) {
            if(xmlStrEqual(currentNs->href, BAD_CAST nsUrl) == 0){
                srcNs = currentNs;
                break;
            }
            currentNs = currentNs->next;
        }
        return srcNs;
    }

    xmlChar const* const return_tag = BAD_CAST "return";
    xmlChar const* const expr_stmt_tag = BAD_CAST "expr_stmt";
}

void xpath_exfun_has_return(xmlXPathParserContextPtr ctxt, int nargs) {

    CHECK_ARITY(0);
    std::cout << "Starting has_return" << std::endl;
    xmlNodePtr currentNode = ctxt->context->node;
    xmlNodePtr input = currentNode;
    // xmlNsPtr srcNs = 0;

    // srcNs = locate_ns(ctxt, SRCML_SRC_NS_URI);
    // // If I can't locate the src namespace
    // // then there are no statements.
    // if (!srcNs) {
    //     std::cout << "Can't locate src namespace" << std::endl;
    //     xmlXPathReturnFalse(ctxt); return;
    // }
    
    // Searching out required nodes.
    // int dictSize = xmlDictSize(currentNode->doc->dict);
    // if(!currentNode->doc->dict) {
    //     std::cout << "Invalid dictionary" << std::endl;
    // }
    // std::cout << "Current # of items in Dictionary: " << dictSize << std::endl;
    // if(dictSize == -1) {
    //     xmlErrorPtr err = xmlGetLastError();
    //     if(err) {
    //         std::cout << "Current error Information: \n"
    //             << "    Domain: " << err->domain << "\n"
    //             << "    Error Code: " << err->code << "\n"
    //             << "    Message: "  << err->message << "\n"
    //             << "    Files: " << err->file << std::endl;
    //     }else{
    //         std::cout << "The last error wasn't set correctly" << std::endl;
    //     }
    // }
    // ConstXmlCharPtr returnNodeName = 0;
    // returnNodeName = xmlDictExists(currentNode->doc->dict, ConstXmlCharPtr("return"), -1);
    // if(!returnNodeName) {
    //     std::cout << "Can't locate return statement" << std::endl;
    //     xmlXPathReturnFalse(ctxt); return;
    // }
    // std::cout << "Everythings OK moving on." << std::endl;

    xmlNodePtr temp = 0;
// START:
    if(!currentNode) {
        goto EXIT;
    }
VISIT:
    if(currentNode->type == XML_ELEMENT_NODE) {
        if(xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI)) {
            if(xmlStrEqual(return_tag, currentNode->name)) {
                std::cout << "Located return" << std::endl;
                xmlXPathReturnTrue(ctxt); return;


            }else if (xmlStrEqual(expr_stmt_tag, currentNode->name)) {
                goto STRAIF_SIBLINGS;
            }
        }
    }
// DESCENDING:
    temp = currentNode->children;
    if (temp) {
        currentNode = temp;
        goto VISIT;
    } 
STRAIF_SIBLINGS:
    temp = currentNode->next;
    if (temp && currentNode != input) {
        currentNode = temp;
        goto VISIT;
    }
// ASCENDING:
    if (currentNode->parent != input) {
        currentNode = currentNode->parent;
        goto STRAIF_SIBLINGS;
    }
EXIT:
    xmlXPathReturnFalse(ctxt); return;

}

void xpath_exfun_is_nested(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_class_template_partial_specialization(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_local(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_has_init(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_simple_asm(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_within_catch(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_within_finally(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_has_break(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_fixed(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_resource(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_unsafe(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_mutually_exclusive(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_returns(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_throw_stmts(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_attributes(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_parameters(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_arguments(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_templates(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_friends(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_enum_decls(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_initializers(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_owning_function(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_compute_prev_decl_type(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_indexing_expr(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_true_expr(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_false_expr(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_capture_variable_names(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_captures(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_caught_exception_parameter(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_remaining_catches(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_condition(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_function_call_name(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_case_labels(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_statements(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_declarations(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_functions(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_member_functions(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_members(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_data_members(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_public_members(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_private_members(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_protected_members(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_internal_members(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_protected_internal_members(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}
