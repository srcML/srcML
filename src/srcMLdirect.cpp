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

  // create translator object
  srcMLTranslator::srcMLTranslator translator(srcMLTranslator::LANGUAGE_C, argv[2], options);

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
