/*
  test_srcMLTranslatorCore.cpp

  Unit tests for srcMLTranslatorCore.

 */
#include <stdio.h>
#include <string.h>
#include <srcMLTranslatorCore.hpp>
#include <Language.hpp>

int main(int argc, char * argv[]) {

  /*

    srcMLTranslatorCore

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op;

    srcMLTranslatorCore translator(Language::getLanguage("C++"), "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", 0, 4);

    xmlBufferFree(buffer);
    
  }

  /*

    srcMLTranslatorCore

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op;

    srcMLTranslatorCore translator(Language::getLanguage("C++"), "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", 0, 4);

    xmlBufferFree(buffer);
    
  }

  /*

    setInput

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op;

    srcMLTranslatorCore translator(Language::getLanguage("C++"), "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", 0, 4);

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem("a;", 2, xmlParseCharEncoding(0));
    try {
      translator.setInput(input);
    } catch(...) { assert(false); }
    xmlFreeParserInputBuffer(input);
    xmlBufferFree(buffer);
    
  }

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op;

    srcMLTranslatorCore translator(Language::getLanguage("C++"), "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", 0, 4);

    try {
      translator.setInput((xmlParserInputBufferPtr)NULL);
      assert(false);
    } catch(...) {}

    xmlBufferFree(buffer);
    
  }

  return 0;
}
