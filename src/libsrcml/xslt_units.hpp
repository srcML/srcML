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

#include <unit_dom.hpp>

#if defined(__GNUG__) && !defined(__MINGW32__)
typedef void * __attribute__ ((__may_alias__)) VOIDPTR;

typedef xmlDocPtr (*xsltApplyStylesheetUser_function) (xsltStylesheetPtr,xmlDocPtr,const char **,const char *, FILE *,
                                                       xsltTransformContextPtr);
typedef xmlDocPtr (*xsltApplyStylesheet_function) (xsltStylesheetPtr,xmlDocPtr,const char **);

//typedef int (*xsltSaveResultTo_function) (xmlOutputBufferPtr, xmlDocPtr, xsltStylesheetPtr);
//xsltSaveResultTo_function xsltSaveResultToDynamic;
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
class xslt_units : public unit_dom {
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
    xslt_units(const char* a_context_element, OPTION_TYPE & options, xsltStylesheetPtr stylesheet,
               const char** params, srcml_archive* oarchive)
        : unit_dom(options),
          stylesheet(stylesheet), params(params), oarchive(oarchive) {

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
        handle = dlopen("libxslt.so", RTLD_LAZY);
        if (!handle) {
            handle = dlopen("libxslt.so.1", RTLD_LAZY);
            if (!handle) {
                handle = dlopen("libxslt.dylib", RTLD_LAZY);
                if (!handle) {
                    fprintf(stderr, "Unable to open libxslt library\n");
                    return;
                }
            }
        }

        dlerror();
        *(VOIDPTR *)(&xsltApplyStylesheetUserDynamic) = dlsym(handle, "xsltApplyStylesheetUser");
        char* error;
        if ((error = dlerror()) != NULL) {
            dlclose(handle);
            return;
        }
        dlerror();
        *(VOIDPTR *)(&xsltApplyStylesheetDynamic) = dlsym(handle, "xsltApplyStylesheet");
        if ((error = dlerror()) != NULL) {
            dlclose(handle);
            return;
        }
        /*
          dlerror();
          xsltSaveResultToDynamic = (xsltSaveResultTo_function)dlsym(handle, "xsltSaveResultTo");
          if ((error = dlerror()) != NULL) {
          dlclose(handle);
          return;
          }
        */
#endif
    }

    /**
     * ~xslt_units
     *
     * Destructor.  Closes dynamically loaded library.
     */
    virtual ~xslt_units() {
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
        dlclose(handle);
#endif
    }

    /**
     * start_output
     *
     * Create output buffer.
     */
    virtual void start_output() {}

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
#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
        xmlDocPtr res = xsltApplyStylesheetUserDynamic(stylesheet, ctxt->myDoc, params, 0, 0, 0);
#else
        xmlDocPtr res = xsltApplyStylesheetUser(stylesheet, ctxt->myDoc, params, 0, 0, 0);
#endif
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

    virtual void outputResult(xmlNodePtr a_node) {

        static xmlBufferPtr lbuffer = xmlBufferCreate();
        int size = xmlNodeDump(lbuffer, ctxt->myDoc, a_node, 0, 1);

        srcml_unit* punit = srcml_unit_create(oarchive);

        oarchive->translator->add_unit(punit, (const char*) xmlBufferContent(lbuffer));

        srcml_unit_free(punit);

        xmlBufferEmpty(lbuffer);
    }
    /**
     * end_output
     *
     * Finish the archive and close buffer.
     */
    virtual void end_output() {}

private :

    xsltStylesheetPtr stylesheet;
    const char** params;
#ifndef WIN32
    xsltApplyStylesheetUser_function xsltApplyStylesheetUserDynamic;
    xsltApplyStylesheet_function xsltApplyStylesheetDynamic;
#endif
    void * handle;
    srcml_archive* oarchive;
};

#endif
