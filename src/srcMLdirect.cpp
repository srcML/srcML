#include "srcmlapps.hpp"
#include "srcmlns.hpp"
#include "Options.hpp"
#include "srcMLTranslator.hpp"

int main(int argc, char * argv[]) {

  if(argc < 3) {

    fprintf(stderr, "Usage: srcMLdirect inputfile outputfile\n");
    return 1;
  }
  if(strcmp(argv[1], argv[2]) == 0) {

    fprintf(stderr, "Input and output file must be different\n");
    return 1;
  }

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

  // create translator object
  srcMLTranslator::srcMLTranslator translator(srcMLTranslator::LANGUAGE_C,
                                              DEFAULT_TEXT_ENCODING,
                                              DEFAULT_XML_ENCODING,
                                              argv[2],
                                              options,
                                              "",
                                              argv[1],
                                              "1",
                                              urisprefix,
                                              8
                                              );

  // set input file (must be done)
  translator.setInput(argv[1]);

  // translate file
  translator.translate(argv[1],
                       "",
                       argv[1], 
                       "1",
                       srcMLTranslator::LANGUAGE_C,
                       8
                       );

  return 0;
}
