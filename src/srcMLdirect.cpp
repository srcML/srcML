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

  srcMLTranslator::srcMLTranslator translator(srcMLTranslator::LANGUAGE_C,
		  "UTF-8",
		  "ISO-8859-1",
		  "test.c.xml",
		  options,
		  "",
		  "test.c",
		  "1",
		  urisprefix,
		  8
		  );

  fprintf(stderr, "HERE:%s\n", "test.c");

  // translate from input stream to output stream
  translator.translate("test.c.xml", 
                       "",
                       "test.c.xml", 
                       "1",
                       srcMLTranslator::LANGUAGE_C,
                       8);

  fprintf(stderr, "HERE\n");

  return 0;
}
