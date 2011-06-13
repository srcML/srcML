#include "srcmlapps.hpp"
#include "Options.hpp"
#include "srcMLTranslator.hpp"
#include "Language.cpp"

/*
  Example of using libsrcml to translate a file
*/
int main(int argc, char * argv[]) {

  if(argc < 3) {

    fprintf(stderr, "Usage: srcMLdirect inputfile outputfile\n");
    return 1;
  }
  if(strcmp(argv[1], argv[2]) == 0) {

    fprintf(stderr, "Input and output file must be different\n");
    return 1;
  }

  Language::register_standard_file_extensions();

  OPTION_TYPE options = OPTION_CPP_MARKUP_ELSE;
  int language = Language::getLanguageFromFilename(argv[1]);
  fprintf(stderr, "Language: %d\n", language);

  // create translator object
  srcMLTranslator::srcMLTranslator translator(language, argv[2], options);

  // set input file (must be done)
  translator.setInput(argv[1]);

  // translate file
  translator.translate(argv[1],
                       NULL,
                       argv[1], 
                       NULL,
                       language,
                       8
                       );

  return 0;
}
