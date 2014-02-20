/**
 * @file srcml_sax2_utilities.cpp
 * @copyright
 *
 * Copyright (C) 2013-2014  SDML (www.srcML.org)
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
#include <srcMLSAX2Reader.hpp>
#include <srcml.h>

#include <SAX2ExtractUnitsSrc.hpp>
#include <sstream>
#include <XPathQueryUnits.hpp>
#include <XSLTUnits.hpp>
#include <RelaxNGUnits.hpp>


#include <srcexfun.hpp>

#include <libexslt/exslt.h>

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
 *
 * Extract a given unit from supplied srcML input buffer.
 *
 * @returns Return SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_extract_text(const char * input_buffer, size_t size, xmlOutputBufferPtr output_buffer, OPTION_TYPE options, int unit) {

    if(input_buffer == NULL || size == 0 || output_buffer == NULL) return SRCML_STATUS_ERROR;

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem(input_buffer, (int)size, xmlParseCharEncoding(0));

    if(input == NULL) return SRCML_STATUS_ERROR;

    srcMLSAX2Reader reader(input);
    reader.readsrc(output_buffer);

    xmlOutputBufferClose(output_buffer);

    return SRCML_STATUS_OK;

}

/**
 * srcml_extract_text_filename
 * @param ifilename name of srcML file to extract text
 * @param ofilename name of output file to put source
 * @param encoding output encoding
 * @param options srcml options
 * @param unit unit number to extract
 *
 *
 * Extract a given unit from supplied srcML directly to file.
 *
 * @returns Return SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_extract_text_filename(const char * ifilename, const char * ofilename, const char * encoding, OPTION_TYPE options, int unit) {

  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateFilename(ofilename, xmlFindCharEncodingHandler(encoding), options & SRCML_OPTION_COMPRESS);

    srcMLSAX2Reader reader(ifilename);
    reader.readsrc(output_buffer);

    xmlOutputBufferClose(output_buffer);

    return SRCML_STATUS_OK;

}

/**
 * srcml_xpath
 * @param input_buffer a parser input buffer
 * @param context_element a srcML element that is to be used as the context
 * @param xpaths NULL-terminated list of xpath expressions
 * @param fd output file descriptor
 * @param options srcml options
 *
 * XPath evaluation of the nested units.
 *
 * @returns Return SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_xpath(xmlParserInputBufferPtr input_buffer, const char* context_element, const char* xpaths[], int fd, OPTION_TYPE options) {

    if(input_buffer == NULL || context_element == NULL || xpaths == NULL || xpaths[0] == NULL || fd < 0) return SRCML_STATUS_ERROR;

    // relative xpath changed to at any level
    std::string s = xpaths[0];
    //  if (s[0] != '/')
    //    s = "//" + s;

    // compile the xpath that will be applied to each unit
    xmlXPathCompExprPtr compiled_xpath = xmlXPathCompile(BAD_CAST s.c_str());
    if (compiled_xpath == 0) {
        fprintf(stderr, "srcml2src:  Unable to compile XPath '%s'\n", s.c_str());
        return SRCML_STATUS_ERROR;
    }

    // setup process handling
    XPathQueryUnits process(options, compiled_xpath, fd);
    srcMLControlHandler control(input_buffer);

    control.parse(&process);

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
    typedef void (*exsltRegisterAll_function)();

    dlerror();
    exsltRegisterAll_function exsltRegisterAll;
    *(void **)(&exsltRegisterAll) = dlsym(handle, "exsltRegisterAll");
    char* error;
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return;
    }

    // allow for all exslt functions
    exsltRegisterAll();

#endif

}

/**
 * srcml_xslt
 * @param input_buffer a parser input buffer
 * @param context_element a srcml element to be used as the context
 * @param xslts NULL-terminated list of XSLT program filenames.
 * @param params NULL-terminated list of XSLT parameters
 * @param paramcount number of XSLT parameters
 * @param fd output file descriptor
 * @param options srcml options
 *
 * XSLT evaluation of the nested units.
 *
 * @returns Return SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_xslt(xmlParserInputBufferPtr input_buffer, const char* context_element, const char* xslts[], const char* params[], int paramcount, int fd, OPTION_TYPE options) {

    if(input_buffer == NULL || context_element == NULL || xslts == NULL || xslts[0] == NULL || fd < 0) return SRCML_STATUS_ERROR;

    xmlInitParser();

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    typedef xsltStylesheetPtr (*xsltParseStylesheetFile_function) (const xmlChar*);
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
    xsltParseStylesheetFile_function xsltParseStylesheetFile;
    *(void **)(&xsltParseStylesheetFile) = dlsym(handle, "xsltParseStylesheetFile");
    char* error;
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return SRCML_STATUS_ERROR;
    }

    dlerror();
    xsltCleanupGlobals_function xsltCleanupGlobals;
    *(void **)(&xsltCleanupGlobals) = dlsym(handle, "xsltCleanupGlobals");
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return SRCML_STATUS_ERROR;
    }

    dlerror();
    xsltFreeStylesheet_function xsltFreeStylesheet;
    *(void **)(&xsltFreeStylesheet) = dlsym(handle, "xsltFreeStylesheet");
    if ((error = dlerror()) != NULL) {
        dlclose(handle);
        return SRCML_STATUS_ERROR;
    }
#endif

    // parse the stylesheet
    xsltStylesheetPtr stylesheet = xsltParseStylesheetFile(BAD_CAST xslts[0]);
    if (!stylesheet)
        return SRCML_STATUS_ERROR;

    // setup parser
    xmlParserCtxtPtr ctxt = srcMLCreateParserCtxt(input_buffer);
    if (ctxt == NULL) return SRCML_STATUS_ERROR;

    // setup sax handler
    xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
    xmlSAXHandlerPtr sax_save = ctxt->sax;
    ctxt->sax = &sax;

    // setup process handling
    XSLTUnits process(context_element, 0, options, stylesheet, params, fd);

    // setup sax handling state
    //SAX2ExtractUnitsSrc state(&process, &options, -1, "");
    //ctxt->_private = &state;

    xsltsrcMLRegister();

    // process the document
    int status = srcMLParseDocument(ctxt, false);

    // local variable, do not want xmlFreeParserCtxt to free
    ctxt->sax = sax_save;

    xsltFreeStylesheet(stylesheet);
    xsltCleanupGlobals();
    // all done with parsing
    xmlParserInputPtr input = inputPop(ctxt);
    input->buf = NULL;
    xmlFreeInputStream(input);
    xmlFreeParserCtxt(ctxt);

#if defined(__GNUG__) && !defined(__MINGW32__) && !defined(NO_DLLOAD)
    dlclose(handle);
#endif

    return status;

}

/**
 * srcml_relaxng
 * @param input_buffer a parser input buffer
 * @param xslts a NULL-terminated list of RelaxNG schemas
 * @param fd output file descriptor
 * @param options srcml options
 *
 * RelaxNG evaluation of the nested units.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_relaxng(xmlParserInputBufferPtr input_buffer, const char** xslts, int fd, OPTION_TYPE options) {

    if(input_buffer == NULL || xslts == NULL || xslts[0] == NULL || fd < 0) return SRCML_STATUS_ERROR;

    xmlRelaxNGParserCtxtPtr relaxng = xmlRelaxNGNewParserCtxt(xslts[0]);
    xmlRelaxNGPtr rng = xmlRelaxNGParse(relaxng);
    xmlRelaxNGValidCtxtPtr rngctx = xmlRelaxNGNewValidCtxt(rng);

    RelaxNGUnits process(0, options, rngctx, fd);
    srcMLControlHandler control(input_buffer);

    control.parse(&process);

    xmlRelaxNGFreeValidCtxt(rngctx);
    xmlRelaxNGFree(rng);
    xmlRelaxNGFreeParserCtxt(relaxng);

    return 0;//status;

}


/**
 * srcMLParseDocument
 * @param ctxt an XML parser ctxt
 * @param allowendearly allow early termination of SAX2 parser
 *
 * Process srcML document with error reporting.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcMLParseDocument(xmlParserCtxtPtr ctxt, bool allowendearly) {

    // process the document
    int status;
    if ((status = xmlParseDocument(ctxt)) == -1) {

        xmlErrorPtr ep = xmlCtxtGetLastError(ctxt);

        // special case
        if (allowendearly && (ep->code == XML_ERR_EXTRA_CONTENT || ep->code == XML_ERR_DOCUMENT_END))
            return SRCML_STATUS_OK;

        return SRCML_STATUS_ERROR;
    }

    return SRCML_STATUS_OK;

}

/**
 * srcMLCreateMemoryParserCtxt
 * @param buffer a buffer in memory to read from
 * @param size size of input buffer
 *
 * Create srcml parser from memory.
 *
 * @returns xml parser ctxt
 */
xmlParserCtxtPtr srcMLCreateMemoryParserCtxt(const char * buffer, int size) {

    xmlParserCtxtPtr ctxt = xmlCreateMemoryParserCtxt(buffer, size);

    return ctxt;

}

#ifdef LIBXML2_NEW_BUFFER
struct _xmlBuf {
    xmlChar *content;           /* The buffer content UTF8 */
    unsigned int compat_use;    /* for binary compatibility */
    unsigned int compat_size;   /* for binary compatibility */
    xmlBufferAllocationScheme alloc; /* The realloc method */
    xmlChar *contentIO;         /* in IO mode we may have a different base */
    size_t use;                 /* The buffer size used */
    size_t size;                /* The buffer size */
    xmlBufferPtr buffer;        /* wrapper for an old buffer */
    int error;                  /* an error code if a failure occured */
};
#define CHECK_COMPAT(buf)                       \
    if (buf->size != (size_t) buf->compat_size) \
        if (buf->compat_size < INT_MAX)         \
            buf->size = buf->compat_size;       \
    if (buf->use != (size_t) buf->compat_use)   \
        if (buf->compat_use < INT_MAX)          \
            buf->use = buf->compat_use;

/**
 * xmlBufResetInput
 * @param buf XML buffer
 * @param input XML parser input
 *
 * Function is taken from libxml2.
 *
 * @returns 0 on success and -1 on error.
 */
int
xmlBufResetInput(xmlBuf * buf, xmlParserInputPtr input) {
    if ((input == NULL) || (buf == NULL) || (buf->error))
        return(-1);
    CHECK_COMPAT(buf)
        input->base = input->cur = buf->content;
    input->end = &buf->content[buf->use];
    return(0);

}
#else
/**
 * xmlBufResetInput
 * @param buf XML buffer
 * @param input XML parser input
 *
 * Function is taken fro libxml2.
 *
 * @returns 0
 */
int
xmlBufResetInput(xmlBuffer * buf, xmlParserInputPtr input) {
    input->base = input->buf->buffer->content;
    input->cur = input->buf->buffer->content;
    input->end = &input->buf->buffer->content[input->buf->buffer->use];
    return 0;

}
#endif

/**
 * srcMLCreateParserCtxt
 * @param buffer_input a parser input buffer
 *
 * Create a ctxt from a parser input buffer.
 * Modeled after function in libxml2.
 *
 * @returns xml parser ctxt
 */
xmlParserCtxtPtr
srcMLCreateParserCtxt(xmlParserInputBufferPtr buffer_input) {
    xmlParserCtxtPtr ctxt;
    xmlParserInputPtr input;
    xmlParserInputBufferPtr buf;

    ctxt = xmlNewParserCtxt();
    if (ctxt == NULL)
        return(NULL);

    buf = buffer_input;
    if (buf == NULL) {
        xmlFreeParserCtxt(ctxt);
        return(NULL);
    }

    input = xmlNewInputStream(ctxt);
    if (input == NULL) {
        xmlFreeParserCtxt(ctxt);
        return(NULL);
    }

    input->filename = NULL;
    input->buf = buf;
    xmlBufResetInput(input->buf->buffer, input);

    inputPush(ctxt, input);
    return(ctxt);

}
