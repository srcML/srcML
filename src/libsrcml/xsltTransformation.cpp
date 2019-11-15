/**
 * @file xsltTransformation.cpp
 *
 * @copyright Copyright (C) 2008-2014 srcML, LLC. (www.srcML.org)
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

    xmlInitParser();

#ifdef DLLOAD
    libxslt_handle = dlopen_libxslt();
    if (!libxslt_handle) {
        fprintf(stderr, "Unable to open libxslt library\n");
        return;
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
    dlexsltRegisterAll(libexslt_handle);
#endif

    // parse the stylesheet
    stylesheet = xsltParseStylesheetDoc(xslt);
    if (!stylesheet)
        throw;

//    xsltsrcMLRegister();
}

/**
 * ~xsltTransformation
 *
 * Destructor.  Closes dynamically loaded library.
 */
xsltTransformation::~xsltTransformation() {

    // @todo Memory leak. Crashes when more than one transformation
//    xsltFreeStylesheet(stylesheet);
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

    // position passed to XSLT program
//    setPosition(position);

    // convert to c-array of c-strings, null terminated
    // @todo Extract Function
    std::vector<const char*> cparams(xsl_parameters.size() + 1);
    for (size_t i = 0; i < xsl_parameters.size(); ++i) {
        cparams[i] = xsl_parameters[i].c_str();
    }
    cparams.back() = 0;

    // apply the style sheet to the document, which is the individual unit
    xmlDocPtr res = xsltApplyStylesheetUser(stylesheet, doc, cparams.data(), 0, 0, 0);
    if (!res) {
        fprintf(stderr, "libsrcml:  Error in applying stylesheet\n");

        return TransformationResult();
    }

    // transformation result is nodeset with single unit, and the unit is wrapped
    return TransformationResult(xmlXPathNodeSetCreate(res->children), true);
}

/**
 * dlexsltRegisterAll
 *
 * Allow for all exslt functions by dynamic load
 * of exslt library.
 */
void dlexsltRegisterAll(void* handle) {

#ifdef DLLOAD
    typedef void (*exsltRegisterAll_t)();

    dlerror();
    exsltRegisterAll_t exsltRegisterAll;
    *(void **) (&exsltRegisterAll) = dlsym(handle, "exsltRegisterAll");
    char* error;
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return;
    }

    // allow for all exslt functions
    exsltRegisterAll();
#endif
}
