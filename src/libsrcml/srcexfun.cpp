/**
 * @file srcexfun.cpp
 *
 * @copyright Copyright (C) 2009-2014 srcML, LLC. (www.srcML.org)
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

#include <srcexfun.hpp>
#include <srcmlns.hpp>
#include <cmath>
#include <cstring>
#include <string>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlreader.h>

/*
  static xmlChar* unit_url = 0;
  static xmlChar* unit_filename = 0;
*/

#include <libxml/tree.h>


#include <srcexfun_impl.hpp>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#else
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>
#endif

#include <iostream>

/** @todo these might block treading. */
/** postion */
static int Position;

/** old attributes */
static PROPERTIES_TYPE* oldpattributes;

/** root attributes */
static const xmlChar** pattributes;

/** number of root attributes */
static int nb_attributes;

/** set of MACRO functions I think */
static std::vector<struct xpath_ext_function> MACROS;

/**
 * setPosition
 * @param n position
 *
 * Set position.
 */
void setPosition(int n) {
    Position = n;
}

/**
 * setRootAttributes
 * @param attributes the root attributes
 * @param pnb_attributes number of root attributes
 * 
 * Set the root attributes.
 */
void setRootAttributes(const xmlChar** attributes, int pnb_attributes) {
    pattributes = attributes;
    nb_attributes = pnb_attributes;
}

/**
 * setRootAttributes
 * @param attributes the root attributes
 *
 * Save the root attributes?
 */
void setRootAttributes(PROPERTIES_TYPE& attributes) {
    oldpattributes = &attributes;
}

/**
 * srcContextFunction
 * @param ctxt an xml XPath parser context
 * @param nargs number for arguments
 *
 * ???
 */
static void srcContextFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    if (nargs != 0) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    fprintf(stderr, "DEBUG:  %s %s %d\n", __FILE__,  __FUNCTION__, __LINE__);

    valuePush(ctxt, xmlXPathNewFloat(Position));
}

/**
 * find_attribute_index
 * @param nb_attributes number of attributes
 * @param attributes attribute list
 * @param attribute an individual attribute
 *
 * Find the index of attribute in list of attributes.
 * @returns the position of attribute in attributes array.
 */
int find_attribute_index(int nb_attributes, const xmlChar** attributes, const char* attribute) {

    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
        if (strcmp((const char*) attributes[index], attribute) == 0)
            return index;

    return -1;
}

/**
 * srcRootFunction
 * @param ctxt an xml XPath parser context
 * @param nargs number for arguments
 *
 * ???
 */
static void srcRootFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    if (nargs != 1) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    xmlChar* name = xmlXPathPopString(ctxt);

    int n = find_attribute_index(nb_attributes, pattributes, (const char*) name);
    if (n == -1) {
        valuePush(ctxt, NULL);
        return;
    }

    std::string s(pattributes[n + 3], pattributes[n + 4]);

    valuePush(ctxt, xmlXPathNewString(BAD_CAST s.c_str()));
}

/**
 * srcMacrosFunction
 * @param ctxt an xml XPath parser context
 * @param nargs number for arguments
 *
 * ???
 */
static void srcMacrosFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    // as of now, all of these have no arguments
    if (nargs != 0) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    // find out which expression is being used based on the name
    std::vector<struct xpath_ext_function>::size_type i;
    for (i = 0; i < MACROS.size(); ++i)
        if (strcmp(MACROS[i].name.c_str(), (const char*) ctxt->context->function) == 0)
            break;

    // evaluate the expression on the given context
    xmlXPathObjectPtr ret = xmlXPathEval(BAD_CAST MACROS[i].expr.c_str(), ctxt->context);
    CHECK_ERROR;

    if (ret) {
        valuePush(ctxt, ret);
    }else{
        xmlXPathReturnFalse(ctxt);
    }
}

/**
 * srcInFunction
 * @param ctxt an xml XPath parser context
 * @param nargs number for arguments
 *
 * ???
 */
static void srcInFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    // need at least one argument
    if (nargs == 0) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    // find the first xpath that produces a result
    for (int i = 0; i < nargs; ++i) {

        std::string path = "ancestor::";

        // find the name of the element
        xmlChar* name = xmlXPathPopString(ctxt);

        path.append((const char*) name);

        // evaluate the expression on the given context
        xmlXPathObjectPtr ret = xmlXPathEval(BAD_CAST path.c_str(), ctxt->context);
        if (ret) {
            valuePush(ctxt, xmlXPathNewBoolean(1));
            return;
        }
    }

    valuePush(ctxt, xmlXPathNewBoolean(0));
}

/**
 * srcPosersetFunction
 * @param ctxt an xml XPath parser context
 * @param nargs number for arguments
 *
 * XPath extension function to compute powerset.
 */
static void srcPowersetFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    if (nargs != 1) {
        xmlXPathSetArityError(ctxt);
        return;
    }

    // node set to form powerset over
    xmlNodeSetPtr master = xmlXPathPopNodeSet(ctxt);

    // node set of sets
    xmlNodeSetPtr ret = xmlXPathNodeSetCreate(NULL);

    // number of sets
    int setsize = (int)pow(2, master->nodeNr);

    // create all the sets
    for (int setnum = 0; setnum < setsize; ++setnum) {

        // set node
        xmlNodePtr setnode = xmlNewNodeEatName(0, (xmlChar*)"set");
        xmlXPathNodeSetAdd(ret, setnode);

        // create this set, only leaving in what fits the bit position
        for (int i = 0; i < master->nodeNr; ++i) {

            if (setnum & (1 << i)) {
                xmlNodePtr node = xmlCopyNode(master->nodeTab[i], 1);
                xmlAddChild(setnode, node);
            }
        }
    }

    if (ret) {
        valuePush(ctxt, xmlXPathNewNodeSetList(ret));
    }
}

/**
 * xpathsrcMLRegister
 * @param context an xml XPath context
 *
 * Register srcML XPath extension functions.
 */
void xpathsrcMLRegister(xmlXPathContextPtr context) {

    xmlXPathRegisterFuncNS(context, (const xmlChar *)"unit",
                           BAD_CAST SRCML_SRC_NS_URI,
                           srcContextFunction);

    xmlXPathRegisterFuncNS(context, (const xmlChar *)"archive",
                           BAD_CAST SRCML_SRC_NS_URI,
                           srcRootFunction);

    xmlXPathRegisterFuncNS(context, (const xmlChar *)"powerset",
                           BAD_CAST SRCML_SRC_NS_URI,
                           srcPowersetFunction);
                           
    xmlXPathRegisterFuncNS(context, (const xmlChar *)"has_return",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_has_return);
                           
    xmlXPathRegisterFuncNS(context, (const xmlChar *)"has_throw",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_has_throw);
                           
    xmlXPathRegisterFuncNS(context, (const xmlChar *)"is_nested",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_is_nested);
                           
    xmlXPathRegisterFuncNS(context, (const xmlChar *)"is_class_template_partial_specialization",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_is_class_template_partial_specialization);

    xmlXPathRegisterFuncNS(context, (const xmlChar *)"has_init",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_has_init);

    xmlXPathRegisterFuncNS(context, (const xmlChar *)"has_break",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_has_break);
                           
    xmlXPathRegisterFuncNS(context, (const xmlChar *)"is_unsafe",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_is_unsafe);
                           
    xmlXPathRegisterFuncNS(context, (const xmlChar *)"is_mutually_exclusive",
                           BAD_CAST SRCML_SRC_NS_URI,
                           xpath_exfun_is_mutually_exclusive);
                           
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI,
        "is_static",
        "src:decl/src:type[src:specifier[.= 'static']] or src:type[src:specifier[.= 'static']] or src:specifier[.='static']"
    );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI,
        "block_is_static",
        "parent::src:static"
    );

    char const* const is_abstract_or_is_pure_virtual_xpath =
    "("
        "ancestor::src:unit[@language='C++']"
        "  and "
        "("
            "("
                "(self::src:destructor_decl or self::src:function_decl) and src:literal[.='0']"
            ")"
            "  or  "
            "("
                "(self::src:struct or self::src:class or self::src:union) "
                " and "
                "src:block["
                    " (src:private | src:protected | src:public)["
                        " (src:function_decl | src:destructor_decl)["
                            "src:literal[@type='number'][.='0'] "
                        "]"
                    "]"
                "]"
            ")"
        ")"
    ")"
    " or "
    "("
        "ancestor::src:unit[@language='C#' or @language='Java']"
        " and ("
        "("
            "(self::src:destructor_decl or self::src:struct or self::src:class)"
            " and "
            "(src:specifier[.='abstract'] or src:type/src:specifier[.='abstract'])"
        ")"
        " or "
        "("
            "(self::src:property or self::src:event)"
            " and "
            "src:type/src:specifier[.='abstract']"
        ")"
        "  or "
        "("
            "(self::src:function_decl)"
            "  and "
            "("
                "src:type/src:specifier[.='abstract']"
                "  or "
                "parent::src:block["
                    "(parent::src:property | parent::src:event)[src:type/src:specifier[.='abstract'] or parent::src:block/parent::src:interface]"
                "]"
            ")"
        ")"
        "  or "
        "((self::src:property or self::src:event or self::src:function_decl) and parent::src:block/parent::src:interface)"
        "  or "
        "(self::src:interface))"
    ")";
    xpathRegisterExtensionFunction(
        SRCML_SRC_NS_URI,
        "is_abstract",
        is_abstract_or_is_pure_virtual_xpath
    );
    xpathRegisterExtensionFunction(
        SRCML_SRC_NS_URI,
        "is_pure_virtual",
        is_abstract_or_is_pure_virtual_xpath
    );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_default_impl", "src:specifier[.='default'] or src:type[src:specifier[.='default']]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_default_property_value", "src:default");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_final", "src:specifier[.='final'] or src:decl[src:type[src:specifier[.='final']]] or src:type[src:specifier[.='final']]");
    // xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_sealed", "src:specifier[.='final'] or src:decl[src:type[src:specifier[.='final']]] or src:type[src:specifier[.='final']]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_synchronized", "src:type[src:specifier[.='synchronized']]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_transient", "src:decl/src:type/src:specifier[.='transient'] or src:type/src:specifier[.='transient']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_native", "src:type[src:specifier[.='native']] | src:type/src:attribute/src:expr/src:call/src:name[.='DllImport']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_private",
        "("
            "ancestor::src:unit[@language='C++']"
            " and "
            "parent::src:private"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C#']"
            " and "
            "("
                "("
                    "(self::src:class or self::src:struct or self::src:interface)"
                    " and "
                    "("
                        "src:specifier[.='private']"
                        " or "
                        "("
                            "not("
                                "src:specifier[.='protected' or .='public' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block[parent::src:class or parent::src:struct]"
                        ")"
                    ")"

                ")"
                " or "
                "("
                    "(self::src:destructor or self::src:destructor_decl or self::src:constructor or self::src:constructor_decl or self::src:function or self::src:function_decl or self::src:property or self::src:event)"
                    " and "
                    "("
                        "src:type/src:specifier[.='private']"
                        " or "
                        "("
                            "not("
                                "src:type/src:specifier[.='protected' or .='public' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block[parent::src:class or parent::src:struct]"
                        ")"
                        " or "
                        "("
                            "(self::src:function or self::src:function_decl)"
                            " and "
                            "src:name[.='get' or .='set' or .='add' or .='remove']"
                            " and "
                            "("
                                "src:specifier[.='private']"
                                " or "
                                "("
                                    "not("
                                        "src:specifier[.='protected' or .='public' or .='internal']"
                                    ")"
                                    " and "
                                    "parent::src:block["
                                        "(parent::src:property | parent::src:event)["
                                            "src:type/src:specifier[.='private']"
                                            " or "
                                            "("
                                                "not("
                                                    "src:type/src:specifier[.='protected' or .='public' or .='internal']"
                                                ")"
                                                " and "
                                                "parent::src:block[parent::src:class or parent::src:struct]"
                                            ")"
                                        "]"
                                    "]"
                                ")"
                            ")"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "self::src:decl_stmt"
                    " and "
                    "("
                        "src:decl/src:type/src:specifier[.='private']"
                        " or "
                        "("
                            "not("
                                "src:decl/src:type/src:specifier[.='public' or .='protected' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block[parent::src:class or parent::src:struct]"
                        ")"
                    ")"
                ")"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='Java']"
            " and "
            "("
                "("
                    "(self::src:decl_stmt)"
                    " and "
                    "("
                        "src:decl/src:type/src:specifier[.='private']"
                        " or "
                        "("
                            "not("
                                "src:decl/src:type/src:specifier[.='public' or .='protected' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block[self::src:class]"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:function or self::src:function_decl or self::src:constructor or self::src:constructor_decl)"
                    " and "
                    "("
                        "src:type/src:specifier[.='private']"
                        " or "
                        "("
                            "not("
                                "src:type/src:specifier[.='public' or .='protected' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block[parent::src:class]"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:class or self::src:class_decl or self::src:annotation_defn)"
                    " and "
                    "("
                        "src:specifier[.='private']"
                        " or "
                        "("
                            "not("
                                "src:specifier[.='public' or .='protected' or .='internal']"
                            ")"
                        ")"
                    ")"
                    " or "
                    "("
                        "self::src:interface/src:specifier[.='private']"
                    ")"
                ")"
            ")"
        ")"
    );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_protected",
        "("
            "ancestor::src:unit[@language='C++']"
            " and "
            "parent::src:protected"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C#']"
            " and "
            "("
                "("
                    "(self::src:class or self::src:struct or self::src:interface)"
                    " and "
                    "src:specifier[.='protected']"
                ")"
                " or "
                "("
                    "(self::src:destructor or self::src:destructor_decl or self::src:constructor or self::src:constructor_decl or self::src:function or self::src:function_decl or self::src:property or self::src:event)"
                    " and "
                    "("
                        "src:type/src:specifier[.='protected']"
                        " or "
                        "("
                            "(self::src:function or self::src:function_decl)"
                            " and "
                            "src:name[.='get' or .='set' or .='add' or .='remove']"
                            " and "
                            "("
                                "src:specifier[.='protected']"
                                " or "
                                "("
                                    "not("
                                        "src:specifier[.='private' or .='public' or .='internal']"
                                    ")"
                                    " and "
                                    "parent::src:block["
                                        "(parent::src:property | parent::src:event)["
                                            "src:type/src:specifier[.='protected']"
                                        "]"
                                    "]"
                                ")"
                            ")"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "self::src:decl_stmt"
                    " and "
                    "("
                        "src:decl/src:type/src:specifier[.='protected']"
                    ")"
                ")"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='Java']"
            " and "
            "("
                "("
                    "(self::src:decl_stmt)"
                    " and "
                    "("
                        "src:decl/src:type/src:specifier[.='protected']"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:function or self::src:function_decl or self::src:constructor or self::src:constructor_decl)"
                    " and "
                    "("
                        "src:type/src:specifier[.='protected']"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:class or self::src:class_decl or self::src:annotation_defn)"
                    " and "
                    "("
                        "src:specifier[.='protected']"
                    ")"
                    " or "
                    "("
                        "self::src:interface/src:specifier[.='protected']"
                    ")"
                ")"
            ")"
        ")"
    );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_public",
        "("
            "ancestor::src:unit[@language='C++']"
            " and "
            "parent::src:public"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C#']"
            " and "
            "("
                "("
                    "(self::src:class or self::src:struct or self::src:interface)"
                    " and "
                    "("
                        "src:specifier[.='public']"
                        " or "
                        "("
                            "not("
                                "src:specifier[.='protected' or .='private' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block/parent::src:interface"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:destructor or self::src:destructor_decl or self::src:constructor or self::src:constructor_decl or self::src:function or self::src:function_decl or self::src:property or self::src:event)"
                    " and "
                    "("
                        "src:type/src:specifier[.='public']"
                        " or "
                        "("
                            "not("
                                "src:type/src:specifier[.='protected' or .='private' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block/parent::src:interface"
                        ")"
                        " or "
                        "("
                            "(self::src:function or self::src:function_decl)"
                            " and "
                            "src:name[.='get' or .='set' or .='add' or .='remove']"
                            " and "
                            "("
                                "src:specifier[.='public']"
                                " or "
                                "("
                                    "not("
                                        "src:specifier[.='protected' or .='private' or .='internal']"
                                    ")"
                                    " and "
                                    "parent::src:block["
                                        "(parent::src:property | parent::src:event)["
                                            "src:type/src:specifier[.='public']"
                                            " or "
                                            "("
                                                "not("
                                                    "src:type/src:specifier[.='protected' or .='private' or .='internal']"
                                                ")"
                                                " and "
                                                "parent::src:block/parent::src:interface"
                                            ")"
                                        "]"
                                    "]"
                                ")"
                            ")"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "self::src:decl_stmt"
                    " and "
                    "("
                        "src:decl/src:type/src:specifier[.='public']"
                        " or "
                        "("
                            "not("
                                "src:decl/src:type/src:specifier[.='private' or .='protected' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block/parent::src:interface"
                        ")"
                    ")"
                ")"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='Java']"
            " and "
            "("
                "("
                    "(self::src:decl_stmt)"
                    " and "
                    "("
                        "src:decl/src:type/src:specifier[.='public']"
                        " or "
                        "("
                            "not("
                                "src:decl/src:type/src:specifier[.='private' or .='protected' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block/parent::src:interface"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:function or self::src:function_decl or self::src:constructor or self::src:constructor_decl)"
                    " and "
                    "("
                        "src:type/src:specifier[.='public']"
                        " or "
                        "("
                            "not("
                                "src:type/src:specifier[.='private' or .='protected' or .='internal']"
                            ")"
                            " and "
                            "parent::src:block/parent::src:interface"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:class or self::src:class_decl or self::src:annotation_defn)"
                    " and "
                    "("
                        "src:specifier[.='public']"
                    ")"
                    " or "
                    "("
                        "self::src:interface["
                            "not("
                                "src:specifier[.='private' or .='protected' or .='internal']"
                            ")"
                        "]"
                    ")"
                ")"
            ")"
        ")");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_internal",
            "ancestor::src:unit[@language='C#']"
            " and "
            "("
                "("
                    "(self::src:class or self::src:struct or self::src:interface)"
                    " and "
                    "("
                        "src:specifier[.='internal']"
                        " or "
                        "("
                            "not("
                                "src:specifier[.='protected' or .='private' or .='public']"
                            ")"
                            " and "
                            "not(parent::src:block[parent::src:interface or parent::src:class or parent::src:struct])"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:destructor or self::src:destructor_decl or self::src:constructor or self::src:constructor_decl or self::src:function or self::src:function_decl or self::src:property or self::src:event)"
                    " and "
                    "("
                        "src:type/src:specifier[.='internal']"
                        " or "
                        "("
                            "(self::src:function or self::src:function_decl)"
                            " and "
                            "src:name[.='get' or .='set' or .='add' or .='remove']"
                            " and "
                            "("
                                "src:specifier[.='internal']"
                                " or "
                                "("
                                    "not("
                                        "src:specifier[.='protected' or .='private' or .='public']"
                                    ")"
                                    " and "
                                    "parent::src:block["
                                        "(parent::src:property | parent::src:event)["
                                            "src:type/src:specifier[.='internal']"
                                        "]"
                                    "]"
                                ")"
                            ")"
                        ")"
                    ")"
                ")"
                " or "
                "("
                    "self::src:decl_stmt"
                    " and "
                    "("
                        "src:decl/src:type/src:specifier[.='internal']"
                    ")"
                ")"
            ")"

        );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_volatile", "( (self::src:type or self::src:function or self::src:function_decl or self::src:asm) and src:specifier[.='volatile'] ) or ( (self::src:decl or self::src:typedef) and src:type/src:specifier[.='volatile'] ) or ( (self::src:decl_stmt) and src:decl/src:type/src:specifier[.='volatile'] ) or ( self::src:using and src:init/src:type/src:expr/specifier[.='volatile'] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_const", "( (self::src:type or self::src:function or self::src:function_decl) and src:specifier[.='const'] ) or ( (self::src:decl or self::src:typedef) and src:type/src:specifier[.='const'] ) or ( (self::src:decl_stmt) and src:decl/src:type/src:specifier[.='const'] ) or ( self::src:using and src:init/src:type/src:expr/specifier[.='const'] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_inline", "src:type/src:specifier[.='inline'] or src:specifier[.='inline']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_restrict", "src:type/src:specifier[.='restrict'] or src:decl/src:type/src:specifier[.='restrict']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_register", "src:type/src:specifier[.='register'] or src:decl/src:type/src:specifier[.='register']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_mutable", "src:type/src:specifier[.='mutable'] or src:decl/src:type/src:specifier[.='mutable'] or src:specifier[.='mutable']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_thread_local", "src:type/src:specifier[.='thread_local'] or src:type/src:specifier[.='_Thread_local'] or src:decl/src:type/src:specifier[.='thread_local'] or src:decl/src:type/src:specifier[.='_Thread_local']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_constexpr", "src:type/src:specifier[.='constexpr']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_override", "src:specifier[.='override'] or src:type/src:specifier[.='override'] or src:type/src:annotation/src:name[.='Override']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_virtual",
        "("
            "ancestor::src:unit[@language='C++']"
            " and "
            "("
                "src:type/src:specifier[.='virtual']"
                " or src:specifier[.='virtual']"
                " or src:specifier[.='override']"
                " or src:specifier[.='final']"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C#']"
            " and "
            "("
                "("
                    "(self::src:function or self::src:function_decl or self::src:property or self::src:event)"
                    " and "
                    "("
                        "src:type/src:specifier[.='virtual']"
                        " or src:type/src:specifier[.='override']"
                        " or src:type/src:specifier[.='sealed']"
                        " or src:type/src:specifier[.='abstract']"
                        " or "
                        "("
                            "(self::src:function or self::src:function_decl)"
                            " and "
                            "src:name[ .='get' or .='set' or .='add' or .='remove']"
                            " and "
                            "("
                                "("
                                    "src:specifier[.='virtual']"
                                    " or src:specifier[.='override']"
                                    " or src:specifier[.='sealed']"
                                    " or src:specifier[.='abstract']"
                                ")"
                                " or "
                                "("
                                    "parent::src:block["
                                        "(parent::src:property | parent::src:event)["
                                            "parent::src:block/parent::src:interface"
                                            " or "
                                            "src:type/src:specifier[.='virtual']"
                                            " or src:type/src:specifier[.='override']"
                                            " or src:type/src:specifier[.='sealed']"
                                            " or src:type/src:specifier[.='abstract']"
                                        "]"
                                    "]"
                                ")"
                            ")"
                        ")"
                        " or "
                        "parent::src:block/parent::src:interface"
                    ")"
                ")"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='Java']"
            " and "
            "not(src:type/src:specifier[.='final'])"
            " and "
            "not(parent::src:block/parent::src:class/src:specifier[.='final'])"
        ")"
        );


    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_overridable",
        "("
            "ancestor::src:unit[@language='C++']"
            " and "
            "("
                "src:type/src:specifier[.='virtual']"
                " or src:specifier[.='virtual']"
                " or src:specifier[.='override']"
            ")"
            " and "
            "not(src:specifier[.='final'])"
            " and "
            "(parent::src:public | parent::src:private | parent::src:protected)/parent::src:block["
                "(parent::src:class | parent::src:struct | parent::src:union)["
                    "not(src:specifier[.='final'])"
                "]"
            "]"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C#']"
            " and "
            "("
                "("
                    "(self::src:function or self::src:function_decl or self::src:property or self::src:event)"
                    " and "
                    "("
                        "("
                            "("
                                "src:type/src:specifier[.='virtual']"
                                " or src:type/src:specifier[.='override']"
                                " or src:type/src:specifier[.='abstract']"
                            ")"
                            " and "
                            "not(src:type/src:specifier[.='sealed'] or src:specifier[.='sealed'])"
                            " and "
                            "parent::src:block["
                                "(parent::src:class | parent::src:struct)["
                                    "not(src:specifier[.='sealed'])"
                                "]"
                            "]"
                        ")"
                        " or "
                        "("
                            "(self::src:function or self::src:function_decl)"
                            " and "
                            "src:name[ .='get' or .='set' or .='add' or .='remove']"
                            " and "
                            "("
                                "("
                                    "("
                                        "src:specifier[.='virtual']"
                                        " or src:specifier[.='override']"
                                        " or src:specifier[.='abstract']"
                                    ")"
                                    " and "
                                    "not(src:specifier[.='sealed'])"
                                ")"
                                " or "
                                "("
                                    "parent::src:block["
                                        "(parent::src:property | parent::src:event)["
                                           "("
                                                "(src:type/src:specifier[.='virtual']"
                                                " or src:type/src:specifier[.='override']"
                                                " or src:type/src:specifier[.='abstract'])"
                                                " and "
                                                "not(src:type/src:specifier[.='sealed'] or src:specifier[.='sealed'])"
                                                " and "
                                                "parent::src:block[(parent::src:class | parent::src:struct)[not(src:specifier[.='sealed'])]]"
                                            ")"
                                            " or "
                                            "parent::src:block/parent::src:interface"
                                        "]"
                                    "]"
                                ")"
                            ")"
                        ")"
                    ")"
                ")"
                " or "
                "parent::src:block/parent::src:interface"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='Java']"
            " and "
            "not(src:type/src:specifier[.='final'])"
            " and "
            "not(parent::src:block/parent::src:class/src:specifier[.='final'])"
        ")"
    );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_auto_specifier", "(src:decl/src:type/src:specifier[.='auto']) or (src:type/src:specifier[.='auto'])");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_extern_block", "parent::src:extern");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_extern", "(parent::src:extern) or (src:type/src:specifier[.='extern']) or (src:decl/src:type/src:specifier[.='extern'])");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_deduced_type", "src:type/src:name[.='auto'] or src:decl/src:type/src:name[.='auto'] or src:type/src:name[.='var'] or src:decl/src:type/src:name[.='var']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_deleted", "src:specifier[.='delete']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_explicit", "src:type/src:specifier[.='explicit'] or src:specifier[.='explicit']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_bitfield", "(src:range or src:decl/src:range) and (./parent::src:decl_stmt or name()='decl_stmt')");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_member",
        "("
            "ancestor::src:unit[@language[.='C' or .='Java' or .='C#']]"
            " and "
            "("
                "parent::src:block["
                    "parent::src:enum"
                    " or parent::src:struct"
                    " or parent::src:union"
                    " or parent::src:class"
                    " or parent::src:interface"
                    " or parent::src:annotation_defn"
                "]"
                " or "
                "parent::src:decl_stmt["
                    "parent::src:block["
                        "parent::src:enum"
                        " or parent::src:struct"
                        " or parent::src:union"
                        " or parent::src:class"
                        " or parent::src:interface"
                        " or parent::src:annotation_defn"
                    "]"
                "]"
                " or "
                "("
                    "(self::src:function or self::src:function_decl)"
                    " and "
                    "parent::src:block["
                        "parent::src:property["
                            "parent::src:block["
                                "parent::src:enum"
                                " or parent::src:struct"
                                " or parent::src:union"
                                " or parent::src:class"
                                " or parent::src:interface"
                            "]"
                        "]"
                        " or "
                        "parent::src:event["
                            "parent::src:block["
                                "parent::src:enum"
                                " or parent::src:struct"
                                " or parent::src:union"
                                " or parent::src:class"
                                " or parent::src:interface"
                            "]"
                        "]"
                    "]"
                ")"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C++']"
            " and "
            "("
                "parent::src:public"
                " or parent::src:protected"
                " or parent::src:private"
                " or parent::src:decl_stmt["
                    "parent::src:public"
                    " or parent::src:protected"
                    " or parent::src:private"
                "]"
                " or parent::src:block[parent::src:enum]"
            ")"
        ")"
    );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_default", "src:specifier[.='default'] | src:type/src:specifier[.='default']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_pointer", "(self::src:decl and src:type/src:modifier[.='*']) or ((self::src:decl_stmt or self::src:param) and src:decl/src:type/src:modifier[.='*']) or (self::src:argument and src:expr/src:modifier[.='*']) or ((self::src:type or self::src:name)and src:modifier[.='*']) or (self::src:function_decl and src:modifier[.='*'])");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_parameter_pack", "(self::src:decl and src:type/src:modifier[.='...']) or (self::src:param and ( src:decl/src:type/src:modifier[.='...'] or src:type/src:modifier[.='...'] ) ) or (self::src:argument and src:expr/src:modifier[.='...']) or (self::src:type and src:modifier[.='...'])");

/*    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_generic",
        "("
            "ancestor::src:unit[@language='C#']"
            " and "
            "("
                "("
                    "self::src:class"
                    " or self::src:interface"
                    " or self::src:struct"
                    " or self::src:function"
                    " or self::src:function_decl"
                    " or self::src:constructor"
                    " or self::src:constructor_decl"
                    " or self::src:delegate"
                ")"
                " and "
                "("
                    "src:name[last()]/src:argument_list[@type='template']"
                    " or "
                    "src:name/src:name[last()]/src:argument_list[@type='template']"
                ")"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C++']"
            " and "
            "src:template"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='Java']"
            " and "
            "("
                "("
                    "(self::src:class or self::src:interface or self::src:annotation_defn)"
                    " and "
                    "("
                        "src:name[last()]/src:argument_list[@type='template']"
                        " or "
                        "src:name/src:name[last()]/src:argument_list[@type='template']"
                    ")"
                ")"
                " or "
                "("
                    "(self::src:function or self::src:function_decl)"
                    " and "
                    "src:type/src:argument_list[@type='template']"
                ")"
            ")"
        ")"
    );
*/
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_aligned", "(src:type/src:alignas or src:alignas or src:decl/src:type/src:alignas)");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_noexcept", "(src:noexcept or src:throw/src:argument_list[count(src:argument) = 0])");

    // xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_atomic", "( src:name[ src:name[.='std']/following-sibling::src:operator[.='::']/following-sibling::src:name[ src:name[.='atomic']/following-sibling::src:argument_list[@type='template'][count(src:argument) = 1] ] ] or src:name[ src:name[.='atomic']/following-sibling::src:argument_list[@type='template'][count(src:argument) = 1] ] or src:type[ src:atomic or src:name[ src:name[.='std']/following-sibling::src:operator[.='::']/following-sibling::src:name[ src:name[.='atomic']/following-sibling::src:argument_list[@type='template'][count(src:argument) = 1] ] ] or src:name[ src:name[.='atomic']/following-sibling::src:argument_list[@type='template'][count(src:argument) = 1] ] ] or src:atomic or src:decl/src:type[ src:atomic or src:name[ src:name[.='std']/following-sibling::src:operator[.='::']/following-sibling::src:name[ src:name[.='atomic']/following-sibling::src:argument_list[@type='template'][count(src:argument) = 1] ] ] or src:name[ src:name[.='atomic']/following-sibling::src:argument_list[@type='template'][count(src:argument) = 1] ] ] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_reference", "( ancestor::src:unit[@language='C#'] and ( src:decl/src:type/src:modifier[.='ref'] or src:type/src:modifier[.='ref'] or src:modifier[.='ref'] ) ) or ( ancestor::src:unit[@language='C++'] and src:decl/src:type/src:modifier[.='&'] or src:type/src:modifier[.='&'] or src:modifier[.='&'] or src:expr/src:modifier[.='&'] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_rvalue_reference", "( src:decl/src:type/src:modifier[.='&&'] or src:type/src:modifier[.='&&'] or src:modifier[.='&&'] or src:expr/src:modifier[.='&&'] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_noreturn", "( src:type/src:specifier[.='_Noreturn'] or src:type/src:attribute/src:expr/src:name[.='noreturn'] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_readonly", "(self::src:type and src:specifier[.='readonly']) or (self::src:decl and src:type/src:specifier[.='readonly']) or (self::src:decl_stmt and src:decl/src:type/src:specifier[.='readonly'])");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_protected_internal", "( src:specifier[.='protected']/following-sibling::src:specifier[.='internal'] or src:type/src:specifier[.='protected']/following-sibling::src:specifier[.='internal'] or src:decl/src:type/src:specifier[.='protected']/following-sibling::src:specifier[.='internal'] ) or ( not( src:specifier[.='public'] or src:type/src:specifier[.='public'] or src:decl/src:type/src:specifier[.='public'] or src:specifier[.='private'] or src:type/src:specifier[.='private'] or src:decl/src:type/src:specifier[.='private'] or src:specifier[.='protected'] or src:type/src:specifier[.='protected'] or src:decl/src:type/src:specifier[.='protected'] or src:specifier[.='internal'] or src:type/src:specifier[.='internal'] or src:decl/src:type/src:specifier[.='internal'] ) and ( ancestor::node()[self::src:property or self::src:event or self::src:class or self::src:struct or self::src:unit][1] [ (self::src:property or self::src:event) and src:type/src:specifier[.='protected']/following-sibling::src:specifier[.='internal'] ] ) )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_out_param", "src:modifier[.='out'] or src:specifier[.='out'] or src:type/src:modifier[.='out'] or src:decl/src:type/src:modifier[.='out']");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_out_argument", "src:modifier[.='out'] or src:specifier[.='out'] or src:type/src:modifier[.='out'] or src:decl/src:type/src:modifier[.='out']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_variadic", "( ancestor::src:unit[@language='C#'] and ( src:parameter_list/src:param/src:decl/src:type/src:specifier[.='params'] or self::src:parameter_list/src:param/src:decl/src:type/src:specifier[.='params'] ) ) or ( ancestor::src:unit[@language='C++' or @language='C' or @language='Java'] and ( ( ( src:parameter_list/src:param[ src:type/src:modifier[.='...'] or src:decl/src:type/src:modifier[.='...'] ] or self::src:parameter_list/src:param[ src:type/src:modifier[.='...'] or src:decl/src:type/src:modifier[.='...'] ] ) ) or self::src:template[ not( parent::function or parent::function_decl or parent::constructor or parent::constructor_decl or parent::destructor or parent::destructor_decl ) ]/src:parameter_list/src:param[ src:type/src:modifier[.='...'] or src:decl/src:type/src:modifier[.='...'] ] or ( ( self::src:using or self::src:class or self::src:class_decl or self::src:struct or self::src:struct_decl or self::src:union or self::src:union_decl ) and ( ( count(src:template) = 1 and src:template/src:parameter_list/src:param[ src:type/src:modifier[.='...'] or src:decl/src:type/src:modifier[.='...'] ] ) ) ) ) )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_new_override", "src:type/src:specifier[.='new']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_sealed",
        "("
            "src:specifier[.='sealed']"
            " or "
            "src:type/src:specifier[.='sealed']"
            " or "
            "("
                "parent::src:block["
                    "(parent::src:class | parent::src:struct)[src:specifier[.='sealed']]"
                    " or "
                    "(parent::src:event | parent::src:property)[src:type/src:specifier[.='sealed']]"
                "]"
            ")"
        ")");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_async", "src:type/src:specifier[.='async']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_implicit", "src:type/src:specifier[.='implicit']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_yield", "src:specifier[.='yield']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_extension_method", "src:parameter_list/src:param[1]/src:decl/src:type/src:specifier[.='this']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_main", "( ancestor::src:unit[@language='C#' or @language='Java'] and src:type/src:specifier[.='static'] and ( src:name[.='main'] or src:name[.='Main'] ) ) or ( ancestor::src:unit[@language='C++' or @language='C'] and parent::src:unit and ( src:name[.='main'] or src:name[.='_tmain'] or src:name[.='_Tmain'] ) )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_extern_c", "ancestor::src:extern[src:literal[.='\"C\"' or .='\"c\"' ]]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_global", "parent::src:unit or ( not( ancestor::src:block[ not( parent::src:extern or parent::src:namespace[ not( src:name ) ] ) ] ) )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_operator", "@type='operator'");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_ref_qualifier", "src:ref_qualifier");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_trailing_return_type", "src:parameter_list/following-sibling::src:type");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_empty_return", "not(src:expr)");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_attributes", "src:attribute or src:annotation[.!='@interface'] or src:type/src:attribute or src:type/src:annotation or src:decl/src:type/src:attribute or src:decl/src:type/src:annotation");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_annotations", "src:attribute or src:annotation[.!='@interface'] or src:type/src:attribute or src:type/src:annotation or src:decl/src:type/src:attribute or src:decl/src:type/src:annotation");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_function_try", "src:try");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_KnR_C", "src:decl_stmt");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_template_parameter", "parent::src:parameter_list/parent::src:template or parent::src:argument_list[@type='template']");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_generic_parameter", "parent::src:parameter_list/parent::src:template or parent::src:argument_list[@type='template']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_template_parameter_pack", "src:type/src:modifier[.='...']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_template_template_parameter", "src:type/src:template");

    char const* const is_tempalateAndis_generic_impl =
    "("
        "ancestor::src:unit[@language='C++']"
        " and "
        "("
            "self::src:class or self::src:class_decl or self::src:struct or self::src:struct_decl or self::src:union or self::src:union_decl or self::src:enum or self::src:using or self::src:function or self::src:function_decl or self::src:constructor or self::src:constructor_decl or self::src:destructor or self::src:destructor_decl "
        ")"
        " and "
        "src:template "
    ")"
    " or "
    "("
        "self::src:name"
        " and "
        "(src:argument_list[@type='template'] or src:name/src:argument_list[@type='template'] or src:name/src:name/src:argument_list[@type='template'])"
    ")"
    " or "
    "("
        "(self::src:decl_stmt or self::src:param)"
        " and "
        "src:decl/src:type/src:name["
            " src:argument_list[@type='template'] or src:name/src:argument_list[@type='template'] or src:name/src:name/src:argument_list[@type='template'] "
        "]"
    ")"
    " or "
    "("
        "("
            "self::src:decl or self::src:argument/src:expr[count(*)=1])"
            " and "
            "src:type/src:name["
                "src:argument_list[@type='template']"
                " or src:name/src:argument_list[@type='template']"
                " or src:name/src:name/src:argument_list[@type='template'] "
            "]"
        ")"
    " or "
    "("
        "("
            "self::src:type "
            "or self::src:call "
            "or self::src:argument/src:expr[count(*)=1]"
        ")"
        " and "
        "src:name["
            "src:argument_list[@type='template']"
            " or src:name/src:argument_list[@type='template']"
            " or src:name/src:name/src:argument_list[@type='template'] "
        "]"
    ")"
    " or "
    "("
        "ancestor::src:unit[@language='Java']"
        " and "
        "("
            "("
                "(self::src:class or self::src:interface or self::src:annotation_defn)"
                " and "
                "("
                    "src:name[last()]/src:argument_list[@type='template']"
                    " or "
                    "src:name/src:name[last()]/src:argument_list[@type='template']"
                ")"
            ")"
            " or "
            "("
                "(self::src:function or self::src:function_decl)"
                " and "
                "src:type/src:argument_list[@type='template']"
            ")"
        ")"
    ")"
    " or "
    "("
        "ancestor::src:unit[@language='C#']"
        " and "
        "("
            "("
                "(self::src:class or self::src:class_decl or self::src:struct or self::src:struct_decl)"
                " and "
                "("
                    "src:name/src:argument_list[@type='template'] "
                    "or src:name/src:name/src:argument_list[@type='template']"
                ")"
            ")"
            " or "
            "("
                "(self::src:function or self::src:function_decl or self::src:constructor or self::src:constructor_decl or self::src:destructor or self::src:destructor_decl)"
                " and "
                "(src:name/src:argument_list[@type='template'] or src:name/src:name/src:argument_list[@type='template'])"
            ")"
        ")"
    ")";

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_template", is_tempalateAndis_generic_impl);
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_generic", is_tempalateAndis_generic_impl);

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_default_parameter", "src:decl/src:init or src:init");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_in_param", "src:specifier[.='in']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_constrained", 
        "("
            "ancestor::src:unit[@language='Java']"
             " and "
            "("
                "("
                    "(self::src:function or self::src:function_decl)"
                    "  and "
                    "("
                        "src:type/src:argument_list[@type='template']/src:argument[src:extends or src:super]"
                    ")"
                ")"
                " or "
                "("
                    "self::src:argument"
                    "  and "
                    "("
                        "src:extends"
                        "  or "
                        "src:super"
                    ")"
                ")"
                " or  "
                "("
                    "(self::src:type or self::src:class or self::src:class_decl)"
                    "  and "
                    "("
                        "src:name/src:argument_list[@type='template']/src:argument[src:extends or src:super]"
                        "  or "
                        "src:name/src:name/src:argument_list[@type='template']/src:argument[src:extends or src:super]"
                    ")"
                ")"
                " or  "
                "("
                    "(self::src:decl)"
                    "  and "
                    "("
                        "src:type/src:name/src:argument_list[@type='template']/src:argument[src:extends or src:super]"
                        " or "
                        "src:type/src:name/src:name/src:argument_list[@type='template']/src:argument[src:extends or src:super]"
                    ")"
                ")"
                " or  "
                "("
                    "(self::src:param)"
                    "  and "
                    "("
                        "src:decl/src:type/src:name/src:argument_list[@type='template']/src:argument[src:extends or src:super]"
                        "  or "
                        "src:decl/src:type/src:name/src:name/src:argument_list[@type='template']/src:argument[src:extends or src:super]"
                    ")"
                ")"
                " or "
                "("
                    "self::src:argument_list[@type='template']"
                    "  and "
                    "src:argument[src:extends or src:super]"
                ")"
                " or "
                "("
                    "self::src:argument_list[@type='template']"
                    "  and "
                    "src:argument[src:extends or src:super]"
                ")"
            ")"
        ")"
        " or "
        "("
            "ancestor::src:unit[@language='C#']"
            "  and "
            "src:where"
        ")"
    );

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_bound", "self::src:argument/ancestor::src:argument_list[@type='template'] and (src:extends or src:super)");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_unbound", "self::src:argument/ancestor::src:argument_list[@type='template'] and not(src:extends or src:super)");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_upper_bound", "self::src:argument/ancestor::src:argument_list[@type='template'] and src:extends");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_wildcard", "self::src:argument/ancestor::src:argument_list[@type='template'] and src:name[.='?']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_lower_bound", "self::src:argument/ancestor::src:argument_list[@type='template'] and src:super");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_anonymous", "( self::src:param[ src:decl[not(src:name)] or src:type[not(following-sibling::src:name)] ] or self::src:decl[not(src:name)] or self::src:argument/src:name[.='?'] or ( (self::src:enum or self::src:struct or self::src:union or self::src:namespace or self::src:class) and not(src:name) ) )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "enum_specifies_integer_type", "self::src:enum[src:super or src:type]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_complete", "( ( ( self::src:struct or self::src:union or self::src:class or self::src:function or self::src:constructor or self::src:destructor or self::src:property or self::src:event ) and not(src:specifier[.='partial']) ) or ( self::src:enum and src:block ) )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "enum_is_scoped", "( self::src:enum[ @type='class' or parent::src:public or parent::src:private or parent::src:protected ] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_using_access_decl", "self::src:using/src:name/src:name");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_type_alias", "self::src:using/src:init");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_using_namespace", "self::src:using/src:namespace");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_initializers", "self::src:constructor/src:member_list");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_converting_constructor", "self::src:constructor [ not(src:specifier[.='explicit']) and src:parameter_list[ ( count(src:param) = 1 ) or ( count(src:param) = count(src:param[src:decl/src:init]) ) or ( ( count(src:param) - 1) = count(src:param[src:decl/src:init]) ) ] ]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_local_storage", "( self::src:param ) or ( self::src:decl_stmt/src:decl/src:type[ not(src:specifier[.='static']) and not(src:specifier[.='extern']) ] ) or ( self::src:decl/src:type[ not(src:specifier[.='static']) and not(src:specifier[.='extern']) ]/parent::node()[parent::src:param or parent::src:decl_stmt] ) or ( self::src:type[ not(src:specifier[.='static']) and not(src:specifier[.='extern']) ]/parent::node()[ self::src:decl[parent::src:param or parent::src:decl_stmt] ] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_scoped_to_file", "(src:decl/src:type/src:specifier[.='static'] and not(ancestor::src:class or ancestor::src:struct or ancestor::src:union or ancestor::src:enum)) or (src:type/src:specifier[.='static'] and not(ancestor::src:class or ancestor::src:struct or ancestor::src:union or ancestor::src:enum)) or ancestor::src:namespace[not(src:name)]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_single_decl", "count(src:decl) = 1");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_multi_decl", "count(src:decl) > 1");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_array", "src:name/src:index or src:index or src:decl/src:name/src:index or src:decl/src:type/src:index or src:type/src:name/src:index or src:decl/src:type/src:name/src:index or src:type/src:index or self::src:argument[parent::src:argument_list[@type='template']]/src:expr/src:name/src:index");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_event_function", "parent::src:block/parent::src:event");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_property_function", "parent::src:block/parent::src:property");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "defined_event_function", "self::src:event/src:block[src:function or src:function_decl]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_add", "self::src:event/src:block[src:function/src:name[.='add'] or src:function_decl/src:name[.='add']]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_remove", "self::src:event/src:block[src:function/src:name[.='remove'] or src:function_decl/src:name[.='remove']]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "defined_property_function", "self::src:property/src:block[src:function or src:function_decl]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_get", "self::src:property/src:block[src:function/src:name[.='get'] or src:function_decl/src:name[.='get']]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_set", "self::src:property/src:block[src:function/src:name[.='set'] or src:function_decl/src:name[.='set']]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_indexer", "self::src:property/src:parameter_list");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_closure", "self::src:delegate or self::src:lambda");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_capture_all_by_value", "self::src:lambda/src:capture[ count(src:argument) = 1 and src:argument[count(*) = 1]/src:modifier[.='='] ] or self::src:capture[ count(src:argument) = 1 and src:argument[count(*) = 1]/src:modifier[.='='] ] or self::src:argument[count(*) = 1]/src:modifier[.='=']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_capture_all_by_reference", "self::src:lambda/src:capture[ count(src:argument) = 1 and src:argument[count(*) = 1]/src:modifier[.='&'] ] or self::src:capture[ count(src:argument) = 1 and src:argument[count(*) = 1]/src:modifier[.='&'] ] or self::src:argument[count(*) = 1]/src:modifier[.='&']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "captures_named_variable", "self::src:lambda/src:capture/src:argument/src:name or self::src:capture/src:argument/src:name or self::src:argument/src:name");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_function_try", "parent::src:constructor or parent::src:destructor or parent::src:function");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_finally", "self::src:try/src:finally or self::src:catch/following-sibling::src:finally");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_resource_try", "self::src:try/src:init");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_indexing_expr", "src:name/src:index or src:expr/src:name/src:index or src:init/src:expr/src:name/src:index or src:decl/src:init/src:expr/src:name/src:index");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_call", "src:call or src:expr/src:call or src:init/src:expr/src:call or src:decl/src:init/src:expr/src:call");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_member_operator", "self::src:operator[.='::' or .='.' or .='->' or .='.*' or .='->*']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_global_scope_operator", "( ancestor::src:unit[@language='C++'] and count(preceding-sibling::*) = 0 and self::src:operator[.='::'] )");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_pointer_member_operator", "self::src:operator[.='->' or .='->*']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_bitwise_operator", "self::src:operator[ .='>>' or .='<<' or .='^' or .='~' or .='&' or .='|' or .='>>=' or .='<<=' or .='^=' or .='&=' or .='|=' or .='>>>' or .='>>>=' ]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_arithmetic_operator", "self::src:operator[ .='+' or .='-' or .='*' or .='/' or .='%' or .='+=' or .='-=' or .='*=' or .='/=' or .='%=' ]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_multiplicative_operator", "self::src:operator[ .='*' or .='*=']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_divisible_operator", "self::src:operator[.='/' or .='/=']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_additive_operator", "self::src:operator[.='+' or .='+=']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_subtraction_operator", "self::src:operator[.='-' or .='-=']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_shift_operator", "self::src:operator[ .='>>' or .='<<' or .='>>=' or .='<<=' or .='>>>' or .='>>>=' ]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_relational_operator", "self::src:operator[ .='>' or .='<' or .='>=' or .='<=' or .='==' or .='!=' ]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_equality_operator", "self::src:operator[.='!=' or .='==']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_logical_operator", "self::src:operator[ .='!' or .='&&' or .='||' ]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_assignment_operator", "self::src:operator[ .='=' or .='&=' or .='>>=' or .='<<=' or .='|=' or .='^=' or .='+=' or .='-=' or .='*=' or .='/=' or .='%=' or .='>>>=' ]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_compound_assignment", "self::src:expr_stmt/src:expr[count(src:operator[ .='=' or .='&=' or .='>>=' or .='<<=' or .='|=' or .='^=' or .='+=' or .='-=' or .='*=' or .='/=' or .='%=' or .='>>>=' ]) > 1] or self::src:expr[count(src:operator[ .='=' or .='&=' or .='>>=' or .='<<=' or .='|=' or .='^=' or .='+=' or .='-=' or .='*=' or .='/=' or .='%=' or .='>>>=' ]) > 1]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_shift_assign_operator", "self::src:operator[.='>>=' or .='<<=' or .='>>>=']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_unsigned_shift_operator", "self::src:operator[.='>>>=' or .='>>>']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_ranged_for", "self::src:foreach or self::src:for/src:control/src:init/src:decl/src:range");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "has_default_case", "self::src:switch/src:block/src:default");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_continue_to_label", "self::src:continue/src:name");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_break_to_label", "self::src:break/src:name");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_goto_case", "self::src:goto/src:name[contains(., 'case')]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_rethrow", "self::src:throw[not(*)]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_nullable", "src:decl/src:type/src:modifier[.='?'] or src:type/src:modifier[.='?'] or src:modifier[.='?']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_type_definition", "self::src:interface or self::src:class or self::src:union or self::src:struct or self::src:typedef or self::src:using[src:init] or self::src:enum");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_class", "self::src:class[not(@type)] or self::src:class_decl[not(@type)]");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_enum", "self::src:enum");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_enum_class", "self::src:enum[@type='class']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_struct", "self::src:struct or self::src:struct_decl");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_union", "self::src:union or self::src:union_decl");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_partial", "src:specifier[.='partial']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_delegate_type", "self::src:function_decl/src:type/src:specifier[.='delegate']");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_interface", "self::src:interface");

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "is_annotation", "self::src:annotation_defn");

    // xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "ref_qualifiers", "src:ref_qualifier");

    // xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "return_type", "src:type/*[ not( self::src:specifier[ .='static' or .='extern' or .='public' or .='internal' or .='protected' or .='private' or .='unsafe' or .='synchronized' or .='partial' or .='inline' or .='constexpr' or .='override' or .='friend' or .='virtual' or .='explicit' or .='implicit' or .='async' or .='new' or .='sealed' or .='final' or .='native' or .='abstract' or .='default' ] or self::src:annotation or self::src:attribute ) ]");

    // register all the xpath extension functions
    for (std::vector<struct xpath_ext_function>::size_type i = 0; i < MACROS.size(); ++i) {

        xmlXPathRegisterFuncNS(context, BAD_CAST MACROS[i].name.c_str(),
                               BAD_CAST MACROS[i].prefix.c_str(),
                               srcMacrosFunction);
    }

    xmlXPathRegisterFuncNS(context, (const xmlChar *)"in",
                           BAD_CAST SRCML_SRC_NS_URI,
                           srcInFunction);
}

/**
 * xsltsrcMLRegister
 *
 * Register srcML XSLT extension functions.
 */
void xsltsrcMLRegister () {

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    typedef void * __attribute__ ((__may_alias__)) VOIDPTR;
    typedef int (*xsltRegisterExtModuleFunction_function) (const xmlChar *, const xmlChar *, xmlXPathFunction);
    void* handle = dlopen("libexslt.so", RTLD_LAZY);
    if (!handle) {
        handle = dlopen("libexslt.so.0", RTLD_LAZY);
        if (!handle) {
            handle = dlopen("libexslt.dylib", RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "Unable to open libexslt library\n");
                return;
            }
        }
    }

    dlerror();
    xsltRegisterExtModuleFunction_function xsltRegisterExtModuleFunction;
    *(VOIDPTR *)(&xsltRegisterExtModuleFunction) = dlsym(handle, "xsltRegisterExtModuleFunction");
    char* error;
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return;
    }
#endif

    xsltRegisterExtModuleFunction(BAD_CAST "unit",
                                  BAD_CAST SRCML_SRC_NS_URI,
                                  srcContextFunction);

    xsltRegisterExtModuleFunction(BAD_CAST "archive",
                                  BAD_CAST SRCML_SRC_NS_URI,
                                  srcRootFunction);

    xsltRegisterExtModuleFunction(BAD_CAST "powerset",
                                  BAD_CAST SRCML_SRC_NS_URI,
                                  srcPowersetFunction);

    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "statement", "/src:unit//node()[self::src:while or self::src:if or self::src:return or self::src:for]");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "statement_node", "[self::src:while or self::src:if or self::src:return or self::src:for]");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "if", "/src:unit//src:if");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "while", "/src:unit//src:while");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "nestedwhile", ".//src:while//src:while");
    xpathRegisterExtensionFunction(SRCML_SRC_NS_URI, "returntype", "/src:unit//src:function/src:type");

    // srcdiff containing functions
    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hascommon",
                                   "self::*[not(descendant::diff:*) or descendant::diff:common]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hasinsert",
                                   "descendant::diff:insert[1]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hasdelete",
                                   "descendant::diff:delete[1]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "hasdifference",
                                   "descendant::diff:*[self::diff:insert or self::diff:delete]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "haschange",
                                   "descendant::diff:insert[following-sibling::node()[1][self::diff:delete] or preceding-sibling::node()[1][self::diff:delete]][1]");

    // srcdiff includes functions
    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "common",
                                   "not(ancestor::diff:*[1][self::diff:insert or self::diff:delete])");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "inserted",
                                   "ancestor::diff:*[1][self::diff:insert]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "deleted",
                                   "ancestor::diff:*[1][self::diff:delete]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "difference",
                                   "ancestor::diff:*[1][self::diff:insert or self::diff:delete]");

    xpathRegisterExtensionFunction(SRCML_DIFF_NS_URI, "changed",
                                   "ancestor::diff:*[1][self::diff:insert[following-sibling::node()[1][self::diff:delete] or preceding-sibling::node()[1][self::diff:delete]] or self::diff:delete[following-sibling::node()[1][self::diff:insert] or preceding-sibling::node()[1][self::diff:insert]]]");


    // register all the xpath extension functions
    for (std::vector<struct xpath_ext_function>::size_type i = 0; i < MACROS.size(); ++i) {

        xsltRegisterExtModuleFunction(BAD_CAST MACROS[i].name.c_str(),
                                      BAD_CAST MACROS[i].prefix.c_str(),
                                      srcMacrosFunction);
    }

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    dlclose(handle);
#endif
}

/**
 * xpathRegisterExtensionFunction
 * @param prefix a prefix for extension function
 * @param name a name for extension function
 * @param xpath the xpath expression
 *
 * Save extension functions to MACROS.
 */
void xpathRegisterExtensionFunction(const std::string& prefix, const std::string & name, const std::string & xpath) {

    xpath_ext_function xpath_function = {prefix, name, xpath};

    MACROS.push_back(xpath_function);
}

/**
 * getXPathExtensionFunctions
 *
 * @returns the registered XPath extension function in MACROS.
 */
const std::vector<xpath_ext_function> getXPathExtensionFunctions() {

    return MACROS;
}
