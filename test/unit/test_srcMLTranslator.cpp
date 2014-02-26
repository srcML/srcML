/**
 * @file test_srcMLTranslator.cpp
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
  test_srcMLTranslator.cpp

  Unit tests for srcMLTranslator.

 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <srcMLTranslator.hpp>
#include <UTF8CharBuffer.hpp>
#include <Language.hpp>
#include <srcmlns.hpp>

int main() {

  std::ofstream file("a.cpp");
  file << "a;";
  file.close();

  std::string urisprefix[] = {

    SRCML_SRC_NS_PREFIX_DEFAULT,
    SRCML_CPP_NS_PREFIX_DEFAULT,
    SRCML_ERR_NS_PREFIX_DEFAULT,
    SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
    SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
    SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
    SRCML_EXT_POSITION_NS_PREFIX_DEFAULT,
  };

  /*

    srcMLTranslator

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    OPTION_TYPE op = 0;

    srcMLTranslator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 output_buffer, op, "", "", "", urisprefix, 4);

    translator.close();
    xmlBufferFree(buffer);
    
  }

  /*

    srcMLTranslator

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    OPTION_TYPE op = 0;

    srcMLTranslator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 output_buffer, op, "", "", "", urisprefix, 4);


    translator.close();
    xmlBufferFree(buffer);
    
  }

  /*

    setInput

   */

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    OPTION_TYPE op = 0;

    srcMLTranslator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 output_buffer, op, "", "", "", urisprefix, 4);

    try {
      translator.setInput("a.cpp");
    } catch(...) { assert(false); }
    translator.close();
    xmlBufferFree(buffer);
    
  }

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    OPTION_TYPE op = 0;

    srcMLTranslator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 output_buffer, op, "", "", "", urisprefix, 4);

    try {
      translator.setInput(NULL);
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
    OPTION_TYPE op = 0;

    srcMLTranslator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 output_buffer, op, 0, 0, 0, urisprefix, 4);

    UTF8CharBuffer * input = new UTF8CharBuffer("a;", 2, "UTF-8");

    xmlBufferPtr output = xmlBufferCreate();
    translator.translate_separate(0,  0, 0, Language::LANGUAGE_CXX, input, output, op);
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
    OPTION_TYPE op = OPTION_XMLDECL | OPTION_NAMESPACEDECL;

    srcMLTranslator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 output_buffer, op, 0, 0, 0, urisprefix, 4);

    std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
    std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

    translator.add_unit(s.c_str());
    translator.close();
    std::string result = (const char *)buffer->content;
    assert(result == decl + "\n" + s + "\n");

    xmlBufferFree(buffer);
    
  }

  {

    xmlBufferPtr buffer = xmlBufferCreate();
    xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(0));
    OPTION_TYPE op = OPTION_ARCHIVE | OPTION_XMLDECL | OPTION_NAMESPACEDECL;

    srcMLTranslator translator(Language::LANGUAGE_CXX, "ISO-8859-1", "ISO-8859-1", 
                                 output_buffer, op, 0, 0, 0, urisprefix, 4);

    std::string decl = "<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>";
    std::string s = "<unit language=\"C++\"><expr_stmt><expr><name>a</name></expr>;</expr_stmt></unit>";

    translator.add_unit(s.c_str());
    translator.add_unit(s.c_str());
    translator.close();
    std::string result = (const char *)buffer->content;
    assert(result == decl + "\n<unit xmlns=\"http://www.sdml.info/srcML/src\">\n\n" + s + "\n\n" + s + "\n\n</unit>\n");

    xmlBufferFree(buffer);
    
  }

  unlink("a.cpp");

  return 0;
}
