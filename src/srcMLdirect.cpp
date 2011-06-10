#include "srcmlapps.hpp"
#include "srcmlns.hpp"
#include "Options.hpp"
#include "srcMLTranslator.hpp"

int main(int argc, char * argv[]) {

  OPTION_TYPE options = OPTION_CPP_MARKUP_ELSE;
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
    srcMLTranslator(int language,
		  const char* src_encoding,
		  const char* xml_encoding,
		  const char* srcml_filename,
		  OPTION_TYPE& options,
		  const char* directory,
		  const char* filename,
		  const char* version,
		  const char* uri[],
		  int tabsize
		  );

    void* setInput(const char* path);

    // translate from input stream to output stream
    void translate(const char* path,
		 const char* unit_directory = 0,
		 const char* unit_filename = 0,
		 const char* unit_version = 0,
		 int language = 0,
		 int tabsize = 8);
   */

  srcMLTranslator::srcMLTranslator translator(srcMLTranslator::LANGUAGE_C,
                                              DEFAULT_TEXT_ENCODING,
                                              DEFAULT_TEXT_ENCODING,
                                              "test.c.xml",
                                              options,
                                              ".",
                                              "test.c",
                                              "1",
                                              urisprefix,
                                              8
                                              );

  translator.setInput("test.c");

  // translate from input stream to output stream
  translator.translate("test.c.xml",
                       ".",
                       "test.c.xml", 
                       "1",
                       srcMLTranslator::LANGUAGE_C,
                       8
                       );

  return 0;
}
