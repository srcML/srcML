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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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
        : params(params), cparams(params.size() + 1) {

    // cparams must be null terminated
    for (size_t i = 0; i < params.size(); ++i) {
        cparams[i] = params[i].c_str();
    }
    cparams.back() = 0;

    xmlInitParser();

#ifdef DLLOAD
    libxslt_handle = dlopen_libxslt();
    if (!libxslt_handle) {
        fprintf(stderr, "Unable to open libxslt library\n");
        return;
    }

    char* error;

    dlerror();
    xsltApplyStylesheetUser = (xsltApplyStylesheetUser_t) dlsym(libxslt_handle, "xsltApplyStylesheetUser");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    xsltParseStylesheetDoc = (xsltParseStylesheetDoc_t) dlsym(libxslt_handle, "xsltParseStylesheetDoc");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    xsltCleanupGlobals = (xsltCleanupGlobals_t) dlsym(libxslt_handle, "xsltCleanupGlobals");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    xsltFreeStylesheet = (xsltFreeStylesheet_t) dlsym(libxslt_handle, "xsltFreeStylesheet");
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

    xsltsrcMLRegister();
}

/**
 * ~xsltTransformation
 *
 * Destructor.  Closes dynamically loaded library.
 */
xsltTransformation::~xsltTransformation() {

    xsltFreeStylesheet(stylesheet);
    xsltCleanupGlobals();

    dlclose(libxslt_handle);
    dlclose(libexslt_handle);
}

/**
 * apply
 *
 * Apply XSLT program, writing results.
 * 
 * @returns true on success false on failure.
 */
xmlDocPtr xsltTransformation::apply(xmlDocPtr doc, int position) {

    // position passed to XSLT program
    setPosition(position);

    // apply the style sheet to the document, which is the individual unit
    xmlDocPtr res = xsltApplyStylesheetUser(stylesheet, doc, cparams.data(), 0, 0, 0);
    if (!res) {
        fprintf(stderr, "libsrcml:  Error in applying stylesheet\n");

        return nullptr;
    }

    return res;
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
    auto exsltRegisterAll = (exsltRegisterAll_t) dlsym(handle, "exsltRegisterAll");
    char* error;
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return;
    }

    // allow for all exslt functions
    exsltRegisterAll();
#endif
}
