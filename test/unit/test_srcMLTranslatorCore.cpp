/**
 * @file test_srcMLTranslatorCore.cpp
 * @copyright
 *
 * Copyright (C) 2013  SDML (www.srcML.org)
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
    OPTION_TYPE op = 0;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", urisprefix, 4);

    translator.close();
    xmlBufferFree(buffer);
    
  }

  /*

    srcMLTranslatorCore

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op = 0;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", urisprefix, 4);


    translator.close();
    xmlBufferFree(buffer);
    
  }

  /*

    setInput

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op = 0;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", urisprefix, 4);

    xmlParserInputBufferPtr input = xmlParserInputBufferCreateMem("a;", 2, xmlParseCharEncoding(0));
    try {
      translator.setInput(input);
    } catch(...) { assert(false); }
    xmlFreeParserInputBuffer(input);
    translator.close();
    xmlBufferFree(buffer);
    
  }

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op = 0;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, "", "", "", urisprefix, 4);

    try {
      translator.setInput((xmlParserInputBufferPtr)NULL);
      assert(false);
    } catch(...) {}

    translator.close();
    xmlBufferFree(buffer);
    
  }

  /*

    translate_separate

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op = 0;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, 0, 0, 0, urisprefix, 4);

    translator.setInputString("a;", 2);
    xmlBufferPtr output = xmlBufferCreate();
    translator.translate_separate(0, 0,  0, 0, Language::LANGUAGE_CXX, output);
    assert((char *)output->content == std::string("<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>\n"));

    xmlBufferFree(output);
    translator.close();
    xmlBufferFree(buffer);
    
  }

  /*

    add_unit

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op = 0;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, 0, 0, 0, urisprefix, 4);

    std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
    std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

    translator.add_unit(s.c_str());
    translator.close();
    std::string result = (const char *)buffer->content;
    assert(result == decl + "\n" + s + "\n\n\n");

    xmlBufferFree(buffer);
    
  }


  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    xmlTextWriterPtr writer = xmlNewTextWriter(output_buffer);
    OPTION_TYPE op = OPTION_ARCHIVE;

    srcMLTranslatorCore translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 writer, op, 0, 0, 0, urisprefix, 4);

    std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
    std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

    translator.add_unit(s.c_str());
    translator.add_unit(s.c_str());
    translator.close();
    std::string result = (const char *)buffer->content;
    assert(result == decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

    xmlBufferFree(buffer);
    
  }

  return 0;
}
