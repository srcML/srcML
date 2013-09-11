// extract a given unit
void srcMLUtility::extract_text(xmlOutputBufferPtr output_buffer, int unit) {

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateMemoryParserCtxt(buffer, size);

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  ExtractUnitsSrc process(output_buffer);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, unit, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, true);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  xmlFreeParserCtxt(ctxt);

  // make sure we did not end early
  if (state.unit && state.count < state.unit)
    throw OutOfRangeUnitError(state.count);
}

// xpath evaluation of the nested units
void srcMLUtility::xpath(const char* ofilename, const char* context_element, const char* xpaths[], int fd) {

  // relative xpath changed to at any level
  if(ofilename) xmlMemSetup(xmlMemFree, xmlMemMalloc, xmlMemRealloc, xmlMemoryStrdup);
  std::string s = xpaths[0];
  //  if (s[0] != '/')
  //    s = "//" + s;

  // compile the xpath that will be applied to each unit
  xmlXPathCompExprPtr compiled_xpath = xmlXPathCompile(BAD_CAST s.c_str());
  if (compiled_xpath == 0) {
    fprintf(stderr, "srcml2src:  Unable to compile XPath '%s'\n", s.c_str());
    return;
  }

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateParserCtxt(buffer_input);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  XPathQueryUnits process(context_element, ofilename, options, compiled_xpath, fd);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1, diff_version);
  ctxt->_private = &state;

  // process the document
  srcMLParseDocument(ctxt, false);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  // all done with parsing
  if(buffer_input) inputPop(ctxt);
  xmlFreeParserCtxt(ctxt);
}

// allow for all exslt functions
void dlexsltRegisterAll() {

#if defined(__GNUG__) && !defined(__MINGW32__)
  typedef void (*exsltRegisterAll_function)();

  void* handle = dlopen("libexslt.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libexslt.dylib", RTLD_LAZY);
    if (!handle) {
      fprintf(stderr, "Unable to open libexslt library\n");
      return;
    }
  }

  dlerror();
  exsltRegisterAll_function exsltRegisterAll = (exsltRegisterAll_function)dlsym(handle, "exsltRegisterAll");
  char* error;
  if ((error = dlerror()) != NULL) {
    dlclose(handle);
    return;
  }

  // allow for all exslt functions
  exsltRegisterAll();

#endif


#if defined(__GNUG__) && !defined(__MINGW32__)
  dlclose(handle);
#endif
}

// xslt evaluation of the nested units
void srcMLUtility::xslt(const char* context_element, const char* ofilename, const char* xslts[], const char* params[], int paramcount, int fd) {

  if(ofilename) xmlMemSetup(xmlMemFree, xmlMemMalloc, xmlMemRealloc, xmlMemoryStrdup);
  xmlInitParser();

  // allow for all exstl functions
  dlexsltRegisterAll();

#if defined(__GNUG__) && !defined(__MINGW32__)
  typedef xsltStylesheetPtr (*xsltParseStylesheetFile_function) (const xmlChar*);

  void* handle = dlopen("libexslt.so", RTLD_LAZY);
  if (!handle) {
    handle = dlopen("libexslt.dylib", RTLD_LAZY);
    if (!handle) {
      fprintf(stderr, "Unable to open libexslt library\n");
      return;
    }
  }

  dlerror();
  xsltParseStylesheetFile_function xsltParseStylesheetFile = (xsltParseStylesheetFile_function)dlsym(handle, "xsltParseStylesheetFile");
  char* error;
  if ((error = dlerror()) != NULL) {
    dlclose(handle);
    return;
  }
#endif

  // parse the stylesheet
  xsltStylesheetPtr stylesheet = xsltParseStylesheetFile(BAD_CAST xslts[0]);
  if (!stylesheet)
    return;

  // setup parser
  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateParserCtxt(buffer_input);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  // setup process handling
  XSLTUnits process(context_element, ofilename, options, stylesheet, params, fd);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1, diff_version);
  ctxt->_private = &state;

  xsltsrcMLRegister();

  // process the document
  srcMLParseDocument(ctxt, false);

  // local variable, do not want xmlFreeParserCtxt to free
  ctxt->sax = NULL;

  if(buffer_input) inputPop(ctxt);
  // all done with parsing
  xmlFreeParserCtxt(ctxt);
}

// relaxng evaluation of the nested units
void srcMLUtility::relaxng(const char* ofilename, const char** xslts, int fd) {

  xmlParserCtxtPtr ctxt = 0;
  if(infile)
    ctxt = srcMLCreateURLParserCtxt(infile);
  else
    ctxt = srcMLCreateParserCtxt(buffer_input);
  if (ctxt == NULL) return;

  // setup sax handler
  xmlSAXHandler sax = SAX2ExtractUnitsSrc::factory();
  ctxt->sax = &sax;

  xmlRelaxNGParserCtxtPtr relaxng = xmlRelaxNGNewParserCtxt(xslts[0]);
  xmlRelaxNGPtr rng = xmlRelaxNGParse(relaxng);
  xmlRelaxNGValidCtxtPtr rngctx = xmlRelaxNGNewValidCtxt(rng);
  RelaxNGUnits process(ofilename, options, rngctx, fd);

  // setup sax handling state
  SAX2ExtractUnitsSrc state(&process, &options, -1, diff_version);
  ctxt->_private = &state;

  srcMLParseDocument(ctxt, false);

  ctxt->sax = NULL;

  if(buffer_input) inputPop(ctxt);
  xmlFreeParserCtxt(ctxt);
  xmlRelaxNGFreeValidCtxt(rngctx);
  xmlRelaxNGFree(rng);
  xmlRelaxNGFreeParserCtxt(relaxng);
}


// process srcML document with error reporting
static void srcMLParseDocument(xmlParserCtxtPtr ctxt, bool allowendearly) {

  // process the document
  int status;
  if ((status = xmlParseDocument(ctxt)) == -1) {

    xmlErrorPtr ep = xmlCtxtGetLastError(ctxt);

    // special case
    if (allowendearly && (ep->code == XML_ERR_EXTRA_CONTENT || ep->code == XML_ERR_DOCUMENT_END))
      return;

    if (incount)
      fprintf(stderr, "\n");

    // report error
    char* partmsg = strdup(ep->message);
    partmsg[strlen(partmsg) - 1] = '\0';
    fprintf(stderr, "%s: %s in '%s'\n", "srcml2src", partmsg, ep->file);
    exit(STATUS_INPUTFILE_PROBLEM);
  }
}

// create srcml parser with error reporting
static xmlParserCtxtPtr srcMLCreateURLParserCtxt(const char * infile) {

  xmlParserCtxtPtr ctxt = xmlCreateURLParserCtxt(infile, XML_PARSE_COMPACT | XML_PARSE_HUGE);
  if (ctxt == NULL) {

    // report error
    xmlErrorPtr ep = xmlGetLastError();
    fprintf(stderr, "%s: %s", "srcml2src", ep->message);
    exit(STATUS_INPUTFILE_PROBLEM);
  }

  return ctxt;
}

// create srcml parser with error reporting
static xmlParserCtxtPtr srcMLCreateMemoryParserCtxt(const char * buffer, int size) {

  xmlParserCtxtPtr ctxt = xmlCreateMemoryParserCtxt(buffer, size);

  if (ctxt == NULL) {

    // report error
    xmlErrorPtr ep = xmlGetLastError();
    fprintf(stderr, "%s: %s", "srcml2src", ep->message);
    exit(STATUS_INPUTFILE_PROBLEM);
  }

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
#define CHECK_COMPAT(buf)                                   \
  if (buf->size != (size_t) buf->compat_size)            \
    if (buf->compat_size < INT_MAX)                    \
      buf->size = buf->compat_size;                  \
  if (buf->use != (size_t) buf->compat_use)              \
    if (buf->compat_use < INT_MAX)                     \
      buf->use = buf->compat_use;

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
int
xmlBufResetInput(xmlBuffer * buf, xmlParserInputPtr input) {
  input->base = input->buf->buffer->content;
  input->cur = input->buf->buffer->content;
  input->end = &input->buf->buffer->content[input->buf->buffer->use];
}

#endif

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
