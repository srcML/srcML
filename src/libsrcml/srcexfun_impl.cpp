/**
 * @file srcmlexfun_impl.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

namespace {
    /* Static constants used by this file that don't need to be visible to other areas of the language. */
    typedef const xmlChar* ConstXmlCharPtr;
    typedef const char* ConstCharPtr;
    xmlChar const* const unit_tag = BAD_CAST "unit";
    xmlChar const* const language_attr = BAD_CAST "language";
    xmlChar const* const specifier_tag = BAD_CAST "specifier";
    xmlChar const* const asm_tag = BAD_CAST "asm";
    xmlChar const* const return_tag = BAD_CAST "return";
    xmlChar const* const break_tag = BAD_CAST "break";
    xmlChar const* const typedef_tag = BAD_CAST "typedef";    
    xmlChar const* const using_tag = BAD_CAST "using";

    xmlChar const* const property_tag = BAD_CAST "property";
    xmlChar const* const event_tag = BAD_CAST "event";
    xmlChar const* const enum_tag = BAD_CAST "enum";
    xmlChar const* const expr_stmt_tag = BAD_CAST "expr_stmt";
    xmlChar const* const decl_stmt_tag = BAD_CAST "decl_stmt";
    xmlChar const* const empty_stmt_tag = BAD_CAST "empty_stmt";
    xmlChar const* const decl_tag = BAD_CAST "decl";
    xmlChar const* const param_tag = BAD_CAST "param";
    xmlChar const* const argument_list_tag = BAD_CAST "argument_list";


    xmlChar const* const constructor_tag = BAD_CAST "constructor";
    xmlChar const* const constructor_decl_tag = BAD_CAST "constructor_decl";
    xmlChar const* const destructor_tag = BAD_CAST "destructor";
    xmlChar const* const destructor_decl_tag = BAD_CAST "destructor_decl";
    xmlChar const* const throw_tag = BAD_CAST "throw";
    xmlChar const* const type_tag = BAD_CAST "type";

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
    xmlChar const* const forever_tag = BAD_CAST "forever";
    xmlChar const* const switch_tag = BAD_CAST "switch";
    xmlChar const* const using_stmt_tag = BAD_CAST "using_stmt";
    xmlChar const* const fixed_tag = BAD_CAST "fixed";
    xmlChar const* const lock_tag = BAD_CAST "lock";
    xmlChar const* const synchronized_tag = BAD_CAST "synchronized";
    xmlChar const* const unsafe_tag = BAD_CAST "unsafe";
    xmlChar const* const checked_tag = BAD_CAST "checked";
    xmlChar const* const unchecked_tag = BAD_CAST "unchecked";
    

    xmlChar const* const template_tag = BAD_CAST "template";
    xmlChar const* const name_tag = BAD_CAST "name";
//    xmlChar const* const goto_tag = BAD_CAST "goto";
    xmlChar const* const function_tag = BAD_CAST "function";
    xmlChar const* const function_decl_tag = BAD_CAST "function_decl";
    xmlChar const* const delegate_tag = BAD_CAST "delegate";
    xmlChar const* const lambda_tag = BAD_CAST "lambda";

    xmlChar const* const class_tag = BAD_CAST "class";
    xmlChar const* const class_decl_tag = BAD_CAST "class_decl";
    
    xmlChar const* const struct_tag = BAD_CAST "struct";
    xmlChar const* const struct_decl_tag = BAD_CAST "struct_decl";

    xmlChar const* const union_tag = BAD_CAST "union";
    xmlChar const* const union_decl_tag = BAD_CAST "union_decl";
    xmlChar const* const namespace_tag = BAD_CAST "namespace";

    xmlChar const* const type_attr = BAD_CAST "type";
    xmlChar const* const template_attr_value = template_tag;

    xmlChar const* const init_tag = BAD_CAST "init";



    /* Implementation helper functions, typedefs and templates. */
    typedef xmlChar const* XmlCharConstPtr;
    typedef char const* CharConstPtr;

    /* Used for hashing the xml document string (poorly). */
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
            return xmlStrEqual(lhs, rhs) != 0;
        }
    };
    /* Type used for handling nodesets without storing any more then then a pointer to the statically allocated tag name. */
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

    NodeNameSet has_throw_node_init() {
        NodeNameSet ret;
        ret.insert(block_tag);
        ret.insert(while_tag);
        ret.insert(if_tag);
        ret.insert(then_tag);
        ret.insert(else_tag);
        ret.insert(elseif_tag);
        ret.insert(try_tag);
        // ret.insert(finally_tag);
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

    /* 
        has_break_node_init
        Initializes the set of nodes used to restrict the scope of the 
        has_break to not search into other loop structures, or statements
        that shouldn't be searched.
     */
    NodeNameSet has_break_node_init() {
        NodeNameSet ret;
        ret.insert(do_tag);
        ret.insert(while_tag);
        ret.insert(for_tag);
        ret.insert(foreach_tag);
        ret.insert(forever_tag);
        ret.insert(switch_tag);
        ret.insert(expr_stmt_tag);
        ret.insert(decl_stmt_tag);
        ret.insert(empty_stmt_tag);
        ret.insert(delegate_tag);
        ret.insert(lambda_tag);
        // ret.insert(delegate_tag);
        return ret;
    }    


    xmlChar const* getSrcMLDocLanguage(xmlNodePtr node) {
        xmlChar const* currentAttrValue = 0;
        xmlNodePtr currentNode = node;
        while(currentNode) {
            if (currentNode->type == XML_ELEMENT_NODE
                && xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0
                && xmlStrEqual(unit_tag, currentNode->name) != 0
                && (currentAttrValue = xmlGetProp(currentNode, language_attr)))
            {
                return currentAttrValue;
            }
            currentNode = currentNode->parent;
        }
        return currentAttrValue;
    }

    enum Scoping {
        EXCLUSIVE, /* Excludes items in the set */
        INCLUSIVE /* Includes items in the set. */
    };
    template<Scoping> struct ScopedDescendentsTraversalImpl;

    template<> struct ScopedDescendentsTraversalImpl<INCLUSIVE> {
        static void traverse(NodeNameSet const& scope, xmlChar const* const returnTrueOn, xmlXPathParserContextPtr ctxt) {
            xmlNodePtr currentNode = ctxt->context->node;
            xmlNodePtr input = currentNode;
            xmlNodePtr temp = 0;
            
            if(!currentNode) {
                goto EXIT;
            } else {
                goto DESCENDING;
            }
        VISIT:
            if(currentNode->type == XML_ELEMENT_NODE) {
                if(xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
                    if(xmlStrEqual(currentNode->name, returnTrueOn) != 0) {
                        xmlXPathReturnTrue(ctxt); return;
                    } else {
                        NodeNameSet::const_iterator locatedElementIter = scope.find(currentNode->name);
                        if(locatedElementIter == scope.end()) {
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
            xmlXPathReturnFalse(ctxt);
        }
    };

    template<> struct ScopedDescendentsTraversalImpl<EXCLUSIVE> {
        static void traverse(NodeNameSet const& scope, xmlChar const* const returnTrueOn, xmlXPathParserContextPtr ctxt) {
            xmlNodePtr currentNode = ctxt->context->node;
            xmlNodePtr input = currentNode;
            xmlNodePtr temp = 0;
            
            if(!currentNode) {
                goto EXIT;
            } else {
                goto DESCENDING;
            }
        VISIT:
            if(currentNode->type == XML_ELEMENT_NODE) {
                if(xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
                    if(xmlStrEqual(returnTrueOn, currentNode->name) != 0) {
                        xmlXPathReturnTrue(ctxt); return;
                    } else {
                        NodeNameSet::iterator locatedElementIter = scope.find(currentNode->name);
                        if(locatedElementIter != scope.end()) {
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
            xmlXPathReturnFalse(ctxt);
        }
    };

    template<Scoping NodeSetScoping>
    inline void scopedDescendentsTraversal(NodeNameSet const& scope, xmlChar const* const returnTrueOn, xmlXPathParserContextPtr ctxt) {
        ScopedDescendentsTraversalImpl<NodeSetScoping>::traverse(scope, returnTrueOn, ctxt);
    }

}

void xpath_exfun_has_return(xmlXPathParserContextPtr ctxt, int nargs) {

    CHECK_ARITY(0);
    static NodeNameSet hasReturnValidNodes = has_return_node_init();
    scopedDescendentsTraversal<INCLUSIVE>(hasReturnValidNodes, return_tag, ctxt);
}

void xpath_exfun_has_throw(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    static NodeNameSet hasThrowValidNodes = has_throw_node_init();
    scopedDescendentsTraversal<INCLUSIVE>(hasThrowValidNodes, throw_tag, ctxt);
}

void xpath_exfun_is_nested(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    xmlNodePtr currentNode = ctxt->context->node;
    if(currentNode->type == XML_ELEMENT_NODE && xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
        if (xmlStrEqual(asm_tag, currentNode->name) != 0) {
            // ASM tag is nested if it's within another asm tag.
            if (currentNode->parent->type == XML_ELEMENT_NODE
                && xmlStrEqual(currentNode->parent->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0
                && xmlStrEqual(asm_tag, currentNode->parent->name) != 0)
            {
                xmlXPathReturnTrue(ctxt); return;
            }else {
                xmlXPathReturnFalse(ctxt); return;
            }
        }else if (xmlStrEqual(typedef_tag, currentNode->name) != 0
            || xmlStrEqual(using_tag, currentNode->name) != 0
            || xmlStrEqual(function_tag, currentNode->name) != 0
            || xmlStrEqual(function_decl_tag, currentNode->name) != 0
            || xmlStrEqual(constructor_tag, currentNode->name) != 0
            || xmlStrEqual(constructor_decl_tag, currentNode->name) != 0
            || xmlStrEqual(destructor_tag, currentNode->name) != 0
            || xmlStrEqual(destructor_decl_tag, currentNode->name) != 0
            || xmlStrEqual(property_tag, currentNode->name) != 0
            || xmlStrEqual(event_tag, currentNode->name) != 0
            || xmlStrEqual(union_tag, currentNode->name) != 0
            || xmlStrEqual(union_decl_tag, currentNode->name) != 0
            || xmlStrEqual(struct_tag, currentNode->name) != 0
            || xmlStrEqual(struct_decl_tag, currentNode->name) != 0
            || xmlStrEqual(class_tag, currentNode->name) != 0
            || xmlStrEqual(class_decl_tag, currentNode->name) != 0
            || xmlStrEqual(enum_tag, currentNode->name) != 0
            || xmlStrEqual(namespace_tag, currentNode->name) != 0)
        {
            currentNode = currentNode->parent;
            while(currentNode) {

                if(xmlStrEqual(function_tag, currentNode->name) != 0
                    || xmlStrEqual(constructor_tag, currentNode->name) != 0
                    || xmlStrEqual(destructor_tag, currentNode->name) != 0
                    || xmlStrEqual(property_tag, currentNode->name) != 0
                    || xmlStrEqual(event_tag, currentNode->name) != 0
                    || xmlStrEqual(union_tag, currentNode->name) != 0
                    || xmlStrEqual(struct_tag, currentNode->name) != 0
                    || xmlStrEqual(class_tag, currentNode->name) != 0
                    || xmlStrEqual(enum_tag, currentNode->name) != 0
                    || xmlStrEqual(namespace_tag, currentNode->name) != 0)
                {
                    xmlXPathReturnTrue(ctxt); return;
                }
                currentNode = currentNode->parent;
            }
        }
    }
    xmlXPathReturnFalse(ctxt); return;
}

void xpath_exfun_is_class_template_partial_specialization(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    xmlNodePtr currentNode = ctxt->context->node;
    if(currentNode->type == XML_ELEMENT_NODE) {
        if(xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
            if (    xmlStrEqual(class_tag, currentNode->name) != 0
                ||  xmlStrEqual(class_decl_tag, currentNode->name) != 0
                ||  xmlStrEqual(struct_tag, currentNode->name) != 0
                ||  xmlStrEqual(struct_decl_tag, currentNode->name) != 0
                ||  xmlStrEqual(union_tag, currentNode->name) != 0
                ||  xmlStrEqual(union_decl_tag, currentNode->name) != 0
                )
            {
                // Checking for template element if I don't find one then false.
                xmlNodePtr currentChildNode = xmlFirstElementChild(currentNode);
                xmlNodePtr lastNameNode = 0;
                xmlNodePtr lastTemplate = 0;
                // std::size_t templateCount = 0;
                while(currentChildNode) {
                    if(currentChildNode->type == XML_ELEMENT_NODE && xmlStrEqual(currentChildNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
                        if(xmlStrEqual(template_tag, currentChildNode->name) != 0) {
                            lastTemplate = currentChildNode;
                        } else if(xmlStrEqual(name_tag, currentChildNode->name) != 0) {
                            lastNameNode = currentChildNode;
                        } else if(xmlStrEqual(block_tag, currentChildNode->name) != 0) {
                            break;
                        }
                    }
                    currentChildNode = currentChildNode->next;
                }

                // Ignore anonymous structs and things.
                if(!lastTemplate || !lastNameNode) {
                    xmlXPathReturnFalse(ctxt); return;
                }

                // Check the last template for children.
                // It must have at least one child, otherwise this is an explicit specialization.
                // OR a member specialization of some sort.
                xmlNodePtr templateParameterListNode = xmlFirstElementChild(lastTemplate);
                xmlNodePtr possibleParameter = xmlFirstElementChild(templateParameterListNode);
                if(!(possibleParameter != 0
                    && possibleParameter->type == XML_ELEMENT_NODE
                    && xmlStrEqual(possibleParameter->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0
                    && xmlStrEqual(param_tag, possibleParameter->name) != 0) )
                {
                    xmlXPathReturnFalse(ctxt); return;
                }

                // Check for template arguments within the name.
                /*
1. <name>Bar</name>

2. <name>
    <name>Foo1</name>
    <argument_list type="template">&lt;
        <argument><expr><name><name>vector</name><argument_list type="template">&lt;<argument><expr><name>T</name></expr></argument>&gt;</argument_list></name></expr></argument>&gt;
    </argument_list>
</name>

BUG here!!! but this is still handled using the same situation
<macro><name>Name</name></macro>
3. <name>
    <operator>::</operator>
    <name>
        <name>Bar</name>
        <argument_list type="template">&lt;
            <argument><expr><name>int</name></expr></argument>&gt;
        </argument_list>
    </name>
</name>

4. <name>
    <name>
        <name>Name</name>
        <argument_list type="template">&lt;
            <argument><expr><name>int</name></expr></argument>&gt;
        </argument_list>
    </name>
    <operator>::</operator>
    <name>
        <name>Bar</name>
        <argument_list type="template">&lt;
            <argument><expr><name>int</name></expr></argument>&gt;
        </argument_list>
    </name>
</name>
                */
                xmlNodePtr nameElem = xmlFirstElementChild(lastNameNode);
                // Handling case 1
                if(!nameElem) {
                    xmlXPathReturnFalse(ctxt); return;
                }
                xmlChar const* currentAttrValue = 0;
                // Handling case 2.
                xmlNodePtr lastNameInClassName = 0;
                xmlNodePtr possibleArgListNode = nameElem;
                while(possibleArgListNode) {
                    if (possibleArgListNode->type == XML_ELEMENT_NODE
                        && xmlStrEqual(possibleArgListNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
                        if (xmlStrEqual(argument_list_tag, possibleArgListNode->name) != 0
                            && (currentAttrValue = xmlGetProp(possibleArgListNode, type_attr))
                            && xmlStrEqual(currentAttrValue, template_attr_value) != 0)
                        {
                            // Handles case 2
                            xmlXPathReturnTrue(ctxt); return;
                        }else if(xmlStrEqual(name_tag, possibleArgListNode->name) != 0) {
                            // Used for case 3 and 4.
                            lastNameInClassName = possibleArgListNode;
                        }
                    }
                    possibleArgListNode = possibleArgListNode->next;
                }

                // Used for case 3 and 4.
                possibleArgListNode = xmlFirstElementChild(lastNameInClassName);
                // If only text then this isn't a partial specialization.
                if(!possibleArgListNode) {
                    xmlXPathReturnFalse(ctxt); return;
                }
                while(possibleArgListNode) {
                    if (possibleArgListNode->type == XML_ELEMENT_NODE
                        && xmlStrEqual(possibleArgListNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
                        if (xmlStrEqual(argument_list_tag, possibleArgListNode->name) != 0
                            && (currentAttrValue = xmlGetProp(possibleArgListNode, type_attr))
                            && xmlStrEqual(currentAttrValue, template_attr_value) != 0)
                        {
                            // Handles case 2
                            xmlXPathReturnTrue(ctxt); return;
                        }
                    }
                    possibleArgListNode = possibleArgListNode->next;
                }
            }
        }
    }
    xmlXPathReturnFalse(ctxt); return;
}

void xpath_exfun_has_init(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);

    enum JumpToLocation {
        DECL_IS_CHILD,
        INIT_IS_CHILD,
        IS_INVALD
    };
    xmlNodePtr currentNode = ctxt->context->node;
    if(currentNode->type == XML_ELEMENT_NODE) {
        if(xmlStrEqual(BAD_CAST SRCML_SRC_NS_URI, currentNode->ns->href) != 0) {
            JumpToLocation jumpTo = IS_INVALD;
            xmlNodePtr declParent = 0;
            xmlNodePtr initParent = 0;
            if (xmlStrEqual(decl_stmt_tag, currentNode->name) != 0
                ||  xmlStrEqual(param_tag, currentNode->name) != 0)
            {
                declParent = currentNode;
                jumpTo = DECL_IS_CHILD;
            } else if (xmlStrEqual(using_stmt_tag, currentNode->name) != 0
                ||  xmlStrEqual(try_tag, currentNode->name) != 0
                ||  xmlStrEqual(synchronized_tag, currentNode->name) != 0
                ||  xmlStrEqual(fixed_tag, currentNode->name) != 0
                ||  xmlStrEqual(lock_tag, currentNode->name) != 0
                ||  xmlStrEqual(decl_tag, currentNode->name) != 0
                ||  xmlStrEqual(using_tag, currentNode->name) != 0)
            {
                initParent = currentNode;
                jumpTo = INIT_IS_CHILD;
            }

            xmlNodePtr initLocator = 0;
            switch(jumpTo) {
                case DECL_IS_CHILD:
                    initParent = xmlFirstElementChild(declParent);
                    while(initParent) {
                        if(initParent->type == XML_ELEMENT_NODE
                            && xmlStrEqual(BAD_CAST SRCML_SRC_NS_URI, initParent->ns->href) != 0
                            && xmlStrEqual(decl_tag, initParent->name) != 0)
                        {
                            initLocator = xmlFirstElementChild(initParent);
                            while (initLocator) {
                                if(initLocator->type == XML_ELEMENT_NODE
                                    && xmlStrEqual(BAD_CAST SRCML_SRC_NS_URI, initLocator->ns->href) != 0
                                    && xmlStrEqual(init_tag, initLocator->name) != 0)
                                {
                                    xmlXPathReturnTrue(ctxt); return;
                                }
                                initLocator = initLocator->next;
                            }
                        }
                        initParent = initParent->next;
                    }
                    break;

                case INIT_IS_CHILD:
                    initLocator = xmlFirstElementChild(initParent);
                    while (initLocator) {
                        if(initLocator->type == XML_ELEMENT_NODE
                            && xmlStrEqual(BAD_CAST SRCML_SRC_NS_URI, initLocator->ns->href) != 0
                            && xmlStrEqual(init_tag, initLocator->name) != 0)
                        {
                            xmlXPathReturnTrue(ctxt); return;
                        }
                        initLocator = initLocator->next;
                    }
                    break;
                case IS_INVALD:
                default:
                    // If this isn't called on one of the above named tags then 
                    // simply return with an error because this is
                    // considered an invalid predicate.
                    // xmlXPathErr(ctxt, XPATH_INVALID_PREDICATE_ERROR);
                    // return;
                    xmlXPathReturnFalse(ctxt); return;
            }
        }
    }
    xmlXPathReturnFalse(ctxt); return;
}

void xpath_exfun_has_break(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    static NodeNameSet hasBreakInvalidNodes = has_break_node_init();
    scopedDescendentsTraversal<EXCLUSIVE>(hasBreakInvalidNodes, break_tag, ctxt);
}

void xpath_exfun_is_unsafe(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    xmlNodePtr currentNode = ctxt->context->node;
    while(currentNode) {
        if (currentNode->type == XML_ELEMENT_NODE
            && xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
            if (xmlStrEqual(currentNode->name, unsafe_tag) != 0) {
                xmlXPathReturnTrue(ctxt); return;
            }
        }
        currentNode = currentNode->parent;
    }
    xmlXPathReturnFalse(ctxt);
}

void xpath_exfun_is_mutually_exclusive(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
    xmlNodePtr currentNode = ctxt->context->node;
    xmlChar const* language = getSrcMLDocLanguage(currentNode);
    if(xmlStrcasecmp(language, BAD_CAST "C#") == 0) {
        while(currentNode) {
            if (currentNode->type == XML_ELEMENT_NODE
                && xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
                if (xmlStrEqual(currentNode->name, lock_tag) != 0) {
                    xmlXPathReturnTrue(ctxt); return;
                }
            }
            currentNode = currentNode->parent;
        }
    }else if(xmlStrcasecmp(language, BAD_CAST "java") == 0) {
        while(currentNode) {
            if (currentNode->type == XML_ELEMENT_NODE
                && xmlStrEqual(currentNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0) {
                if (xmlStrEqual(currentNode->name, synchronized_tag) != 0) {
                    xmlXPathReturnTrue(ctxt); return;
                }else if(xmlStrEqual(currentNode->name, function_tag) != 0) {
                    xmlNodePtr typeNode = xmlFirstElementChild(currentNode);
                    // Function without a return type.
                    if (!typeNode) {
                        xmlXPathReturnFalse(ctxt); return;
                    }else if ( !(typeNode->type == XML_ELEMENT_NODE
                        && xmlStrEqual(typeNode->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0
                        && xmlStrEqual(typeNode->name, type_tag) != 0))
                    {
                        xmlXPathReturnFalse(ctxt); return;
                    }
                    xmlNodePtr currentTypeElement = xmlFirstElementChild(typeNode);
                    while(currentTypeElement) {
                        if (currentTypeElement->type == XML_ELEMENT_NODE
                            && xmlStrEqual(currentTypeElement->ns->href, BAD_CAST SRCML_SRC_NS_URI) != 0
                            && xmlStrEqual(currentTypeElement->name, specifier_tag) != 0)
                        {
                            bool result = currentTypeElement->children
                                && currentTypeElement->children->type == XML_TEXT_NODE
                                && xmlStrEqual(currentTypeElement->children->content, BAD_CAST "synchronized") != 0;
                            if(result){
                                xmlXPathReturnTrue(ctxt); return;
                            }
                        }
                        currentTypeElement = currentTypeElement->next;
                    }
                    xmlXPathReturnFalse(ctxt); return;
                }
            }
            currentNode = currentNode->parent;
        }
    }
    xmlXPathReturnFalse(ctxt);
}

void xpath_exfun_is_upper_bound(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}

void xpath_exfun_is_lower_bound(xmlXPathParserContextPtr ctxt, int nargs) {
    CHECK_ARITY(0);
}
