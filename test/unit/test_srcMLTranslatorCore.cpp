/*
  test_srcMLTranslatorCore.cpp

  Unit tests for srcMLTranslatorCore.

 */
#include <stdio.h>
#include <string.h>
#include <srcMLTranslatorCore.hpp>
#include <Language.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

  const char* urisprefix[] = {

    SRCML_SRC_NS_PREFIX_DEFAULT,
    SRCML_CPP_NS_PREFIX_DEFAULT,
    SRCML_ERR_NS_PREFIX_DEFAULT,
    SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
    SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
    SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
    SRCML_EXT_POSITION_NS_PREFIX_DEFAULT,
  };

  /*

    srcMLTranslatorCore

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op;

    srcMLTranslatorCore translator(Language::getLanguage("C++"), "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", urisprefix, 4);

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
                                 writer, op, "", "", "", urisprefix, 4);


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
                                 writer, op, "", "", "", urisprefix, 4);

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
                                 writer, op, "", "", "", urisprefix, 4);

    try {
      translator.setInput((xmlParserInputBufferPtr)NULL);
      assert(false);
    } catch(...) {}

    xmlBufferFree(buffer);
    
  }

  /*

    translate_separate

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, 0, 0, 0, urisprefix, 4);

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem("a;", 2, xmlParseCharEncoding(0));
    translator.setInput(input);
    xmlBufferPtr output = xmlBufferCreate();
    translator.translate_separate("", "",  "", "", Language::LANGUAGE_CXX, output);
    xmlFreeParserInputBuffer(input);
    xmlBufferFree(buffer);
    
  }

  return 0;
}
