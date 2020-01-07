/**
 * @file xsltTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2019 srcML, LLC. (www.srcML.org)
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

 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <xsltTransformation.hpp>

#include <libxml/parser.h>
#include <libxml/parserInternals.h>

#include <srcml_sax2_utilities.hpp>

#ifdef DLLOAD
#include <dlfcn.h>
#else
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/xsltutils.h>
#include <libxslt/transform.h>
#include <libexslt/exslt.h>
#endif

#ifdef _MSC_BUILD
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

#ifdef DLLOAD

    libxslt_handle = dlopen_libxslt();
    if (!libxslt_handle) {
        fprintf(stderr, "Unable to open libxslt library\n");
        throw;
    }

    char* error;

    dlerror();
    *(void**)(&xsltApplyStylesheetUser) = dlsym(libxslt_handle, "xsltApplyStylesheetUser");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    *(void**)(&xsltParseStylesheetDoc) = dlsym(libxslt_handle, "xsltParseStylesheetDoc");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    *(void**)(&xsltCleanupGlobals) = dlsym(libxslt_handle, "xsltCleanupGlobals");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    *(void**)(&xsltFreeStylesheet) = dlsym(libxslt_handle, "xsltFreeStylesheet");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    libexslt_handle = dlopen_libexslt();

    // allow for all exslt functions
    typedef void (*exsltRegisterAll_t)();
    dlerror();
    exsltRegisterAll_t exsltRegisterAll;
    *(void **) (&exsltRegisterAll) = dlsym(libexslt_handle, "exsltRegisterAll");
    if ((error = dlerror()) != NULL) {
        dlclose(libexslt_handle);
        throw;
    }
#endif

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

#ifdef DLLOAD
    dlclose(libxslt_handle);
    dlclose(libexslt_handle);
#endif
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
        cparams[i] = xsl_parameters[i].c_str();
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
