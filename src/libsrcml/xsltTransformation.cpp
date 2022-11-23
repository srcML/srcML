// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file xsltTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#include <xsltTransformation.hpp>

#include <libxml/parser.h>
#include <libxml/parserInternals.h>

#include <srcml_sax2_utilities.hpp>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libxslt/transform.h>
#include <libexslt/exslt.h>

#ifdef _MSC_VER
#include <io.h>
#endif

/**
 * xsltTransformation
 * @param options list of srcML options
 * @param stylesheet an XSLT stylesheet
 * @param params XSLT parameters
 *
 * Constructor.  Dynamically loads XSLT functions.
 */
xsltTransformation::xsltTransformation(/* OPTION_TYPE& options, */ xmlDocPtr xslt, const std::vector<std::string>& params)
        : params(params) {

    // allow for all exslt functions
    exsltRegisterAll();

    // parse the stylesheet
    stylesheet = xsltParseStylesheetDoc(xslt);
    if (!stylesheet)
        throw;
}

/**
 * ~xsltTransformation
 *
 * Destructor.  Closes dynamically loaded library.
 */
xsltTransformation::~xsltTransformation() {

    xsltFreeStylesheet(stylesheet);

    xsltCleanupGlobals();
}

/**
 * apply
 *
 * Apply XSLT program, writing results.
 *
 * @returns true on success false on failure.
 */
TransformationResult xsltTransformation::apply(xmlDocPtr doc, int /* position */) const {

    // convert to c-array of c-strings, null terminated
    std::vector<const char*> cparams(xsl_parameters.size() + 1);
    for (size_t i = 0; i < xsl_parameters.size(); ++i) {
        cparams[i] = xsl_parameters[i].data();
    }
    cparams.back() = 0;

    // apply the style sheet to the document, which is the individual unit
    std::shared_ptr<xmlDoc> res(xsltApplyStylesheetUser(stylesheet, doc, cparams.data(), 0, 0, 0), [](xmlDoc* doc) { xmlFreeDoc(doc); });
    if (!res) {
        fprintf(stderr, "libsrcml:  Error in applying stylesheet\n");

        return TransformationResult();
    }
    // empty (but valid) result
    if (!res->children) {
        return TransformationResult();
    }

    // transformation result is nodeset with single unit, and the unit is wrapped
    TransformationResult result(xmlXPathNodeSetCreate(res->children), true);
    result.doc = res;
    result.nodeType = res->children->type == 1 ? SRCML_RESULT_UNITS : SRCML_RESULT_STRING;

    // transformation may produce more than one unit, but not be well-formed due to lack of root element
    // so create a nodeset of those parts, and change the type since libxml2 has this as text
    if (res->children->next && res->children->next->type == 1) {

        // libxml2 types this as text since there is no root element
        result.nodeType = SRCML_RESULT_UNITS;

        // remove the res->children since it may not be a unit
        xmlXPathNodeSetRemove(result.nodeset.get(), 0);

        // add each unit to the nodeset
        for (auto p = res->children; p; p = p->next) {
            if (p->type != 1)
                continue;
            xmlXPathNodeSetMerge(result.nodeset.get(), xmlXPathNodeSetCreate(p));
        }
    }

    // text result
    if (result.nodeType == SRCML_RESULT_STRING) {
        auto str = (char*) xmlNodeListGetRawString(res.get(), res->children, 0);
        result.stringValue = str;
        xmlFree(str);
    }
    return result;
}
