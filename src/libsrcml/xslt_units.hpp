/**
 * @file xslt_units.hpp
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

#ifndef INCLUDED_XSLT_UNITS_HPP
#define INCLUDED_XSLT_UNITS_HPP

#include <libxml/parser.h>
#include <libxslt/transform.h>

/** size of string then the literal */
#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s
 /** literal followed by its size */
#define LITERALPLUSSIZE(s) s, sizeof(s) - 1

#include <srcexfun.hpp>
#include <transform_units.hpp>

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#define DLLOAD
#else
#undef DLLOAD
#endif

#ifdef DLLOAD
typedef void * __attribute__ ((__may_alias__)) VOIDPTR;

typedef xmlDocPtr (*xsltApplyStylesheetUser_function) (xsltStylesheetPtr,xmlDocPtr,const char **,const char *, FILE *,
                                                       xsltTransformContextPtr);

typedef void * __attribute__ ((__may_alias__)) VOIDPTR;
typedef xsltStylesheetPtr (*xsltParseStylesheetDoc_function) (xmlDocPtr);
typedef void (*xsltCleanupGlobals_function)();
typedef void (*xsltFreeStylesheet_function)(xsltStylesheetPtr);

void dlexsltRegisterAll(void * handle);
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
 *
 * Extends unit_dom to execute XSLT program and write results.
 */
class xslt_units : public transform_units {
public :

    /**
     * xslt_units
     * @param a_context_element an element that provides a context
     * @param options list of srcML options
     * @param stylesheet an XSLT stylesheet
     * @param params XSLT parameters
     *
     * Constructor.  Dynamically loads XSLT functions.
     */
    xslt_units(const char* a_context_element, OPTION_TYPE & options, xmlDocPtr xslt,
               const std::vector<std::string>& params, srcml_archive* oarchive)
        : transform_units(options, oarchive), params(params), cparams(params.size() + 1), xslt(xslt) {

        // cparams must be null terminated
        for (size_t i = 0; i < params.size(); ++i) {
            cparams[i] = params[i].c_str();
        }
        cparams.back() = 0;

#ifdef DLLOAD
        handle = dlopen_libxslt();
        if (!handle) {
            fprintf(stderr, "Unable to open libxslt library\n");
            return;
        }

        dlerror();
        *(VOIDPTR *)(&xsltApplyStylesheetUser) = dlsym(handle, "xsltApplyStylesheetUser");
        char* error;
        if ((error = dlerror()) != NULL) {
            dlclose(handle);
            handle = 0;
            return;
        }
#endif
    }

#ifdef DLLOAD
    /**
     * ~xslt_units
     *
     * Destructor.  Closes dynamically loaded library.
     */
    virtual ~xslt_units() {
        dlclose(handle);
    }
#endif

    /**
     * apply
     *
     * Apply XSLT program, writing results.
     * 
     * @returns true on success false on failure.
     */
    virtual bool apply() {

        // position passed to XSLT program
        setPosition((int)unit_count);

        // apply the style sheet to the document, which is the individual unit
        xmlDocPtr res = xsltApplyStylesheetUser(stylesheet, doc, cparams.data(), 0, 0, 0);
        if (!res) {
            fprintf(stderr, "libsrcml:  Error in applying stylesheet\n");
            return SRCML_STATUS_ERROR;
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

    virtual void start_output() {

        xmlInitParser();

    #if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
        handle = dlopen_libexslt();

        // allow for all exslt functions
        dlexsltRegisterAll(handle);

        dlerror();
        *(VOIDPTR *)(&xsltParseStylesheetDoc) = dlsym(handle, "xsltParseStylesheetDoc");
        char* error;
        if ((error = dlerror()) != NULL) {
            dlclose(handle);
            return; // SRCML_STATUS_ERROR;
        }

        dlerror();
        *(VOIDPTR *)(&xsltCleanupGlobals) = dlsym(handle, "xsltCleanupGlobals");
        if ((error = dlerror()) != NULL) {
            dlclose(handle);
            return; // SRCML_STATUS_ERROR;
        }

        dlerror();
        *(VOIDPTR *)(&xsltFreeStylesheet) = dlsym(handle, "xsltFreeStylesheet");
        if ((error = dlerror()) != NULL) {
            dlclose(handle);
            return; // SRCML_STATUS_ERROR;
        }
    #endif

        // parse the stylesheet
        stylesheet = xsltParseStylesheetDoc(xslt);
        if (!stylesheet)
            return; // SRCML_STATUS_ERROR;

        xsltsrcMLRegister();
    }

    void end_output() {

        stylesheet->doc = 0;
        xsltFreeStylesheet(stylesheet);
        xsltCleanupGlobals();

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
        dlclose(handle);
#endif
    }

private :
    xsltStylesheetPtr stylesheet;
    std::vector<std::string> params;
    std::vector<const char*> cparams;
    void* handle = nullptr;
    xmlDocPtr xslt;
#ifdef DLLOAD
    xsltApplyStylesheetUser_function xsltApplyStylesheetUser;
    xsltParseStylesheetDoc_function xsltParseStylesheetDoc;
    xsltCleanupGlobals_function xsltCleanupGlobals;
    xsltFreeStylesheet_function xsltFreeStylesheet;
#endif
};

/**
 * dlexsltRegisterAll
 *
 * Allow for all exslt functions by dynamic load
 * of exslt library.
 */
void dlexsltRegisterAll(void * handle) {

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    typedef void * __attribute__ ((__may_alias__)) VOIDPTR;
    typedef void (*exsltRegisterAll_function)();
    dlerror();
    exsltRegisterAll_function exsltRegisterAll;
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
 * @param context_element a srcml element to be used as the context
 * @param xslt xmlDocPtr containing an XSLT program
 * @param params NULL-terminated list of XSLT parameters
 * @param paramcount number of XSLT parameters
 * @param options srcml options
 *
 * XSLT evaluation of the nested units.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_xslt(xmlParserInputBufferPtr input_buffer, const char* context_element, xmlDocPtr xslt, const std::vector<std::string>& params, int /* paramcount */, OPTION_TYPE options,
                srcml_archive* out_archive) {

    if (input_buffer == NULL || context_element == NULL || xslt == NULL)
        return SRCML_STATUS_INVALID_ARGUMENT;

    // setup process handling
    xslt_units process(context_element, options, xslt, params, out_archive);
    srcSAXController control(input_buffer);

    try {

        control.parse(&process);

    } catch(SAXError error) {

        fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());
        return 1;
    }

    return SRCML_STATUS_OK;
}
#endif

#endif
