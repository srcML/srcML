#include "../Options.hpp"

#include <libxml/parserInternals.h>

#ifndef INCLUDDED_SRCML_SAX2_UTILITIES_HPP
#define INCLUDDED_SRCML_SAX2_UTILITIES_HPP

void extract_text(const char * input_buffer, int size, xmlOutputBufferPtr output_buffer, OPTION_TYPE options, int unit = 0);
void xpath(xmlParserInputBufferPtr input_buffer, const char* context_element, const char* xpaths[], int fd, OPTION_TYPE options);
void xslt(xmlParserInputBufferPtr input_buffer, const char* context_element, const char* xslts[], const char* params[], int paramcount, int fd, OPTION_TYPE options);
void relaxng(xmlParserInputBufferPtr input_buffer, const char** xslts, int fd, OPTION_TYPE options);

#endif
