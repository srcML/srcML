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

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#define DLLOAD
#else
#undef DLLOAD
#endif

#include <libxslt/transform.h>

#include <srcexfun.hpp>
#include <transform_units.hpp>

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
                srcml_archive* out_archive);

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
    xslt_units(OPTION_TYPE& options,
               xmlDocPtr xslt,
               const std::vector<std::string>& params,
               srcml_archive* oarchive);

#ifdef DLLOAD
    /**
     * ~xslt_units
     *
     * Destructor.  Closes dynamically loaded library.
     */
    virtual ~xslt_units();
#endif

    /**
     * apply
     *
     * Apply XSLT program, writing results.
     * 
     * @returns true on success false on failure.
     */
    virtual bool apply();

    virtual int start_output();

    int end_output();

private :
    xsltStylesheetPtr stylesheet;
    std::vector<std::string> params;
    std::vector<const char*> cparams;
    xmlDocPtr xslt;
#ifdef DLLOAD
    void* libxslt_handle = nullptr;
    void* libexslt_handle = nullptr;
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
void dlexsltRegisterAll(void * handle);

#endif