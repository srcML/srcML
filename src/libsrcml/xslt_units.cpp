/**
 * @file xslt_units.cpp
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

#include <xslt_units.hpp>

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
 * xslt_units
 * @param a_context_element an element that provides a context
 * @param options list of srcML options
 * @param stylesheet an XSLT stylesheet
 * @param params XSLT parameters
 *
 * Constructor.  Dynamically loads XSLT functions.
 */
xslt_units::xslt_units(OPTION_TYPE & options, xmlDocPtr xslt,
                       const std::vector<std::string>& params, srcml_archive* oarchive)
        : transform_units(options, oarchive), params(params), cparams(params.size() + 1), xslt(xslt) {

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
    *(VOIDPTR *)(&xsltApplyStylesheetUser) = dlsym(libxslt_handle, "xsltApplyStylesheetUser");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    *(VOIDPTR *)(&xsltParseStylesheetDoc) = dlsym(libxslt_handle, "xsltParseStylesheetDoc");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    *(VOIDPTR *)(&xsltCleanupGlobals) = dlsym(libxslt_handle, "xsltCleanupGlobals");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    dlerror();
    *(VOIDPTR *)(&xsltFreeStylesheet) = dlsym(libxslt_handle, "xsltFreeStylesheet");
    if ((error = dlerror()) != NULL) {
        dlclose(libxslt_handle);
        libxslt_handle = 0;
        throw;
    }

    libexslt_handle = dlopen_libexslt();

    // allow for all exslt functions
    dlexsltRegisterAll(libexslt_handle);
#endif

    xsltsrcMLRegister();
}

#ifdef DLLOAD
/**
 * ~xslt_units
 *
 * Destructor.  Closes dynamically loaded library.
 */
xslt_units::~xslt_units() {
    dlclose(libxslt_handle);
    dlclose(libexslt_handle);
}
#endif

/**
 * apply
 *
 * Apply XSLT program, writing results.
 * 
 * @returns true on success false on failure.
 */
bool xslt_units::apply() {

    // position passed to XSLT program
    setPosition((int)unit_count);

    // apply the style sheet to the document, which is the individual unit
    xmlDocPtr res = xsltApplyStylesheetUser(stylesheet, doc, cparams.data(), 0, 0, 0);
    if (!res) {
        fprintf(stderr, "libsrcml:  Error in applying stylesheet\n");
        return false;
    }

    // output the transformed result
    for (xmlNodePtr child = res->children; child != NULL; child = child->next) {

        if (child->type == XML_TEXT_NODE)
            xmlOutputBufferWriteString(oarchive->translator->output_buffer(), (const char *) child->content);
        else
            outputResult(child);
    }

    // finished with the result of the transformation
    xmlFreeDoc(res);

    return true;
}

int xslt_units::start_output() {

    // parse the stylesheet
    stylesheet = xsltParseStylesheetDoc(xslt);
    if (!stylesheet)
        return SRCML_STATUS_ERROR;

    return 0;
}

int xslt_units::end_output() {

    stylesheet->doc = 0;
    xsltFreeStylesheet(stylesheet);
    xsltCleanupGlobals();

    return 0;
}

/**
 * dlexsltRegisterAll
 *
 * Allow for all exslt functions by dynamic load
 * of exslt library.
 */
void dlexsltRegisterAll(void* handle) {

#ifdef DLLOAD
    typedef void (*exsltRegisterAll_function)();

    exsltRegisterAll_function exsltRegisterAll;

    dlerror();
    *(VOIDPTR *)(&exsltRegisterAll) = dlsym(handle, "exsltRegisterAll");
    char* error;
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return;
    }

    // allow for all exslt functions
    exsltRegisterAll();
#endif
}

#ifdef WITH_LIBXSLT
/**
 * srcml_xslt
 * @param input_buffer a parser input buffer
 * @param xslt xmlDocPtr containing an XSLT program
 * @param params NULL-terminated list of XSLT parameters
 * @param paramcount number of XSLT parameters
 * @param options srcml options
 *
 * XSLT evaluation of the nested units.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_xslt(xmlParserInputBufferPtr input_buffer, xmlDocPtr xslt, const std::vector<std::string>& params, int /* paramcount */, OPTION_TYPE options,
                srcml_archive* out_archive) {

    if (input_buffer == NULL || xslt == NULL)
        return SRCML_STATUS_INVALID_ARGUMENT;

    try {

        // setup process handling
        xslt_units process(options, xslt, params, out_archive);
        srcSAXController control(input_buffer);

        control.parse(&process);

    } catch(SAXError error) {

        fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());
        return 1;
    }

    return SRCML_STATUS_OK;
}
#endif
