/**
 * @file srcml_sax2_utilities.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

#include <srcml_sax2_utilities.hpp>
#include <srcml_sax2_reader.hpp>

#include <sstream>
#include <xpath_query_units.hpp>
#ifdef WITH_LIBXSLT
#include <xslt_units.hpp>
#endif
#include <relaxng_units.hpp>

#include <srcexfun.hpp>

#ifdef WITH_LIBXSLT
#include <libexslt/exslt.h>
#endif

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
#include <dlfcn.h>
#endif

/**
 * srcml_extract_text
 * @param input_buffer srcML to extract text
 * @param size size of input buffer
 * @param output_buffer output buffer to write text
 * @param options srcml options
 * @param unit unit number to extract
 *
 * Extract a given unit from supplied srcML input buffer.
 *
 * @returns Return SRCML_STATUS_OK on success and SRCML_STATUS_INVALID_ARGUMENT on failure.
 */
int srcml_extract_text(const char * input_buffer, size_t size, xmlOutputBufferPtr output_buffer, OPTION_TYPE options, int unit) {

    if(input_buffer == NULL || size == 0) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem(input_buffer, (int)size, XML_CHAR_ENCODING_NONE);

    if(input == NULL) return SRCML_STATUS_IO_ERROR;

    srcml_sax2_reader reader(input);
    reader.read_src(output_buffer);

    
    xmlFreeParserInputBuffer(input);

    return SRCML_STATUS_OK;

}

/**
 * srcml_extract_text_filename
 * @param ifilename name of srcML file to extract text
 * @param ofilename name of output file to put source
 * @param encoding output encoding
 * @param compression amount of compression 0 none to 9 max
 * @param unit unit number to extract
 *
 * Extract a given unit from supplied srcML directly to file.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_extract_text_filename(const char * ifilename, const char * ofilename, const char * encoding, unsigned short compression, int unit) {

    if(compression > 9) compression = 9;

    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(ofilename, xmlFindCharEncodingHandler(encoding), compression);

    srcml_sax2_reader reader(ifilename);
    reader.read_src(output_buffer);

    xmlOutputBufferClose(output_buffer);

    return SRCML_STATUS_OK;

}

/**
 * srcml_xpath
 * @param input_buffer a parser input buffer
 * @param context_element a srcML element that is to be used as the context
 * @param xpath the xpath expression
 * @param prefix an element namepace prefix
 * @param uri an element namepace uri
 * @param element the tag name
 * @param attr_prefix an attribute namepace prefix
 * @param attr_uri an attribute namepace uri
 * @param attr_name the attribute name
 * @param attr_value the attribute value
 * @param options srcml options
 *
 * XPath evaluation of the nested units.
 *
 * @returns Return SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_xpath(xmlParserInputBufferPtr input_buffer, const char * context_element, const char * xpath,
                const char * prefix, const char * uri, const char * element, const char * attr_prefix, const char * attr_uri, const char * attr_name, const char * attr_value,
                OPTION_TYPE options, srcml_archive* out_archive) {

    if(input_buffer == NULL || context_element == NULL ||
       xpath == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    // relative xpath changed to at any level
    std::string s = xpath;
    //  if (s[0] != '/')
    //    s = "//" + s;

    // compile the xpath that will be applied to each unit
    xmlXPathCompExprPtr compiled_xpath = xmlXPathCompile(BAD_CAST s.c_str());
    if (compiled_xpath == 0) {
        fprintf(stderr, "libsrcml:  Unable to compile XPath '%s'\n", s.c_str());
        return SRCML_STATUS_INVALID_INPUT;
    }

    // setup process handling
    xpath_query_units process(options, compiled_xpath, out_archive, prefix, uri, element, attr_prefix, attr_uri, attr_name, attr_value);
    srcSAXController control(input_buffer);

    try {

        control.parse(&process);

    } catch(SAXError error) {

        fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());

    }

    xmlXPathFreeCompExpr(compiled_xpath);

    return 0;//status;

}


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
int srcml_xslt(xmlParserInputBufferPtr input_buffer, const char* context_element, xmlDocPtr xslt, const char* params[], int paramcount, OPTION_TYPE options,
                srcml_archive* out_archive) {

    if(input_buffer == NULL || context_element == NULL ||
       xslt == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlInitParser();

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    typedef void * __attribute__ ((__may_alias__)) VOIDPTR;
    typedef xsltStylesheetPtr (*xsltParseStylesheetDoc_function) (xmlDocPtr);
    typedef void (*xsltCleanupGlobals_function)();
    typedef void (*xsltFreeStylesheet_function)(xsltStylesheetPtr);

    void* handle = dlopen("libexslt.so", RTLD_LAZY);
    if (!handle) {
        handle = dlopen("libexslt.so.0", RTLD_LAZY);
        if (!handle) {
            handle = dlopen("libexslt.dylib", RTLD_LAZY);
            if (!handle) {
                fprintf(stderr, "Unable to open libexslt library\n");
                return SRCML_STATUS_ERROR;
            }
        }
    }

    // allow for all exstl functions
    dlexsltRegisterAll(handle);

    dlerror();
    xsltParseStylesheetDoc_function xsltParseStylesheetDoc;
    *(VOIDPTR *)(&xsltParseStylesheetDoc) = dlsym(handle, "xsltParseStylesheetDoc");
    char* error;
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return SRCML_STATUS_ERROR;
    }

    dlerror();
    xsltCleanupGlobals_function xsltCleanupGlobals;
    *(VOIDPTR *)(&xsltCleanupGlobals) = dlsym(handle, "xsltCleanupGlobals");
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return SRCML_STATUS_ERROR;
    }

    dlerror();
    xsltFreeStylesheet_function xsltFreeStylesheet;
    *(VOIDPTR *)(&xsltFreeStylesheet) = dlsym(handle, "xsltFreeStylesheet");
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return SRCML_STATUS_ERROR;
    }
#endif

    // parse the stylesheet
    xsltStylesheetPtr stylesheet = xsltParseStylesheetDoc(xslt);
    if (!stylesheet)
        return SRCML_STATUS_ERROR;



    xsltsrcMLRegister();

    // setup process handling
    xslt_units process(context_element, options, stylesheet, params, out_archive);
    srcSAXController control(input_buffer);

    try {

        control.parse(&process);

    } catch(SAXError error) {

        fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());

    }

    stylesheet->doc = 0;
    xsltFreeStylesheet(stylesheet);
    xsltCleanupGlobals();

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    dlclose(handle);
#endif

    return 0;//status;

}
#endif

/**
 * srcml_relaxng
 * @param input_buffer a parser input buffer
 * @param relaxng xmlDocPtr containing a RelaxNG schema
 * @param fd output file descriptor
 * @param options srcml options
 *
 * RelaxNG evaluation of the nested units.
 *
 * @returns Returns SRCML_STATUS_OK on success and a status error code on failure.
 */
int srcml_relaxng(xmlParserInputBufferPtr input_buffer, xmlDocPtr relaxng, OPTION_TYPE options, srcml_archive* oarchive) {

    if(input_buffer == NULL || relaxng == NULL) return SRCML_STATUS_INVALID_ARGUMENT;

    xmlRelaxNGParserCtxtPtr relaxng_parser_ctxt = xmlRelaxNGNewDocParserCtxt(relaxng);
    xmlRelaxNGPtr rng = xmlRelaxNGParse(relaxng_parser_ctxt);
    xmlRelaxNGValidCtxtPtr rngctx = xmlRelaxNGNewValidCtxt(rng);

    relaxng_units process(options, rngctx, oarchive);
    srcSAXController control(input_buffer);

    try {

        control.parse(&process);

    } catch(SAXError error) {

        fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());

    }

    xmlRelaxNGFreeValidCtxt(rngctx);
    xmlRelaxNGFree(rng);
    xmlRelaxNGFreeParserCtxt(relaxng_parser_ctxt);

    return 0;//status;

}
