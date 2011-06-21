/*
  Example of using libsrcml to translate a file.

  Michael J. Decker
  mjd52@zips.uakron.edu
  $Date$
  $Author$
  $Id$
*/

#include "srcmlapps.hpp"
#include "Options.hpp"
#include "srcMLTranslator.hpp"
#include "Language.cpp"

int main(int argc, char * argv[]) {

  // test for correct input
  if(argc < 3) {

    fprintf(stderr, "Usage: srcmldirect inputfile outputfile\n");
    return 1;
  }
  if(strcmp(argv[1], argv[2]) == 0) {

    fprintf(stderr, "Input and output file must be different\n");
    return 1;
  }

  // register default language extensions
  Language::register_standard_file_extensions();

  // get the language from the filename
  int language = Language::getLanguageFromFilename(argv[1]);

  // select basic options
  OPTION_TYPE options = OPTION_CPP_MARKUP_ELSE;

  // create translator object
  srcMLTranslator translator(language, argv[2], options);

  // set input file (must be done)
  translator.setInput(argv[1]);

  // translate file
  translator.translate(argv[1], NULL, argv[1], NULL, language);

  return 0;
}
