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
#include <boost/unordered_set.hpp>
#include <algorithm>
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

    xmlChar const* const asm_tag = BAD_CAST "asm";
    xmlChar const* const return_tag = BAD_CAST "return";
    xmlChar const* const typedef_tag = BAD_CAST "typedef";    
    xmlChar const* const using_tag = BAD_CAST "using";

    xmlChar const* const property_tag = BAD_CAST "property";
    xmlChar const* const event_tag = BAD_CAST "event";
    xmlChar const* const enum_tag = BAD_CAST "enum";
    xmlChar const* const expr_stmt_tag = BAD_CAST "expr_stmt";
    xmlChar const* const decl_stmt_tag = BAD_CAST "decl_stmt";


    xmlChar const* const constructor_tag = BAD_CAST "constructor";
    xmlChar const* const constructor_decl_tag = BAD_CAST "constructor_decl";
    xmlChar const* const destructor_tag = BAD_CAST "destructor";
    xmlChar const* const destructor_decl_tag = BAD_CAST "destructor_decl";

    // Compound statements
    xmlChar const* const block_tag = BAD_CAST "block";
    xmlChar const* const while_tag = BAD_CAST "while";
    xmlChar const* const if_tag = BAD_CAST "if";
    xmlChar const* const then_tag = BAD_CAST "then";
    xmlChar const* const else_tag = BAD_CAST "else";
    xmlChar const* const elseif_tag = BAD_CAST "elseif";
    xmlChar const* const try_tag = BAD_CAST "try";
    xmlChar const* const catch_tag = BAD_CAST "catch";
    xmlChar const* const finally_tag = BAD_CAST "finally";
    xmlChar const* const do_tag = BAD_CAST "do";
    xmlChar const* const for_tag = BAD_CAST "for";
    xmlChar const* const foreach_tag = BAD_CAST "foreach";
    xmlChar const* const switch_tag = BAD_CAST "switch";
    xmlChar const* const using_stmt_tag = BAD_CAST "using_stmt";
    xmlChar const* const fixed_tag = BAD_CAST "fixed";
    xmlChar const* const lock_tag = BAD_CAST "lock";
    xmlChar const* const synchronized_tag = BAD_CAST "synchronized";
    xmlChar const* const unsafe_tag = BAD_CAST "unsafe";
    xmlChar const* const checked_tag = BAD_CAST "checked";
    xmlChar const* const unchecked_tag = BAD_CAST "unchecked";
    

    xmlChar const* const goto_tag = BAD_CAST "goto";

    xmlChar const* const function_tag = BAD_CAST "function";
    xmlChar const* const function_decl_tag = BAD_CAST "function_decl";
    
    xmlChar const* const class_tag = BAD_CAST "class";
    xmlChar const* const class_decl_tag = BAD_CAST "class_decl";
    
    xmlChar const* const struct_tag = BAD_CAST "struct";
    xmlChar const* const struct_decl_tag = BAD_CAST "struct_decl";

    xmlChar const* const union_tag = BAD_CAST "union";
    xmlChar const* const union_decl_tag = BAD_CAST "union_decl";
    xmlChar const* const namespace_tag = BAD_CAST "namespace";

    typedef xmlChar const* XmlCharConstPtr;
    typedef char const* CharConstPtr;

    struct XmlCharHasher
        :boost::hash<char const*>
    {
        typedef std::size_t result_type;
        result_type operator()(XmlCharConstPtr tagName) const {
            result_type ret = 0;
            XmlCharConstPtr cur = tagName;
            while(*cur) {
                ret += *cur;
                ++cur;
            }
            return ret;
        }
    };
    struct XmlCharEqualityComparer {
        bool operator()(XmlCharConstPtr lhs, XmlCharConstPtr rhs) const {
            return xmlStrEqual(lhs, rhs);
        }
    };

    typedef boost::unordered_set<XmlCharConstPtr, XmlCharHasher, XmlCharEqualityComparer> NodeNameSet;

    NodeNameSet has_return_node_init() {
        NodeNameSet ret;
        ret.insert(block_tag);
        ret.insert(while_tag);
        ret.insert(if_tag);
        ret.insert(then_tag);
        ret.insert(else_tag);
        ret.insert(elseif_tag);
        ret.insert(try_tag);
        ret.insert(catch_tag);
        ret.insert(finally_tag);
        ret.insert(do_tag);
        ret.insert(for_tag);
        ret.insert(foreach_tag);
        ret.insert(switch_tag);
        ret.insert(using_stmt_tag);
        ret.insert(fixed_tag);
        ret.insert(lock_tag);
        ret.insert(synchronized_tag);
        ret.insert(unsafe_tag);
        ret.insert(checked_tag);
        ret.insert(unchecked_tag);
        return ret;
    }    
}

void xpath_exfun_has_return(xmlXPathParserContextPtr ctxt, int nargs) {

    CHECK_ARITY(0);
    static NodeNameSet hasReturnValidNodes = has_return_node_init();
    xmlNodePtr currentNode = ctxt->context->node;
    xmlNodePtr input = currentNode;
    xmlNodePtr temp = 0;
    
// START:
    if(!currentNode) {
        goto EXIT;
    } else {
        goto DESCENDING;
    }
VISIT:
    if(currentNode->type == XML_ELEMENT_NODE) {
        if(xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI)) {
            if(xmlStrEqual(return_tag, currentNode->name)) {
                xmlXPathReturnTrue(ctxt); return;
            } else {
                NodeNameSet::iterator locatedElementIter = hasReturnValidNodes.find(currentNode->name);
                if(locatedElementIter == hasReturnValidNodes.end()) {
                    goto STRAIF_SIBLINGS;
                }
            }
        }
    }
DESCENDING:
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
    xmlNodePtr currentNode = ctxt->context->node;
    if(currentNode->type == XML_ELEMENT_NODE && xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI)) {
        if (xmlStrEqual(asm_tag, currentNode->name)) {
            // ASM tag is nested if it's within another asm tag.
            
        }else if (xmlStrEqual(typedef_tag, currentNode->name)
            || xmlStrEqual(using_tag, currentNode->name)
            || xmlStrEqual(function_tag, currentNode->name)
            || xmlStrEqual(function_decl_tag, currentNode->name)
            || xmlStrEqual(constructor_tag, currentNode->name)
            || xmlStrEqual(constructor_decl_tag, currentNode->name)
            || xmlStrEqual(destructor_tag, currentNode->name)
            || xmlStrEqual(destructor_decl_tag, currentNode->name)
            || xmlStrEqual(property_tag, currentNode->name)
            || xmlStrEqual(event_tag, currentNode->name)
            || xmlStrEqual(union_tag, currentNode->name)
            || xmlStrEqual(union_decl_tag, currentNode->name)
            || xmlStrEqual(struct_tag, currentNode->name)
            || xmlStrEqual(struct_decl_tag, currentNode->name)
            || xmlStrEqual(class_tag, currentNode->name)
            || xmlStrEqual(class_decl_tag, currentNode->name)
            || xmlStrEqual(enum_tag, currentNode->name)
            || xmlStrEqual(namespace_tag, currentNode->name))
        {
            // typedefis nested if it's within a function/method/constructor/destructor/namespace/class/
            // union/struct/class.
        }

    }else{
        xmlXPathReturnFalse(ctxt); return;
    }
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
