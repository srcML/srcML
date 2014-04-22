/**
 * @file srcexfun.cpp
 *
 * @copyright Copyright (C) 2009-2014 SDML (www.srcML.org)
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
  static xmlChar* unit_directory = 0;
  static xmlChar* unit_filename = 0;
*/

#include <libxml/tree.h>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#else
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>
#endif

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

    if (ret) {
        valuePush(ctxt, ret);
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
